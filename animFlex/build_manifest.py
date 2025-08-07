import sys
import os
import re
import json

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

def fetch_headers(directory):
    if not os.path.isdir(directory):
        return []
    return [
        os.path.join(directory, item)
        for item in os.listdir(directory)
        if os.path.isfile(os.path.join(directory, item)) and item.endswith(".h")
    ]

def normalize_default(default_raw: str, default_inner: str | None) -> str:
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

if __name__ == "__main__":
    headers = fetch_headers(sys.argv[1])
    nodes = []

    for file in headers:
        class_param_pairs = extract_macros(file)

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

    outputNodes = { "Nodes": nodes }

    with open("graphManifest.json", "w", encoding="utf-8") as f:
        json.dump(outputNodes, f, indent=4)
