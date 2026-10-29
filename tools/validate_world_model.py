#!/usr/bin/env python3
"""
validate_world_model.py
Standard library validator for the CO1301 World Model.
Checks:
- propositions.jsonl format and schema
- reasoning_chains.jsonl format and schema
- Valid IDs, prerequisite existence, premise existence, conclusion existence
- Existence of referenced source files on disk
- Valid domains, weeks, kinds, and relations
"""

import json
import os
import sys

ALLOWED_DOMAINS = {
    "setup",
    "cpp_basics",
    "engine_loop",
    "scene_objects",
    "input",
    "camera",
    "sprites_ui",
    "timing",
    "vectors",
    "collision",
    "fsm",
    "arrays_loops",
    "game_design",
    "industry",
    "ethics_ratings",
    "assessment",
}

ALLOWED_KINDS = {"fact", "procedure", "definition", "api", "warning"}
ALLOWED_RELATIONS = {"enables", "explains", "contradicts", "refines"}


def validate():
    workspace_root = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
    wm_dir = os.path.join(workspace_root, "world_model")

    props_file = os.path.join(wm_dir, "propositions.jsonl")
    chains_file = os.path.join(wm_dir, "reasoning_chains.jsonl")
    api_file = os.path.join(wm_dir, "api_reference.md")
    rundown_file = os.path.join(wm_dir, "RUNDOWN.md")
    readme_file = os.path.join(wm_dir, "README.md")

    errors = []

    # Check required files exist
    for req_file in [props_file, chains_file, api_file, rundown_file, readme_file]:
        if not os.path.isfile(req_file):
            errors.append(f"Missing required file: {os.path.relpath(req_file, workspace_root)}")

    # Check README line count
    if os.path.isfile(readme_file):
        with open(readme_file, "r", encoding="utf-8") as f:
            readme_lines = [l for l in f.readlines() if l.strip()]
        if len(readme_lines) > 10:
            errors.append(f"README.md has {len(readme_lines)} non-empty lines (max allowed is 10)")

    prop_ids = set()
    prop_entries = []

    # 1. Parse propositions.jsonl
    if os.path.isfile(props_file):
        with open(props_file, "r", encoding="utf-8") as f:
            for line_no, line in enumerate(f, 1):
                line = line.strip()
                if not line:
                    continue
                try:
                    obj = json.loads(line)
                except Exception as e:
                    errors.append(f"propositions.jsonl:{line_no}: JSON parse error: {e}")
                    continue

                p_id = obj.get("id")
                if not p_id:
                    errors.append(f"propositions.jsonl:{line_no}: missing 'id'")
                elif p_id in prop_ids:
                    errors.append(f"propositions.jsonl:{line_no}: duplicate proposition id '{p_id}'")
                else:
                    prop_ids.add(p_id)

                domain = obj.get("domain")
                if domain not in ALLOWED_DOMAINS:
                    errors.append(f"propositions.jsonl:{line_no} ({p_id}): invalid domain '{domain}'")

                week = obj.get("week")
                if week not in (1, 2, 3, None):
                    errors.append(f"propositions.jsonl:{line_no} ({p_id}): invalid week '{week}'")

                kind = obj.get("kind")
                if kind not in ALLOWED_KINDS:
                    errors.append(f"propositions.jsonl:{line_no} ({p_id}): invalid kind '{kind}'")

                statement = obj.get("statement")
                if not statement or not isinstance(statement, str):
                    errors.append(f"propositions.jsonl:{line_no} ({p_id}): missing or non-string statement")

                prereqs = obj.get("prerequisites", [])
                if not isinstance(prereqs, list):
                    errors.append(f"propositions.jsonl:{line_no} ({p_id}): prerequisites must be a list")

                source_refs = obj.get("source_refs", [])
                if not source_refs or not isinstance(source_refs, list):
                    errors.append(f"propositions.jsonl:{line_no} ({p_id}): source_refs must be a non-empty list")
                else:
                    for s_idx, ref in enumerate(source_refs):
                        s_file = ref.get("file")
                        s_loc = ref.get("loc")
                        if not s_file or not s_loc:
                            errors.append(f"propositions.jsonl:{line_no} ({p_id}): source_ref #{s_idx} missing 'file' or 'loc'")
                        else:
                            # Normalize path relative to workspace root
                            norm_file = os.path.normpath(os.path.join(workspace_root, s_file))
                            if not os.path.isfile(norm_file):
                                errors.append(f"propositions.jsonl:{line_no} ({p_id}): source file not found: '{s_file}'")

                prop_entries.append((line_no, obj))

    # Validate prerequisites point to known proposition IDs
    for line_no, obj in prop_entries:
        p_id = obj.get("id")
        for req in obj.get("prerequisites", []):
            if req not in prop_ids:
                errors.append(f"propositions.jsonl:{line_no} ({p_id}): unknown prerequisite '{req}'")

    # 2. Parse reasoning_chains.jsonl
    chain_ids = set()
    chain_count = 0
    if os.path.isfile(chains_file):
        with open(chains_file, "r", encoding="utf-8") as f:
            for line_no, line in enumerate(f, 1):
                line = line.strip()
                if not line:
                    continue
                try:
                    obj = json.loads(line)
                except Exception as e:
                    errors.append(f"reasoning_chains.jsonl:{line_no}: JSON parse error: {e}")
                    continue

                chain_count += 1
                c_id = obj.get("id")
                if not c_id:
                    errors.append(f"reasoning_chains.jsonl:{line_no}: missing 'id'")
                elif c_id in chain_ids:
                    errors.append(f"reasoning_chains.jsonl:{line_no}: duplicate chain id '{c_id}'")
                else:
                    chain_ids.add(c_id)

                relation = obj.get("relation")
                if relation not in ALLOWED_RELATIONS:
                    errors.append(f"reasoning_chains.jsonl:{line_no} ({c_id}): invalid relation '{relation}'")

                premises = obj.get("premise_ids", [])
                if not isinstance(premises, list) or not premises:
                    errors.append(f"reasoning_chains.jsonl:{line_no} ({c_id}): premise_ids must be non-empty list")
                else:
                    for p in premises:
                        if p not in prop_ids:
                            errors.append(f"reasoning_chains.jsonl:{line_no} ({c_id}): premise '{p}' not in propositions")

                conclusion = obj.get("conclusion_id")
                if not conclusion or conclusion not in prop_ids:
                    errors.append(f"reasoning_chains.jsonl:{line_no} ({c_id}): conclusion '{conclusion}' not in propositions")

                note = obj.get("note")
                if not note or not isinstance(note, str):
                    errors.append(f"reasoning_chains.jsonl:{line_no} ({c_id}): missing or non-string note")

    # Report results
    print("=" * 60)
    print("CO1301 WORLD MODEL VALIDATION REPORT")
    print("=" * 60)
    print(f"Total Propositions: {len(prop_ids)}")
    print(f"Total Reasoning Chains: {chain_count}")

    if errors:
        print(f"\n[FAIL] Found {len(errors)} error(s):")
        for err in errors[:50]:
            print(f"  - {err}")
        if len(errors) > 50:
            print(f"  ... and {len(errors) - 50} more errors")
        sys.exit(1)
    else:
        print("\n[PASS] All validation checks passed successfully!")
        sys.exit(0)


if __name__ == "__main__":
    validate()
