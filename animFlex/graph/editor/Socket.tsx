import * as React from 'react'
import { ClassicPreset } from 'rete'
import styled from 'styled-components'

const Styles = styled.div<{highlighted: boolean}>`
  display: inline-block;
  cursor: pointer;

  border-radius: 10px;

  width: 100%;
  height: 100%;
  vertical-align: middle;
  background: ${(p) => p.highlighted ? 'rgba(221, 255, 0, 0.4)' : 'rgba(180, 180, 180, 0.2)'};
  z-index: 2;
  box-sizing: border-box;

  &:hover {
    background: rgba(255, 255, 255, 0.2);
    background: ${(p) => p.highlighted ? 'rgba(221, 255, 0, 0.6)' : 'rgba(255, 255, 255, 0.2)'};
  }
`

export function CircleSocket<T extends ClassicPreset.Socket>(props: { 
  data: T, 
  highlighted: boolean
}) {
  return <Styles title={props.data.name} highlighted={props.highlighted} />
}