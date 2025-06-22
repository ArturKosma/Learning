import sys
import os
import re
import json

RE_AFCLASS = re.compile(
    r'\bAFCLASS\s*\(\s*([a-zA-Z_][\w:]*)\s*,\s*"([^"]+)"(?:\s*,\s*"([^"]*)")?\s*\)'
)
RE_AFPARAM = re.compile(
    r'\bAFPARAM\s*\(\s*([a-zA-Z_][\w:]*)\s*,\s*([a-zA-Z_][\w:]*)\s*,\s*"([^"]*)"\s*,\s*"([^"]*)"(?:\s*,\s*"([^"]*)")?\s*\)'
)

def fetch_headers(directory):
    if not os.path.isdir(directory):
        return []

    return [
        os.path.join(directory, item)
        for item in os.listdir(directory)
        if os.path.isfile(os.path.join(directory, item)) and item.endswith(".h")
    ]

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
            # If we were processing a class, save it before starting a new one
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
            var_type, var_name, label, direction, meta_raw = param_match.groups()
            param_meta = meta_raw.split("|") if meta_raw else []
            current_params.append((var_type, var_name, label, direction, param_meta))

    # Final class at end of file
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

            for var_type, var_name, label, direction, param_meta in params:
                node["params"].append({
                    "var_type": var_type,
                    "var_name": var_name,
                    "label": label,
                    "direction": direction,
                    "meta": param_meta
                })

            nodes.append(node)

    outputNodes = {
        "Nodes": nodes
    }

    with open("graphManifest.json", "w", encoding="utf-8") as f:
        json.dump(outputNodes, f, indent=4)