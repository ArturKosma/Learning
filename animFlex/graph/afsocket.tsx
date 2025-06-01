import * as React from "react";
import { ClassicPreset, NodeEditor } from "rete";
import styled from "styled-components";
import * as AFNodeVars from './affunclib';
import { EventBus } from "./afnode";
import { ClassicScheme } from "rete-react-plugin";
import { Drag } from "rete-react-plugin";

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
          />
       </Styles> 
  );
}