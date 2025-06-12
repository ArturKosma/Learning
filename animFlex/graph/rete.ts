import { createRoot } from "react-dom/client";
import { NodeEditor, GetSchemes, ClassicPreset } from "rete";
import { AreaPlugin, AreaExtensions, Drag } from "rete-area-plugin";
import { ConnectionPlugin, Presets as ConnectionPresets } from "rete-connection-plugin";
import { ReactPlugin, Presets, ReactArea2D } from "rete-react-plugin";
import { addCustomBackground } from "./custom-background";
import { AFNodeFactory } from './afnodeFactory';
import { AFNode } from './afnode';
import { AFSocket } from './afsocket';
import { AFConnection, AFFlow } from './afconnection';
import {
  ContextMenuExtra,
  ContextMenuPlugin,
  Presets as ContextMenuPresets
} from "rete-context-menu-plugin";
import styled from "styled-components";
import {DropdownControl, CustomDropdown} from './afdropdown'; 
import {GraphUpdate} from './affunclib'

type Schemes = GetSchemes<ClassicPreset.Node, ClassicPreset.Connection<ClassicPreset.Node, ClassicPreset.Node>>;
type AreaExtra = ReactArea2D<Schemes> | ContextMenuExtra;

export async function createEditor(container: HTMLElement) {

  await GraphUpdate(); 

  // Create plugins.
  const editor = new NodeEditor<Schemes>();
  const area = new AreaPlugin<Schemes, AreaExtra>(container);
  const connection = new ConnectionPlugin<Schemes, AreaExtra>();
  const render = new ReactPlugin<Schemes, AreaExtra>({ createRoot });
  const selector = AreaExtensions.selector();;
  const contextMenu = new ContextMenuPlugin<Schemes>({
    items: ContextMenuPresets.classic.setup([
      ["Play Sequence", () => AFNodeFactory.create("PlaySequence", editor, true).node]
    ])
  });

  // Enables node selection in the editor area.
  AreaExtensions.selectableNodes(area, selector, {
    accumulating: AreaExtensions.accumulateOnCtrl(),
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
            },
            control(data) {
                if(data.payload instanceof DropdownControl) {
                  return CustomDropdown;
                }
            }
            }}));
    connection.addPreset(() => new AFFlow(editor));

  // Context menu stylized.
  const { Menu, Common, Search, Item, Subitems } = Presets.contextMenu
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

  // Create default nodes.
  const outputPoseNode = await AFNodeFactory.create("OutputPose", editor);

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

  if (distance > 40) {
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

  // Bind custom keys.
  window.addEventListener('keydown', async (e) => {

      // Delete for nodes deletion.
      if(e.key === 'Delete') {
        for (const selectedEntity of selector.entities.values()) {
        const node = editor.getNode(selectedEntity.id) as ClassicPreset.Node & {
          meta?: {isRemovable?: boolean}
        };
        if(node?.meta?.isRemovable) {
          await editor.removeNode(selectedEntity.id);
        }
      }
      }

      // Pass backspace to text editor in order to delete characters.
      if(e.key === "Backspace") {

        const el = document.activeElement as HTMLInputElement | HTMLTextAreaElement;
        if(!el) {
          return;
        }

        if (el.tagName === 'INPUT') {
        
          const start = el.selectionStart ?? 0;
          const end = el.selectionEnd ?? 0;

          if (start === end && start > 0) {
            el.value = el.value.slice(0, start - 1) + el.value.slice(end);
            el.selectionStart = el.selectionEnd = start - 1;
          } else {
            el.value = el.value.slice(0, start) + el.value.slice(end);
            el.selectionStart = el.selectionEnd = start;
          }

          el.dispatchEvent(new Event('input', { bubbles: true }));
        }
    }
  }, {capture: true})

  // Listen for new connections/disconnections.
  editor.addPipe(context => {
  if (context.type === 'connectioncreated' || context.type === 'connectionremoved') {
      GraphUpdate();
  }

  return context;
});

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