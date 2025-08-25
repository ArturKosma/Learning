import { ClassicPreset, NodeEditor, GetSchemes } from "rete";
import {DropdownControl} from './afdropdown'; 
import resultPoseIcon from './resultPose.png';
import { NodeRef } from "rete-area-plugin/_types/extensions/shared/types";
import { Input, Socket } from "rete/_types/presets/classic";
import { classIdToMeta, classIdToName, classIdToParams, GraphNode, GraphNodeParam } from './afnodeFactory';
import { getSourceTarget } from 'rete-connection-plugin'
import { BoolControl, CustomChecker } from "./afchecker";
import { FloatControl, CustomFloatField } from "./affloatfield";
import { createView, getManifestNodes, switchToView } from "./afmanager";
import { ReteViewType } from "./aftypes";
import { createRoot } from "react-dom/client";
import { DropdownControlEnum } from "./afdropdownEnum";
import { AreaPlugin } from "rete-area-plugin";
import { ReactArea2D } from "rete-react-plugin";
import { ContextMenuExtra } from "rete-context-menu-plugin";

export interface AFSerializeInterface {
  serializeLoad(data: any): void;
}

class PoseSocket extends ClassicPreset.Socket {
  constructor(name: string) {
    super(name);
  }
  isCompatibleWith(other: ClassicPreset.Socket) {
    return other instanceof PoseSocket;
  }
}

class FloatSocket extends ClassicPreset.Socket {
  constructor(name: string) {
    super(name);
  }
  isCompatibleWith(other: ClassicPreset.Socket) {
    return other instanceof FloatSocket;
  }
}

class BoolSocket extends ClassicPreset.Socket {
  constructor(name: string) {
    super(name);
  }
  isCompatibleWith(other: ClassicPreset.Socket) {
    return other instanceof BoolSocket;
  }
}

class StringSocket extends ClassicPreset.Socket {
  constructor(name: string) {
    super(name);
  }
  isCompatibleWith(other: ClassicPreset.Socket) {
    return other instanceof StringSocket;
  }
}

class ExecSocket extends ClassicPreset.Socket {
  constructor(name: string) {
    super(name);
  }
  isCompatibleWith(other: ClassicPreset.Socket) {
    return other instanceof ExecSocket;
  }
}

export function GetDefaultControlPerType(editor: NodeEditor<Schemes>, node: ClassicPreset.Node, varName: string, socketType: string, meta: any, initial: any) {

    const toBool = (v: any) =>
        typeof v === 'string' ? v.toLowerCase() === 'true' : Boolean(v);

    const toFloat = (v: any) =>
        typeof v === 'string' ? Number(v) : Number(v);

  switch (socketType) {
    case "bool":
      return new BoolControl(editor, node, varName, toBool(initial));
    case "float":
      return new FloatControl(editor, node, varName, toFloat(initial));
    case "string": {
        const dropdownMeta = meta.find((m: string) => m.includes("Dropdown_")); // Any dropdowns.
        if (dropdownMeta) {
            return new DropdownControl(dropdownMeta, editor, node, varName);
        }
        const enumMeta = meta.find((m: string) => m.endsWith("_Enum")); // Enum dropdowns.
        if (enumMeta) {
            return new DropdownControlEnum(enumMeta, editor, node, varName);
        }
        return new DropdownControl("", editor, node, varName); // Simple text field.
    }
    default:
      return undefined;
  }
}

export function GetConnectionSockets(
  editor: NodeEditor<any>,
  connection: ClassicPreset.Connection<ClassicPreset.Node, ClassicPreset.Node>
): {
  source?: ClassicPreset.Socket;
  target?: ClassicPreset.Socket;
} {
  const sourceNode = editor.getNode(connection.source);
  const targetNode = editor.getNode(connection.target);

  const sourceSocket = sourceNode?.outputs[connection.sourceOutput]?.socket;
  const targetSocket = targetNode?.inputs[connection.targetInput]?.socket;

  return {
    source: sourceSocket,
    target: targetSocket
  };
}

export function CanCreateConnection(editor: NodeEditor<Schemes>, connection: Schemes["Connection"]) {
  const { source, target } = GetConnectionSockets(editor, connection);

  return source && target && (source as any).isCompatibleWith(target)
}

type Schemes = GetSchemes<ClassicPreset.Node, ClassicPreset.Connection<ClassicPreset.Node, ClassicPreset.Node>>;

