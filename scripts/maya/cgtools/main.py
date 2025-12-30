import maya.cmds as cmds

class CGTools:
    def __init__(self):

        # Delete existing window.
        if cmds.window("cgToolsWin", exists=True):
            cmds.deleteUI("cgToolsWin")

        # Create window & tabs.
        self.win = cmds.window("cgToolsWin", title="CGTools", widthHeight=(400, 500))
        self.tabs = cmds.tabLayout(innerMarginWidth=5, innerMarginHeight=5)

        # Build tabs.
        self.buildTabs()

        # Show window.
        cmds.showWindow("cgToolsWin")

    def buildTabs(self):
        self.exportTab()
        self.optionsTab()

    def exportTab(self):
        col = cmds.columnLayout(adj=True)
        cmds.button(label="Export")
        cmds.setParent("..")
        cmds.tabLayout(self.tabs, edit=True, tabLabel=(col, "Export"))

    def optionsTab(self):
        col = cmds.columnLayout(adj=True)
        cmds.button(label="Options")
        cmds.setParent("..")
        cmds.tabLayout(self.tabs, edit=True, tabLabel=(col, "Options"))
        