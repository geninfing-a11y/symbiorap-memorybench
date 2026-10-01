#!/usr/bin/env python3
import json, sys
from pathlib import Path
root=Path(sys.argv[1] if len(sys.argv)>1 else "results/m14-reference")
required=["results.json","results.csv","claim-matrix.json","OPEN_BENCHMARK_REPORT.md"]
missing=[x for x in required if not (root/x).exists()]
if missing: raise SystemExit("missing: "+", ".join(missing))
r=json.loads((root/"results.json").read_text(encoding="utf-8"))
c=json.loads((root/"claim-matrix.json").read_text(encoding="utf-8"))
assert r["publication_allowed"] is True
assert r["claim_scope"]=="claim-matrix-only"
assert c["publication_allowed"] is True
assert all(x["status"]!="NOT_SUPPORTED" for x in c["claims"] if x.get("public"))
assert any(x["status"]=="NOT_SUPPORTED" for x in c["claims"])
print("M14_VERIFY_PASS")