export const $socketmargin = 6;
export const $socketsize = 32;

export const SocketTypes: Map<string, string> = new Map();
SocketTypes.set("AFPose", "Pose");
SocketTypes.set("std::string", "string");
SocketTypes.set("float", "float");
SocketTypes.set("bool", "bool");
SocketTypes.set("AFExec", "Exec");
SocketTypes.set("int", "string"); // We assume every int is really an enum in C++, and we want to show the enum as list of strings to choose, so a string.

export function GetNodeMeta(type: string, graphType: ReteViewType): any {

    const dynamicTitle = classIdToName.get(type) ?? "Unknown";
    const classMeta = classIdToMeta.get(type);

    let defaultMeta: any;

    switch(graphType) {
        case ReteViewType.StateMachine: {
            defaultMeta = {
                type,
                color: "#121212",
                titleBarColor: 'linear-gradient(to right, rgba(85, 85, 85, 0.0), rgba(85, 85, 85, 0.0))',
                title: dynamicTitle,
                showTitle: true,
                titleEditable: true,
                showSubTitle: false,
                nodeWidth: 80,
                nodeHeight: 80,
                bigIcon: false,
                isRemovable: true,
                classMeta,
                valuesMap: {} as Record<string, string>
            };
            break;
        }
        case ReteViewType.Graph: {
            defaultMeta = {
                type,
                color: "#121212",
                titleBarColor: 'linear-gradient(to right, rgba(85, 85, 85, 0.9), rgba(85, 85, 85, 0.3))',
                title: dynamicTitle,
                showTitle: true,
                titleEditable: false,
                showSubTitle: false,
                nodeWidth: 240,
                nodeHeight: 80,
                bigIcon: false,
                isRemovable: true,
                classMeta,
                valuesMap: {} as Record<string, string>
            };
            break;
        }
        default: {
             defaultMeta = {
                type,
                color: "#121212",
                titleBarColor: 'linear-gradient(to right, rgba(85, 85, 85, 0.9), rgba(85, 85, 85, 0.3))',
                title: dynamicTitle,
                showTitle: true,
                titleEditable: false,
                showSubTitle: false,
                nodeWidth: 240,
                nodeHeight: 80,
                bigIcon: false,
                isRemovable: true,
                classMeta,
                valuesMap: {} as Record<string, string>
            };
            break;
        }
    }

    switch (type) {
        case "OutputPose":
            return {
                ...defaultMeta,
                isRemovable: false,
                title: "Output Pose",
                showSubTitle: true,
                subTitle: "AnimGraph",
                nodeHeight: 160,
                bigIcon: true,
                bigIcon_path: resultPoseIcon,
            };
        case "AFGraphNode_Graph":
            return {
                ...defaultMeta,
                showSubTitle: true,
                subTitle: "Graph",
                title: "",
                titleEditable: true,
                nodeWidth: 160,
                color: 'linear-gradient(to right, rgba(12, 12, 12, 1.0), rgba(28, 28, 28, 0.9))',
                titleBarColor: "rgba(0, 0, 0, 0.0)",
                onDoubleClick: (currentTitle: string, nodeId: string) => {
                    createView(currentTitle, nodeId, ReteViewType.Graph);
                }
            };
        case "AFGraphNode_StateMachine":
            return {
                ...defaultMeta,
                showSubTitle: true,
                subTitle: "State Machine",
                title: "",
                titleEditable: true,
                nodeWidth: 160,
                color: 'linear-gradient(to right, rgba(85, 85, 85, 1.0), rgba(70, 70, 70, 0.9))',
                titleBarColor: "rgba(0, 0, 0, 0.0)",
                onDoubleClick: (currentTitle: string, nodeId: string) => {
                    createView(currentTitle, nodeId, ReteViewType.StateMachine);
                }
            };
        case "StateStart":
            return {
                ...defaultMeta,
                isRemovable: false,
                showSubTitle: false,
                subTitle: "",
                title: "",
                titleEditable: false,
                nodeWidth: 80,
                color: 'linear-gradient(to right, rgba(85, 85, 85, 1.0), rgba(70, 70, 70, 0.9))',
                titleBarColor: "rgba(0, 0, 0, 0.0)",
            }
        case "OutputCond":
            return {
                ...defaultMeta,
                isRemovable: false,
                title: "Output",
                showSubTitle: false,
                subTitle: "",
                nodeHeight: 80,
                nodeWidth: 160,
                bigIcon: false,
                bigIcon_path: resultPoseIcon,
            };
        default:
            return {
                ...defaultMeta,
            }
    }
} 

