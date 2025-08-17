import { ClassicPreset as Classic, ClassicPreset, GetSchemes, NodeEditor } from 'rete';
import { Area2D, AreaExtensions, AreaPlugin, Drag } from 'rete-area-plugin';
import { ConnectionPlugin } from 'rete-connection-plugin';
import { ReactPlugin, ReactArea2D, Presets as ReactPresets } from 'rete-react-plugin';
import { createRoot } from 'react-dom/client';
import { AutoArrangePlugin, Presets as ArrangePresets } from 'rete-auto-arrange-plugin';
import { ContextMenuPlugin, ContextMenuExtra, Presets as ContextMenuPresets } from 'rete-context-menu-plugin';
import { ConnectionPathPlugin } from "rete-connection-path-plugin";
import { curveNatural } from "d3-shape";
import { ComputedSocketPosition } from './circle-socket';
import { CircleNode } from './editor/Node';
import { CircleSocket } from './editor/Socket';
import { CustomConnection } from './editor/Connection';
import { createSelector } from './selector';
import { UniPortConnector } from './connector';
import { Shape, ShapeProps } from './types';
import { pathTransformer, useTransformerUpdater } from './path';
import { setupSelection } from './selection';
import { addCustomBackground } from "./custom-background";
import styled from 'styled-components';
import { createView, getCurrentView } from './afmanager';
import { ReteViewType } from './aftypes';
import { OnNodeCreated, OnNodeRemoved, OnStateConnectionCreated, OnStateConnectionRemoved, setDetailsPanelVisible } from './affunclib';

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

type Schemes = GetSchemes<Node, Connection<Node, Node>>;

export async function create(label: string, editor: NodeEditor<Schemes>, nodeSelector: any, entry: boolean = false, connectionOwner: string = "", addNode: boolean = true) {

  const node = connectionOwner != "" ? new ConditionNode("") : (entry ? new StateNodeEntry(label) : new StateNode(""));  

  let nodeType = "Unknown";
  if (node instanceof ConditionNode) nodeType = "StateCond";
  else if (node instanceof StateNodeEntry) nodeType = "StateStart";
  else if (node instanceof StateNode) nodeType = "State";

  (node as any).meta = { 
    isEntry: entry, 
    titleEditable: !entry && connectionOwner.length == 0,
    isRemovable: !entry && connectionOwner.length == 0,
    connectionOwner: connectionOwner,
    isConditional: connectionOwner.length > 0,
    nodeSelect: nodeSelector,
    onDoubleClick: (currentTitle: string, nodeId: string) => {
       connectionOwner.length > 0 ? createView(currentTitle, nodeId, ReteViewType.ConditionalGraph) : 
       entry ? undefined : createView(currentTitle, nodeId, ReteViewType.Graph);
    },
    nodeFrom: undefined,
    nodeTo: undefined,
    editor: editor,
    type: nodeType
  };

  if(addNode) {
    await editor.addNode(node);
  }

  return node;
}

class Connection<A extends Node, B extends Node> extends Classic.Connection<
  A,
  B
> {
  selected?: boolean
  click: (data: Connection<A, B>) => void
  remove: (data: Connection<A, B>) => void

  constructor(events: { click: (data: Connection<A, B>) => void, remove: (data: Connection<A, B>) => void }, source: A, target: B, public isLoop?: boolean) {
    super(source, 'default', target, 'default')
    this.click = events.click
    this.remove = events.remove
  }
}

class Node extends Classic.Node {
  width = 160;
  height = 80;

  constructor(label: string, public shape: Shape = 'rect') {
    super(label);
  }
}

class StateNode extends Node {
   constructor(label: string, public shape: Shape = 'rect') {
    super(label);

      this.addInput('default', new Classic.Input(socket));
      this.addOutput('default', new Classic.Output(socket));
  }
}

class StateNodeEntry extends Node {
  width = 80;
  constructor(label: string, public shape: Shape = 'rect') {
    super(label);

    this.addOutput('default', new Classic.Output(socket));
  }
}

class ConditionNode extends Node {
  height = 20;
  width = 20;

