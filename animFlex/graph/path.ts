import { BaseSchemes, ClassicPreset as Classic, NodeEditor } from 'rete';
import { AreaPlugin } from 'rete-area-plugin';
import { getPerpendicularOffset, Position } from './math';

// radius of your socket‐hit‐area
const SOCKET_RADIUS = 15;
// extra px so the arrow tip clears the node edge
const ARROW_MARGIN  = 12;

function getSiblingInfo(
  editor: NodeEditor<BaseSchemes>,
  connection: Classic.Connection<Classic.Node, Classic.Node>
) {
  const { source, target } = connection;
  const sibs = editor.getConnections().filter(c =>
    (c.source === source && c.target === target) ||
    (c.source === target && c.target === source)
  );
  return {
    total: sibs.length,
    index: sibs.indexOf(connection),
    siblings: sibs
  };
}

/**
 * Trims each endpoint by SOCKET_RADIUS (plus ARROW_MARGIN on the end side),
 * then fans out parallel edges by ±32px if needed.
 */
export function pathTransformer(
  editor: NodeEditor<BaseSchemes>,
  connection: Classic.Connection<Classic.Node, Classic.Node>
) {
  const { total, index } = getSiblingInfo(editor, connection);
  const spacing = 32;
  const mid     = (total - 1) / 2;
  const fanOff  = total > 1 ? (index - mid) * spacing : 0;

  return (pts: Position[]) => {
    if (pts.length < 2) return pts;

    // 1) raw endpoints
    let A = pts[0];
    let B = pts[pts.length - 1];

    // 2) unit vector A→B
    const dx = B.x - A.x, dy = B.y - A.y;
    const dist = Math.hypot(dx, dy) || 1;
    const ux = dx / dist, uy = dy / dist;

    // 3) trim the start by SOCKET_RADIUS, the end by SOCKET_RADIUS+ARROW_MARGIN
    A = {
      x: A.x + ux * SOCKET_RADIUS,
      y: A.y + uy * SOCKET_RADIUS
    };
    B = {
      x: B.x - ux * (SOCKET_RADIUS + ARROW_MARGIN),
      y: B.y - uy * (SOCKET_RADIUS + ARROW_MARGIN)
    };

    // 4) if there's no fan‐out, return a straight line
    if (fanOff === 0) {
      return [A, B];
    }

    // 5) otherwise shift both endpoints by the perpendicular offset
    const perpX = uy, perpY = -ux;
    const sign  = connection.source > connection.target ? -1 : 1;
    const off   = fanOff * sign;

    const A2 = { x: A.x + perpX * off, y: A.y + perpY * off };
    const B2 = { x: B.x + perpX * off, y: B.y + perpY * off };

    return [A2, B2];
  };
}

/**
 * After connections are created or removed, force all siblings
 * to re-render so they recalc their trimmed/fanned geometry.
 */
export function useTransformerUpdater<A>(
  editor: NodeEditor<BaseSchemes>,
  area: AreaPlugin<BaseSchemes, A>
) {
  editor.addPipe(async ctx => {
    if (ctx.type === 'connectioncreated' || ctx.type === 'connectionremoved') {
      const conn = ctx.data as Classic.Connection<Classic.Node, Classic.Node>;
      const { siblings } = getSiblingInfo(editor, conn);
      for (const c of siblings) {
        await area.update('connection', c.id);
      }
    }
    return ctx;
  });
}