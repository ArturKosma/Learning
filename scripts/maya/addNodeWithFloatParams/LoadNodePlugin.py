# Import Maya commands module.
import maya.cmds as cmds

# Load custom plugin MyCustomNode.
# MyCustomNode has to be in plugin search path, 
# usually Documents\maya\2026\plug-ins.
cmds.loadPlugin("MyCustomNode.py")