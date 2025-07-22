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
  area: AreaPlugin<BaseSchemes, A>,
  connection: Classic.Connection<Classic.Node, Classic.Node>
) {
  const { total, index } = getSiblingInfo(editor, connection);
  const spacing = 32;
  const mid = (total - 1) / 2;
  const fanOff = total > 1 ? (index - mid) * spacing : 0;

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

    // 4) apply perpendicular fan-out if needed
    let A2 = A;
    let B2 = B;
    if (fanOff !== 0) {
      const perpX = uy, perpY = -ux;
      const sign = connection.source > connection.target ? -1 : 1;
      const off = fanOff * sign;

      A2 = { x: A.x + perpX * off, y: A.y + perpY * off };
      B2 = { x: B.x + perpX * off, y: B.y + perpY * off };
    }

    // 5) Find node with meta.connectionOwner === connection.id
    const ownerNode = editor.getNodes().find(n => (n as any).meta?.connectionOwner === connection.id);
    if (ownerNode) {
      const x = A2.x + (B2.x - A2.x) * 0.25;
      const y = A2.y + (B2.y - A2.y) * 0.25;

      const centeredX = x - 20 / 2;
      const centeredY = y - 20 / 2;

      // Update its position.
      area.translate(ownerNode.id, {x: centeredX, y: centeredY});
    }

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