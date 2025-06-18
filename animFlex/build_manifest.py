import sys
import os
import re
import json

# Regex patterns.
RE_AFCLASS = re.compile(r'\bAFCLASS\s*\(\s*([a-zA-Z_][\w:]*)\s*,\s*"([^"]+)"\s*\)')
RE_AFPARAM = re.compile(r'\bAFPARAM\s*\(\s*([a-zA-Z_][\w:]*)\s*,\s*([a-zA-Z_][\w:]*)\s*,\s*"([^"]*)"\s*,\s*"([^"]*)"\s*\)')

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
    current_class_name = None
    current_params = []

    for line in lines:
        class_match = RE_AFCLASS.search(line)
        if class_match:

            if current_class_id:
                result.append([(current_class_id, current_class_name), current_params])

            current_class_id = class_match.group(1)
            current_class_name = class_match.group(2)
            current_params = []
            continue

        param_match = RE_AFPARAM.search(line)
        if param_match and current_class_id:
            current_params.append(param_match.groups())

    if current_class_id:
        result.append([(current_class_id, current_class_name), current_params])

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

            for var_type, var_name, label, direction in params:
                node["params"].append({
                            "var_type": var_type,
                            "var_name": var_name,
                            "label": label,
                            "direction": direction
                        })

            nodes.append(node)
    
    # Wrap nodes.
    outputNodes = {
        "Nodes": nodes
    }

    # Write to JSON file.
    with open("graphManifest.json", "w", encoding="utf-8") as f:
        json.dump(outputNodes, f, indent=4)

        
                
                


        