function CreateSocketWithMeta(
    uid: string,
    socketType: string,
    editor: NodeEditor<Schemes>,
    node: ClassicPreset.Node
): ClassicPreset.Socket & {
    meta?: {
        socketType: string;
        editor: NodeEditor<Schemes>;
        node: ClassicPreset.Node;
    };
} {
    // Create new socket object.
    let baseSocket: ClassicPreset.Socket;

    switch (socketType) {
        case "Pose":
            baseSocket = new PoseSocket(uid);
            break;
        case "float":
            baseSocket = new FloatSocket(uid);
            break;
        case "bool":
            baseSocket = new BoolSocket(uid);
            break;
        case "string":
            baseSocket = new StringSocket(uid);
            break;
        case "Exec":
            baseSocket = new ExecSocket(uid);
            break;    
        default:
            baseSocket = new ClassicPreset.Socket(uid); // fallback
    }

    const socket = baseSocket as ClassicPreset.Socket & {
        meta?: {
            socketType: string;
            editor: NodeEditor<Schemes>;
            node: ClassicPreset.Node;
        };
    };

    // Assign meta to socket.
    socket.meta = {
        socketType,
        editor,
        node,
    };

    return socket;
}

function CreateAndAddSocket(
    node: ClassicPreset.Node,
    param: any,
    editor: NodeEditor<Schemes>
) {
    // Get pin meta.
    const meta = param.meta;

    // Generate unique ID for the socket.
    const uid = crypto.randomUUID();

    // Find mapped socket type string.
    const socketType = SocketTypes.get(param.var_type) as string;

    // Create meta for the socket.
    const socket = CreateSocketWithMeta(
        uid,
        socketType,
        editor,
        node
    );

    // Make sure valuesMap exists on the node.
    const nodeMeta: any = (node as any).meta ?? ((node as any).meta = {});
    if (!nodeMeta.valuesMap) nodeMeta.valuesMap = {};

    // Parse the meta params.
    const existingMeta = (socket as any).meta ?? {};
    const paramFlags = Array.isArray(param.meta)
    ? Object.fromEntries(
        (param.meta as string[]).map((flag) => {
            if (typeof flag === 'string') {
            const eq = flag.indexOf('=');
            // Convert "Key=Value" -> ["Key", "Value"], otherwise ["Key", true]
            return eq >= 0 ? [flag.slice(0, eq), flag.slice(eq + 1)] : [flag, true];
            }
            return [String(flag), true];
        })
        )
    : {};

    // Add the meta to the socket.
    (socket as any).meta = {
        ...existingMeta,
        ...paramFlags,
        var_name: param.var_name
    };

    // Add default value to the values map.
    nodeMeta.valuesMap[param.var_name] = param.default ?? "";

    // Auto hide pin and control when no direction specified.
    if (param.direction === "") {
        (socket as any).meta.HidePin = true;
        (socket as any).meta.HideControl = true;
    }

    // Create socket object casted.
    const newSocket = param.direction === "Input"
        ? new ClassicPreset.Input(socket, undefined, false)
        : new ClassicPreset.Output(socket, undefined, true);

    // Add it a label.
    newSocket.label = param.label;

    // Add it to the node.
    if (param.direction === "Input") {
        node.addInput(uid, newSocket);

        // Create default control and assign it to the input.
        const defaultControl = GetDefaultControlPerType(editor, node, param.var_name, socketType, meta, param.default);
        if (defaultControl) {
            (newSocket as ClassicPreset.Input).control = defaultControl;
            (newSocket as ClassicPreset.Input).showControl = true;
        }

    } else {
        node.addOutput(uid, newSocket);
    }
}

function ReadSockets(node: ClassicPreset.Node, editor: NodeEditor<Schemes>) {

    // Find node type.
    const nodeType = (node as any).meta?.type;

    // Find node params.
    const nodeParams = classIdToParams.get(nodeType);

    // Go through node params and add sockets for them.
    if (nodeParams) {
        for (const param of nodeParams) {
            CreateAndAddSocket(node, param, editor);
        }
    }
}

