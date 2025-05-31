import * as React from "react";
import { ClassicScheme, RenderEmit, Presets } from "rete-react-plugin";
import styled, { css } from "styled-components";
import * as AFNodeVars from './affunclib';

const { RefSocket, RefControl } = Presets.classic;

export const NodeStyles = styled.div <{
  selected: boolean; 
      styles?: (props: any) => any; 
      meta?: any;
}>`
  background: #121212;
  opacity: 0.9;
  border: 1px solid black;
  border-radius: 10px;
  cursor: move;
  box-sizing: border-box;
  width: ${(props) => `${props.meta.nodeWidth}px`};
  height: ${(props) => `${props.meta.nodeHeight}px`};
  padding-bottom: 6px;
  position: relative;
  user-select: none;
  display: flex;
  flex-direction: column;
  ${(props) =>
    props.selected &&
    css`
      border-color: white;
    `}
  .node-body {
    display: flex;
    flex-direction: column;
    justify-content: center;
    flex: 1
    }
  .node-big-icon {
    position: absolute;
    bottom: 4px;
    right: 4px;
    width: 100px;
    height: 100px;
    object-fit: contain;
    pointer-events: none;
    opacity: 0.8
  }
  .titleBar {
    background-image: linear-gradient(to right, rgba(85, 85, 85, 0.9), rgba(85, 85, 85, 0.3));
    border-top-left-radius: 10px;
    border-top-right-radius: 10px;
    z-index: 0;
    height: ${(props) => (props.meta.showSubTitle ? "40px" : "25px")};
  }
  .titleText {
    color: rgba(235, 235, 235, 0.93);
    font-family: "Segoe UI", sans-serif;
    font-weight: bold;
    font-style: normal;
    font-size: 12px;
    padding: 2px 10px
  }
   .subTitleText {
    color: rgba(170, 170, 170, 0.8);
    font-family: "Segoe UI", sans-serif;
    font-weight: normal;
    font-style: italic;
    font-size: 12px;
    padding: 2px 10px
  }
  .output {
    text-align: right;
    display: flex;
    flex-direction: row-reverse;
    gap: 10px;
  }
  .output.socketHovered {
    background: linear-gradient(to left, rgba(85, 85, 85, 0.9) 0%, rgba(85, 85, 85, 0.0) 30%);
  }
  .input {
    text-align: left;
    display: flex;
    flex-direction: row;
    gap: 6px;
  }
  .input.socketHovered {
    background: linear-gradient(to right, rgba(85, 85, 85, 0.9) 0%, rgba(85, 85, 85, 0.0) 30%);
  }
  .output-socket {
    text-align: right;
    margin-right: 10px;
    display: inline-block;
  }
  .input-socket {
    text-align: left;
    margin-left: 10px;
    display: inline-block;
  }
  .input-title,
  .output-title {
    vertical-align: middle;
    color: rgba(235, 235, 235, 0.93);
    font-size: 12px;
    line-height: ${AFNodeVars.$socketsize}px;
    font-family: "Segoe UI", Tahoma;
    margin-top: auto;
    margin-bottom: auto;
  }
  .input-control {
    z-index: 1;
    width: calc(100% - ${AFNodeVars.$socketsize + 2 * AFNodeVars.$socketmargin}px);
    vertical-align: middle;
    display: inline-block;
  }
  .control {
    display: block;
    padding: ${AFNodeVars.$socketmargin}px ${AFNodeVars.$socketsize / 2 + AFNodeVars.$socketmargin}px;
  }
  ${(props) => props.styles && props.styles(props)}
`;

function sortByIndex<T extends [string, undefined | { index?: number }][]>(
  entries: T
) {
  entries.sort((a, b) => {
    const ai = a[1]?.index || 0;
    const bi = b[1]?.index || 0;

    return ai - bi;
  });
}

type Props<S extends ClassicScheme> = {
  data: S["Node"] & NodeExtraData;
  styles?: () => any;
  emit: RenderEmit<S>;
};
export type NodeComponent<Scheme extends ClassicScheme> = (
  props: Props<Scheme>
) => JSX.Element;

type SocketHoverCallback = (socketId: string) => void;
type ConnectCallback = (sourceNodeId: string, targetNodeId: string) => void;

