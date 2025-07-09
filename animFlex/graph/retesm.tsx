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

// Stop any backspace keydown propagation from Rete.
// This allows using backspace in all input fields.
window.addEventListener('keydown', e => {
  if (e.key === 'Backspace') {
    e.stopImmediatePropagation();
  }
}, true);

type Schemes = GetSchemes<Node, Connection<Node, Node>>;

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
  width = 80;
  height = 80;

  constructor(label: string, public shape: Shape = 'rect') {
    super(label);

    this.addInput('default', new Classic.Input(socket));
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

  // Enable node selection.
  const reteSelector   = AreaExtensions.selector();
  AreaExtensions.simpleNodesOrder(area);
  const nodeSelector   = AreaExtensions.selectableNodes(area, reteSelector, {
    accumulating: AreaExtensions.accumulateOnCtrl()
  });
  const selection = setupSelection(area, {
    selected(ids) {
      reteSelector.unselectAll();
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

  const pathPlugin = new ConnectionPathPlugin<Schemes, AreaExtra>({
    transformer: (connection) => pathTransformer(editor, connection),
    curve: () => curveNatural,
    arrow: () => true
  });

  useTransformerUpdater(editor, area);
  reactRender.use(pathPlugin);

  const a = new Node('A');
  const b = new Node('B');
  const c = new Node('C');

  await editor.addNode(a);
  await editor.addNode(b);
  await editor.addNode(c);

  await editor.addConnection(new Connection(connectionEvents, a, c));
  await editor.addConnection(new Connection(connectionEvents, b, c));
  await editor.addConnection(new Connection(connectionEvents, c, c, true));

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
