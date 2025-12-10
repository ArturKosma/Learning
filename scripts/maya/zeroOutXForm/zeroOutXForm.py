import maya.cmds as cmds

def zeroOut(*args):
    selection = cmds.ls(sl=True)
    for selected in selection:
        cmds.xform(selected, t=[0.0, 0.0, 0.0], ro=[0.0, 0.0, 0.0], s=[1.0, 1.0, 1.0])

window_id = "zero_out"
if cmds.window(window_id, exists=True):
    cmds.deleteUI(window_id)

# Create the window.
window = cmds.window(window_id, title="Zero-Out")

# Create form.
cmds.formLayout("main_form")

# Resize the window.
cmds.window(window_id, edit=True, widthHeight=(100, 100), toolbox=True)

# Create layout inside the window.
clayout = cmds.columnLayout(columnAlign="center")

# Create a button inside the layout.
cmds.button(label="Press", command=zeroOut)

# Attach the layout to the form in the center.
# TODO shift by half wight and height, not hardcoded 25
cmds.formLayout(
    "main_form", edit=True,
    attachPosition=[(clayout, 'left', -25, 50), (clayout, 'top', -25, 50)]
)

cmds.showWindow(window_id)