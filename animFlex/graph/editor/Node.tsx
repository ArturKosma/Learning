import * as React from 'react'
import styled, { css } from 'styled-components'
import transitionRuleImgPath from '../transitionRule.png?inline';

import { RefComponent, ClassicScheme, RenderEmit, Presets } from 'rete-react-plugin'
import { ClassicPreset, GetSchemes, NodeEditor } from 'rete'
import { useCallback, useRef, useEffect } from 'react'
import { Shape } from '../types'
import { StyledInput } from '../afnode'

const { $nodecolor, $nodecolorselected, $nodewidth } = Presets.classic.vars

// --- override StyledInput so placeholder/text are centered ---
const CenteredInput = styled(StyledInput)`
  width: 100%;
  box-sizing: border-box;
  text-align: center;

  &::placeholder {
    text-align: center;
  }
`

type NodeExtraData = {
  width: number,
  height: number,
  meta?: any
}
type Schemes = GetSchemes<ClassicScheme['Node'] & NodeExtraData, ClassicScheme['Connection']>

const TitleText = styled.div`
  width: 100%;
  text-align: center;
  padding: 0 8px;
  box-sizing: border-box;
  overflow: hidden;
  white-space: nowrap;
  text-overflow: ellipsis;
`;

function getConditionalNodeLabel(editor: NodeEditor<Schemes>, nodeFrom: string, nodeTo: string) : string {
  return editor.getNode(nodeFrom)?.label + "->" + editor.getNode(nodeTo)?.label;
}

export const NodeStyles = styled.div<NodeExtraData & {
  selected: boolean,
  highlighted: boolean,
  shape?: Shape,
  styles?: (props: any) => any
}>`
  background: ${props => {
    if (props.meta?.isEntry) {
      return 'linear-gradient(to right, rgba(51, 51, 51, 1), rgba(85, 85, 85, 0.3))';
    } else if (props.meta?.isConditional) {
      return 'rgba(11, 11, 11, 0.85)';
    } else {
      return '#121212';
    }
  }};
  border: 1px solid black;
  border-radius: 10px;
  border-color: ${props => props.meta?.isConditional ? '#121212' : '#000000ff'};
  cursor: ${(props) =>
        props.meta?.isConditional ? 'pointer' : 'move'};
  box-sizing: border-box;
  width: ${props => Number.isFinite(props.width)
    ? `${props.width}px`
    : `${$nodewidth}px`};
  height: ${props => Number.isFinite(props.height)
    ? `${props.height}px`
    : 'auto'};
  position: relative;
  user-select: none;

  &:hover {
    background: lighten(${$nodecolor}, 4%);
  }

  ${props => props.selected && css`
    border-color: white;
  `}

  .content {
    display: flex;
    align-items: center;
    justify-content: center;
    width: 100%;
    height: 100%;
    border-radius: 4em;

    .title {
      color: rgba(235, 235, 235, 0.93);
      font-family: "Segoe UI", sans-serif;
      font-size: 12px;
      font-weight: bold;
      padding: 8px;
      cursor: ${(props) =>
        props.meta?.titleEditable ? 'text' : 'move'};
      pointer-events: auto;
    }
  }

  .socket {
    position: absolute;
    width: calc(100% + 20px);
    height: calc(100% + 20px);
    left: -10px;
    top: -10px;
    z-index: -1;
    border-radius: 0;
    overflow: hidden;
    box-sizing: border-box;
    cursor: pointer;
  }

  .conditionalImage {
    max-width: 100%;
    max-height: 100%;
    object-fit: contain;
    margin-top: 3px;
  }

  ${props => props.styles && props.styles(props)}
`

function sortByIndex<T extends [string, undefined | { index?: number }][]>(entries: T) {
  entries.sort((a, b) => {
    const ai = a[1]?.index ?? 0
    const bi = b[1]?.index ?? 0
    return ai - bi
  })
}

type Props<S extends Schemes> = {
  data: S['Node'] & { shape?: Shape }
  styles?: () => any
  emit: RenderEmit<S>
}

