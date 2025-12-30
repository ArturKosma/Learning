import sys
import importlib
sys.path.append(r"C:\_programming\Learning\scripts\maya")

import cgtools.main
importlib.reload(cgtools.main)

cgtools = main.CGTools()