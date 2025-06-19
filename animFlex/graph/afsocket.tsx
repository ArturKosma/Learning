import * as React from "react";
import { ClassicPreset, NodeEditor } from "rete";
import styled from "styled-components";
import * as AFNodeVars from './affunclib';
import { EventBus } from "./afnode";
import { ClassicScheme } from "rete-react-plugin";
import { GraphUpdate } from './affunclib';

const Styles = styled.div`
  pointer-events: auto;
  display: block;
  cursor: pointer;
  width: ${AFNodeVars.$socketsize * 1.15}px;
  height: ${AFNodeVars.$socketsize}px;
  vertical-align: middle;
  z-index: 2;
  margin-top: auto;
  margin-down: auto;
  display: flex; 
  justify-content: center;    
  align-items: center;              
`;

function GetSocketFilter(type: string): string {
  switch (type) {
    case "float": // green
      return "brightness(0) saturate(100%) invert(82%) sepia(64%) saturate(527%) hue-rotate(39deg) brightness(98%) contrast(96%)";
    case "bool": // red
      return "brightness(0) saturate(100%) invert(26%) sepia(85%) saturate(4687%) hue-rotate(0deg) brightness(101%) contrast(102%)";
    case "string": // violet
      return "brightness(0) saturate(100%) invert(37%) sepia(88%) saturate(1100%) hue-rotate(279deg) brightness(95%) contrast(102%)";
    default:
      return "none";
  }
}

export function AFSocket<T extends ClassicPreset.Socket>(props: {
  data: T;
}) {

  const meta = (props.data as any).meta ?? {};
  const port = props.data as unknown as ClassicPreset.Socket;
  const editor = meta?.editor as NodeEditor<ClassicScheme>;

  // Listen for connections to occur to change the socket icon.
  const [connected, setConnected] = React.useState(false);
    React.useEffect(() => {
        if(!editor) return;

        editor.addPipe(context => {

            if (context.type === 'connectioncreated') {
              console.log("portname:", port.name)
              console.log("sourceOutput:", context.data.sourceOutput)
              console.log("targetInput:", context.data.targetInput)
                if (port.name === context.data.sourceOutput || port.name === context.data.targetInput) {
                    setConnected(true);
                 }
            }

            if (context.type === 'connectionremoved') {
                 if (port.name === context.data.sourceOutput || port.name === context.data.targetInput) {
                    setConnected(false);
                 }
            }

            return context
        })

        // Hacky way to prevent RMB-creation of connection when clicking on a socket.
        document.addEventListener("pointerdown", (e) => {
          if(e.button !== 0 && (e.target as HTMLElement)?.closest('.input, .output')) {
            e.stopPropagation();
            e.preventDefault();
          }
        }, {capture: true});

        return;
             
    }, []);

   return (
      <Styles 
          title={props.data.name}
          meta={meta}
          onMouseEnter={(e) => {
              const socketWrapper = e.currentTarget.closest('.input, .output');
              const socketWrapperId = socketWrapper?.getAttribute('data-testid');
              EventBus.OnSocketHoverStart(socketWrapperId);
          }}
          onMouseLeave={(e) => {
              const socketWrapper = e.currentTarget.closest('.input, .output');
              const socketWrapperId = socketWrapper?.getAttribute('data-testid')
              EventBus.OnSocketHoverEnd(socketWrapperId);
          }}
            
      >
       <img
          src={`${connected ? meta.socketIconConnected_path : meta.socketIconDisconnected_path}`}
          alt="socket icon"
          className="socket-icon"
          style={{
            filter: GetSocketFilter(meta.socketType)
          }}
          />
       </Styles> 
  );
}