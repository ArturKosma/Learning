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

const Path = styled.path<{ highlighted: boolean; socketType: string }>`
  fill: none;
  stroke-width: ${(p) => (p.highlighted ? "3px" : "2px")};
  stroke: ${(p) => {
    if (p.highlighted) return "rgb(255, 255, 255)";

    if (p.socketType === "float") return "rgba(134, 249, 52, 0.8)";
    if (p.socketType === "bool") return "rgba(255, 0, 0, 0.8)";
    if (p.socketType === "string") return "rgba(147, 0, 139, 0.8)";
    if (p.socketType === "vector") return "rgba(199, 199, 0, 0.85)";

    return "rgba(190, 190, 190, 0.8)";
  }};
  pointer-events: none;
`;

export function AFConnection(props: {
  data: ClassicScheme["Connection"] & { isLoop?: boolean };
  styles?: () => any;
  highlighted: boolean;
  socketType: string;
}) {
  const { path } = useConnection();
  if (!path) return null;

  return (
    <Svg data-testid="connection">
      <Path
        d={path}
        highlighted={props.highlighted}
        socketType={props.socketType}
      />
    </Svg>
  );
}

type Schemes = GetSchemes< ClassicPreset.Node, ClassicPreset.Connection<ClassicPreset.Node, ClassicPreset.Node> >;

export class AFFlow extends ClassicFlow<ClassicScheme, any[]> {
  constructor(editor: NodeEditor<Schemes>) {
    super();
  }
}