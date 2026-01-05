import maya.cmds as cmds
import maya.mel as mel

sel = cmds.ls(sl=True, long=True)

if len(sel) < 2:
    cmds.warning("Select source transforms, then target transform last.")
    raise RuntimeError

target = sel[-1]
sources = sel[:-1]

# Freeze all selected transforms. Non frozen is breaking the shapes during "parent -s -r".
for obj in sel:
    if cmds.nodeType(obj) == "transform":
        cmds.makeIdentity(obj, apply=True, t=True, r=True, s=True)

# Collect shapes from sources.
shapes = []
for src in sources:
    if cmds.nodeType(src) != "transform":
        continue
    sh = cmds.listRelatives(src, s=True, ni=True, f=True) or []
    shapes.extend(sh)

# Select shapes, then target.
cmds.select(clear=True)
cmds.select(shapes, add=True)
cmds.select(target, add=True)

# Parent the shapes relatively to target.
mel.eval("parent -s -r")

# Delete source transforms.
for src in sources:
    if cmds.objExists(src):
        cmds.delete(src)
