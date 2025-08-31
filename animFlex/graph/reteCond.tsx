import { createRoot } from "react-dom/client";
import { NodeEditor, GetSchemes, ClassicPreset } from "rete";
import { AreaPlugin, AreaExtensions, Drag } from "rete-area-plugin";
import { ConnectionPlugin, Presets as ConnectionPresets, makeConnection } from "rete-connection-plugin";
import { ReactPlugin, Presets, ReactArea2D } from "rete-react-plugin";
import { addCustomBackground } from "./custom-background";
import { AFNodeFactory, classIdToName, GraphNode, GraphNodeParam, classIdToParams, classIdToMeta } from './afnodeFactory';
import { AFNode } from './afnode';
import { AFSocket, preloadPins} from './afsocket';
import { AFConnection, AFFlow } from './afconnection';
import {
  ContextMenuExtra,
  ContextMenuPlugin,
  Presets as ContextMenuPresets
} from "rete-context-menu-plugin";
import styled from "styled-components";
import {DropdownControl, CustomDropdown} from './afdropdown'; 
import {CanCreateConnection, GetConnectionSockets, OnNodeCreated, OnNodeRemoved, OnNodeUpdated} from './affunclib'
import { setupSelection } from './selection';
import { BoolControl, CustomChecker } from "./afchecker";
import { CustomFloatField, FloatControl } from "./affloatfield";
import { getCurrentView } from "./afmanager";
import { ReteViewType } from "./aftypes";
import { CustomDropdownEnum, DropdownControlEnum } from "./afdropdownEnum";
import { useEffect, useRef } from "react";

  // Context menu item type
  type Item = {
    label: string;
    key: string;
    handler: () => void;
  };

declare const Module: any;

const nodeQueue: (() => Promise<void>)[] = [];
let isProcessing = false;

function enqueueNode(fn: () => Promise<void>) {
  nodeQueue.push(fn);
  processQueue();
}

async function processQueue() {
  if (isProcessing || nodeQueue.length === 0) return;
  isProcessing = true;

  while (nodeQueue.length > 0) {
    const task = nodeQueue.shift();
    if (task) await task();
  }

  isProcessing = false;
}

// Stop any backspace keydown propagation from Rete.
// This allows using backspace in all input fields.
window.addEventListener('keydown', e => {
  if (e.key === 'Backspace') {
    e.stopImmediatePropagation();
  }
}, true);

type Schemes = GetSchemes<ClassicPreset.Node, ClassicPreset.Connection<ClassicPreset.Node, ClassicPreset.Node>>;
type AreaExtra = ReactArea2D<Schemes> | ContextMenuExtra;

