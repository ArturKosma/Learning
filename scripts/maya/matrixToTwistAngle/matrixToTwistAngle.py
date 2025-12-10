import maya.api.OpenMaya as om
import math

def maya_useNewAPI():
    pass

node_id = om.MTypeId(0x00127FFF)

# Decompose input quaternion into twist and swing.
def quat_twist_swing(q, twistAxis):    

    # Avoid division by zero.
    if twistAxis.length() < 1e-8:
        return om.MQuaternion(), q
    
    # Normalize the input twist axis.    
    twistAxis.normalize()
    
    # Project rotation axis onto twist axis.
    rotAxis = om.MVector(q.x, q.y, q.z)
    proj = twistAxis * (rotAxis * twistAxis) # Dot.
    
    # Build twist quaternion.
    twist = om.MQuaternion(proj.x, proj.y, proj.z, q.w)
    twist = twist.normal()
    
    # Swing is what remains.
    swing = q * twist.inverse()
    return twist, swing

class MatrixToTwistAngle(om.MPxNode):
    
    inputMat = None
    inputTwistVec = None
    outputTwistAngle = None
    
    @staticmethod
    def creator():
        return MatrixToTwistAngle()
        
    @staticmethod
    def initialize():
        
        # Create attributes.
        numAttr = om.MFnNumericAttribute()
        matAttr = om.MFnMatrixAttribute()
                
        # Input matrix.
        MatrixToTwistAngle.inputMat = matAttr.create(
            "inputMatrix", "inMat", om.MFnMatrixAttribute.kDouble
        )
        matAttr.keyable = True
        matAttr.storable = True
        matAttr.writable = True
        matAttr.readable = True
        
        # Input twist vector.
        MatrixToTwistAngle.inputTwistVec = numAttr.create(
            "inputTwistVector", "inTwVec", om.MFnNumericData.k3Float
        )
        numAttr.keyable = True
        numAttr.storable = True
        numAttr.writable = True
        numAttr.readable = True
         
        # Output.
        MatrixToTwistAngle.outputTwistAngle = numAttr.create(
            "output", "out", om.MFnNumericData.kFloat, 0.0
        )
        numAttr.keyable = False
        numAttr.storable = False
        
        # Add them to the node.
        MatrixToTwistAngle.addAttribute(MatrixToTwistAngle.inputMat)
        MatrixToTwistAngle.addAttribute(MatrixToTwistAngle.inputTwistVec)
        MatrixToTwistAngle.addAttribute(MatrixToTwistAngle.outputTwistAngle)
        
        # Make output depend on inputs.
        MatrixToTwistAngle.attributeAffects(MatrixToTwistAngle.inputMat, MatrixToTwistAngle.outputTwistAngle)
        MatrixToTwistAngle.attributeAffects(MatrixToTwistAngle.inputTwistVec, MatrixToTwistAngle.outputTwistAngle)
        
    def compute(self, plug, dataBlock):
        if plug != MatrixToTwistAngle.outputTwistAngle:
            return 
            
        # Fetch twist vector.   
        twistVecHandle = dataBlock.inputValue(MatrixToTwistAngle.inputTwistVec)   
        x, y, z = twistVecHandle.asFloat3()
        twistVec = om.MVector(x, y, z)
                    
        # Fetch quaternion from the input matrix.
        mat = dataBlock.inputValue(MatrixToTwistAngle.inputMat).asMatrix()
        tm = om.MTransformationMatrix(mat)
        quat = tm.rotation(asQuaternion=True)  
      
        # Get twist & swing from it.
        twist, swing = quat_twist_swing(quat, twistVec)
        
        # Get the twist angle.
        outHandle = dataBlock.outputValue(MatrixToTwistAngle.outputTwistAngle)
        radians = 2.0 * math.acos(twist.w)
        
        # Signed angle.
        axis = om.MVector(twist.x, twist.y, twist.z)
        sign = 1.0 if axis * twistVec >= 0.0 else -1.0
        radians *= sign
        
        # Wrap to [-pi, pi].
        if radians > math.pi:
            radians -= 2.0 * math.pi
        elif radians < -math.pi:
            radians += 2.0 * math.pi
        
        outHandle.setFloat(math.degrees(radians))
        outHandle.setClean()
        
def initializePlugin(mobject):
    pluginFn = om.MFnPlugin(mobject)
    pluginFn.registerNode(
        "matrixToTwistAngle",
        node_id,
        MatrixToTwistAngle.creator,
        MatrixToTwistAngle.initialize
    )
    
def uninitializePlugin(mobject):
    pluginFn = om.MFnPlugin(mobject)
    pluginFn.deregisterNode(node_id)