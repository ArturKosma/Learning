import sys
import os
import re
import json
from typing import Optional

RE_AFCLASS = re.compile(
    r'\bAFCLASS\s*\(\s*([a-zA-Z_][\w:]*)\s*,\s*"([^"]+)"(?:\s*,\s*"([^"]*)")?\s*\)'
)

# AFPARAM(Type, VarName, DefaultValue, Label, Direction, Meta)
# DefaultValue may be "quoted" or a bare token like 1.0f, 0.25, false, true, MyEnum::Value
RE_AFPARAM = re.compile(
    r'\bAFPARAM\s*\(\s*'
    r'([a-zA-Z_][\w:]*)\s*,\s*'                       # Type
    r'([a-zA-Z_][\w:]*)\s*,\s*'                       # VarName
    r'("([^"]*)"|[^,]+)\s*,\s*'                       # DefaultValue (grp3 raw, grp4 inner if quoted)
    r'"([^"]*)"\s*,\s*'                               # Label
    r'"([^"]*)"\s*,\s*'                               # Direction
    r'"([^"]*)"\s*'                                   # Meta
    r'\)'
)

# === ENUM SUPPORT ===
RE_AFENUM = re.compile(r'\bAFENUM\s*\(\s*\)')                 # AFENUM()
RE_ENUM_CLASS = re.compile(r'\benum\s+class\s+([a-zA-Z_]\w*)\b')  # enum class Name

def fetch_headers(directory):
    if not os.path.isdir(directory):
        return []
    return [
        os.path.join(directory, item)
        for item in os.listdir(directory)
        if os.path.isfile(os.path.join(directory, item)) and item.endswith(".h")
    ]

def normalize_default(default_raw: str, default_inner: Optional[str]) -> str:
    """
    Return a string representation of the default suitable for JSON.
    - If quoted: use the inner contents (including empty string).
    - If bare token:
        * strip trailing 'f' from numeric literals like 1.0f
        * keep 'true'/'false' as lower-case strings
        * otherwise return as-is (e.g., identifiers / enums) as a string token
    """
    if default_inner is not None:
        # It was quoted -> use inner without quotes
        return default_inner

    token = default_raw.strip()

    # Special case: {} → empty string
    if token == "{}":
        return ""

    # Lowercase for boolean detection
    low = token.lower()
    if low in ('true', 'false'):
        return low

    # Strip trailing 'f' from float literals like 1.0f
    if re.fullmatch(r'[+-]?(\d+(\.\d*)?|\.\d+)([eE][+-]?\d+)?f', token):
        return token[:-1]  # remove 'f'

    # If it looks like a plain number, keep as text anyway (JS can parseFloat)
    if re.fullmatch(r'[+-]?(\d+(\.\d*)?|\.\d+)([eE][+-]?\d+)?', token):
        return token

    # Fallback: enum/identifier -> keep as string token
    return token

def extract_macros(filepath):
    with open(filepath, 'r', encoding='utf-8') as f:
        lines = f.readlines()

    result = []
    current_class_id = None
    current_class_name = None
    current_class_meta = None
    current_params = []

    for line in lines:
        class_match = RE_AFCLASS.search(line)
        if class_match:
            if current_class_id:
                result.append([
                    (current_class_id, current_class_name, current_class_meta),
                    current_params
                ])
            current_class_id = class_match.group(1)
            current_class_name = class_match.group(2)
            meta_raw = class_match.group(3)
            current_class_meta = meta_raw.split("|") if meta_raw else []
            current_params = []

        param_match = RE_AFPARAM.search(line)
        if param_match and current_class_id:
            (
                var_type,
                var_name,
                default_raw,
                default_inner,
                label,
                direction,
                meta_raw
            ) = param_match.groups()

            default_str = normalize_default(default_raw, default_inner)
            param_meta = meta_raw.split("|") if meta_raw else []

            current_params.append((var_type, var_name, default_str, label, direction, param_meta))

    if current_class_id:
        result.append([
            (current_class_id, current_class_name, current_class_meta),
            current_params
        ])

    return result

# === ENUM SUPPORT ===
def extract_enums(filepath):
    """
    Return a list with a single dict (per file) containing:
      { "enum_name": <Name>, "values": [ "A", "B", ... ] }
    Only one AFENUM() per file is expected.
    """
    with open(filepath, 'r', encoding='utf-8') as f:
        lines = f.readlines()

    enums = []
    pending_afenum = False
    enum_name = None
    capturing = False   # inside { ... } of the enum
    waiting_for_brace = False
    enum_values = []

    for raw_line in lines:
        line_no_block = re.sub(r'/\*.*?\*/', '', raw_line)  # strip /* ... */ inline blocks (not multi-line-safe)
        line = line_no_block

        # 1) Look for AFENUM()
        if not pending_afenum and not enum_name and RE_AFENUM.search(line):
            pending_afenum = True
            # continue scanning this line; enum might also be declared here

        # 2) Once AFENUM() was seen, find 'enum class <Name>'
        if pending_afenum and not enum_name:
            m = RE_ENUM_CLASS.search(line)
            if m:
                enum_name = m.group(1)
                enum_values = []
                pending_afenum = False
                # Start capturing either immediately if '{' on same line, or wait for it
                if '{' in line:
                    capturing = True
                    waiting_for_brace = False
                else:
                    waiting_for_brace = True
                continue  # next line

        # 3) Wait for opening brace if needed
        if enum_name and waiting_for_brace and '{' in line:
            capturing = True
            waiting_for_brace = False
            # fallthrough to capture this line content too

        # 4) Capture values while inside enum body
        if capturing:
            # Check for closing brace first
            if '}' in line:
                # finalize this enum
                enums.append({
                    "enum_name": enum_name,
                    "values": enum_values
                })
                # single AFENUM per file -> we can stop early
                break

            # Remove line comments and whitespace
            clean = line.split('//', 1)[0].strip()
            if not clean:
                continue

            # Lines can include the opening brace; trim it off
            if clean.startswith('{'):
                clean = clean[1:].strip()
                if not clean:
                    continue

            # Split by commas because multiple entries could be on one line
            parts = [p.strip() for p in clean.split(',')]

            for part in parts:
                if not part:
                    continue
                # Remove assignments like "Name = 5" or "Name = SomeOtherEnum"
                name_only = part.split('=', 1)[0].strip()
                # Filter out stray braces or semicolons if they sneak in
                name_only = name_only.strip('{}; ')
                if name_only:
                    enum_values.append(name_only)

    return enums

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python script.py <headers_dir>")
        sys.exit(1)

    headers = fetch_headers(sys.argv[1])
    nodes = []
    enums = []

    for file in headers:
        class_param_pairs = extract_macros(file)
        file_enums = extract_enums(file)

        for (class_id, class_name, meta), params in class_param_pairs:
            node = {
                "class_id": class_id,
                "node_name": class_name,
                "meta": meta,
                "params": []
            }

            for var_type, var_name, default_str, label, direction, param_meta in params:
                node["params"].append({
                    "var_type": var_type,
                    "var_name": var_name,
                    "default": default_str,
                    "label": label,
                    "direction": direction,
                    "meta": param_meta
                })

            nodes.append(node)

        # One AFENUM per file; this returns [] or [ {enum_name, values} ]
        enums.extend(file_enums)

    output = {
        "Nodes": nodes,
        "Enums": enums
    }

    with open("graphManifest.json", "w", encoding="utf-8") as f:
        json.dump(output, f, indent=4)
