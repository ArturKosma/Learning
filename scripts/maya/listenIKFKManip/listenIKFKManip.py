import maya.cmds as cmds

try:
    from PySide2 import QtCore, QtWidgets
    import shiboken2 as shiboken
except Exception:
    from PySide6 import QtCore, QtWidgets
    import shiboken6 as shiboken

import maya.OpenMayaUI as omui

CONTROL_TYPE_ATTR = "ControlType"

_installed = []   # list of (widget, filter)
_last_tr = {}


def _tool_kind():
    ctx = (cmds.currentCtx() or "").lower()
    if "move" in ctx:
        return "move"
    if "rotate" in ctx:
        return "rot"
    return None


def _get_control_type_label(node_long: str):
    plug = f"{node_long}.{CONTROL_TYPE_ATTR}"
    if not cmds.objExists(plug):
        return None
    try:
        if cmds.getAttr(plug, type=True) != "enum":
            return None
        return cmds.getAttr(plug, asString=True)
    except Exception:
        return None


def _get_tr(node_long: str):
    try:
        t = cmds.getAttr(node_long + ".translate")[0]
        r = cmds.getAttr(node_long + ".rotate")[0]
        return (t[0], t[1], t[2], r[0], r[1], r[2])
    except Exception:
        return None


def _wrap_qwidget_from_name(qt_name: str):
    ptr = omui.MQtUtil.findControl(qt_name)
    if ptr is None:
        ptr = omui.MQtUtil.findLayout(qt_name)
    if ptr is None:
        return None
    return shiboken.wrapInstance(int(ptr), QtWidgets.QWidget)


class _ViewportEventFilter(QtCore.QObject):
    def eventFilter(self, obj, event):
        if event.type() == QtCore.QEvent.MouseMove:
            # SANITY: if you never see this, you’re still not attached to the right widget
            # print("mouse move")  # uncomment for debugging

            if not (event.buttons() & QtCore.Qt.LeftButton):
                return False

            # If this blocks you (custom tool ctx), comment it out temporarily
            if _tool_kind() is None:
                return False

            sel = cmds.ls(sl=True, l=True) or []
            if not sel:
                return False

            node = sel[0]
            label = _get_control_type_label(node)
            if label is None:
                return False

            tr = _get_tr(node)
            if tr is None:
                return False

            prev = _last_tr.get(node)
            _last_tr[node] = tr
            if prev is None or tr != prev:
                print(label)
                if label == "IK":
                    _fix_controls("FK")

        return False


def _fix_controls(_type):
    if _type != "FK":
        return

    src = "LeftForeArm"
    dst = "ctrl_LeftForeArm_FK_offset"
    if not cmds.objExists(src) or not cmds.objExists(dst):
        return

    pos = cmds.xform(src, q=True, ws=True, t=True)
    rot = cmds.xform(src, q=True, ws=True, ro=True)
    cmds.xform(dst, ws=True, t=pos, ro=rot)


def uninstall():
    global _installed, _last_tr
    for w, flt in _installed:
        try:
            w.removeEventFilter(flt)
        except Exception:
            pass
    _installed = []
    _last_tr = {}
    print("uninstalled")


def install():
    uninstall()

    panels = cmds.getPanel(type="modelPanel") or []
    if not panels:
        cmds.warning("No modelPanel viewports found.")
        return

    for p in panels:
        # ✅ IMPORTANT: get the *viewport control* of the modelPanel
        try:
            view_ctl = cmds.modelPanel(p, q=True, control=True)
        except Exception:
            continue

        w = _wrap_qwidget_from_name(view_ctl)
        if w is None:
            cmds.warning(f"Could not wrap QWidget for viewport control: {p}")
            continue

        flt = _ViewportEventFilter()
        w.installEventFilter(flt)
        _installed.append((w, flt))

    print("installed on:", ", ".join(panels))


install()
