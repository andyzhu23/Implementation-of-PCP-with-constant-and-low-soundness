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

    # bootstrap CIs for the mean of each test
    ci_test1 = bootstrap_mean_confidence_interval(amplified_gaps[0], alpha=0.05, n_resamples=10000)
    ci_test2 = bootstrap_mean_confidence_interval(amplified_gaps[1], alpha=0.05, n_resamples=10000)
    ci_test3 = bootstrap_mean_confidence_interval(amplified_gaps[2], alpha=0.05, n_resamples=10000)
    print(f"95% CI for mean (test1): ({ci_test1[0]:.5f}, {ci_test1[1]:.5f})")
    print(f"95% CI for mean (test2): ({ci_test2[0]:.5f}, {ci_test2[1]:.5f})")
    print(f"95% CI for mean (test3): ({ci_test3[0]:.5f}, {ci_test3[1]:.5f})")

if __name__ == "__main__":
    main()