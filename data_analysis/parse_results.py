"""Parse results from log files to extract original and amplified gaps.

Each log file is expected to contain lines indicating the original and amplified gaps
for various test cases.
"""

import sys

def parse_results(lines):
    original_gaps = []
    amplified_gaps = []

    for line in lines:
        if "Running test case" in line:
            original_gaps.append([])
            amplified_gaps.append([])

        if "original gap" in line and "amplified gap" in line:
            try:
                parts = line.split(",")
                original_gap_part = parts[0].strip()
                amplified_gap_part = parts[1].strip()
                original_gap = float(original_gap_part.split(":")[-1].strip())
                amplified_gap = float(amplified_gap_part.split(":")[-1].strip())
            except Exception:
                print(f"Warning: failed to parse line: {line.strip()}", file=sys.stderr)
                continue

            if not original_gaps:
                original_gaps.append([])
                amplified_gaps.append([])

            original_gaps[-1].append(original_gap)
            amplified_gaps[-1].append(amplified_gap)

    return original_gaps, amplified_gaps
