import { useState } from 'react';
import { ClassicScheme, Presets } from 'rete-react-plugin';
import styled from 'styled-components';

const Svg = styled.svg`
  overflow: visible !important;
  position: absolute;
  pointer-events: none;
  width: 9999px;
  height: 9999px;
`;

const Path = styled.path<{ selected?: boolean }>`
  fill: none;
  stroke-width: ${p => (p.selected ? 3 : 2)}px;
  stroke: ${p =>
    p.selected ? 'rgb(255,255,255)' : 'rgba(190,190,190,1)'};
  pointer-events: none;
`;

const HoverPath = styled.path`
  fill: none;
  stroke-width: 15px;
  stroke: transparent;
  pointer-events: auto;
`;

const Button = styled.button`
  position: absolute;
  transform: translate(-50%,-50%);
  background: white;
  border: 0;
  font-size: 0.7em;
  width: 20px; height: 20px;
  border-radius: 1em;
  box-shadow: rgba(0,0,0,0.3) 0px 2px 10px;
  cursor: pointer;
`;

type Connection = ClassicScheme['Connection'] & {
  selected?: boolean;
  click?: (c: Connection) => void;
  remove?: (c: Connection) => void;
};

export function CustomConnection(props: { data: Connection }) {
  const { path } = Presets.classic.useConnection();
  const sel = !!props.data.selected;
  const [el, setEl] = useState<SVGPathElement|null>(null);

  const d = path ?? 'M0 0 L0 0';

  const markerId = `arrow-${props.data.id}`;

  return (
    <>
      <Svg>
        <defs>
          <marker
            id={markerId}
            viewBox="0 0 10 10"
            refX="2"                /* tail sits 2px before endpoint */
            refY="5"
            markerUnits="userSpaceOnUse"
            markerWidth="10" markerHeight="10"
            orient="auto"
          >
            <path
              d="M0,0 L10,5 L0,10 Z"
              fill={sel ? '#fff' : 'rgb(190,190,190)'}
              stroke="none"
            />
          </marker>
        </defs>

        {/* big transparent hit-target */}
        <HoverPath
          d={d}
          onPointerDown={e => e.stopPropagation()}
          onClick={() => props.data.click?.(props.data)}
        />

        {/* thin visible line with marker */}
        <Path
          d={d}
          selected={sel}
          markerEnd={`url(#${markerId})`}
          ref={setEl}
        />
      </Svg>

      {el && sel && (
        <ConnectionButton
          path={el}
          onClick={() => props.data.remove?.(props.data)}
        />
      )}
    </>
  );
}

function ConnectionButton(props: {
  path: SVGPathElement;
  onClick?: () => void;
}) {
  const len = props.path.getTotalLength();
  const pt  = props.path.getPointAtLength(len / 2);
  return (
    <Button
      onPointerDown={e => e.stopPropagation()}
      onClick={props.onClick}
      style={{ left: pt.x, top: pt.y }}
    >✕</Button>
  );
}