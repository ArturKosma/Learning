import { AreaPlugin, Zoom } from "rete-area-plugin";
import anime from "animejs/lib/anime.es.js";

function screenToArea(x: number, y: number, t: any) {
  const { x: tx, y: ty, k } = t;

  return { x: (x - tx) / k, y: (y - ty) / k };
}

function areaToScreen(x: number, y: number, t: any) {
  const { x: tx, y: ty, k } = t;

  return { x: x * k + tx, y: y * k + ty };
}

export class SmoothZoom extends Zoom {
  private currentZoom = 1;
  private targetZoom = 1;
  private animationFrame?: number;

  constructor(
    intensity: number,
    private duration: number,
    private easing: string,
    private area: AreaPlugin<any, any>
  ) {
    super(intensity);
  }

  private easeOutCubic(t: number) {
    return 1 - Math.pow(1 - t, 3);
  }

  wheel = (e: WheelEvent) => {
    e.preventDefault();

   if (e.ctrlKey) return false;

    const delta = -Math.sign(e.deltaY) * 0.1;
    const newScale = this.area.transform.scale + delta;

    const clamped = Math.min(1.5, Math.max(0.2, newScale)); // clamp to limits
    const { x, y } = this.area.screenToPoint(e.clientX, e.clientY);

    this.area.zoom(clamped, { x, y }, 'custom');
    return true;
  };

  destroy() {
    super.destroy();
    if (this.animationFrame) cancelAnimationFrame(this.animationFrame);
  }
}