export const EventBus = {
    HoverStartListeners: new Set<SocketHoverCallback>(),
    HoverEndListeners: new Set<SocketHoverCallback>(),
    ConnectListeners: new Set<ConnectCallback>(),
    DisconnectListeners: new Set<ConnectCallback>(),

    OnSocketHoverStart(socketId: string) {
        for (const listener of this.HoverStartListeners) {
            listener(socketId);
        }
    },
    OnSocketHoverEnd(socketId: string) {
        for (const listener of this.HoverEndListeners) {
            listener(socketId);
        }
    },
    OnConnect(sourceNodeId: string, targetNodeId: string) {
        for (const listener of this.ConnectListeners) {
            listener(sourceNodeId, targetNodeId);
        }
    },
    OnDisconnect(sourceNodeId: string, targetNodeId: string) {
        for (const listener of this.DisconnectListeners) {
            listener(sourceNodeId, targetNodeId);
        }
    }
}
export function AFNode<Scheme extends ClassicScheme>(props: Props<Scheme>) {
  const inputs = Object.entries(props.data.inputs);
  const outputs = Object.entries(props.data.outputs);
  const controls = Object.entries(props.data.controls);
  const selected = props.data.selected || false;
  const { id, label, width, height } = props.data;
  const meta = (props.data as any).meta ?? {};

  sortByIndex(inputs);
  sortByIndex(outputs);
  sortByIndex(controls);

  // Listen for change in which socket is hovered to highlight it's input/output wrapper.
  const [hoveredSocketId, setHoveredSocketId] = React.useState<string | null>(null);
    React.useEffect(() => {
        const onHoverStart = (socketId: string) => {setHoveredSocketId(socketId)};
        const onHoverEnd = (_: string) => {setHoveredSocketId(null)};

        EventBus.HoverStartListeners.add(onHoverStart);
        EventBus.HoverEndListeners.add(onHoverEnd);

        return () => {
            EventBus.HoverStartListeners.delete(onHoverStart);
            EventBus.HoverEndListeners.delete(onHoverEnd);
        }
    }, []);

  return (
    <NodeStyles
      selected={selected}
      width={width}
      height={height}
      styles={props.styles}
      meta={meta}
      data-testid="node"
    >
      <div className="titleBar" data-testid="titleBar" style={{ display: "flex", flexDirection: "column", justifyContent: "center" }}>
        <div className="titleText" data-testid="titleText">
            {meta.title}
        </div>
        <div className="subTitleText" data-testid="subTitleText">
            {meta.subTitle}
        </div>
      </div>
      <div className="node-body">
      {/* Optional Big Icon */}
      {meta.bigIcon && meta.bigIcon_path && (
      <img
      src={meta.bigIcon_path}
      alt="node icon"
      className="node-big-icon"
      />
      )}
      {/* Outputs */}
      {outputs.map(
          ([key, output]) => {
            const uniqueId_output = React.useMemo(() => crypto.randomUUID(), []);

              return output ? (
               <div className={`output ${hoveredSocketId === uniqueId_output ? "socketHovered" : ""}`} 
               key={key} 
               data-testid={`${uniqueId_output}`}
               >
               <div
               style={{ display: 'inline-block' }}
               >
               <RefSocket
                name="output-socket"
                side="output"
                emit={props.emit}
                socketKey={key}
                nodeId={id}
                payload={output.socket}
                />
                </div>
              <span className="output-title" data-testid="output-title">
                {output?.label}
              </span>
            </div>
          ) : null
          }
      )}
      {/* Controls */}
      {controls.map(([key, control]) => {
        return control ? (
          <RefControl
            key={key}
            name="control"
            emit={props.emit}
            payload={control}
          />
        ) : null;
      })}
      {/* Inputs */}
      {inputs.map(
          ([key, input]) => {
            const uniqueId_input = React.useMemo(() => crypto.randomUUID(), []);

            return input ? (
            <div className={`input ${hoveredSocketId === uniqueId_input ? "socketHovered" : ""}`} 
            key={key} 
            data-testid={`${uniqueId_input}`}
            >
            <div
            style={{ display: 'inline-block' }}
            >
              <RefSocket
                name="input-socket"
                emit={props.emit}
                side="input"
                socketKey={key}
                nodeId={id}
                payload={input.socket}
              />
              </div>
              {input && (!input.control || !input.showControl) && (
                <span className="input-title" data-testid="input-title">
                  {input?.label}
                </span>
              )}
              {input?.control && input?.showControl && (
                <span className="input-control">
                  <RefControl
                    key={key}
                    name="input-control"
                    emit={props.emit}
                    payload={input.control}
                  />
                </span>
              )}
            </div>
          ) : null
          }
      )}
      </div>
    </NodeStyles>
  );
}
