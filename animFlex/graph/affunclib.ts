import { ClassicPreset, NodeEditor, GetSchemes } from "rete";
import {DropdownControl} from './afdropdown'; 
import resultPoseIcon from './resultPose.png';
import poseIcon_connected from './pinPose_connected.png';
import poseIcon_disconnected from './pinPose_disconnected.png';
import { NodeRef } from "rete-area-plugin/_types/extensions/shared/types";
import { editorInstance } from "./afeditorinstance";
import { Input, Socket } from "rete/_types/presets/classic";
import { classIdToName, classIdToParams, GraphNodeParam } from './afnodeFactory';

type Schemes = GetSchemes<ClassicPreset.Node, ClassicPreset.Connection<ClassicPreset.Node, ClassicPreset.Node>>;

export const $socketmargin = 6;
export const $socketsize = 32;

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

const SocketTypes: Map<string, string> = new Map();
SocketTypes.set("AFPose", "Pose");
SocketTypes.set("std::string", "string");
SocketTypes.set("float", "float");

function ReadSockets(node: ClassicPreset.Node, editor: NodeEditor<Schemes>) {

    const nodeType = (node as any).meta?.type;
    const nodeParams = classIdToParams.get(nodeType);

    if (nodeParams) {

        // Go through all sockets in node type.
        for (const param of nodeParams) {

            // Generate random UID for this socket.
            const uid = crypto.randomUUID();

            // Create the socket and attach new meta to it.
            const socket = new ClassicPreset.Socket(uid) as ClassicPreset.Socket & {
                meta?: {
                    socketType: string,
                    editor: NodeEditor<Schemes>,
                    node: ClassicPreset.Node,
                    socketIconConnected_path: string,
                    socketIconDisconnected_path: string
                }
            };

            // Fill the default socket meta.
            socket.meta = {
                socketType: "",
                editor,
                node,
                socketIconConnected_path: poseIcon_connected,
                socketIconDisconnected_path: poseIcon_disconnected
            }

            // Map the socket type.
            socket.meta.socketType = SocketTypes[param.var_type];

            // Create the socket direction.
            let newSocket;
            if(param.direction == "Input") {
                newSocket = new ClassicPreset.Input(socket);
            }
            else {
                newSocket = new ClassicPreset.Output(socket);
            }

            // Add label.
            newSocket.label = param.label;

            // Add is as input or output.
            if(param.direction == "Input") {
                node.addInput(uid, newSocket);
            }
            else {
                node.addOutput(uid, newSocket);
            }
        }
    }
}

export function CreateSockets(node: ClassicPreset.Node, editor: NodeEditor<Schemes>) {

    const nodeType = (node as any).meta?.type;

    switch (nodeType) {
        case "OutputPose":

            // Generate random UID for this socket.
            const uid = crypto.randomUUID();

            // Create the socket and attach new meta to it.
            const socket = new ClassicPreset.Socket(uid) as ClassicPreset.Socket & {
                meta?: {
                    socketType: string,
                    editor: NodeEditor<Schemes>,
                    node: ClassicPreset.Node,
                    socketIconConnected_path: string,
                    socketIconDisconnected_path: string
                }
            };

            // Fill the default socket meta.
            socket.meta = {
                socketType: "",
                editor,
                node,
                socketIconConnected_path: poseIcon_connected,
                socketIconDisconnected_path: poseIcon_disconnected
            }

            socket.meta.socketType = "Pose"
            const input = new ClassicPreset.Input(socket);
            input.label = "Result";
            node.addInput(uid, input);

            break;

        default:
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