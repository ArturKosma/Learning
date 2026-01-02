import maya.cmds as cmds

class OptionsTab:
    
    ADD_WIN = "cgToolsAddJointWin"
    RENAME_WIN = "cgToolsRenameWin" 

    def __init__(self, parent, tabs):
        
        self.parent = parent # CGTools.
        self.tabs = tabs

        self.joints_container = None
        self.add_joint_field = None
        self.rename_index = None

    def build(self):

        # Root which pivots to the upper left.
        root = cmds.columnLayout(adjustableColumn=True, parent=self.tabs)

        # Correctives section.
        correctives_layout = cmds.frameLayout(
            label="Correctives",
            collapsable=True,
            marginWidth=6,
            marginHeight=6,
            parent=root
        )

        inner_layout = cmds.rowLayout(
            numberOfColumns=2,
            adjustableColumn=1,
            columnWidth2=(260, 120),
            columnAttach=[(1, "both", 2), (2, "both", 2)],
            parent=correctives_layout
        )

        self.joints_container = cmds.textScrollList(
            parent=inner_layout,
            allowMultiSelection=True
        )

        # Load saved data if there's any.
        self.load()

        buttons = cmds.columnLayout(adjustableColumn=True, parent=inner_layout)
        cmds.button(label="Add", command=self.openAddJointWindow)
        cmds.button(label="Rename", command=self.renameSelected)
        cmds.button(label="Remove", command=self.removeSelected)

        return root

    # Window for adding corrective joints.
    def openAddJointWindow(self, *_):
        if cmds.window(self.ADD_WIN, exists=True):
            cmds.showWindow(self.ADD_WIN)
            return

        win = cmds.window(self.ADD_WIN, title="Add corrective joint")
        cmds.columnLayout(adj=True)

        self.add_joint_field = cmds.textField(placeholderText="Joint name")

        cmds.rowLayout(numberOfColumns=2)
        cmds.button(label="Add", command=self.confirm_add_joint)
        cmds.button(label="Add From Selection", command=self.add_from_selection)

        cmds.showWindow(win)

    def add_from_selection(self, *_):
        sel = cmds.ls(sl=True, type="joint") or []
        if not sel:
            cmds.warning("Select a joint first.")
            return

        existing = cmds.textScrollList(self.joints_container, q=True, allItems=True) or []
        for joint in sel:
            if joint not in existing:
                cmds.textScrollList(self.joints_container, e=True, append=joint)

        self.save()

    def confirm_add_joint(self, *_):

        name = cmds.textField(self.add_joint_field, q=True, text=True).strip()
        if not name:
            cmds.warning("Please enter a joint name.")
            return

        existing = cmds.textScrollList(self.joints_container, q=True, allItems=True) or []
        if name in existing:
            cmds.warning('"{}" is already in the list.'.format(name))
            return

        cmds.textScrollList(self.joints_container, e=True, append=name)

        self.save()

    def renameSelected(self, *_):
        idxs = cmds.textScrollList(self.joints_container, q=True, selectIndexedItem=True) or []
        if not idxs:
            cmds.warning("Select an item.")
            return

        self.rename_index = idxs[0]
        old = (cmds.textScrollList(self.joints_container, q=True, selectItem=True) or [""])[0]

        # Delete existing rename window BEFORE creating a new one
        if cmds.window(self.RENAME_WIN, exists=True):
            cmds.deleteUI(self.RENAME_WIN)

        w = max(300, len(old) * 8 + 80)

        win = cmds.window(self.RENAME_WIN, title="Rename", widthHeight=(w, 80), sizeable=True)
        cmds.columnLayout(adj=True, rowSpacing=6)

        field = cmds.textField(text=old)
        cmds.button(label="OK", command=lambda *_: self.confirmRename(old, field))

        cmds.showWindow(win)

    def confirmRename(self, old, field):
        new = cmds.textField(field, q=True, text=True).strip()
        if not new:
            return

        items = cmds.textScrollList(self.joints_container, q=True, allItems=True) or []
        if new in items and new != old:
            cmds.warning('"{}" is already in the list.'.format(new))
            return

        cmds.textScrollList(self.joints_container, e=True, removeItem=old)

        # Make sure we add the renamed item in the same position.
        cmds.textScrollList(self.joints_container, e=True, appendPosition=(self.rename_index, new))

        cmds.textScrollList(self.joints_container, e=True, deselectAll=True)
        cmds.textScrollList(self.joints_container, e=True, selectIndexedItem=self.rename_index)

        self.save()

        cmds.deleteUI(self.RENAME_WIN)

    def removeSelected(self, *_):
        idxs = cmds.textScrollList(self.joints_container, q=True, selectIndexedItem=True) or []
        if not idxs:
            cmds.warning("Select item(s) to remove.")
            return

        items = cmds.textScrollList(self.joints_container, q=True, allItems=True) or []
        for i in sorted(idxs, reverse=True):
            if 1 <= i <= len(items):
                cmds.textScrollList(self.joints_container, e=True, removeIndexedItem=i)
                items.pop(i - 1)

        self.save()

    def save(self):
        attr = self.parent.ensureStore(attribute="correctiveJoints")
        items = cmds.textScrollList(self.joints_container, q=True, allItems=True) or []
        cmds.setAttr(attr, "|".join(items), type="string")

    def load(self):
        attr = self.parent.ensureStore(attribute="correctiveJoints")
        data = cmds.getAttr(attr) or ""
        items = [s for s in data.split("|") if s]
        cmds.textScrollList(self.joints_container, e=True, removeAll=True)
        if items:
            cmds.textScrollList(self.joints_container, e=True, append=items)