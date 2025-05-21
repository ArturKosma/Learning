import * as React from "react";
import { ClassicScheme, RenderEmit, Presets } from "rete-react-plugin";
import styled, { css } from "styled-components";
import * as AFNodeVars from './afnodevars';

const { RefSocket, RefControl } = Presets.classic;

type NodeExtraData = { width?: number; height?: number };

export const NodeStyles = styled.div<
  NodeExtraData & { 
      selected: boolean; 
      styles?: (props: any) => any; 
      showTitle?: boolean;
      title?: string;
      showSubTitle?: boolean;
      subTitle?: string;
  }
>`
  background: #121212;
  opacity: 0.9;
  border: 1px solid black;
  border-radius: 10px;
  cursor: pointer;
  box-sizing: border-box;
  width: ${(props) =>
    Number.isFinite(props.width) ? `${props.width}px` : `${AFNodeVars.$nodewidth}px`};
  height: ${(props) =>
    Number.isFinite(props.height) ? `${props.height}px` : `${AFNodeVars.$nodeheight}px`};
  padding-bottom: 6px;
  position: relative;
  user-select: none;
  &:hover {
    background: #282828;
    opacity: 0.9;
  }
  ${(props) =>
    props.selected &&
    css`
      border-color: white;
    `}
  .titleBar {
    background-image: linear-gradient(to right, rgba(85, 85, 85, 0.9), rgba(85, 85, 85, 0.3));
    border-top-left-radius: 10px;
    border-top-right-radius: 10px;
    z-index: 0;
    height: ${(props) => (props.showSubTitle ? "40px" : "25px")};
  }
  .titleText {
    color: white;
    font-family: "Segoe UI", sans-serif;
    font-weight: bold;
    font-style: normal;
    font-size: 12px;
    padding: 2px 10px
  }
   .subTitleText {
    color: #AAAAAA;
    font-family: "Segoe UI", sans-serif;
    font-weight: normal;
    font-style: italic;
    font-size: 12px;
    padding: 2px 10px
  }
  .output {
    text-align: right;
  }
  .input {
    text-align: left;
  }
  .output-socket {
    text-align: right;
    margin-right: -1px;
    display: inline-block;
  }
  .input-socket {
    text-align: left;
    margin-left: -1px;
    display: inline-block;
  }
  .input-title,
  .output-title {
    vertical-align: middle;
    color: white;
    display: inline-block;
    font-family: sans-serif;
    font-size: 14px;
    margin: ${AFNodeVars.$socketmargin}px;
    line-height: ${AFNodeVars.$socketsize}px;
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

export function AFAnimGraphNode<Scheme extends ClassicScheme>(props: Props<Scheme>) {
  const inputs = Object.entries(props.data.inputs);
  const outputs = Object.entries(props.data.outputs);
  const controls = Object.entries(props.data.controls);
  const selected = props.data.selected || false;
  const { id, label, width, height } = props.data;
  const meta = (props.data as any).meta ?? {};
  const showTitle = meta.showTitle ?? true;
  const title = showTitle ? meta.title ?? "" : ""
  const showSubTitle = meta.showSubTitle ?? false;
  const subTitle = showSubTitle ? meta.subTitle ?? "" : ""

  sortByIndex(inputs);
  sortByIndex(outputs);
  sortByIndex(controls);

  return (
    <NodeStyles
      selected={selected}
      width={width}
      height={height}
      styles={props.styles}
      showTitle={showTitle}
      title={title}
      showSubTitle={showSubTitle}
      subTitle={subTitle}
      data-testid="node"
    >
      <div className="titleBar" data-testid="titleBar" style={{ display: "flex", flexDirection: "column", justifyContent: "center" }}>
        <div className="titleText" data-testid="titleText">
            {title}
        </div>
        <div className="subTitleText" data-testid="subTitleText">
            {subTitle}
        </div>
      </div>
      {/* Outputs */}
      {outputs.map(
        ([key, output]) =>
          output && (
            <div className="output" key={key} data-testid={`output-${key}`}>
              <div className="output-title" data-testid="output-title">
                {output?.label}
              </div>
              <RefSocket
                name="output-socket"
                side="output"
                emit={props.emit}
                socketKey={key}
                nodeId={id}
                payload={output.socket}
              />
            </div>
          )
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
        ([key, input]) =>
          input && (
            <div className="input" key={key} data-testid={`input-${key}`}>
              <RefSocket
                name="input-socket"
                emit={props.emit}
                side="input"
                socketKey={key}
                nodeId={id}
                payload={input.socket}
              />
              {input && (!input.control || !input.showControl) && (
                <div className="input-title" data-testid="input-title">
                  {input?.label}
                </div>
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
          )
      )}
    </NodeStyles>
  );
}
