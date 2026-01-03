import maya.cmds as cmds

class ExportTab:

    def __init__(self, parent, tabs):

        self.parent = parent # CGTools.
        self.tabs = tabs

    def build(self):

        # Root which pivots to the upper left.
        self.root = cmds.formLayout(parent=self.tabs)

        # Button to select directory.
        self.export_button = cmds.button(label="Export", width=70, height=60, parent=self.root, command=lambda *_: self.export())
        
        # Directory label.
        self.directory_label = cmds.text(label="Directory:", align="right", parent=self.root)

        # Directory text field.
        self.directory_text = cmds.textField(
            parent=self.root,
            placeholderText="Choose an export directory"
        )

        # Directory browse button.
        self.directory_browse = cmds.iconTextButton(
            parent=self.root,
            style="iconOnly",
            image1="fileOpen.png",
            annotation="Browse for directory.",
            command=lambda *_: self.directoryButton(directoryText=self.directory_text)
        )

        # Filename label.
        self.filename_label = cmds.text(label="Filename:", align="right", parent=self.root)

        # Filename text field.
        self.filename_text = cmds.textField(
            parent=self.root,
            placeholderText="Choose filename",
            tcc=lambda *_: self.updateResultingText()
        )

        # Resulting path + filename + extension.
        self.resulting_text = cmds.text(label="", align="right", parent=self.root, enable=False)

        # Specify layout.
        cmds.formLayout(
            self.root, e=True,
            attachForm=[
                (self.export_button, "left", 2), (self.export_button, "top", 2),
                (self.directory_label, "top", 2),
                (self.directory_browse, "top", 2), (self.directory_browse, "right", 2),
                (self.filename_text, "right", 2)
            ],
            attachControl=[
                (self.directory_label, "left", 10, self.export_button),
                (self.directory_text, "left", 10, self.directory_label), (self.directory_text, "right", 2, self.directory_browse), 
                (self.filename_label, "left", 10, self.export_button), (self.filename_label, "top", 5, self.directory_label),
                (self.filename_text, "top", 0, self.directory_text), (self.filename_text, "left", 10, self.filename_label),
                (self.resulting_text, "left", 10, self.filename_label), (self.resulting_text, "top", 5, self.filename_text),
            ]
        )

        self.load()

        return self.root
    
    # Function called upon directory selection.
    def directoryButton(self, directoryText, *_):

        result = cmds.fileDialog2(
            dialogStyle=2,
            fileMode=3,
            caption="Select Output Folder"
        )

        if result:
            directory_path = result[0]
            cmds.textField(directoryText, e=True, text=directory_path)

        self.updateResultingText()
        self.save()

    # Function updating the grey resulting full path text.
    def updateResultingText(self, *_):

        directory = cmds.textField(self.directory_text, q=True, text=True).strip()
        filename  = cmds.textField(self.filename_text,  q=True, text=True).strip()

        if directory and filename:

            directory = directory.rstrip("/\\")
            cmds.text(
                self.resulting_text,
                e=True,
                label=f"{directory}/{filename}.fbx"
            )
        else:
            cmds.text(self.resulting_text, e=True, label="")

        self.save()

    # Export function.
    # Bakes and exports selected joints, without names specified by correctives in options.
    def export(self, *_):

        # Get all selected joints.
        joints = cmds.ls(sl=True, type="joint")

        # Bake them.
        cmds.bakeResults(joints, t=(cmds.playbackOptions(q=True, min=True),
                                cmds.playbackOptions(q=True, max=True))) 

        # Get all corrective joints.
        correctives = self.getCorrectives()

        # Remove all keyframes from correctives.
        cmds.cutKey(correctives, clear=True)

        # Export.
        cmds.file(cmds.text(self.resulting_text, q=True, label=True),
                  force=True,
                  type="FBX Export",
                  pr=True,
                  es=True)

    def save(self):
        attrDirectory = self.parent.ensureStore(attribute="exportDirectory")
        attrFilename = self.parent.ensureStore(attribute="exportFilename")
        directory = cmds.textField(self.directory_text, q=True, text=True).strip()
        filename  = cmds.textField(self.filename_text,  q=True, text=True).strip()
        cmds.setAttr(attrDirectory, directory, type="string")
        cmds.setAttr(attrFilename, filename, type="string")

    def load(self):
        attrDirectory = self.parent.ensureStore(attribute="exportDirectory")
        attrFilename = self.parent.ensureStore(attribute="exportFilename")
        dataDirectory = cmds.getAttr(attrDirectory) or ""
        dataFilename = cmds.getAttr(attrFilename) or ""
        cmds.textField(self.directory_text, e=True, text=dataDirectory)
        cmds.textField(self.filename_text, e=True, text=dataFilename)

    def getCorrectives(self):
        attr = self.parent.ensureStore(attribute="correctiveJoints")
        data = cmds.getAttr(attr) or ""
        items = [s for s in data.split("|") if s]
        return items

        