  constructor(label: string, public shape: Shape = 'rect') {
    super(label);
  }
}

type AreaExtra = Area2D<Schemes> | ReactArea2D<Schemes> | ContextMenuExtra;

const socket = new Classic.Socket('socket');

export async function createEditorSM(container: HTMLElement, id: string) {

  const viewId = id;

  const editor      = new NodeEditor<Schemes>();

  // We need additional meta isLoading field to prevent from automatic conditional node creation during load. Load creates those by itself.
  (editor as any).meta ??= {};
  (editor as any).meta.isLoading = false;

  const area        = new AreaPlugin<Schemes, AreaExtra>(container);
  const connection  = new ConnectionPlugin<Schemes, AreaExtra>();
  const reactRender = new ReactPlugin<Schemes, AreaExtra>({ createRoot });

    // Context menu item type
    type Item = {
      label: string;
      key: string;
      handler: () => void;
    };
  
    // Context menu.
    const contextMenu = new ContextMenuPlugin<Schemes>({
      items: (context) => {
        if (context === "root") {
          return {
            list: [
              {
                label: "State",
                key: "state",
                handler: async () => {
                  const { x, y } = area.area.pointer;
                  const node = await create("State", editor, nodeSelector);
                  await area.translate(node.id, { x, y });
                }
              }
            ]
          };
        }

        return {
          list: []
        };
      }
    });

  // Context menu stylized.
  const { Menu, Common, Search, Item, Subitems } = ReactPresets.contextMenu;
  const CustomMenu = styled(Menu)`
  width: 180px;
  opacity: 0.9;
  background-image: linear-gradient(to right, rgba(85, 85, 85, 0.9), rgba(18, 18, 18, 0.9));
  border: 1px solid black;
  border-radius: 10px;
`
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
  const CustomSearch = styled(Search)`
    background:rgb(18, 18, 18);
    opacity: 0.9;
    border: 1px solid rgb(78, 78, 78);
    transition: border 0.2s ease;
    font-family: "Segoe UI", sans-serif;
    font-size: 10px;
    min-height: 28px;
    border-radius: 6px;

    &:hover,
    & input:hover {
      border: 1px solid rgb(67, 150, 238);
    }

    &:focus,
    & input:focus {
      outline: none;
      border: 1px solid rgb(67, 150, 238);
    }
  `
  const CustomSubitems = styled(Subitems)`
    background:rgb(18, 18, 18);
    opacity: 0.9;
    &:hover {
      background-image: linear-gradient(to right, rgba(85, 85, 85, 1.0), rgba(18, 18, 18, 1.0));
    }
    border: none;
  `
  reactRender.addPreset(ReactPresets.contextMenu.setup({
    customize: {
      main: () => CustomMenu,
      item: () => CustomItem,
      common: () => CustomCommon,
      search: () => CustomSearch,
      subitems: () => CustomSubitems
    }
  }));

  area.use(contextMenu);

  // Custom grid background.
  addCustomBackground(area);

  // Install Rete plugins.
  editor.use(area);
  area.use(reactRender);
  area.use(connection);

  // Make sockets circle-positioned.
  const socketPositionWatcher = new ComputedSocketPosition('circle');

  // Hook up custom connection-highlight selector.
  const connectionSelector = createSelector(area);
  const connectionEvents = {
    click:   data => connectionSelector.selectConnection(data),
    remove:  data => editor.removeConnection(data.id),
  };
  connection.addPreset(() => new UniPortConnector(connectionEvents));

  // Expose the connection creation.
  (editor as any).meta.createSMConnection = (source: Node, target: Node) => {
    return new Connection(connectionEvents, source as any, target as any);
  };

  // Allow only a single output on the Entry node.
  editor.addPipe(async ctx => {
    if (ctx.type === 'connectioncreate') {
      const conn    = ctx.data as Schemes['Connection'];
      const srcNode = editor.getNode(conn.source)!;
      const tgtNode = editor.getNode(conn.target)!;

      if (conn.source === conn.target)
        return;

      if ((tgtNode as any).meta?.isEntry)
        return;

      if ((srcNode as any).meta?.isEntry) {
        const existing = editor.getConnections()
                              .filter(c => c.source === conn.source && c.output === conn.output);
        if (existing.length > 0) {
          await editor.removeConnection(existing[0].id);
        }
      }
    }

    return ctx;
  });

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

  AreaExtensions.simpleNodesOrder(area);

setDetailsPanelVisible(editor, true, "");

  // Custom selector for custom events.
  class AFSelector extends AreaExtensions.Selector<any> {

    add(entity, accumulate) {

      if(this.isSelected(entity) && this.entities.size === 1) {

        return;
      }

      super.add(entity, accumulate);

      if(this.entities.size === 1) {
        setDetailsPanelVisible(editor, true, entity.id);
        const node = editor.getNode(entity.id);
        if (node?.meta?.valuesMap) {
        }
      } else {
        setDetailsPanelVisible(editor, false);
      }
    }
    
    remove(entity) {
      super.remove(entity);

      if(!this.isSelected(entity)) {
        setDetailsPanelVisible(editor, false);
      }
    }
  }
  const customSelector = new AFSelector();
  const nodeSelector = AreaExtensions.selectableNodes(area, customSelector, {
    accumulating: AreaExtensions.accumulateOnCtrl()
  });

  // Selection setup.
  const selection = setupSelection(area, {
    selected(ids) {
      customSelector.unselectAll();
        ids.forEach((id, i) => {
          nodeSelector.select(id, i !== 0);
        });
    },
  });

  selection.setShape('marquee');
  selection.setButton(0);

  // Don't translate conditional nodes.
  area.addPipe(ctx => {
    if (ctx.type === 'nodedragged') {
      const { id } = ctx.data
      const node = editor.getNode(id) as any

      if (node?.meta?.isConditional) {
        // prevent actual drag from moving the node

        // on drag end, force the owning connection to re-render,
        // which will call pathTransformer and snap the node back
        const connId = node.meta.connectionOwner
        const conn = connId && editor.getConnections().find(c => c.id === connId)
        if (conn) {
          // defer so we don't re-enter the same pipe synchronously
          queueMicrotask(() => area.update('connection', conn.id))
        }
        return
      }
    }
    return ctx
  })
  // Highlight states.
  let highlightRef: string[] = [];

  // Wrapper for CircleNode.
  function NodeWrapper(props: any) {
    return <CircleNode {...props} highlighted={highlightRef.includes(props.data.id)} />;
  }

  // Wrapper for CircleSocket.
  function SocketWrapper(context: { nodeId: string; key: string; side: string }) {
    const { nodeId, key, side } = context;
    return (props: any) => (
      <CircleSocket
        {...props}
        highlighted={highlightRef.includes(nodeId)}
        data-node-id={nodeId}
        data-socket-key={key}
        data-side={side}
      />
    );
  }

  // Register React presets once with stable component references
  reactRender.addPreset(ReactPresets.classic.setup({
    socketPositionWatcher,
    customize: {
      node: () => NodeWrapper,
      socket: (context) => SocketWrapper(context),
      connection: () => CustomConnection
    }
  }));
  reactRender.addPreset(ReactPresets.contextMenu.setup());

  // Enable dragging with right-mouse button.
  area.area.setDragHandler(new Drag({
    down: e => {
      if (e.pointerType === 'mouse' && e.button !== 2) return false

      e.preventDefault()
      return true
    },
    move: () => true
  }))

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

  // Node remove.
  editor.addPipe(async ctx => {
    if(ctx.type === "noderemove") {

      const node = ctx.data as Schemes['Node'];
      const connections = editor.getConnections();
          
      // Remove all connections where this node is source or target.
      for (const conn of connections) {
        if (
          conn.source === node.id ||
          conn.target === node.id  
        ) {
            await editor.removeConnection(conn.id);
          }
      }
    }

    return ctx;
  });

  // Connection create.
  editor.addPipe(async ctx => {
  if (ctx.type === 'connectioncreate') {
      const conn = ctx.data as Schemes['Connection'];

       // Prevent connecting node with itself.
      if (conn.source === conn.target) {
        return;
      }

      const sourceNode = editor.getNode(conn.source);
      const targetNode = editor.getNode(conn.target);

      if(sourceNode == undefined || targetNode == undefined) {
        return;
      }

      if ((editor as any).meta?.isLoading) {
        const existing = editor.getNodes().find(n => (n as any)?.meta?.connectionOwner === conn.id);
        OnStateConnectionCreated(
          viewId,
          sourceNode,
          targetNode,
          conn.id,
          existing ? existing.id : ""
        );
        return ctx;
      }

      let condNode;

      // Connecting normal state nodes creates a condition nodes inbetween them.
      if (editor.getNode(conn.source) instanceof StateNode && editor.getNode(conn.target) instanceof StateNode){
        condNode = await create("C", editor, nodeSelector, false, conn.id);
        (condNode as any).meta.nodeFrom = conn.source;
        (condNode as any).meta.nodeTo = conn.target;
      }

      OnStateConnectionCreated(viewId, sourceNode, targetNode, conn.id, condNode == undefined ? "" : condNode.id);
    }

    return ctx;
  });

  // Connection remove.
  editor.addPipe(async ctx => {
  if (ctx.type === 'connectionremoved') {
      const conn = ctx.data as Schemes['Connection'];

      // Removing a connection between normal nodes removes the conditional node assigned to the connection.
      if (editor.getNode(conn.source) instanceof StateNode && editor.getNode(conn.target) instanceof StateNode){
        let nodes = editor.getNodes();
        for(const node of nodes){
          const nodeCasted = node as any;

          // Do not remove the node when editor is destroying. The removal is handled from outside.
          if(!(editor as any).meta.isDestroying) {
            if(nodeCasted.meta?.connectionOwner == conn.id){
            editor.removeNode(node.id);
          }
          }
        }
      }

      OnStateConnectionRemoved(viewId, conn.id);
    }
    return ctx;
  });

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
  let highlightedState : string = "";

    // Keep getting last active state from C++ to highlight it.
    async function GetLastActiveState() {
  
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
          'GetLastActiveStates',   
          'number',              
          [],       
          []
      );
  
      // Convert the pointer to a JavaScript string.
      const stateString = UTF8ToString(ptr);
  
      if (!stateString) return;
  
      // If the string didn't change, don't update highlighted state.
      const hash = HashString(stateString);
      if(hash == lastHash) {
        return;
      } else {
        lastHash = hash;
        highlightRef = [];
      }
  
      try {
          const stateStringJson = JSON.parse(stateString);
          for(const item of stateStringJson) {
            const node = editor.getNode(item.nodeId);
            if(node) {
              highlightRef.push(node.id);
            }
          }
          
          // Re-render the nodes.
          for (const node of editor.getNodes()) {
            area.update('node', node.id);
          }
  
      } catch (error) {
          console.error('Failed to parse active states JSON:', error);
      }
  
      // Free the allocated memory.
      Module.ccall(
          'FreeLastActiveStates',
          null,                    
          ['number'],              
          [ptr]                    
      );
    }
    const intervalId = setInterval(GetLastActiveState, 1000 / 30);

  const pathPlugin = new ConnectionPathPlugin<Schemes, AreaExtra>({
    transformer: (connection) => pathTransformer(editor, area, connection),
    curve: () => curveNatural,
    arrow: () => false
  });

  useTransformerUpdater(editor, area);
  reactRender.use(pathPlugin);

  const arrange = new AutoArrangePlugin<Schemes>();
  const arrangeOptions = {
    top: 35,
  }

  arrange.addPreset(ArrangePresets.classic.setup({
    get top() {
      return arrangeOptions.top
    }
  }));

  area.use(arrange);
  await arrange.layout();
  AreaExtensions.simpleNodesOrder(area);

  const entry = await create("Entry", editor, nodeSelector, true);

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
      editor, selector: customSelector, area, destroy: () => {
        clearInterval(intervalId);
        area.destroy()
      },
  };
}
