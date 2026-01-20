"""Parse a test run result file and plot original vs amplified gaps per test case.

Usage:
  python3 analyze_amplification.py --input PATH [--outdir DIR]
Example:
  python3 data_analysis/analyze_amplification.py --input build/test_three_color_gap_amplification_amplification_result.txt --outdir data_analysis/output

This script uses the non-interactive Agg backend and writes PNGs to the output
directory (defaults to the script directory). It prints the saved filenames.
"""
import argparse
import os
import statistics
import sys
import matplotlib

import parse_results

# Use non-interactive backend so the script works in headless environments and
# always saves files to disk.
matplotlib.use("Agg")
import matplotlib.pyplot as plt

def plot_and_save(original_gaps, amplified_gaps, outdir):
    os.makedirs(outdir, exist_ok=True)
    saved_files = []
    # Compute mean and variance (and std) for each test case
    orig_means = []
    ampl_means = []
    orig_vars = []
    ampl_vars = []
    orig_stds = []
    ampl_stds = []

    num_cases = max(len(original_gaps), len(amplified_gaps))
    for i in range(num_cases):
        o = original_gaps[i] if i < len(original_gaps) else []
        a = amplified_gaps[i] if i < len(amplified_gaps) else []

        if o:
            om = statistics.mean(o)
            if len(o) > 1:
                ov = statistics.variance(o)
                osd = statistics.stdev(o)
            else:
                ov = 0.0
                osd = 0.0
        else:
            om = float('nan')
            ov = float('nan')
            osd = float('nan')

        if a:
            am = statistics.mean(a)
            if len(a) > 1:
                av = statistics.variance(a)
                asd = statistics.stdev(a)
            else:
                av = 0.0
                asd = 0.0
        else:
            am = float('nan')
            av = float('nan')
            asd = float('nan')

        orig_means.append(om)
        ampl_means.append(am)
        orig_vars.append(ov)
        ampl_vars.append(av)
        orig_stds.append(osd)
        ampl_stds.append(asd)

    # Create grouped bar chart: means with std as error bars, annotate variance
    indices = list(range(num_cases))
    width = 0.35

    fig, ax = plt.subplots(figsize=(12, 7))
    left_positions = [i - width / 2 for i in indices]
    right_positions = [i + width / 2 for i in indices]

    # Use 0 for nan/std to avoid matplotlib errors; bars will be NaN heights if mean is nan
    orig_bar_vals = [0.0 if (x is None or (isinstance(x, float) and (x != x))) else x for x in orig_means]
    ampl_bar_vals = [0.0 if (x is None or (isinstance(x, float) and (x != x))) else x for x in ampl_means]

    # Replace NaN stds with 0
    orig_err = [0.0 if (isinstance(s, float) and (s != s)) else s for s in orig_stds]
    ampl_err = [0.0 if (isinstance(s, float) and (s != s)) else s for s in ampl_stds]

    bars1 = ax.bar(left_positions, orig_bar_vals, width, yerr=orig_err, capsize=5, label='Original')
    bars2 = ax.bar(right_positions, ampl_bar_vals, width, yerr=ampl_err, capsize=5, label='Amplified')

    ax.set_xticks(indices)
    ax.set_xticklabels([f"Test {i+1}" for i in indices], rotation=45)
    ax.set_ylabel('Mean Gap')
    ax.set_title('Means and standard deviations per test (Original vs Amplified)')
    ax.legend()
    ax.grid(axis='y')

    # Annotate variance values above each bar
    def annotate_bars(bars, vars_list):
        for bar, var in zip(bars, vars_list):
            height = bar.get_height()
            if isinstance(var, float) and (var != var):
                txt = 'var: N/A'
            else:
                txt = f"var: {var:.3g}"
            ax.annotate(txt,
                        xy=(bar.get_x() + bar.get_width() / 2, height),
                        xytext=(0, 3),  # 3 points vertical offset
                        textcoords="offset points",
                        ha='center', va='bottom', fontsize=8)

    annotate_bars(bars1, orig_vars)
    annotate_bars(bars2, ampl_vars)

    fname = os.path.join(outdir, f"gap_means_variances_bar.png")
    fig.tight_layout()
    fig.savefig(fname)
    plt.close(fig)
    saved_files.append(fname)

    return saved_files


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--input", "-i", required=True, help="Path to the result text file")
    parser.add_argument("--outdir", "-o", default=".", help="Directory to save PNGs")
    args = parser.parse_args()

    if not os.path.isfile(args.input):
        print(f"Error: input file not found: {args.input}", file=sys.stderr)
        sys.exit(2)

    with open(args.input, "r") as f:
        lines = f.readlines()

    original_gaps, amplified_gaps = parse_results.parse_results(lines)

    if not original_gaps:
        print("No gap data found in the input file.", file=sys.stderr)
        sys.exit(1)

    saved = plot_and_save(original_gaps, amplified_gaps, args.outdir)

    print("Saved plots:")
    for s in saved:
        print("  ", s)


if __name__ == "__main__":
    main()
        