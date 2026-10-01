#!/usr/bin/env python3
from __future__ import annotations
import hashlib, json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
REF = ROOT / "results" / "m14-reference"
required = ["results.json", "results.csv", "claim-matrix.json", "OPEN_BENCHMARK_REPORT.md"]
missing = [x for x in required if not (REF / x).is_file()]
if missing:
    raise SystemExit("missing reference files: " + ", ".join(missing))

with (REF / "results.json").open("r", encoding="utf-8") as f:
    json.load(f)
with (REF / "claim-matrix.json").open("r", encoding="utf-8") as f:
    json.load(f)

for name in required:
    p = REF / name
    print(hashlib.sha256(p.read_bytes()).hexdigest(), name)
print("MEMORYBENCH_REFERENCE_VERIFY_PASS")
