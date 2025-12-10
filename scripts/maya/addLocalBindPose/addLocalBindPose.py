import maya.cmds as cmds
import maya.api.OpenMaya as om
import math

# Select all joints.
cmds.select(cmds.ls(type="joint"))
joints = cmds.ls(sl=True, type="joint")

# For each joint.
for joint in joints:
    
    # Remove the attribute if exists.
    if cmds.attributeQuery("Local_Bind_Matrix", node=joint, exists=True):
        cmds.deleteAttr(f"{joint}.Local_Bind_Matrix")
    
    # Get the local matrix.
    flatM = cmds.xform(joint, q=True, m=True, os=True)
    
    # Add a matrix attribute.
    cmds.addAttr(joint, ln="Local_Bind_Matrix", at="matrix")
    
    # Fill the matrix attribute.
    cmds.setAttr(joint + ".Local_Bind_Matrix", *flatM, type="matrix")
    
    # Show in channel box
    # WON'T WORK - you can't see a matrix in channel box. Go to Attribute Editor under Extra Attributes.
    # cmds.setAttr(joint + ".Local_Bind_Matrix", cb=True)
    cmds.setAttr(joint + ".Local_Bind_Matrix", e=True, k=True)
    cmds.setAttr(joint + ".Local_Bind_Matrix", cb=True)
    cmds.setAttr(joint + ".Local_Bind_Matrix", e=True, channelBox=True)
    print(cmds.listAttr(joint, string="Local_Bind_Matrix"))
    
    
    
    