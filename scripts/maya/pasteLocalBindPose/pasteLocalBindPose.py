import maya.cmds as cmds
import maya.api.OpenMaya as om

# Get list of all selected nodes.
joints = cmds.ls(sl=True, type="joint")

# For all selected joints.
for joint in joints:

    # If the local bind matrix attribute exists.
    if cmds.attributeQuery("Local_Bind_Matrix", node=joint, exists=True):
        
        # Cache the matrix as 16 floats.
        mat_list = cmds.getAttr(joint + ".Local_Bind_Matrix")

        # Convert it to MMatrix.
        mat_m = om.MMatrix(mat_list)

        # Wrap it inside MTransformationMatrix to decompose it.
        mat = om.MTransformationMatrix(mat_m)

        # Decompose it.
        translation = mat.translation(om.MSpace.kObject)
        rotation = mat.rotation()
        scale = mat.scale(om.MSpace.kObject)

        # Convert rotation to degrees.
        rotation_deg = [om.MAngle(i).asDegrees() for i in (rotation.x, rotation.y, rotation.z)]

        # Write transformation to the joint.
        cmds.xform(joint,
                   t=[translation.x, translation.y, translation.z],
                   ro=rotation_deg,
                   s=scale)
