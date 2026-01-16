# Maya Python: Tag selected controls as IK or FK
# 1) Pops up a window asking IK or FK
# 2) Sets Outliner color of selected nodes (IK=red, FK=yellow)
# 3) Adds enum attribute "ControlType" and sets it

import maya.cmds as cmds

WINDOW_NAME = "ikFkControlTypeTaggerWin"

# Colors are RGB floats 0..1
IK_COLOR = (1.0, 0.0, 0.0)   # red
FK_COLOR = (1.0, 1.0, 0.0)   # yellow

ENUM_ATTR = "ControlType"
ENUM_VALUES = "IK:FK"        # stored as indices 0=IK, 1=FK

def _safe_selection():
    sel = cmds.ls(sl=True, long=True) or []
    # Filter to DAG nodes (transforms/shapes etc.) — outliner color is per-DAG object.
    # If user selected a shape, we can still color it, but typical rigs select transforms.
    return sel

def _ensure_enum_attr(node):
    if not cmds.attributeQuery(ENUM_ATTR, node=node, exists=True):
        try:
            cmds.addAttr(node, longName=ENUM_ATTR, attributeType="enum", enumName=ENUM_VALUES, keyable=True)
        except Exception as e:
            cmds.warning("Could not add attribute {} to {}: {}".format(ENUM_ATTR, node, e))
            return False
    else:
        # If it exists but isn't enum, warn and skip
        atype = cmds.getAttr("{}.{}".format(node, ENUM_ATTR), type=True)
        if atype != "enum":
            cmds.warning("Attribute {} exists on {} but is type '{}', not enum. Skipping.".format(ENUM_ATTR, node, atype))
            return False
    return True

def _set_outliner_color(node, rgb):
    # Enable outliner color override and set the color
    try:
        if cmds.attributeQuery("useOutlinerColor", node=node, exists=True):
            cmds.setAttr("{}.useOutlinerColor".format(node), 1)
        else:
            # Some non-DAG nodes won't have it
            cmds.warning("{} has no useOutlinerColor. Skipping color.".format(node))
            return

        cmds.setAttr("{}.outlinerColor".format(node), rgb[0], rgb[1], rgb[2], type="double3")
    except Exception as e:
        cmds.warning("Could not set outliner color on {}: {}".format(node, e))

def _apply_tag(control_type):
    """
    control_type: "IK" or "FK"
    """
    sel = _safe_selection()
    if not sel:
        cmds.warning("Nothing selected. Select your controls first.")
        return

    if control_type == "IK":
        rgb = IK_COLOR
        enum_index = 0
    elif control_type == "FK":
        rgb = FK_COLOR
        enum_index = 1
    else:
        cmds.warning("Unknown control type: {}".format(control_type))
        return

    for node in sel:
        # Add/set enum attribute
        if _ensure_enum_attr(node):
            try:
                cmds.setAttr("{}.{}".format(node, ENUM_ATTR), enum_index)
            except Exception as e:
                cmds.warning("Could not set {} on {}: {}".format(ENUM_ATTR, node, e))

        # Outliner color
        _set_outliner_color(node, rgb)

def _on_click_ik(*_):
    _apply_tag("IK")
    if cmds.window(WINDOW_NAME, exists=True):
        cmds.deleteUI(WINDOW_NAME)

def _on_click_fk(*_):
    _apply_tag("FK")
    if cmds.window(WINDOW_NAME, exists=True):
        cmds.deleteUI(WINDOW_NAME)

def show_ik_fk_picker():
    # Make sure we capture selection *now* (and warn early), but still allow user to continue
    sel = _safe_selection()
    if not sel:
        cmds.warning("No selection detected. You can still open the window, but nothing will be tagged until you select nodes.")
    else:
        # Nice info in script editor
        cmds.inViewMessage(amg="Tagging <hl>{}</hl> selected node(s)".format(len(sel)),
                           pos="topCenter", fade=True)

    if cmds.window(WINDOW_NAME, exists=True):
        cmds.deleteUI(WINDOW_NAME)

    cmds.window(WINDOW_NAME, title="Selected Controls: IK or FK?", sizeable=False)
    cmds.columnLayout(adjustableColumn=True, rowSpacing=10, columnAlign="center", columnAttach=("both", 12))

    cmds.text(label="Choose how to tag the CURRENT selection:", align="center")
    cmds.separator(height=8, style="in")

    cmds.rowLayout(numberOfColumns=2, columnWidth2=(150, 150), columnAlign2=("center", "center"), columnAttach=[(1,"both",0),(2,"both",0)])
    cmds.button(label="IK (Red)", height=36, command=_on_click_ik)
    cmds.button(label="FK (Yellow)", height=36, command=_on_click_fk)
    cmds.setParent("..")

    cmds.separator(height=8, style="in")
    cmds.text(label="This will:\n• Set Outliner color\n• Add/Set enum attr: ControlType = IK/FK", align="center")

    cmds.showWindow(WINDOW_NAME)

# Run:
show_ik_fk_picker()
