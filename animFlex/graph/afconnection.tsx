import * as React from "react";
import styled from "styled-components";
import { ClassicScheme, Presets } from "rete-react-plugin";
import { ClassicFlow } from "rete-connection-plugin";

const { useConnection } = Presets.classic;

const Svg = styled.svg`
  overflow: visible !important;
  position: absolute;
  pointer-events: none;
  width: 9999px;
  height: 9999px;
`;

const Path = styled.path<{ styles?: (props: any) => any }>`
  fill: none;
  stroke-width: 2px;
  stroke: rgba(190, 190, 190, 0.8);
  pointer-events: auto;
  ${(props) => props.styles && props.styles(props)}
`;

export function AFConnection(props: {
  data: ClassicScheme["Connection"] & { isLoop?: boolean };
  styles?: () => any;
}) {
  const { path } = useConnection();

  if (!path) return null;

  return (
    <Svg data-testid="connection">
      <Path styles={props.styles} d={path} />
    </Svg>
  );
}
export class AFFlow extends ClassicFlow<ClassicScheme, any[]> {

}
