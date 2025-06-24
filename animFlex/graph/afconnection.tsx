import * as React from "react";
import styled from "styled-components";
import { ClassicScheme, Presets } from "rete-react-plugin";
import { ClassicFlow } from "rete-connection-plugin";
import { ClassicPreset, GetSchemes, NodeEditor } from "rete";

const { useConnection } = Presets.classic;

const Svg = styled.svg`
  overflow: visible !important;
  position: absolute;
  pointer-events: none;
  width: 9999px;
  height: 9999px;
  z-index: 0;
`;

let highlighted = false;

const Path = styled.path<{ highlighted: boolean }>`
  fill: none;
  stroke-width: ${(p) => (p.highlighted ? "3px" : "2px")};
  stroke: ${(p) =>
    p.highlighted
      ? "rgb(255, 255, 255)"
      : "rgba(190, 190, 190, 0.8)"};
  pointer-events: none;
`;

export function AFConnection(props: {
  data: ClassicScheme["Connection"] & { isLoop?: boolean };
  styles?: () => any;
}) {
  const { path } = useConnection();
  const pathRef = React.useRef<SVGPathElement>(null);
  const circlesRef = React.useRef<SVGCircleElement[]>([]);

  // ideal spacing and speed
  const GAP_PX = 30;
  const SPEED_PX_PER_SEC = 100;

  const [dotCount, setDotCount] = React.useState(0);
  const actualGapRef = React.useRef<number>(GAP_PX);

  // measure path length and compute dotCount + actual gap
  React.useEffect(() => {
    if (!pathRef.current) return;
    const L = pathRef.current.getTotalLength();
    const count = Math.max(1, Math.floor(L / GAP_PX));
    setDotCount(count);
    actualGapRef.current = L / count;
  }, [path]);

  // animate dots along the path
  React.useEffect(() => {
    let rafId: number;
    let startTime: number | null = null;

    function animate(time: number) {
      if (!pathRef.current) return;
      if (startTime === null) startTime = time;
      const elapsedSec = (time - startTime) / 1000;
      const pathEl = pathRef.current;
      const L = pathEl.getTotalLength();
      const baseD = (elapsedSec * SPEED_PX_PER_SEC) % L;

      for (let i = 0; i < dotCount; i++) {
        const c = circlesRef.current[i];
        if (!c) continue;
        const dist = (baseD + i * actualGapRef.current) % L;
        const { x, y } = pathEl.getPointAtLength(dist);
        c.setAttribute("cx", x.toString());
        c.setAttribute("cy", y.toString());
      }

      rafId = requestAnimationFrame(animate);
    }

    rafId = requestAnimationFrame(animate);
    return () => cancelAnimationFrame(rafId);
  }, [dotCount, path]);

  if (!path) return null;

  return (
    <Svg data-testid="connection">
      <Path
        ref={pathRef}
        d={path}
        styles={props.styles}
        highlighted={highlighted}
      />

      {highlighted &&
        Array.from({ length: dotCount }).map((_, i) => (
          <circle
            key={i}
            r={3}
            fill="white"
            pointerEvents="none"
            ref={(el) => {
              circlesRef.current[i] = el!;
            }}
            cx={-10}
            cy={-10}
          />
        ))}
    </Svg>
  );
}

type Schemes = GetSchemes<
  ClassicPreset.Node,
  ClassicPreset.Connection<ClassicPreset.Node, ClassicPreset.Node>
>;

export class AFFlow extends ClassicFlow<ClassicScheme, any[]> {
  constructor(editor: NodeEditor<Schemes>) {
    super();
  }
}