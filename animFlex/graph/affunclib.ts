import { ClassicPreset, NodeEditor, GetSchemes } from "rete";
import {DropdownControl} from './afdropdown'; 
import resultPoseIcon from './resultPose.png';
import { NodeRef } from "rete-area-plugin/_types/extensions/shared/types";
import { Input, Socket } from "rete/_types/presets/classic";
import { classIdToMeta, classIdToName, classIdToParams, GraphNodeParam } from './afnodeFactory';
import { getSourceTarget } from 'rete-connection-plugin'
import { BoolControl } from "./afchecker";
import { FloatControl } from "./affloatfield";
import { createView, switchToView } from "./afmanager";
import { ReteViewType } from "./aftypes";

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

export function GetDefaultControlPerType(editor: NodeEditor<Schemes>, node: ClassicPreset.Node, socketType: string, meta: any) {
  switch (socketType) {
    case "bool":
      return new BoolControl(editor, node);
    case "float":
      return new FloatControl(editor, node);
    case "string": {
        const dropdownMeta = meta.find((m: string) => m.includes("Dropdown_"));
        if (dropdownMeta) {
            return new DropdownControl(dropdownMeta, editor, node);
        }
        return undefined;
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
                title: "New Graph",
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
                title: "New Graph",
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

    // Attach full param.meta to the socket.
    const existingMeta = (socket as any).meta ?? {};
    const paramFlags = param.meta && Array.isArray(param.meta)
    ? Object.fromEntries(param.meta.map(flag => [flag, true]))
    : {};
    (socket as any).meta = {
        ...existingMeta,
        ...paramFlags,
        var_name: param.var_name
    };

    // Create socket object casted.
    const newSocket = param.direction === "Input"
        ? new ClassicPreset.Input(socket, undefined, false)
        : new ClassicPreset.Output(socket, undefined, false);

    // Add it a label.
    newSocket.label = param.label;

    // Add it to the node.
    if (param.direction === "Input") {
        node.addInput(uid, newSocket);

        // Create default control and assign it to the input.
        const defaultControl = GetDefaultControlPerType(editor, node, socketType, meta);
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

export async function OnNodeUpdated(editor: NodeEditor<Schemes>, node: ClassicPreset.Node) {

    //console.log("updating: " + node.id);

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
        sockets.push({
            var_name: (input?.socket as any)?.meta?.var_name,
            valueField,
        });
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
        sockets.push({
            var_name: (output?.socket as any)?.meta?.var_name,
            valueField,
        });
    }

    // Add this node to JSON.
    graphJSON.push({
        nodeId: node.id,
        nodeType: nodeType,
        sockets
    });

    // Stringify the JSON.
    const graphString = JSON.stringify(graphJSON);

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
