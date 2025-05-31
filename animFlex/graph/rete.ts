import { createRoot } from "react-dom/client";
import { NodeEditor, GetSchemes, ClassicPreset } from "rete";
import { AreaPlugin, AreaExtensions, Drag } from "rete-area-plugin";
import { ConnectionPlugin, Presets as ConnectionPresets } from "rete-connection-plugin";
import { ReactPlugin, Presets, ReactArea2D } from "rete-react-plugin";
import { addCustomBackground } from "./custom-background";
import { AFNodeFactory } from './afnodefactory';
import { AFNode } from './afnode';
import { AFSocket } from './afsocket';
import { AFConnection, AFFlow } from './afconnection';

type Schemes = GetSchemes<ClassicPreset.Node, ClassicPreset.Connection<ClassicPreset.Node, ClassicPreset.Node>>;
type AreaExtra = ReactArea2D<Schemes>;

export async function createEditor(container: HTMLElement) {

  // Create plugins.
  const socket = new ClassicPreset.Socket("socket");
  const editor = new NodeEditor<Schemes>();
  const area = new AreaPlugin<Schemes, AreaExtra>(container);
  const connection = new ConnectionPlugin<Schemes, AreaExtra>();
  const render = new ReactPlugin<Schemes, AreaExtra>({ createRoot });

  // Enables node selection in the editor area.
  AreaExtensions.selectableNodes(area, AreaExtensions.selector(), {
    accumulating: AreaExtensions.accumulateOnCtrl(),
  });

  // Grid snapping.
  AreaExtensions.snapGrid(area, {size: 16, dynamic: true});

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
                return AFConnection;
            }
            }}));
    connection.addPreset(() => new AFFlow());

  // Custom grid background.
  addCustomBackground(area);

  // Register plugins.
  editor.use(area);
  area.use(connection);
  area.use(render);

  // Nodes layering.
  AreaExtensions.simpleNodesOrder(area);

  // Output pose node creation.
  //const b = new ClassicPreset.Node("B");
  //b.addInput("b", new ClassicPreset.Input(socket));
  //await editor.addNode(b);

  // Default connection.
 // await editor.addConnection(new ClassicPreset.Connection(a, "a", b, "b"));

  // Default positions.
  //await area.translate(b.id, { x: 270, y: 0 });

  // Create default nodes.
  const outputPoseNode = await AFNodeFactory.create("OutputPose", editor);
  const playSequenceNode = await AFNodeFactory.create("PlaySequence", editor);
  const fake = await AFNodeFactory.create("PlaySequence", editor);
  await area.translate(playSequenceNode.node.id, {x: -320, y: 0});
  await area.translate(fake.node.id, {x: -320, y: -160});

  // Enable dragging with right-mouse button.
  area.area.setDragHandler(new Drag({
    down: e => {
      if (e.pointerType === 'mouse' && e.button !== 2) return false

      e.preventDefault()
      return true
    },
    move: () => true
  }))

  // Don't show browser context menu ie. RMB.
  container.addEventListener('contextmenu', (e) => e.preventDefault());

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

  // Zoom the view to fit all nodes after 100ms.
  setTimeout(() => 
  {
    AreaExtensions.zoomAt(area, editor.getNodes(), { scale: 0.7 });
  }, 100);

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
    destroy: () => area.destroy(),
  };
}