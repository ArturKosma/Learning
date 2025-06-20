import React from "react";
import styled from "styled-components";
import { ClassicPreset, NodeEditor } from "rete";
import { EventBus } from "./afnode";
import { ClassicScheme } from "rete-react-plugin";
import * as AFNodeVars from './affunclib';

import pin_connected from './pin_connected.png?inline';
import pin_disconnected from './pin_disconnected.png?inline';
import pinPose_connected from './pinPose_connected.png?inline';
import pinPose_disconnected from './pinPose_disconnected.png?inline';

export function preloadPins() {
  if (memoryImages) return;
}

export const memoryImages = [pin_connected, pin_disconnected, pinPose_connected, pinPose_disconnected].map(src => {
  const img = new Image();
  img.src = src;
  return img;
});

const Styles = styled.div`
  pointer-events: auto;
  display: flex;
  justify-content: center;
  align-items: center;
  cursor: pointer;
  width: ${AFNodeVars.$socketsize * 1.15}px;
  height: ${AFNodeVars.$socketsize}px;
  z-index: 2;
`;

function getIconPath(type: string, connected: boolean) {
  if (type === "Pose") return connected ? pinPose_connected : pinPose_disconnected;
  return connected ? pin_connected : pin_disconnected;
}

function getFilter(type: string) {
  switch (type) {
    case "float":
      return "brightness(0) saturate(100%) invert(82%) sepia(64%) saturate(527%) hue-rotate(39deg) brightness(98%) contrast(96%)";
    case "bool":
      return "brightness(0) saturate(100%) invert(26%) sepia(85%) saturate(4687%) hue-rotate(0deg) brightness(101%) contrast(102%)";
    case "string":
      return "brightness(0) saturate(100%) invert(37%) sepia(88%) saturate(1100%) hue-rotate(279deg) brightness(95%) contrast(102%)";
    default:
      return "none";
  }
}

export function AFSocket<T extends ClassicPreset.Socket>({ data }: { data: T }) {
  const meta = (data as any).meta ?? {};
  const port = data as unknown as ClassicPreset.Socket;
  const editor = meta?.editor as NodeEditor<ClassicScheme>;

  const [connected, setConnected] = React.useState(false);

  React.useEffect(() => {
    if (!editor) return;

    const pipe = (context: any) => {
      if (
        context.type === 'connectioncreated' &&
        (port.name === context.data.sourceOutput || port.name === context.data.targetInput)
      ) {
        setConnected(true);
        meta.isConnected = true;
        EventBus.OnConnect(meta.node.id, port.name);
      }

      if (
        context.type === 'connectionremoved' &&
        (port.name === context.data.sourceOutput || port.name === context.data.targetInput)
      ) {
        setConnected(false);
        meta.isConnected = false;
        EventBus.OnDisconnect(meta.node.id, port.name);
      }

      return context;
    };

    editor.addPipe(pipe);

    return () => {
      editor.removePipe(pipe);
    };
  }, [editor, port.name]);

  const icon = React.useMemo(() => getIconPath(meta.socketType, connected), [meta.socketType, connected]);

  return (
    <Styles
      title={data.name}
      onMouseEnter={(e) => {
        const socketWrapper = e.currentTarget.closest('.input, .output');
        const socketWrapperId = socketWrapper?.getAttribute('data-testid');
        EventBus.OnSocketHoverStart(socketWrapperId);
      }}
      onMouseLeave={(e) => {
        const socketWrapper = e.currentTarget.closest('.input, .output');
        const socketWrapperId = socketWrapper?.getAttribute('data-testid');
        EventBus.OnSocketHoverEnd(socketWrapperId);
      }}
    >
      <img
        src={icon}
        alt="socket icon"
        className="socket-icon"
        style={{
          filter: getFilter(meta.socketType)
        }}
      />
    </Styles>
  );
}