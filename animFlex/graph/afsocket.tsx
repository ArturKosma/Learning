import * as React from "react";
import { ClassicPreset } from "rete";
import styled from "styled-components";
import * as AFNodeVars from './afnodevars';

const Styles = styled.div`
  display: inline-block;
  cursor: pointer;
  border: 1px solid grey;
  width: ${AFNodeVars.$socketsize}px;
  height: ${AFNodeVars.$socketsize * 2}px;
  vertical-align: middle;
  background: #fff;
  z-index: 2;
  box-sizing: border-box;
  &:hover {
    background: #ddd;
  }
`;
export function AFSocket<T extends ClassicPreset.Socket>(props: {
  data: T;
}) {
  return <Styles title={props.data.name} />;
}