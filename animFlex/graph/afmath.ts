export type Position = { x: number; y: number };

/**
 * Given two points A and B and an offset distance, returns a point
 * that is offset perpendicular to the line AB by the given distance,
 * centered at the midpoint of AB.
 */
export function getPerpendicularOffset(
  A: Position,
  B: Position,
  offset: number
): Position {
  const dx = B.x - A.x;
  const dy = B.y - A.y;
  const dist = Math.hypot(dx, dy) || 1;
  // unit direction vector from A to B
  const ux = dx / dist;
  const uy = dy / dist;
  // perpendicular unit vector (rotated 90° clockwise)
  const px = uy;
  const py = -ux;
  // midpoint of A and B
  const mx = (A.x + B.x) / 2;
  const my = (A.y + B.y) / 2;
  // apply offset along perpendicular
  return {
    x: mx + px * offset,
    y: my + py * offset
  };
}