export function CircleNode<Scheme extends Schemes>(
  props: Props<Scheme> & { highlighted: boolean }
) {
  const { data, emit, styles, highlighted } = props
  const { id, inputs, outputs, controls, selected, width, height, label, meta } = data

  // local title state
  const [isEditing, setIsEditing] = React.useState(!!meta?.titleEditable)
  const [title,    setTitle]      = React.useState(data.label ?? "");

  React.useEffect(() => {
    setTitle(data.label ?? "");
  }, [data.label]);

  // ref + focus logic
  const inputRef = useRef<HTMLInputElement|null>(null)
  useEffect(() => {
    if (isEditing && inputRef.current) {
      inputRef.current.focus()
      const len = inputRef.current.value.length
      inputRef.current.setSelectionRange(len, len)
    }
  }, [isEditing])

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

  return dt < 300 && dist2 < 16;
};

  // sort sockets
  const inArr  = Object.entries(inputs)
  const outArr = Object.entries(outputs)
  const ctlArr = Object.entries(controls)
  sortByIndex(inArr)
  sortByIndex(outArr)
  sortByIndex(ctlArr)

  // socket rendering
const onRef = useCallback((ref: HTMLElement) => {
  const pairs: Array<['input' | 'output', ClassicPreset.Socket]> = [];

  const inSock  = inputs['default']?.socket;
  const outSock = outputs['default']?.socket;

  if (inSock)  pairs.push(['input',  inSock]);
  if (outSock) pairs.push(['output', outSock]);

  for (const [side, s] of pairs) {
    emit({ type: 'render',   data: { type: 'socket', side, key: 'default', nodeId: id, element: ref, payload: s } });
    requestAnimationFrame(() => {
      emit({ type: 'rendered', data: { type: 'socket', side, key: 'default', nodeId: id, element: ref, payload: s } });
    });
  }
}, [emit, id, inputs, outputs]);

  // save & exit edit
  const saveTitle = () => {
    setIsEditing(false)
    data.label = title
    emit({ type: 'process', node: data, id })
  }

  return (
    <NodeStyles selected={!!selected} 
                width={width} 
                height={height}
                shape={data.shape} 
                styles={styles}
                meta={meta}
                title={id}
                data-testid="node" 
                highlighted={props.highlighted}
                onPointerDown={(e) => {undefined}
                }
                onClick={(e) => {
                  //meta?.nodeSelect.select(data.id, false);
                }}
                onPointerDownCapture={e => {
                  if (isDoublePointerDown(e)) {
                    e.stopPropagation();
                    if (typeof meta.onDoubleClick === "function") {
                      meta.onDoubleClick(meta?.isConditional ? getConditionalNodeLabel(meta?.editor, meta?.nodeFrom, meta?.nodeTo) : data.label, props.data.id);
                    }
                  }
                }}
                >
      <div className="content">
        {meta?.isConditional ? (
          <div className="conditionalImage">
            <img src={transitionRuleImgPath}></img>
          </div>
        ) : (
          <div className="title" data-testid="title"
             {...(meta?.titleEditable && {
               onMouseDown:   (e: React.MouseEvent)   => e.stopPropagation(),
               onPointerDown: (e: React.PointerEvent) => e.stopPropagation(),
               onClick:       () => setIsEditing(true),
             })}>
          {isEditing && meta?.titleEditable ? (
            <CenteredInput
              ref={inputRef}
              type="text"
              value={title}
              placeholder="State Title"
              onChange={e => setTitle(e.target.value)}
              onBlur={saveTitle}
              onKeyDownCapture={e => {
                // on Enter, blur to trigger onBlur/saveTitle
                if (e.key === 'Enter') {
                  e.preventDefault()
                  inputRef.current?.blur()
                }
                e.stopPropagation()
              }}
              onMouseDown={e => e.stopPropagation()}
            />
          ) : (
            <TitleText>{title}</TitleText>
          )}
        </div>
        )}
      </div>

    {meta?.isConditional ? 
      (
        undefined
      ) 
      : 
      (
        <RefComponent
        className="socket"
        init={onRef}
        unmount={ref => emit({ type: 'unmount', data: { element: ref } })}
        data-testid="socket"
      />
      )}
    </NodeStyles>
  )
}