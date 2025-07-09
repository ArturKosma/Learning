import { ShapeProps } from './types'

export const LOOP_OFFSET = 65
export const LOOP_SCALE = 0.65

export const shapes: ShapeProps[] = [
    {
      label: 'Circle',
      value: 'circle',
      size: { width: 80, height: 80 }
    },
    {
      label: 'Ellipse',
      value: 'ellipse',
      size: { width: 140, height: 60 }
    },
    {
      label: 'Rectangle',
      value: 'rect',
      size: { width: 140, height: 60 }
    }
  ]