export function CreateSockets(node: ClassicPreset.Node, editor: NodeEditor<Schemes>) {

    // Find node type.
    const nodeType = (node as any).meta?.type;

    // Treat OutputPose node specially as it's not a CPP function.
    if (nodeType === "OutputPose") {
        const uid = crypto.randomUUID();
        const socket = CreateSocketWithMeta(
            uid,
            "Pose",
            editor,
            node
        );

        const existingMeta = (socket as any).meta ?? {};
        (socket as any).meta = {
            ...existingMeta,
            var_name: "Pose"
        };

        const input = new ClassicPreset.Input(socket);
        input.label = "Result";

        node.addInput(uid, input);
    } 

    // Treat StateStart node specially as it's not a CPP function.
    if (nodeType === "StateStart") {
        const uid = crypto.randomUUID();
        const socket = CreateSocketWithMeta(
            uid,
            "Exec",
            editor,
            node
        );

        const existingMeta = (socket as any).meta ?? {};
        (socket as any).meta = {
            ...existingMeta,
            var_name: "Exec"
        };

        const output = new ClassicPreset.Output(socket);
        output.label = "";

        node.addOutput(uid, output);
    } 

    // Treat OutputCond node specially as it's not a CPP function.
    if (nodeType === "OutputCond") {
        const uid = crypto.randomUUID();
        const socket = CreateSocketWithMeta(
            uid,
            "bool",
            editor,
            node
        );

        const existingMeta = (socket as any).meta ?? {};
        (socket as any).meta = {
            ...existingMeta,
            var_name: "OutputCond"
        };

        const input = new ClassicPreset.Input(socket);
        input.label = "Result";

        node.addInput(uid, input);
    } 

    // Create every sockets for every other node type.
    else {
        ReadSockets(node, editor);
    }
}

declare const Module: any;

export async function OnNodeCreated(node: ClassicPreset.Node, context: string) {
    
    // Wait until Emscripten runtime is ready.
    if (!Module.calledRun) {
        await new Promise<void>((resolve) => {
            const prevInit = Module.onRuntimeInitialized;
            Module.onRuntimeInitialized = function () {
                if (typeof prevInit === 'function') prevInit();
                resolve();
            };
        });
    }

    // Container for JSON.
    const graphJSON: any[] = [];

    // Casted node type to any, to prevent lack of meta errors.
    const nodeType = (node as any).meta?.type;

    // Add this node to JSON.
    graphJSON.push({
        nodeType: nodeType,
        nodeId: node.id,
        nodeContext: context
    });

    // Stringify JSON.
    const graphString = JSON.stringify(graphJSON);

    // Pass the JSON to C++.
    Module.ccall(
        'OnNodeCreated',   
        null,              
        ['string'],       
        [graphString]
    );  
}

