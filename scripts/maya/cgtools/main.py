import maya.cmds as cmds
from . import options_tab

class CGTools:

    STORE_NODE = "cgToolsData"
    STORE_ATTR = "correctiveJoints"

    def __init__(self):

        # Delete existing window.
        if cmds.window("cgToolsWin", exists=True):
            cmds.deleteUI("cgToolsWin")

        # Create window.
        self.win = cmds.window("cgToolsWin", title="CGTools", widthHeight=(400, 500))

        # Create tabs layout.
        self.tabs = cmds.tabLayout(innerMarginWidth=5, innerMarginHeight=5)

        # Create options tab.
        self.options_tab = options_tab.OptionsTab(parent=self, tabs=self.tabs)

        # Build tabs.
        self.buildTabs()

        # Show window.
        cmds.showWindow("cgToolsWin")

    def buildTabs(self):
        export_root = self.exportTab()
        options_root = self.options_tab.build()

        cmds.tabLayout(self.tabs, e=True, tabLabel=((export_root, "Export"),
                                                   (options_root, "Options")))

    def exportTab(self):
        col = cmds.columnLayout(adj=True, parent=self.tabs)
        cmds.button(label="Export")
        return col
    
    # Helper function which makes sure we have a network node which contains/will contain our saved data,
    # and returns attribute specified by string, to be filled/read by the querying subsystem.
    def ensureStore(self, attribute):
        if not cmds.objExists(self.STORE_NODE):
            cmds.createNode("network", name=self.STORE_NODE)

        attr = f"{self.STORE_NODE}.{attribute}"
        if not cmds.objExists(attr):
            cmds.addAttr(self.STORE_NODE, ln=attribute, dt="string")
            cmds.setAttr(attr, "", type="string")
        return attr