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
    current_params = {}

    for line in lines:
        param_match = RE_AFPARAM.search(line)
        if param_match and current_class_id:
            var_type, var_name, label, direction, meta_raw = param_match.groups()
            meta = meta_raw.split("|") if meta_raw else []
            current_params.setdefault(current_class_id, []).append(
                (var_type, var_name, label, direction, meta)
            )

        class_decl = re.match(r'\s*class\s+([a-zA-Z_][\w]*)', line)
        if class_decl:
            current_class_id = class_decl.group(1)

        class_match = RE_AFCLASS.search(line)
        if class_match:
            class_id = class_match.group(1)
            class_name = class_match.group(2)
            meta_raw = class_match.group(3)
            meta = meta_raw.split("|") if meta_raw else []

            params = current_params.get(class_id, [])
            result.append([(class_id, class_name, meta), params])

            if class_id in current_params:
                del current_params[class_id]

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