import sys
import os
import re
import json

# Regex patterns.
RE_AFCLASS = re.compile(r'\bAFCLASS\s*\(\s*([a-zA-Z_][\w:]*)\s*,\s*"([^"]+)"\s*\)')
RE_AFPARAM = re.compile(
    r'\bAFPARAM\s*\(\s*([a-zA-Z_][\w:]*)\s*,\s*([a-zA-Z_][\w:]*)\s*,\s*"([^"]*)"\s*,\s*"([^"]*)"(?:\s*,\s*"([^"]*)")?\s*\)'
)

# Get all .h files from given directory.
def fetch_headers(directory):
    if not os.path.isdir(directory):
        return

    header_files = []
    for item in os.listdir(directory):
        full_path = os.path.join(directory, item)
        if os.path.isfile(full_path) and item.endswith(".h"):
            header_files.append(full_path)

    return header_files

# Macro extraction.
def extract_macros(filepath):
    with open(filepath, 'r', encoding='utf-8') as f:
        lines = f.readlines()

    result = []
    current_class_id = None
    current_params = {}
    class_def_stack = []

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
            params = current_params.get(class_id, [])
            result.append([(class_id, class_name), params])

            if class_id in current_params:
                del current_params[class_id]

    return result

# Main.
if __name__ == "__main__":

    # Get all headers.
    headers = fetch_headers(sys.argv[1])

    # JSON container.
    nodes = []

    # For each header.
    for file in headers:

         # Get all macros defining classes and params.
        class_param_pairs = extract_macros(file)

        for (class_id, class_name), params in class_param_pairs:
            node = {
                    "class_id": class_id,
                    "node_name": class_name,
                    "params": []
                }

            for var_type, var_name, label, direction, meta in params:
                node["params"].append({
                            "var_type": var_type,
                            "var_name": var_name,
                            "label": label,
                            "direction": direction,
                            "meta": meta
                        })

            nodes.append(node)
    
    # Wrap nodes.
    outputNodes = {
        "Nodes": nodes
    }

    # Write to JSON file.
    with open("graphManifest.json", "w", encoding="utf-8") as f:
        json.dump(outputNodes, f, indent=4)

        
                
                


        