type AreaExtra = ReactArea2D<Schemes> | ContextMenuExtra;
export async function OnNodeUpdated(editor: NodeEditor<Schemes>, node: ClassicPreset.Node) {

    // Wait until Emscripten runtime is ready.
    if (!Module.calledRun) {
        await new Promise<void>((resolve) => {
        const prevInit = Module.onRuntimeInitialized;
        Module.onRuntimeInitialized = function () {
            if (typeof prevInit === 'function') prevInit();
            resolve();
        };
        });
    }

    // Get all connections.
    const allConnections = editor.getConnections?.() ?? [];

    // Container for JSON.
    const graphJSON: any[] = [];

    // Casted node type to any, to prevent lack of meta errors.
    const nodeType = (node as any).meta?.type;

    // Container for sockets.
    const sockets: any[] = [];

    // Go through all inputs.
    for (const [key, input] of Object.entries(node.inputs)) {

        // Get connection for this input.
        const inputConnection = allConnections.filter(conn => {
            return conn.target === node.id && conn.targetInput === key;
        });

        // The socket is either connected..
        let valueField;
        if(inputConnection.length == 1) {
            const conn = inputConnection[0];
            valueField = {
                connectedNodeId: conn.source,
                connectedSocketName: (editor.getNode(conn.source)?.outputs[conn.sourceOutput]?.socket as any)?.meta?.var_name
            }
        }
        //.. or has a plain value
        else {
            
            // Try to find the control attached to this input.
            const control = (input as any).control;
            valueField = {
                value: String(control?.value)
            };
        }

        // Add input.

        // Don't push undefined values.
        // Let the the undefined be filled from valueMap.
        if(valueField.value != "undefined"){
            sockets.push({
            var_name: (input?.socket as any)?.meta?.var_name,
            valueField,
        });
        }
    }

    // Go through all outputs.
    for (const [key, output] of Object.entries(node.outputs)) {

        // Get connection for this output.
        const outputConnection = allConnections.filter(conn => {
            return conn.source === node.id && conn.sourceOutput === key;
        });

        // The socket is either connected..
        let valueField;
        if(outputConnection.length == 1) {
            const conn = outputConnection[0];
            valueField = {
                connectedNodeId: conn.target,
                connectedSocketName: (editor.getNode(conn.target)?.inputs[conn.targetInput]?.socket as any)?.meta?.var_name
            }
        }
        //.. or has a plain value
        else {

            // Try to find the control attached to this output.
            const control = (output as any).control;
            valueField = {
                value: String(control?.value)
            };
        }

        // Add output.

        // Don't push undefined values.
        // Let the the undefined be filled from valueMap.
        if(valueField.value != "undefined") {
            sockets.push({
            var_name: (output?.socket as any)?.meta?.var_name,
            valueField,
        });
        }
    }

    // Get value map to add the remaining node parameters that weren't sockets.
    const valueMap = (node as any).meta?.valuesMap as Record<string, string> | undefined;

    if (valueMap) {
        const existing = new Set<string>(sockets.map(s => String(s?.var_name)));

        for (const [var_name, value] of Object.entries(valueMap)) {
            if (existing.has(var_name)) continue;

            sockets.push({
            var_name,
            valueField: { value } // Already a string.
            });
        }
    }

    // Add this node to JSON.
    graphJSON.push({
        nodeId: node.id,
        nodeType: nodeType,
        sockets
    });

    // Stringify the JSON.
    const graphString = JSON.stringify(graphJSON);

    //console.log(graphString)
    editor.meta?.area.update('node', node.id);

    // Pass the JSON to C++.
    Module.ccall(
        'OnNodeUpdated',   
        null,              
        ['string'],       
        [graphString]
    );   
}

export async function OnNodeRemoved(node: ClassicPreset.Node) {

    // Wait until Emscripten runtime is ready.
    if (!Module.calledRun) {
        await new Promise<void>((resolve) => {
        const prevInit = Module.onRuntimeInitialized;
        Module.onRuntimeInitialized = function () {
            if (typeof prevInit === 'function') prevInit();
            resolve();
        };
        });
    }

    // Container for JSON.
    const graphJSON: any[] = [];

    // Casted node type to any, to prevent lack of meta errors.
    const nodeType = (node as any).meta?.type;

    // Add this node to JSON.
    graphJSON.push({
        nodeType: nodeType,
        nodeId: node.id,
    });

    // Stringify JSON.
    const graphString = JSON.stringify(graphJSON);

    // Pass the JSON to C++.
    Module.ccall(
        'OnNodeRemoved',   
        null,              
        ['string'],       
        [graphString]
    );  
}

export async function OnStateConnectionCreated(context: string, nodeFrom: ClassicPreset.Node, nodeTo: ClassicPreset.Node, connectionId: string, nodeCond: string) {

    // Wait until Emscripten runtime is ready.
    if (!Module.calledRun) {
        await new Promise<void>((resolve) => {
        const prevInit = Module.onRuntimeInitialized;
        Module.onRuntimeInitialized = function () {
            if (typeof prevInit === 'function') prevInit();
            resolve();
        };
        });
    }

    // Container for JSON.
    const graphJSON: any[] = [];

    // Add this node to JSON.
    graphJSON.push({
        nodeContext: context,
        nodeFromID: nodeFrom.id,
        nodeToID: nodeTo.id,
        connectionID: connectionId,
        nodeCondID: nodeCond
    });

    // Stringify JSON.
    const graphString = JSON.stringify(graphJSON);

    // Pass the JSON to C++.
    Module.ccall(
        'OnStateConnectionCreated',   
        null,              
        ['string'],       
        [graphString]
    );  
}

