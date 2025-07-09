export type Shape = 'ellipse' | 'circle' | 'rect'
export type ShapeProps = { label: string, value: Shape, size: Size }
export type Size = { width: number, height: number }