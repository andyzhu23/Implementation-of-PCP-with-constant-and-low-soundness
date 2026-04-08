"""Nonparametric sampling methods for data analysis.

This module provides functions to perform bootstrap resampling to compute
confidence intervals for differences in means between two datasets.

Usage example:
    python data_analysis/nonparametric_sampling.py --input results/test_three_color_gap_amplification_pseudo_result.txt --outdir results/ci_intervals_pseudo.txt

"""

import argparse
import os
import sys
import parse_results

import numpy as np

def bootstrap_mean_confidence_interval(a, alpha=0.05, n_resamples=10000, seed=None):
    """Bootstrap percentile CI for the mean of a single sample."""
    a = np.asarray(a, dtype=float)
    a = a[~np.isnan(a)]
    n = a.size
    if n == 0:
        return (float('nan'), float('nan'))

    rng = np.random.default_rng(seed)
    draws = rng.choice(a, size=(n_resamples, n), replace=True)
    means = draws.mean(axis=1)

    lower = np.percentile(means, 100 * (alpha / 2.0))
    upper = np.percentile(means, 100 * (1 - alpha / 2.0))
    return (float(lower), float(upper))
    

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--input", "-i", required=True, help="Path to the result text file")
    parser.add_argument("--outdir", "-o", default=".", help="Output file path or directory for CI results")
    args = parser.parse_args()

    if not os.path.isfile(args.input):
        print(f"Error: input file not found: {args.input}", file=sys.stderr)
        sys.exit(2)

    with open(args.input, "r") as f:
        lines = f.readlines()

    _, amplified_gaps = parse_results.parse_results(lines)

    if not amplified_gaps:
        print("Error: no test cases found in input file", file=sys.stderr)
        sys.exit(1)

    non_empty_gaps = [ag for ag in amplified_gaps if len(ag) > 0]
    if not non_empty_gaps:
        print("Error: no gap samples found in input file", file=sys.stderr)
        sys.exit(1)

    mean1 = float(np.nanmean(non_empty_gaps[0]))
    print(f"mean of amplified gaps (test 1): {mean1}")

    output_path = args.outdir
    if os.path.isdir(output_path):
        output_path = os.path.join(output_path, "ci_intervals.txt")
    else:
        parent = os.path.dirname(output_path)
        if parent:
            os.makedirs(parent, exist_ok=True)

    # bootstrap CIs for the mean of each test
    with open(output_path, "w") as fout:
        for idx, ag in enumerate(amplified_gaps):
            ci = bootstrap_mean_confidence_interval(ag, alpha=0.05, n_resamples=10000)
            line = f"Constraint {30 + idx * 10}: 95% CI for mean: ({ci[0]:.5f}, {ci[1]:.5f})"
            print(line)
            fout.write(line + "\n")
        
        # overall_ci = bootstrap_mean_confidence_interval(np.concatenate(non_empty_gaps), alpha=0.05, n_resamples=10000)
        # overall_line = f"Overall: 95% CI for mean of all amplified gaps: ({overall_ci[0]:.5f}, {overall_ci[1]:.5f})"
        
        ci_30_60_120 = bootstrap_mean_confidence_interval(np.concatenate([non_empty_gaps[0], non_empty_gaps[3], non_empty_gaps[-1]]), alpha=0.05, n_resamples=10000)
        
        ci_30_60_120_line = f"30-60-120: 95% CI for mean: ({ci_30_60_120[0]:.5f}, {ci_30_60_120[1]:.5f})"
        
        print(ci_30_60_120_line)
        fout.write(ci_30_60_120_line + "\n")

    print(f"Saved CI results to: {output_path}")

if __name__ == "__main__":
    main()