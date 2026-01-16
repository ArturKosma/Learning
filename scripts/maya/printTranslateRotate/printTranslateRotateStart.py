import maya.cmds as cmds
from functools import partial

jobs, started = [], False

def reset(*_):
    global started
    started = False

def fire(kind, *_):
    global started
    if started: return
    ctx = (cmds.currentCtx() or "").lower()
    if (kind == "move" and "move" not in ctx) or (kind == "rot" and "rotate" not in ctx): return
    started = True
    print(cmds.ls(sl=True, l=True), "->", "location" if kind=="move" else "rotation")

def watch():
    global jobs
    for j in jobs:
        if cmds.scriptJob(exists=j): cmds.scriptJob(kill=j, force=True)
    jobs = [cmds.scriptJob(event=["DragRelease", reset], protected=True)]
    sel = cmds.ls(sl=True, l=True) or []
    for o in sel:
        for a in "XYZ":
            jobs += [
                cmds.scriptJob(attributeChange=[f"{o}.translate{a}", partial(fire,"move")], protected=True),
                cmds.scriptJob(attributeChange=[f"{o}.rotate{a}",    partial(fire,"rot")],  protected=True),
            ]

watch()
