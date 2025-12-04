import maya.api.OpenMaya as om

def maya_useNewAPI():
    pass

node_id = om.MTypeId(0x00127FFF)

class MySimpleNode(om.MPxNode):
    
    inputA = None
    inputB = None
    output = None
    
    @staticmethod
    def creator():
        return MySimpleNode()
        
    @staticmethod
    def initialize():
        
        # Create attributes.
        numAttr = om.MFnNumericAttribute()
        
        # Input A.
        MySimpleNode.inputA = numAttr.create(
            "inputA", "a", om.MFnNumericData.kFloat, 0.0
        )
        numAttr.keyable = True
        numAttr.storable = True
       
        # Input B.
        MySimpleNode.inputB = numAttr.create(
            "inputB", "b", om.MFnNumericData.kFloat, 0.0
        )
        numAttr.keyable = True
        numAttr.storable = True
        
        # Output.
        MySimpleNode.output = numAttr.create(
            "output", "out", om.MFnNumericData.kFloat, 0.0
        )
        numAttr.keyable = False
        numAttr.storable = False
        
        # Add them to the node.
        MySimpleNode.addAttribute(MySimpleNode.inputA)
        MySimpleNode.addAttribute(MySimpleNode.inputB)
        MySimpleNode.addAttribute(MySimpleNode.output)
        
        # Make output depend on inputs.
        MySimpleNode.attributeAffects(MySimpleNode.inputA, MySimpleNode.output)
        MySimpleNode.attributeAffects(MySimpleNode.inputB, MySimpleNode.output)
        
    def compute(self, plug, dataBlock):
        if plug != MySimpleNode.output:
            return
            
        a = dataBlock.inputValue(MySimpleNode.inputA).asFloat()
        b = dataBlock.inputValue(MySimpleNode.inputB).asFloat()
        
        result = a + b
        
        outHandle = dataBlock.outputValue(MySimpleNode.output)
        outHandle.setFloat(result)
        outHandle.setClean()
        
def initializePlugin(mobject):
    pluginFn = om.MFnPlugin(mobject)
    pluginFn.registerNode(
        "mySimpleNode",
        node_id,
        MySimpleNode.creator,
        MySimpleNode.initialize
    )
    
def uninitializePlugin(mobject):
    pluginFn = om.MFnPlugin(mobject)
    pluginFn.deregisterNode(node_id)