export async function OnStateConnectionRemoved(context: string, connectionId: string) {

    // Wait until Emscripten runtime is ready.
    if (!Module.calledRun) {
        await new Promise<void>((resolve) => {
        const prevInit = Module.onRuntimeInitialized;
        Module.onRuntimeInitialized = function () {
            if (typeof prevInit === 'function') prevInit();
            resolve();
        };
        });
    }

    // Container for JSON.
    const graphJSON: any[] = [];

    // Add this node to JSON.
    graphJSON.push({
        nodeContext: context,
        connectionID: connectionId
    });

    // Stringify JSON.
    const graphString = JSON.stringify(graphJSON);

    // Pass the JSON to C++.
    Module.ccall(
        'OnStateConnectionRemoved',   
        null,              
        ['string'],       
        [graphString]
    );  
}

// Selection re-renders details panel.
export function setDetailsPanelVisible(editor: NodeEditor<Schemes>, show: boolean, nodeId: string = "") {
    const panel = document.getElementById('details-panel');
    const content = document.getElementById('details-content');

    if (!panel || !content) return;

    panel.style.display = 'block';
    content.innerHTML = ""; // Clear out.

    const showPlaceholder = () => {
        content.innerHTML = `<div style="
            padding: 20px;
            text-align: center;
            font-style: italic;
            color: #999;
        ">Choose object to show details</div>`;
    };

    const showPlaceholderEmpty = () => {
        content.innerHTML = `<div style="
            padding: 20px;
            text-align: center;
            font-style: italic;
            color: #999;
        "></div>`;
    };

    if (!show) {
        showPlaceholder()
        return;
    }

    if (nodeId === "") {
        showPlaceholder();
        return;
    }

    const node = editor.getNode(nodeId);
    if (!node) {
        showPlaceholder();
        return;
    }

    const nodeType = node.meta?.type;
    if (!nodeType) {
        showPlaceholder();
        return;
    }

    const manifest = getManifestNodes().find(n => n.class_id === nodeType);
    if (!manifest) {
        showPlaceholderEmpty();
        return;
    }

    const undirectedParams = manifest.params.filter(p => p.direction === "");
    if (undirectedParams.length === 0) {
        showPlaceholderEmpty();
        return;
    }

    // Create rows for each parameter.
    undirectedParams.forEach((param, idx) => {
        const row = document.createElement('div');
        row.className = `detail-row${idx}`;
        Object.assign(row.style, {
            display: 'flex',
            flexDirection: 'row',
            borderBottom: '1px solid #151515',
            paddingLeft: '20px',
            width: '100%',
            boxSizing: 'border-box'
        });

        // Name cell.
        const nameCell = document.createElement('div');
        nameCell.id = `detail-name${idx}`;
        nameCell.textContent = param.label || param.var_name;
        Object.assign(nameCell.style, {
            flex: '6',
            display: 'flex',
            alignItems: 'center',
            justifyContent: 'flex-start',
            borderRight: '1px solid #151515',
            padding: '10px 0',
        });

        // Control cell.
        const controlCell = document.createElement('div');
        controlCell.id = `detail-control${idx}`;
        Object.assign(controlCell.style, {
            flex: '4',
            padding: '10px 5px',
            display: 'flex',
            alignItems: 'center',
            justifyContent: 'flex-start',
        });

        // Create control.
        let controlInstance: any;
        let reactElement: React.ReactElement;

        switch (param.var_type) {
            case 'float':
                const initialFloat = (() => {
                    const vm: Record<string, string> | undefined = (node as any).meta?.valuesMap;
                    const s = vm?.[param.var_name] ?? String(param.default ?? "");
                    return parseFloat(s) || 0;
                })();

                controlInstance = new FloatControl(editor, node, param.var_name, initialFloat);
                reactElement = <CustomFloatField data={controlInstance} />;
                break;

            case 'bool':
                const initialBool = (() => {
                    const vm: Record<string, string> | undefined = (node as any).meta?.valuesMap;
                    const s = vm?.[param.var_name] ?? String(param.default ?? "");
                    return String(s).toLowerCase() === "true";
                })();

                controlInstance = new BoolControl(editor, node, param.var_name, initialBool);
                reactElement = <CustomChecker data={controlInstance} />;
                break;

            default:
                break;
        }

        if (reactElement) {
            const root = createRoot(controlCell);
            root.render(reactElement);
        }

        // Assemble
        row.appendChild(nameCell);
        row.appendChild(controlCell);
        content.appendChild(row);

    });
}
