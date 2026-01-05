import maya.cmds as cmds
import maya.api.OpenMaya as om

def getScale(object):
    
    m = om.MMatrix(cmds.xform(object, q=True, ws=True, m=True))
    tm = om.MTransformationMatrix(m)
    s = tm.scale(om.MSpace.kWorld)
    return s[0], s[1], s[2]

def bakeScaleIntoShapes(ctrl, sx, sy, sz):

    # Scale shape in object space so visual size stays the same.
    shapes = cmds.listRelatives(ctrl, s=True, ni=True, f=True) or []
    if not shapes:
        return

    # Scale shapes in object space.
    for sh in shapes:
        try:
            cmds.scale(sx, sy, sz, sh + ".cv[*]", r=True, os=True)
        except Exception:
            pass

def freezeWithBakedAxis(object):

    # Cache current world scale so we can bake it later.
    sx, sy, sz = getScale(object)

    # Cache world transformation.
    wm = om.MMatrix(cmds.xform(object, q=True, ws=True, m=True))

    # Create offset parent matrix.
    parent = cmds.listRelatives(object, p=True, f=True)
    if parent:
        p_inv = om.MMatrix(cmds.getAttr(parent[0] + ".worldInverseMatrix[0]"))
        m = wm * p_inv
    else:
        m = wm

    # Get translation and rotation from the offset parent matrix.
    mt = om.MTransformationMatrix(m)
    t = mt.translation(om.MSpace.kWorld)
    q = mt.rotation(asQuaternion=True)

    # Create a clean version without scale.
    clean = om.MTransformationMatrix()
    clean.setTranslation(t, om.MSpace.kWorld)
    clean.setRotation(q)

    # Set new offset parent matrix.
    cmds.setAttr(object + ".offsetParentMatrix", list(clean.asMatrix()), type="matrix")

    # Bake scale into the shape to retain same visuals.
    bakeScaleIntoShapes(object, sx, sy, sz)

    # Zero-out the TRS components.
    cmds.setAttr(object + ".translate", 0, 0, 0)
    cmds.setAttr(object + ".rotate", 0, 0, 0)
    cmds.setAttr(object + ".scale", 1, 1, 1)

    # Zero-out rotate axis just in case.
    if cmds.attributeQuery("rotateAxis", node=object, exists=True):
        cmds.setAttr(object + ".rotateAxis", 0, 0, 0)

for selected in cmds.ls(sl=True, long=True) or []:

    freezeWithBakedAxis(selected)