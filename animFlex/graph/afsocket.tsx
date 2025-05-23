import * as React from "react";
import { ClassicPreset } from "rete";
import styled from "styled-components";
import * as AFNodeVars from './afnodevars';

const Styles = styled.div`
  display: block;
  cursor: pointer;
  width: ${AFNodeVars.$socketsize}px;
  height: ${AFNodeVars.$socketsize * 2}px;
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

  return (
      <Styles title={props.data.name} 
      meta={meta}
      >
       <img
          src={meta.socketIconDisconnected_path}
          alt="socket icon"
          className="socket-icon"
          />
       </Styles>
  );
}