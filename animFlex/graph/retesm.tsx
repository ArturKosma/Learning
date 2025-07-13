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
import { getCurrentView } from './afmanager';

type Schemes = GetSchemes<Node, Connection<Node, Node>>;

async function create(label: string, editor: NodeEditor<Schemes>, entry: boolean = false) {

  const node = entry ? new StateNodeEntry(label) : new StateNode(label);  
  (node as any).meta = { 
    isEntry: entry, 
    titleEditable: !entry,
    isRemovable: !entry
  };

  await editor.addNode(node);
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

type AreaExtra = Area2D<Schemes> | ReactArea2D<Schemes> | ContextMenuExtra;

const socket = new Classic.Socket('socket');

export async function createEditorSM(container: HTMLElement, id: string) {

  const viewId = id;

  const editor      = new NodeEditor<Schemes>();
  const area        = new AreaPlugin<Schemes, AreaExtra>(container);
  const connection  = new ConnectionPlugin<Schemes, AreaExtra>();
  const reactRender = new ReactPlugin<Schemes, AreaExtra>({ createRoot });
  const selector = AreaExtensions.selector();

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
                  const node = await create("State", editor);
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

  // Enable node selection.
  const nodeSelector = AreaExtensions.selectableNodes(area, selector, {
    accumulating: AreaExtensions.accumulateOnCtrl()
  });
  const selection = setupSelection(area, {
    selected(ids) {
      selector.unselectAll();
      ids.forEach((id,i) => nodeSelector.select(id, i!==0));
    }
  });
  selection.setShape('marquee');
  selection.setButton(0);

  // Register React presets.
  reactRender.addPreset(ReactPresets.classic.setup({
    socketPositionWatcher,
    customize: { node:()=>CircleNode,
                 socket:()=>CircleSocket,
                 connection:()=>CustomConnection }
  }));
  reactRender.addPreset(ReactPresets.classic.setup());
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

  // Prevent connecting node with itself.
  editor.addPipe(ctx => {
  if (ctx.type === 'connectioncreate') {
      const conn = ctx.data as Schemes['Connection'];
      if (conn.source === conn.target) {
        return;
      }
    }
    return ctx;
  });

  const pathPlugin = new ConnectionPathPlugin<Schemes, AreaExtra>({
    transformer: (connection) => pathTransformer(editor, connection),
    curve: () => curveNatural,
    arrow: () => false
  });

  useTransformerUpdater(editor, area);
  reactRender.use(pathPlugin);

  const entry = await create("Entry", editor, true);
  const a = await create("A", editor);
  const b = await create("B", editor);

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

  // Enable node deletion on Delete key.
  const onDeleteKey = async (e: KeyboardEvent) => {
    if (e.key === 'Delete') {
      for (const selectedEntity of selector.entities.values()) {
          const node = editor.getNode(selectedEntity.id) as ClassicPreset.Node & {
            meta?: {isRemovable?: boolean}
          }
          if (node && node.meta?.isRemovable) {
            await editor.removeNode(selectedEntity.id);
          }
        e.preventDefault();
        e.stopImmediatePropagation();
      }
    } 
  }

  window.addEventListener('keydown', onDeleteKey, {capture: true});

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
      editor, selector, destroy: () => {
        area.destroy()
      },
  };
}
