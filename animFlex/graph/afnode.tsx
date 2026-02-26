import * as React from "react";
import { ClassicScheme, RenderEmit, Presets } from "rete-react-plugin";
import styled, { css } from "styled-components";
import * as AFNodeVars from './affunclib';

export const StyledInput = styled.input`
  width: 100%;
  background: transparent;
  color: rgba(235, 235, 235, 0.93);
  border: none;
  outline: none;
  font-family: "Segoe UI", sans-serif;
  font-weight: bold;
  font-size: 12px;
  user-select: text;
  caret-color: rgba(235, 235, 235, 0.93);

  &::placeholder {
    color: rgba(235, 235, 235, 0.2);
  }
  
  &::-webkit-input-placeholder {
    color: rgba(235, 235, 235, 0.2);
  }
  
  &::-moz-placeholder {
    color: rgba(235, 235, 235, 0.2);
  }
  
  &:-ms-input-placeholder {
    color: rgba(235, 235, 235, 0.2);
  }
  
  &::-ms-input-placeholder {
    color: rgba(235, 235, 235, 0.2);
  }
`;

const { RefSocket, RefControl } = Presets.classic;

export const NodeStyles = styled.div <{
  selected: boolean; 
      styles?: (props: any) => any; 
      meta?: any;
      classMeta?: string[];
}>`
  background: ${(props) => (props.meta.color)};
  opacity: 0.9;
  border: 1px solid black;
  border-radius: 10px;
  cursor: move;
  box-sizing: border-box;
  width: auto;
  min-width: ${(props) =>
    props.meta?.classMeta?.includes("Compact")
      ? "80px"
      : `${props.meta?.nodeWidth}px`};
  height: auto;
  min-height: ${(props) =>
    props.meta?.classMeta?.includes("Compact")
      ? "40px"
      : `${props.meta.nodeHeight}px`};
  padding-bottom: 0px;
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
    flex-direction: row;
    justify-content: space-between;
    flex-grow: 1;
    align-items: center;
    pointer-events: auto;
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
    background-image: ${(props) => (props.meta.titleBarColor)};
    border-top-left-radius: 10px;
    border-top-right-radius: 10px;
    z-index: 0;
    height: ${(props) => (props.meta.showSubTitle ? "40px" : "25px")};
    pointer-events: auto;
  }
  .titleText {
    color: rgba(235, 235, 235, 0.93);
    font-family: "Segoe UI", sans-serif;
    font-weight: bold;
    font-style: normal;
    font-size: 12px;
    padding: 2px 10px;
    user-select: text;
    cursor: ${(props) =>
       props.meta?.titleEditable ? 'text' : 'move'};
    pointer-events: auto;
  }
   .subTitleText {
    color: rgba(170, 170, 170, 0.8);
    font-family: "Segoe UI", sans-serif;
    font-weight: normal;
    font-style: italic;
    font-size: 12px;
    padding: ${(props) => (props.meta.showSubTitle ? "2px 10px" : "0px 10px")};
    pointer-events: none;
  }
  .output {
    text-align: right;
    display: flex;
    flex-direction: row-reverse;
    align-items: center;
    width: auto;
  }
  .output.socketHovered {
    background: linear-gradient(to left, rgba(85, 85, 85, 0.9) 0%, rgba(85, 85, 85, 0.0) 100%);
  }

  .output.socketHovered.float {
    background: linear-gradient(to left, rgba(134, 249, 52, 0.7) 0%, rgba(0, 185, 30, 0.0) 100%);
  }

  .output.socketHovered.bool {
    background: linear-gradient(to left, rgba(255, 0, 0, 0.7) 0%, rgba(255, 0, 0, 0.0) 100%);
  }

  .inputs-column {
    justify-content: flex-start;
    align-self: center;
    display: flex;
    flex-direction: column;
    padding-top: 6px;
    padding-bottom: 6px;
    gap: 2px;
  }
  .outputs-column {
    justify-content: flex-start;
    align-self: flex-start;
    display: flex;
    flex-direction: column;
    padding-top: 6px;
    padding-bottom: 6px;
    gap: 2px;
  }
  .input {
    text-align: left;
    display: flex;
    flex-direction: row;
    align-items: center;
    width: auto;
  }
  .input.socketHovered {
    background: linear-gradient(to right, rgba(85, 85, 85, 0.9) 0%, rgba(85, 85, 85, 0.0) 100%);
  }
  
  .input.socketHovered.float {
    background: linear-gradient(to right, rgba(134, 249, 52, 0.7) 0%, rgba(0, 140, 20, 0.0) 100%);
  }

  .input.socketHovered.bool {
    background: linear-gradient(to right, rgba(255, 0, 0, 0.7) 0%, rgba(255, 0, 0, 0.0) 100%);
  }

  .output-socket {
    text-align: right;
    margin-right: 0px;
    display: inline-block;
  }
  .input-socket {
    text-align: left;
    margin-left: 0px;
    display: inline-block;
  }
  .input-title,
  .output-title {
    color: rgba(235, 235, 235, 0.93);
    font-size: 12px;
    line-height: ${AFNodeVars.$socketsize}px;
    font-family: "Segoe UI", Tahoma;
    margin-top: auto;
    margin-bottom: auto;
  }
  .input-control {
    margin-left: 6px;
    display: inline-block;
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

    const [refreshFlag, setRefreshFlag] = React.useState(0);

    React.useEffect(() => {
    const rerender = () => setRefreshFlag(f => f + 1);

    EventBus.ConnectListeners.add(rerender);
    EventBus.DisconnectListeners.add(rerender);

    return () => {
      EventBus.ConnectListeners.delete(rerender);
      EventBus.DisconnectListeners.delete(rerender);
    };
  }, []);

  const [isEditing, setIsEditing] = React.useState(meta.titleEditable ? true : false);
  const [title, setTitle] = React.useState(meta.title || '');

  React.useEffect(() => {
  setTitle(meta.title || "");
}, [meta.title, props.data.id]);

const commitTitle = (next: string) => {
  setIsEditing(false);

  // preferred: call an updater if you have one
  if (typeof meta.onTitleChange === "function") {
    meta.onTitleChange(next, props.data.id);
  } else {
    // fallback: mutate and force a rerender
    meta.title = next;
    // if you have refreshFlag:
    // setRefreshFlag(f => f + 1);
  }
};

  React.useEffect(() => {
  const handleKeyDown = (e: KeyboardEvent) => {
    if (e.key === 'F2' && selected && !isEditing && meta.titleEditable) {
      setIsEditing(true);
    }
  };

  window.addEventListener('keydown', handleKeyDown);

  return () => {
    window.removeEventListener('keydown', handleKeyDown);
  };
}, [selected, isEditing, meta.titleEditable]);

const inputRef = React.useRef<HTMLInputElement>(null);

// Normalize bool-like strings/numbers to boolean
const toBool = (v: unknown) => {
  if (typeof v === 'boolean') return v;
  const s = String(v ?? '').trim().toLowerCase();
  return s === 'true' || s === '1';
};

const lastPtrRef = React.useRef<{ t: number; x: number; y: number } | null>(null);

const isDoublePointerDown = (e: React.PointerEvent) => {
  const now = performance.now();
  const cur = { t: now, x: e.clientX, y: e.clientY };
  const prev = lastPtrRef.current;
  lastPtrRef.current = cur;

  if (!prev) return false;

  const dt = now - prev.t;
  const dx = cur.x - prev.x;
  const dy = cur.y - prev.y;
  const dist2 = dx * dx + dy * dy;

  // tweak if you want: time window + allowed movement
  return dt < 300 && dist2 < 16; // 300ms, 4px
};

React.useEffect(() => {
  if (isEditing && inputRef.current) {
    const input = inputRef.current;  
    input.focus();
    const len = input.value.length;
    input.setSelectionRange(len, len);
  }
}, [isEditing]);

  return (
    <NodeStyles
      selected={selected}
      width={width}
      height={height}
      styles={props.styles}
      meta={meta}
      data-testid="node"
      title={`${id}`}
    >
      {!(meta.classMeta as string[])?.includes('Compact') && (
        <div className="titleBar" 
        data-testid="titleBar"
        {...(meta.titleEditable && {
          onPointerDownCapture: (e: React.PointerEvent) => e.stopPropagation(),
          onMouseDownCapture:   (e: React.MouseEvent)   => e.stopPropagation(),
        })} 
        style={{ display: "flex", 
        flexDirection: "column", 
        justifyContent: "center",}}>
          <div 
            className="titleText" 
            data-testid="titleText"
            {...(meta.titleEditable && {
              onMouseDown: (e: React.MouseEvent) => e.stopPropagation(),
              onClick:      () => setIsEditing(true),
            })}
            >
              {isEditing && meta.titleEditable ? (
                  <StyledInput
                    ref={inputRef}
                    type="text"
                    value={title}
                    placeholder="Graph Title"
                    onChange={(e) => {
                      setTitle(e.target.value);
                    }}
                    onBlur={(e) => {
                      commitTitle(e.currentTarget.value)
                    }}
                    onKeyDown={(e) => {
                      if (e.key === 'Enter') {
                        e.preventDefault();
                        commitTitle(e.currentTarget.value);
                      }
                    }}
                    onMouseDown={(e) => e.stopPropagation()}
                  />
                ) : meta.title}
          </div>
          <div className="subTitleText" data-testid="subTitleText">
              {meta.subTitle}
          </div>
        </div>
      )}
      <div 
      className="node-body"
      onPointerDownCapture={(e) => {
        // only react to left-click for mouse; allow pen/touch if you want
        if (e.pointerType === "mouse" && e.button !== 0) return;

        if (isDoublePointerDown(e)) {
          console.log("on double click body (pointer)");
          e.stopPropagation();
          meta.onDoubleClick?.(meta.title, props.data.id);
        }
      }}
      >
      {/* Optional Big Icon */}
      {meta.bigIcon && meta.bigIcon_path && (
      <img
      src={meta.bigIcon_path}
      alt="node icon"
      className="node-big-icon"
      />
      )}

      {inputs.length > 0 && (
        <div className="inputs-column">
      {/* Inputs */}
      {inputs.map(
          ([key, input]) => {

            let isHidden = input.socket.meta?.HidePin;
            let isControlHidden = input.socket.meta?.HideControl;
            const socketType = input.socket.meta?.socketType;

            // Show pin condition.
            const showKey = input.socket.meta?.Show as string | undefined;
            const valuesMap: Record<string, string> | undefined =
              (input.socket.meta?.node as any)?.meta?.valuesMap;
            if (showKey && valuesMap) {
              const condVal = valuesMap[showKey];
              const shouldShow = toBool(condVal);
              isHidden = !shouldShow;
            }

            return input ? (
            <div className={`input ${hoveredSocketId === key ? "socketHovered" : ""} ${socketType ?? ''}`} 
            key={key} 
            data-testid={`${key}`}
            >
          {!isHidden && (
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
              </div>)}
              {!isHidden && input?.label && (
                <span className="input-title">{input.label}</span>
              )}
              {input?.control && input?.showControl && !input.socket.meta?.isConnected && !isControlHidden && (
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
      )}

      {controls.length > 0 && (
        <div className="inputs-column">
        {/* Controls */}
          {controls.map(([key, control]) => {
            return control ? (
              <div className="input" key={key}>
                <span className="input-control">
                  <RefControl
                    key={key}
                    name="input-control"
                    emit={props.emit}
                    payload={control}
                  />
                </span>
              </div>
            ) : null;
          })}
        </div>
        )}
      <div style={{ flex: 1 }} />
      {outputs.length > 0 && (
        <div className="outputs-column">
      {/* Outputs */}
      {outputs.map(
          ([key, output]) => {

              let isHidden = output.socket.meta?.HidePin;
              const socketType = output.socket.meta?.socketType;

              // Show pin condition.
              const showKey = output.socket.meta?.Show as string | undefined;
              const valuesMap: Record<string, string> | undefined =
                (output.socket.meta?.node as any)?.meta?.valuesMap;
              if (showKey && valuesMap) {
                const condVal = valuesMap[showKey];
                const shouldShow = toBool(condVal);
                isHidden = !shouldShow;
              }

              return output ? (
               <div className={`output ${hoveredSocketId === key ? "socketHovered" : ""} ${socketType ?? ''}`} 
               key={key} 
               data-testid={`${key}`}
               >

              {!isHidden && (
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
              )}
              {!isHidden && (
                <span className="output-title" data-testid="output-title">
                {output?.label}
              </span>
              )}
            </div>
          ) : null
          }
      )}
      </div>
      )}
      </div>
    </NodeStyles>
  );
}
