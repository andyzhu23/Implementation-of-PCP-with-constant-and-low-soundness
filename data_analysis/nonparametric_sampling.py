"""Nonparametric sampling methods for data analysis.

This module provides functions to perform bootstrap resampling to compute
confidence intervals for differences in means between two datasets.

Usage example:
    python3 data_analysis/nonparametric_sampling.py --input PATH [--outdir DIR]

"""

import argparse
import os
import sys
import parse_results

import numpy as np

def bootstrap_diff_confidence_interval(a, b, alpha=0.05, n_resamples=10000, seed=None):
    """Bootstrap percentile CI for the mean difference (mean(a) - mean(b))."""
    a = np.asarray(a, dtype=float)
    b = np.asarray(b, dtype=float)
    a = a[~np.isnan(a)]
    b = b[~np.isnan(b)]
    na, nb = a.size, b.size
    if na == 0 or nb == 0:
        return (float('nan'), float('nan'))

    rng = np.random.default_rng(seed)
    draws_a = rng.choice(a, size=(n_resamples, na), replace=True)
    draws_b = rng.choice(b, size=(n_resamples, nb), replace=True)
    means_a = draws_a.mean(axis=1)
    means_b = draws_b.mean(axis=1)
    diffs = means_a - means_b

    lower = np.percentile(diffs, 100 * (alpha / 2.0))
    upper = np.percentile(diffs, 100 * (1 - alpha / 2.0))
    return (float(lower), float(upper))
    

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

    _, amplified_gaps = parse_results.parse_results(lines)

    mean1 = float(np.nanmean(amplified_gaps[0]))
    print(f"mean of amplified gaps (test 1): {mean1}")

    # bootstrap CIs for the difference in means (test1 - other)
    diff_ci_1_vs_4 = bootstrap_diff_confidence_interval(amplified_gaps[0], amplified_gaps[3], alpha=0.05, n_resamples=10000)
    diff_ci_1_vs_10 = bootstrap_diff_confidence_interval(amplified_gaps[0], amplified_gaps[9], alpha=0.05, n_resamples=10000)
    print(f"95% CI for mean difference (test1 - test4): ({diff_ci_1_vs_4[0]:.5f}, {diff_ci_1_vs_4[1]:.5f})")
    print(f"95% CI for mean difference (test1 - test10): ({diff_ci_1_vs_10[0]:.5f}, {diff_ci_1_vs_10[1]:.5f})")

if __name__ == "__main__":
    main()