import { ClassicPreset, NodeEditor, GetSchemes } from "rete";
import {DropdownControl} from './afdropdown'; 
import resultPoseIcon from './resultPose.png';
import poseIcon_connected from './pinPose_connected.png';
import poseIcon_disconnected from './pinPose_disconnected.png';
import icon_connected from './pin_connected.png';
import icon_disconnected from './pin_disconnected.png';
import { NodeRef } from "rete-area-plugin/_types/extensions/shared/types";
import { editorInstance } from "./afeditorinstance";
import { Input, Socket } from "rete/_types/presets/classic";
import { classIdToName, classIdToParams, GraphNodeParam } from './afnodeFactory';

type Schemes = GetSchemes<ClassicPreset.Node, ClassicPreset.Connection<ClassicPreset.Node, ClassicPreset.Node>>;

export const $socketmargin = 6;
export const $socketsize = 32;

export const SocketTypes: Map<string, string> = new Map();
SocketTypes.set("AFPose", "Pose");
SocketTypes.set("std::string", "string");
SocketTypes.set("float", "float");
SocketTypes.set("bool", "bool");

export function GetNodeMeta(type: string): any {

    const dynamicTitle = classIdToName.get(type) ?? "Unknown";

    // Default shared metadata
    const defaultMeta = {
        type,
        title: dynamicTitle,
        showTitle: true,
        showSubTitle: false,
        nodeWidth: 240,
        nodeHeight: 80,
        bigIcon: false,
        isRemovable: true,
    };

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
            break;
        default:
            return {
                ...defaultMeta,
            }
    }
} 

function GetSocketIcon(socketType: string, connected: boolean) {
    switch (socketType) {
        case "Pose": {
            return connected ? poseIcon_connected : poseIcon_disconnected;
        }
        default: {
            return connected ? icon_connected : icon_disconnected;
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
        socketIconConnected_path: string;
        socketIconDisconnected_path: string;
    };
} {
    // Create new socket object.
    const socket = new ClassicPreset.Socket(uid) as ClassicPreset.Socket & {
        meta?: {
            socketType: string;
            editor: NodeEditor<Schemes>;
            node: ClassicPreset.Node;
            socketIconConnected_path: string;
            socketIconDisconnected_path: string;
        };
    };

    // Assign meta to socket.
    socket.meta = {
        socketType,
        editor,
        node,
        socketIconConnected_path: GetSocketIcon(socketType, true),
        socketIconDisconnected_path: GetSocketIcon(socketType, false)
    };

    return socket;
}

function CreateAndAddSocket(
    node: ClassicPreset.Node,
    param: any,
    editor: NodeEditor<Schemes>
) {
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

    // Create socket object casted.
    const newSocket = param.direction === "Input"
        ? new ClassicPreset.Input(socket)
        : new ClassicPreset.Output(socket);

    // Add it a label.
    newSocket.label = param.label;

    // Add it to the node.
    if (param.direction === "Input") {
        node.addInput(uid, newSocket);
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
export async function GraphUpdate() {

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

    // Get all nodes.
    const nodes = editorInstance?.getNodes() ?? [];

    // Get all connections.
    const allConnections = editorInstance?.getConnections?.() ?? [];

    // Container for JSON.
    const graphJSON: any[] = [];

    // Go through each node.
    for (const node of nodes) {

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
                    connectedSocketId: conn.sourceOutput
                }
            }
            //.. or has a plain value
            else {
                valueField = {
                    value: "None"
                }
            }

            // Add input.
            sockets.push({
                sockedId: key,
                direction: "input",
                type: (input?.socket as any)?.meta?.socketType,
                valueField
            });
        }

        // Go through all outputs.
        for (const [key, output] of Object.entries(node.outputs)) {

            // Add output.
            sockets.push({
                sockedId: key,
                direction: "output",
                type: (output?.socket as any)?.meta?.socketType
            });
        }

        // Add this node to JSON.
        graphJSON.push({
            nodeId: node.id,
            nodeType: nodeType,
            sockets
        });
    }

    // Stringify the JSON.
    const graphString = JSON.stringify(graphJSON);

    // Pass the JSON to C++.
    Module.ccall(
        'OnGraphUpdate',   
        null,              
        ['string'],       
        [graphString]
    );   
}