export async function createEditorCond(container: HTMLElement, id: string) {

  const viewId = id;
  preloadPins();

  // Read and parse JSON graph manifest.
  const res = await fetch('./graphManifest.json');
  const data = await res.json();
  const nodes: GraphNode[] = data.Nodes;

  // Cache JSON nodes manifest.
  for (const node of nodes) {
    classIdToName.set(node.class_id, node.node_name);
    classIdToParams.set(node.class_id, node.params);
    classIdToMeta.set(node.class_id, node.meta);
  }

  // Create plugins.
  const editor = new NodeEditor<Schemes>();
  let area = new AreaPlugin<Schemes, AreaExtra>(container);
  const connection = new ConnectionPlugin<Schemes, AreaExtra>();
  let render = new ReactPlugin<Schemes, AreaExtra>({ createRoot });
  const selector = AreaExtensions.selector();

  (editor as any).meta ??= {};
  (editor as any).meta.area = area;


// Context menu
  const contextMenu = new ContextMenuPlugin<Schemes>({
    items: (context) => {
      // If right-clicked on a node
      if (typeof context !== "string") {
        return {
          list: [] // No context menu items for nodes
        };
      }

      // If right-clicked on empty space (root)
      if (context === "root") {
        return {
          searchBar: true,
          list: Array.from(classIdToName.entries())
            .filter(([classId]) => {
              const meta = classIdToMeta.get(classId);
              return !meta?.includes("NoDropdown"); // NoDropdown meta prevents from showing up in items.
            })
            .map(([classId, nodeName]): Item => ({
              label: nodeName,
              key: classId,
              handler: async () => {
                const { x: screenX, y: screenY } = area.area.pointer;

                const gridSize = 16;
                const snappedX = Math.round(screenX / gridSize) * gridSize;
                const snappedY = Math.round(screenY / gridSize) * gridSize;

                const { node } = await AFNodeFactory.create(classId, editor, true, ReteViewType.Graph) as {
                  node: ClassicPreset.Node;
                };

                await editor.addNode(node);
                await area.translate(node.id, { x: snappedX, y: snappedY });
              }
            }))
        };
      }

      return {
        list: []
      };
    }
  });

  // Enables node selection in the editor area.
  AreaExtensions.simpleNodesOrder(area);
  const nodeSelector = AreaExtensions.selectableNodes(area, selector, {
    accumulating: AreaExtensions.accumulateOnCtrl()
  });

  const selection = setupSelection(area, {
    selected(ids) {
      selector.unselectAll();
      ids.forEach((id, i) => {
        nodeSelector.select(id, i !== 0);
      });
    },
  });

selection.setShape('marquee');
selection.setButton(0);

  // Grid snapping.
  AreaExtensions.snapGrid(area, {size: 16, dynamic: false});

  // Limit the zoom.
  AreaExtensions.restrictor(area, {
      scaling: {
        min: 0.2,
        max: 1.5
      },
      translation: false
  });

  // Visual and behavior presets.
    render.addPreset(Presets.classic.setup({
        customize: {
            node(context) {
                return AFNode;
            },
            socket(context) {
                return AFSocket;
            },
            connection(context) {
                return (props) => {

                  const { source: sourceSocket, target: targetSocket } = GetConnectionSockets(editor, props.data);
                  const sourceSocketType = (sourceSocket as any)?.meta?.socketType;
                  const targetSocketType = (targetSocket as any)?.meta?.socketType;
                  const socketType = sourceSocketType || targetSocketType;

                  return (
                    <AFConnection
                        {...props}
                        highlighted={highlightedConnectionIds.has(props.data.id)}
                        socketType={socketType}
                    />
                  );
                }
            },
            control(data) {
                if(data.payload instanceof DropdownControl) {
                  return CustomDropdown;
                }
                if (data.payload instanceof BoolControl) {
                  return CustomChecker;
                }
                if (data.payload instanceof FloatControl) {
                  return CustomFloatField;
                }
                if (data.payload instanceof DropdownControlEnum) {
                  return CustomDropdownEnum;
                }
            }
            }}));
    connection.addPreset(() => new AFFlow(editor));
    connection.addPreset(() => new ConnectionPresets.single(editor));

  // Context menu stylized.
  const { Menu, Common, Search, Item, Subitems } = Presets.contextMenu
const CustomMenu = styled(Menu)`
  width: 180px;
  opacity: 0.95;
  background: rgb(18, 18, 18);
  border: 1px solid #111;
  border-radius: 10px;

  display: flex;
  flex-direction: column;
  box-sizing: border-box;

  /* padding so the sticky search can sit nicely */
  padding: 8px 8px 6px;

  /* cap total height and enable vertical scrolling */
  max-height: min(40vh, 320px);
  overflow-y: auto;
  overflow-x: hidden; /* remove width scrollbar */

  /* dark scrollbar */
  scrollbar-width: thin; /* Firefox */
  scrollbar-color: rgba(60, 60, 60, 0.95) rgba(22, 22, 22, 0.95);

  &::-webkit-scrollbar { width: 8px; }
  &::-webkit-scrollbar-track {
    background: rgba(22, 22, 22, 0.95);
    border-left: 1px solid #0b0b0b;
  }
  &::-webkit-scrollbar-thumb {
    background: linear-gradient(180deg, rgba(48,48,48,1), rgba(38,38,38,1));
    border-radius: 8px;
    border: 1px solid #000;
  }
  &::-webkit-scrollbar-thumb:hover {
    background: linear-gradient(180deg, rgba(70,70,70,1), rgba(50,50,50,1));
  }
`;

  const CustomItem = styled(Item)`
    background:rgb(18, 18, 18);
    transition: background 0.2s ease;
    opacity: 0.9;
    &:hover {
      background:rgb(35, 35, 35);
    }
    border: 1px solid black;
    font-family: "Segoe UI", sans-serif;
    font-size: 10px;
    color: rgba(235, 235, 235, 0.93);
  `
  const CustomCommon = styled(Common)`
    background:rgb(18, 18, 18);
    opacity: 0.9;
    border: 1px solid black;
    &:hover {
      background:rgb(18, 18, 18);
    }
  `
    const AutoFocusSearch: React.FC<any> = (props) => {
      const ref = useRef<HTMLInputElement>(null);

      useEffect(() => {
        // focus as soon as the search bar mounts
        ref.current?.focus();
      }, []);

      return <Search {...props} ref={ref} />;
    };

const CustomSearch = styled(AutoFocusSearch)`
  position: sticky;
  top: 0;
  z-index: 2;

  background: rgb(18, 18, 18);
  border: 1px solid rgb(60, 60, 60);
  border-radius: 8px;

  /* no margins that could cause width overflow; menu has padding already */
  margin: 0 0 6px 0;
  width: 100%;
  box-sizing: border-box;
  padding: 4px 6px;

  font-family: "Segoe UI", sans-serif;
  font-size: 10px;
  min-height: 26px;

  &:hover, & input:hover { border-color: rgb(67, 150, 238); }
  &:focus, & input:focus { outline: none; border-color: rgb(67, 150, 238); }
`;
const CustomSubitems = styled(Subitems)`
  background: rgb(18, 18, 18);
  border: none;
`;
  render.addPreset(Presets.contextMenu.setup({
    customize: {
      main: () => CustomMenu,
      item: () => CustomItem,
      common: () => CustomCommon,
      search: () => CustomSearch,
      subitems: () => CustomSubitems
    }
  }));

  // Custom grid background.
  addCustomBackground(area);

  // Register plugins.
  editor.use(area);
  area.use(connection);
  area.use(render);
  area.use(contextMenu);

  // Nodes layering.
  AreaExtensions.simpleNodesOrder(area);

  // Enable dragging with right-mouse button.
  area.area.setDragHandler(new Drag({
    down: e => {
      if (e.pointerType === 'mouse' && e.button !== 2) return false

      e.preventDefault()
      return true
    },
    move: () => true
  }))

  // Prevent context menu when dragging.
  let currentMouse = { x: 0, y: 0 };
  document.addEventListener("pointermove", (e) => {
    currentMouse = { x: e.clientX, y: e.clientY };
  });
  let dragStart: { x: number; y: number } | null = null;
  container.addEventListener('pointerdown', (e) => {
    if (e.button === 2) {
      dragStart = { x: e.clientX, y: e.clientY };
    }
  });
  container.addEventListener('contextmenu', (e) => {
  const dx = Math.abs(currentMouse.x - dragStart.x);
  const dy = Math.abs(currentMouse.y - dragStart.y);
  const distance = Math.sqrt(dx * dx + dy * dy);

  if (distance > 3) {
    e.preventDefault();
    e.stopPropagation();
  }
}, {capture: true});

  // Disable double click zoom.
  area.addPipe(context => {
    if (context.type === 'zoom' && context.data.source === 'dblclick') return
    return context
  })

  // Show dragging hand when dragging around.
  let dragging = false;
  area.addPipe(context => {
      if (context.type === 'translated') {
          dragging = true;
          document.body.style.cursor = "grabbing";
      }
    return context
  })
  area.addPipe(context => {
      if (context.type === 'pointerup') {
          dragging = false;
          document.body.style.cursor = "default";
      }
    return context
  })

  // Prevent creation when sockets are not compatible.
  editor.addPipe((context) => {
  if (context.type === "connectioncreate") {

    // Force rehash whenever a new connection is created.
    // This is to prevent a bug where you reconnect the exact same connection and the hash is being the same.
    lastHash = 0;

    if (!CanCreateConnection(editor, context.data)) {
      return;
    }
  }
  return context;
});

  // Listen for new connections/disconnections.
  editor.addPipe(context => {

    if (context.type === 'connectioncreated' || context.type === 'connectionremoved') {
      const { source, target } = context.data;

      enqueueNode(() => OnNodeUpdated(editor, editor.getNode(source)!));
      enqueueNode(() => OnNodeUpdated(editor, editor.getNode(target)!));
    }
    return context;
  });

  // Zoom the view to fit all nodes after 100ms.
  setTimeout(() => 
  {
    AreaExtensions.zoomAt(area, editor.getNodes(), { scale: 0.7 });
  }, 100);

  // Listen for node creation and inform C++ about it.
  editor.addPipe(context => {
  if (context.type === 'nodecreated') {

    enqueueNode(() => OnNodeCreated(context.data, viewId));
    }

    return context;
  });

  // Listen for node deletion and inform C++ about it.
  editor.addPipe(context => {
  if (context.type === 'noderemoved') {

    enqueueNode(() => OnNodeRemoved(context.data));
    }

    return context;
  });

  function FindMatchingConnections(editor, json) {
    const matchingConnections = [];

    for (let connection of editor.connections) {

        const inputNodeId = connection.source;
        const outputNodeId = connection.target;

        const { source: sourceSocket, target: targetSocket } = GetConnectionSockets(editor, connection);

        const sourceVarName = (sourceSocket as any)?.meta?.var_name;
        const targetVarName = (targetSocket as any)?.meta?.var_name;

        // Check if the input or output matches any target socket.
        const isMatch = json.some(target =>
          (target.nodeId === inputNodeId && target.socketName === sourceVarName) ||
          (target.nodeId === outputNodeId && target.socketName === targetVarName)
        );

        if (isMatch) {
            matchingConnections.push(connection);
        }
    }

    return matchingConnections;
  } 

function HashString(str) {
    let hash = 0;
    if (str.length === 0) return hash;

    for (let i = 0; i < str.length; i++) {
        const char = str.charCodeAt(i);
        hash = ((hash << 5) - hash) + char;
        hash |= 0;
    }
    return hash;
}

  let lastHash : number = 0;
  let highlightedConnectionIds = new Set<string>();

  // Keep getting last active sockets from C++ to highlight them.
  async function GetLastActiveSockets() {

    if (getCurrentView().id != viewId) return;

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

    const ptr = Module.ccall(
        'GetLastActiveSockets',   
        'number',              
        [],       
        []
    );

    // Convert the pointer to a JavaScript string.
    const socketString = UTF8ToString(ptr);

    if (!socketString) return;

    // If the string didn't change, don't update highlighted connections.
    const hash = HashString(socketString);
    if(hash == lastHash) {
      return;
    } else {
      lastHash = hash;
    }

    try {
        const socketStringJson = JSON.parse(socketString);

        // Find matching connections in the Rete editor.
        const matchingConnections = FindMatchingConnections(editor, socketStringJson);

        // Update the global set of highlighted connection IDs.
        highlightedConnectionIds = new Set(matchingConnections.map(conn => conn.id));
        
        // Re-render the connections.
        for (const conn of editor.getConnections()) {
          area.update('connection', conn.id);
        }

    } catch (error) {
        console.error('Failed to parse active sockets JSON:', error);
    }

    // Free the allocated memory.
    Module.ccall(
        'FreeLastActiveSockets',
        null,                    
        ['number'],              
        [ptr]                    
    );
  }
  const intervalId = setInterval(GetLastActiveSockets, 1000 / 30);

  // Create default nodes.
  const outputCondNode = await AFNodeFactory.create("OutputCond", editor, false, ReteViewType.ConditionalGraph);

  // Wait for the first render (initially rete is hidden) to call zoom.
  const resizeObserver = new ResizeObserver((entries) => {
    for (const entry of entries) {
      const { width, height } = entry.contentRect;

      if (width > 0 && height > 0) {
        AreaExtensions.zoomAt(area, editor.getNodes(), { scale: 0.7 });
        resizeObserver.disconnect();
      }
    }
  });
  resizeObserver.observe(container);
  
  return {
      editor, selector, area, destroy: () => {
        clearInterval(intervalId);
        area.destroy()
      },
  };
}
