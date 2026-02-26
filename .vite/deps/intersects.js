import {
  __commonJS
} from "./chunk-PLDDJCW6.js";

// node_modules/intersects/circle-point.js
var require_circle_point = __commonJS({
  "node_modules/intersects/circle-point.js"(exports, module) {
    "use strict";
    module.exports = function circlePoint(x1, y1, r1, x2, y2) {
      var x = x2 - x1;
      var y = y2 - y1;
      return x * x + y * y <= r1 * r1;
    };
  }
});

// node_modules/intersects/circle-circle.js
var require_circle_circle = __commonJS({
  "node_modules/intersects/circle-circle.js"(exports, module) {
    "use strict";
    module.exports = function circleCircle(x1, y1, r1, x2, y2, r2) {
      var x = x1 - x2;
      var y = y2 - y1;
      var radii = r1 + r2;
      return x * x + y * y <= radii * radii;
    };
  }
});

// node_modules/intersects/line-circle.js
var require_line_circle = __commonJS({
  "node_modules/intersects/line-circle.js"(exports, module) {
    "use strict";
    module.exports = function lineCircle(x1, y1, x2, y2, xc, yc, rc) {
      var ac = [xc - x1, yc - y1];
      var ab = [x2 - x1, y2 - y1];
      var ab2 = dot(ab, ab);
      var acab = dot(ac, ab);
      var t = acab / ab2;
      t = t < 0 ? 0 : t;
      t = t > 1 ? 1 : t;
      var h = [ab[0] * t + x1 - xc, ab[1] * t + y1 - yc];
      var h2 = dot(h, h);
      return h2 <= rc * rc;
    };
    function dot(v1, v2) {
      return v1[0] * v2[0] + v1[1] * v2[1];
    }
  }
});

// node_modules/intersects/circle-line.js
var require_circle_line = __commonJS({
  "node_modules/intersects/circle-line.js"(exports, module) {
    "use strict";
    var lineCircle = require_line_circle();
    module.exports = function circleLine(xc, yc, rc, x1, y1, x2, y2) {
      return lineCircle(x1, y1, x2, y2, xc, yc, rc);
    };
  }
});

// node_modules/intersects/box-circle.js
var require_box_circle = __commonJS({
  "node_modules/intersects/box-circle.js"(exports, module) {
    "use strict";
    module.exports = function boxCircle(xb, yb, wb, hb, xc, yc, rc) {
      var hw = wb / 2;
      var hh = hb / 2;
      var distX = Math.abs(xc - (xb + wb / 2));
      var distY = Math.abs(yc - (yb + hb / 2));
      if (distX > hw + rc || distY > hh + rc) {
        return false;
      }
      if (distX <= hw || distY <= hh) {
        return true;
      }
      var x = distX - hw;
      var y = distY - hh;
      return x * x + y * y <= rc * rc;
    };
  }
});

// node_modules/intersects/circle-box.js
var require_circle_box = __commonJS({
  "node_modules/intersects/circle-box.js"(exports, module) {
    "use strict";
    var boxCircle = require_box_circle();
    module.exports = function circleBox(xc, yc, rc, xb, yb, wb, hb) {
      return boxCircle(xb, yb, wb, hb, xc, yc, rc);
    };
  }
});

// node_modules/intersects/line-point.js
var require_line_point = __commonJS({
  "node_modules/intersects/line-point.js"(exports, module) {
    "use strict";
    function distanceSquared(x1, y1, x2, y2) {
      return Math.sqrt(Math.pow(x1 - x2, 2) + Math.pow(y1 - y2, 2));
    }
    module.exports = function linePoint(x1, y1, x2, y2, xp, yp, tolerance) {
      tolerance = tolerance || 1;
      return Math.abs(distanceSquared(x1, y1, x2, y2) - (distanceSquared(x1, y1, xp, yp) + distanceSquared(x2, y2, xp, yp))) <= tolerance;
    };
  }
});

// node_modules/intersects/polygon-point.js
var require_polygon_point = __commonJS({
  "node_modules/intersects/polygon-point.js"(exports, module) {
    "use strict";
    var linePoint = require_line_point();
    module.exports = function polygonPoint(points, x, y, tolerance) {
      var length = points.length;
      var c = false;
      var i, j;
      for (i = 0, j = length - 2; i < length; i += 2) {
        if (points[i + 1] > y !== points[j + 1] > y && x < (points[j] - points[i]) * (y - points[i + 1]) / (points[j + 1] - points[i + 1]) + points[i]) {
          c = !c;
        }
        j = i;
      }
      if (c) {
        return true;
      }
      for (i = 0; i < length; i += 2) {
        var p1x = points[i];
        var p1y = points[i + 1];
        var p2x, p2y;
        if (i === length - 2) {
          p2x = points[0];
          p2y = points[1];
        } else {
          p2x = points[i + 2];
          p2y = points[i + 3];
        }
        if (linePoint(p1x, p1y, p2x, p2y, x, y, tolerance)) {
          return true;
        }
      }
      return false;
    };
  }
});

// node_modules/intersects/polygon-circle.js
var require_polygon_circle = __commonJS({
  "node_modules/intersects/polygon-circle.js"(exports, module) {
    var polygonPoint = require_polygon_point();
    var lineCircle = require_line_circle();
    module.exports = function polygonCircle(points, xc, yc, rc, tolerance) {
      if (polygonPoint(points, xc, yc, tolerance)) {
        return true;
      }
      var count = points.length;
      for (var i = 0; i < count - 2; i += 2) {
        if (lineCircle(points[i], points[i + 1], points[i + 2], points[i + 3], xc, yc, rc)) {
          return true;
        }
      }
      return lineCircle(points[0], points[1], points[count - 2], points[count - 1], xc, yc, rc);
    };
  }
});

// node_modules/intersects/circle-polygon.js
var require_circle_polygon = __commonJS({
  "node_modules/intersects/circle-polygon.js"(exports, module) {
    "use strict";
    var polygonCircle = require_polygon_circle();
    module.exports = function circlePolygon(xc, yc, rc, points, tolerance) {
      return polygonCircle(points, xc, yc, rc, tolerance);
    };
  }
});

// node_modules/intersects/ellipse-helper.js
var require_ellipse_helper = __commonJS({
  "node_modules/intersects/ellipse-helper.js"(exports, module) {
    var MAX_ITERATIONS = 10;
    var innerPolygonCoef;
    var outerPolygonCoef;
    var initialized;
    function initialize() {
      innerPolygonCoef = [];
      outerPolygonCoef = [];
      for (var t = 0; t <= MAX_ITERATIONS; t++) {
        var numNodes = 4 << t;
        innerPolygonCoef[t] = 0.5 / Math.cos(4 * Math.acos(0) / numNodes);
        outerPolygonCoef[t] = 0.5 / (Math.cos(2 * Math.acos(0) / numNodes) * Math.cos(2 * Math.acos(0) / numNodes));
      }
      initialized = true;
    }
    function iterate(x, y, c0x, c0y, c2x, c2y, rr) {
      for (var t = 1; t <= MAX_ITERATIONS; t++) {
        var c1x = (c0x + c2x) * innerPolygonCoef[t];
        var c1y = (c0y + c2y) * innerPolygonCoef[t];
        var tx = x - c1x;
        var ty = y - c1y;
        if (tx * tx + ty * ty <= rr) {
          return true;
        }
        var t2x = c2x - c1x;
        var t2y = c2y - c1y;
        if (tx * t2x + ty * t2y >= 0 && tx * t2x + ty * t2y <= t2x * t2x + t2y * t2y && (ty * t2x - tx * t2y >= 0 || rr * (t2x * t2x + t2y * t2y) >= (ty * t2x - tx * t2y) * (ty * t2x - tx * t2y))) {
          return true;
        }
        var t0x = c0x - c1x;
        var t0y = c0y - c1y;
        if (tx * t0x + ty * t0y >= 0 && tx * t0x + ty * t0y <= t0x * t0x + t0y * t0y && (ty * t0x - tx * t0y <= 0 || rr * (t0x * t0x + t0y * t0y) >= (ty * t0x - tx * t0y) * (ty * t0x - tx * t0y))) {
          return true;
        }
        var c3x = (c0x + c1x) * outerPolygonCoef[t];
        var c3y = (c0y + c1y) * outerPolygonCoef[t];
        if ((c3x - x) * (c3x - x) + (c3y - y) * (c3y - y) < rr) {
          c2x = c1x;
          c2y = c1y;
          continue;
        }
        var c4x = c1x - c3x + c1x;
        var c4y = c1y - c3y + c1y;
        if ((c4x - x) * (c4x - x) + (c4y - y) * (c4y - y) < rr) {
          c0x = c1x;
          c0y = c1y;
          continue;
        }
        var t3x = c3x - c1x;
        var t3y = c3y - c1y;
        if (ty * t3x - tx * t3y <= 0 || rr * (t3x * t3x + t3y * t3y) > (ty * t3x - tx * t3y) * (ty * t3x - tx * t3y)) {
          if (tx * t3x + ty * t3y > 0) {
            if (Math.abs(tx * t3x + ty * t3y) <= t3x * t3x + t3y * t3y || (x - c3x) * (c0x - c3x) + (y - c3y) * (c0y - c3y) >= 0) {
              c2x = c1x;
              c2y = c1y;
              continue;
            }
          } else if (-(tx * t3x + ty * t3y) <= t3x * t3x + t3y * t3y || (x - c4x) * (c2x - c4x) + (y - c4y) * (c2y - c4y) >= 0) {
            c0x = c1x;
            c0y = c1y;
            continue;
          }
        }
        return false;
      }
      return false;
    }
    function ellipseEllipse(x0, y0, w0, h0, x1, y1, w1, h1) {
      if (!initialized) {
        initialize();
      }
      var x = Math.abs(x1 - x0) * h1;
      var y = Math.abs(y1 - y0) * w1;
      w0 *= h1;
      h0 *= w1;
      var r = w1 * h1;
      if (x * x + (h0 - y) * (h0 - y) <= r * r || (w0 - x) * (w0 - x) + y * y <= r * r || x * h0 + y * w0 <= w0 * h0 || (x * h0 + y * w0 - w0 * h0) * (x * h0 + y * w0 - w0 * h0) <= r * r * (w0 * w0 + h0 * h0) && x * w0 - y * h0 >= -h0 * h0 && x * w0 - y * h0 <= w0 * w0) {
        return true;
      } else {
        if ((x - w0) * (x - w0) + (y - h0) * (y - h0) <= r * r || x <= w0 && y - r <= h0 || y <= h0 && x - r <= w0) {
          return iterate(x, y, w0, 0, 0, h0, r * r);
        }
        return false;
      }
    }
    function ellipseCircle(x0, y0, w, h, x1, y1, r) {
      if (!initialized) {
        initialize();
      }
      var x = Math.abs(x1 - x0);
      var y = Math.abs(y1 - y0);
      if (x * x + (h - y) * (h - y) <= r * r || (w - x) * (w - x) + y * y <= r * r || x * h + y * w <= w * h || (x * h + y * w - w * h) * (x * h + y * w - w * h) <= r * r * (w * w + h * h) && x * w - y * h >= -h * h && x * w - y * h <= w * w) {
        return true;
      } else {
        if ((x - w) * (x - w) + (y - h) * (y - h) <= r * r || x <= w && y - r <= h || y <= h && x - r <= w) {
          return iterate(x, y, w, 0, 0, h, r * r);
        }
        return false;
      }
    }
    module.exports = {
      ellipseCircle,
      ellipseEllipse
    };
  }
});

// node_modules/intersects/ellipse-circle.js
var require_ellipse_circle = __commonJS({
  "node_modules/intersects/ellipse-circle.js"(exports, module) {
    var ellipseHelper = require_ellipse_helper();
    module.exports = function ellipseCircle(xe, ye, rex, rey, xc, yc, rc) {
      return ellipseHelper.ellipseCircle(xe, ye, rex, rey, xc, yc, rc);
    };
  }
});

// node_modules/intersects/circle-ellipse.js
var require_circle_ellipse = __commonJS({
  "node_modules/intersects/circle-ellipse.js"(exports, module) {
    var ellipseCircle = require_ellipse_circle();
    module.exports = function circleEllipse(xc, yc, rc, xe, ye, rex, rey) {
      return ellipseCircle(xe, ye, rex, rey, xc, yc, rc);
    };
  }
});

// node_modules/intersects/circleOutline-box.js
var require_circleOutline_box = __commonJS({
  "node_modules/intersects/circleOutline-box.js"(exports, module) {
    var circlePoint = require_circle_point();
    var boxCircle = require_box_circle();
    module.exports = function circleOutlineBox(xc, yc, rc, x, y, width, height, thickness) {
      thickness = thickness || 1;
      var count = 0;
      count += circlePoint(xc, yc, rc, x, y) ? 1 : 0;
      count += circlePoint(xc, yc, rc, x + width, y) ? 1 : 0;
      count += circlePoint(xc, yc, rc, x, y + height) ? 1 : 0;
      count += circlePoint(xc, yc, rc, x + width, y + height) ? 1 : 0;
      if (count === 0) {
        return boxCircle(x, y, width, height, xc, yc, rc);
      }
      if (count >= 1 && count <= 3) {
        return true;
      }
      if (count === 4) {
        return !circlePoint(xc, yc, rc - thickness, x, y) || !circlePoint(xc, yc, rc - thickness, x + width, y) || !circlePoint(xc, yc, rc - thickness, x, y + height) || !circlePoint(xc, yc, rc - thickness, x + width, y + height);
      }
    };
  }
});

// node_modules/intersects/circleOutline-line.js
var require_circleOutline_line = __commonJS({
  "node_modules/intersects/circleOutline-line.js"(exports, module) {
    var lineCircle = require_line_circle();
    var circlePoint = require_circle_point();
    module.exports = function circleOutlineLine(xc, yc, rc, x1, y1, x2, y2, thickness) {
      thickness = thickness || 1;
      return lineCircle(x1, y1, x2, y2, xc, yc, rc) && !(circlePoint(xc, yc, rc - thickness, x1, y1) && circlePoint(xc, yc, rc - thickness, x2, y2));
    };
  }
});

// node_modules/intersects/circleOutline-point.js
var require_circleOutline_point = __commonJS({
  "node_modules/intersects/circleOutline-point.js"(exports, module) {
    var circlePoint = require_circle_point();
    module.exports = function circleOutlinePoint(xc, yc, rc, x, y, thickness) {
      thickness = thickness || 1;
      return circlePoint(xc, yc, rc, x, y) && !circlePoint(xc, yc, rc - thickness, x, y);
    };
  }
});

// node_modules/intersects/lineToLine.js
var require_lineToLine = __commonJS({
  "node_modules/intersects/lineToLine.js"(exports, module) {
    "use strict";
    module.exports = function lineToLine(x1, y1, x2, y2, x3, y3, x4, y4) {
      var s1_x = x2 - x1;
      var s1_y = y2 - y1;
      var s2_x = x4 - x3;
      var s2_y = y4 - y3;
      var s = (-s1_y * (x1 - x3) + s1_x * (y1 - y3)) / (-s2_x * s1_y + s1_x * s2_y);
      var t = (s2_x * (y1 - y3) - s2_y * (x1 - x3)) / (-s2_x * s1_y + s1_x * s2_y);
      return s >= 0 && s <= 1 && t >= 0 && t <= 1;
    };
  }
});

// node_modules/intersects/line-polygon.js
var require_line_polygon = __commonJS({
  "node_modules/intersects/line-polygon.js"(exports, module) {
    var polygonPoint = require_polygon_point();
    var lineLine = require_lineToLine();
    module.exports = function linePolygon(x1, y1, x2, y2, points, tolerance) {
      var length = points.length;
      if (polygonPoint(points, x1, y1, tolerance)) {
        return true;
      }
      for (var i = 0; i < length; i += 2) {
        var j = (i + 2) % length;
        if (lineLine(x1, y1, x2, y2, points[i], points[i + 1], points[j], points[j + 1])) {
          return true;
        }
      }
      return false;
    };
  }
});

// node_modules/intersects/polygon-line.js
var require_polygon_line = __commonJS({
  "node_modules/intersects/polygon-line.js"(exports, module) {
    var linePolygon = require_line_polygon();
    module.exports = function polygonLine(points, x1, y1, x2, y2, tolerance) {
      return linePolygon(x1, y1, x2, y2, points, tolerance);
    };
  }
});

// node_modules/intersects/polygon-polygon.js
var require_polygon_polygon = __commonJS({
  "node_modules/intersects/polygon-polygon.js"(exports, module) {
    "use strict";
    module.exports = function polygonPolygon(points1, points2) {
      var a = points1;
      var b = points2;
      var polygons = [a, b];
      var minA, maxA, projected, minB, maxB, j;
      for (var i = 0; i < polygons.length; i++) {
        var polygon = polygons[i];
        for (var i1 = 0; i1 < polygon.length; i1 += 2) {
          var i2 = (i1 + 2) % polygon.length;
          var normal = { x: polygon[i2 + 1] - polygon[i1 + 1], y: polygon[i1] - polygon[i2] };
          minA = maxA = null;
          for (j = 0; j < a.length; j += 2) {
            projected = normal.x * a[j] + normal.y * a[j + 1];
            if (minA === null || projected < minA) {
              minA = projected;
            }
            if (maxA === null || projected > maxA) {
              maxA = projected;
            }
          }
          minB = maxB = null;
          for (j = 0; j < b.length; j += 2) {
            projected = normal.x * b[j] + normal.y * b[j + 1];
            if (minB === null || projected < minB) {
              minB = projected;
            }
            if (maxB === null || projected > maxB) {
              maxB = projected;
            }
          }
          if (maxA < minB || maxB < minA) {
            return false;
          }
        }
      }
      return true;
    };
  }
});

// node_modules/intersects/polygon-box.js
var require_polygon_box = __commonJS({
  "node_modules/intersects/polygon-box.js"(exports, module) {
    "use strict";
    var polygonPolygon = require_polygon_polygon();
    module.exports = function polygonBox(points, x, y, w, h) {
      var points2 = [x, y, x + w, y, x + w, y + h, x, y + h];
      return polygonPolygon(points, points2);
    };
  }
});

// node_modules/intersects/ellipse-line.js
var require_ellipse_line = __commonJS({
  "node_modules/intersects/ellipse-line.js"(exports, module) {
    module.exports = function ellipseLine(xe, ye, rex, rey, x1, y1, x2, y2) {
      x1 -= xe;
      x2 -= xe;
      y1 -= ye;
      y2 -= ye;
      var A = Math.pow(x2 - x1, 2) / rex / rex + Math.pow(y2 - y1, 2) / rey / rey;
      var B = 2 * x1 * (x2 - x1) / rex / rex + 2 * y1 * (y2 - y1) / rey / rey;
      var C = x1 * x1 / rex / rex + y1 * y1 / rey / rey - 1;
      var D = B * B - 4 * A * C;
      if (D === 0) {
        var t = -B / 2 / A;
        return t >= 0 && t <= 1;
      } else if (D > 0) {
        var sqrt = Math.sqrt(D);
        var t1 = (-B + sqrt) / 2 / A;
        var t2 = (-B - sqrt) / 2 / A;
        return t1 >= 0 && t1 <= 1 || t2 >= 0 && t2 <= 1;
      } else {
        return false;
      }
    };
  }
});

// node_modules/intersects/line-ellipse.js
var require_line_ellipse = __commonJS({
  "node_modules/intersects/line-ellipse.js"(exports, module) {
    var ellipseLine = require_ellipse_line();
    module.exports = function lineEllipse(x1, y1, x2, y2, xe, ye, rex, rey) {
      return ellipseLine(xe, ye, rex, rey, x1, y1, x2, y2);
    };
  }
});

// node_modules/intersects/polygon-ellipse.js
var require_polygon_ellipse = __commonJS({
  "node_modules/intersects/polygon-ellipse.js"(exports, module) {
    var polygonPoint = require_polygon_point();
    var lineEllipse = require_line_ellipse();
    module.exports = function polygonEllipse(points, xe, ye, rex, rey) {
      if (polygonPoint(points, xe, ye)) {
        return true;
      }
      var count = points.length;
      for (var i = 0; i < count - 2; i += 2) {
        if (lineEllipse(points[i], points[i + 1], points[i + 2], points[i + 3], xe, ye, rex, rey)) {
          return true;
        }
      }
      return lineEllipse(points[0], points[1], points[count - 2], points[count - 1], xe, ye, rex, rey);
    };
  }
});

// node_modules/intersects/box-point.js
var require_box_point = __commonJS({
  "node_modules/intersects/box-point.js"(exports, module) {
    "use strict";
    module.exports = function boxPoint(x1, y1, w1, h1, x2, y2) {
      return x2 >= x1 && x2 <= x1 + w1 && y2 >= y1 && y2 <= y1 + h1;
    };
  }
});

// node_modules/intersects/box-box.js
var require_box_box = __commonJS({
  "node_modules/intersects/box-box.js"(exports, module) {
    "use strict";
    module.exports = function boxBox(x1, y1, w1, h1, x2, y2, w2, h2) {
      return x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2;
    };
  }
});

// node_modules/intersects/lineToPolygon.js
var require_lineToPolygon = __commonJS({
  "node_modules/intersects/lineToPolygon.js"(exports, module) {
    "use strict";
    module.exports = function lineToPolygon(x1, y1, x2, y2, thickness) {
      const angle = Math.atan2(y2 - y1, x2 - x1) - Math.PI / 2;
      const half = thickness / 2;
      const cos = Math.cos(angle) * half;
      const sin = Math.sin(angle) * half;
      return [
        x1 - cos,
        y1 - sin,
        x2 - cos,
        y2 - sin,
        x2 + cos,
        y2 + sin,
        x1 + cos,
        y1 + sin
      ];
    };
  }
});

// node_modules/intersects/line-line.js
var require_line_line = __commonJS({
  "node_modules/intersects/line-line.js"(exports, module) {
    "use strict";
    var lineToPolygon = require_lineToPolygon();
    var polygonPolygon = require_polygon_polygon();
    var linePolygon = require_line_polygon();
    var lineToLine = require_lineToLine();
    module.exports = function lineLine(x1, y1, x2, y2, x3, y3, x4, y4, thickness1, thickness2) {
      if (thickness1 || thickness2) {
        return lineLineThickness(x1, y1, x2, y2, x3, y3, x4, y4, thickness1, thickness2);
      } else {
        return lineToLine(x1, y1, x2, y2, x3, y3, x4, y4);
      }
    };
    function lineLineThickness(x1, y1, x2, y2, x3, y3, x4, y4, thickness1, thickness2) {
      if (thickness1 && thickness2) {
        return polygonPolygon(lineToPolygon(x1, y1, x2, y2, thickness1), lineToPolygon(x3, y3, x4, y4, thickness2));
      } else if (thickness1) {
        return linePolygon(x3, y3, x4, y4, lineToPolygon(x1, y1, x2, y2, thickness1));
      } else if (thickness2) {
        return linePolygon(x1, y1, x2, y2, lineToPolygon(x3, y3, x4, y4, thickness1));
      }
    }
  }
});

// node_modules/intersects/line-box.js
var require_line_box = __commonJS({
  "node_modules/intersects/line-box.js"(exports, module) {
    "use strict";
    var boxPoint = require_box_point();
    var lineLine = require_line_line();
    module.exports = function lineBox(x1, y1, x2, y2, xb, yb, wb, hb) {
      if (boxPoint(xb, yb, wb, hb, x1, y1) || boxPoint(xb, yb, wb, hb, x2, y2)) {
        return true;
      }
      return lineLine(x1, y1, x2, y2, xb, yb, xb + wb, yb) || lineLine(x1, y1, x2, y2, xb + wb, yb, xb + wb, yb + hb) || lineLine(x1, y1, x2, y2, xb, yb + hb, xb + wb, yb + hb) || lineLine(x1, y1, x2, y2, xb, yb, xb, yb + hb);
    };
  }
});

// node_modules/intersects/box-line.js
var require_box_line = __commonJS({
  "node_modules/intersects/box-line.js"(exports, module) {
    "use strict";
    var lineBox = require_line_box();
    module.exports = function boxLine(xb, yb, wb, hb, x1, y1, x2, y2) {
      return lineBox(x1, y1, x2, y2, xb, yb, wb, hb);
    };
  }
});

// node_modules/intersects/box-polygon.js
var require_box_polygon = __commonJS({
  "node_modules/intersects/box-polygon.js"(exports, module) {
    "use strict";
    var polygonBox = require_polygon_box();
    module.exports = function boxPolygon(xb, yb, wb, hb, points) {
      return polygonBox(points, xb, yb, wb, hb);
    };
  }
});

// node_modules/intersects/ellipse-box.js
var require_ellipse_box = __commonJS({
  "node_modules/intersects/ellipse-box.js"(exports, module) {
    var ellipseLine = require_ellipse_line();
    var boxPoint = require_box_point();
    module.exports = function ellipseBox(xe, ye, rex, rey, xb, yb, wb, hb) {
      return boxPoint(xb, yb, wb, hb, xe, ye) || ellipseLine(xe, ye, rex, rey, xb, yb, xb + wb, yb) || ellipseLine(xe, ye, rex, rey, xb, yb + hb, xb + wb, yb + hb) || ellipseLine(xe, ye, rex, rey, xb, yb, xb, yb + hb) || ellipseLine(xe, ye, rex, rey, xb + wb, yb, xb + wb, yb + hb);
    };
  }
});

// node_modules/intersects/box-ellipse.js
var require_box_ellipse = __commonJS({
  "node_modules/intersects/box-ellipse.js"(exports, module) {
    var ellipseBox = require_ellipse_box();
    module.exports = function boxEllipse(xb, yb, wb, hb, xe, ye, rex, rey) {
      return ellipseBox(xe, ye, rex, rey, xb, yb, wb, hb);
    };
  }
});

// node_modules/intersects/box-circleOutline.js
var require_box_circleOutline = __commonJS({
  "node_modules/intersects/box-circleOutline.js"(exports, module) {
    var circleOutlineBox = require_circleOutline_box();
    module.exports = function boxCircleOutline(x, y, width, height, xc, yc, rc, thickness) {
      return circleOutlineBox(xc, yc, rc, x, y, width, height, thickness);
    };
  }
});

// node_modules/intersects/point-box.js
var require_point_box = __commonJS({
  "node_modules/intersects/point-box.js"(exports, module) {
    "use strict";
    var boxPoint = require_box_point();
    module.exports = function pointBox(x1, y1, xb, yb, wb, hb) {
      return boxPoint(xb, yb, wb, hb, x1, y1);
    };
  }
});

// node_modules/intersects/point-polygon.js
var require_point_polygon = __commonJS({
  "node_modules/intersects/point-polygon.js"(exports, module) {
    "use strict";
    var polygonPoint = require_polygon_point();
    module.exports = function pointPolygon(x1, y1, points, tolerance) {
      return polygonPoint(points, x1, y1, tolerance);
    };
  }
});

// node_modules/intersects/point-circle.js
var require_point_circle = __commonJS({
  "node_modules/intersects/point-circle.js"(exports, module) {
    "use strict";
    var circlePoint = require_circle_point();
    module.exports = function pointCircle(x1, y1, xc, yc, rc) {
      return circlePoint(xc, yc, rc, x1, y1);
    };
  }
});

// node_modules/intersects/point-line.js
var require_point_line = __commonJS({
  "node_modules/intersects/point-line.js"(exports, module) {
    "use strict";
    var linePoint = require_line_point();
    module.exports = function pointLine(xp, yp, x1, y1, x2, y2) {
      return linePoint(x1, y1, x2, y2, xp, yp);
    };
  }
});

// node_modules/intersects/ellipse-point.js
var require_ellipse_point = __commonJS({
  "node_modules/intersects/ellipse-point.js"(exports, module) {
    module.exports = function ellipsePoint(xe, ye, rex, rey, x1, y1) {
      var x = Math.pow(x1 - xe, 2) / (rex * rex);
      var y = Math.pow(y1 - ye, 2) / (rey * rey);
      return x + y <= 1;
    };
  }
});

// node_modules/intersects/point-ellipse.js
var require_point_ellipse = __commonJS({
  "node_modules/intersects/point-ellipse.js"(exports, module) {
    var ellipsePoint = require_ellipse_point();
    module.exports = function pointEllipse(x1, y1, xe, ye, rex, rey) {
      return ellipsePoint(xe, ye, rex, rey, x1, y1);
    };
  }
});

// node_modules/intersects/point-circleOutline.js
var require_point_circleOutline = __commonJS({
  "node_modules/intersects/point-circleOutline.js"(exports, module) {
    var circleOutlinePoint = require_circleOutline_point();
    module.exports = function pointCircleOutline(x, y, xc, yc, rc, thickness) {
      return circleOutlinePoint(x, y, xc, yc, rc, thickness);
    };
  }
});

// node_modules/intersects/line-circleOutline.js
var require_line_circleOutline = __commonJS({
  "node_modules/intersects/line-circleOutline.js"(exports, module) {
    var circleOutlineLine = require_circleOutline_line();
    module.exports = function lineCircleOutline(x1, y1, x2, y2, xc, yc, rc, thickness) {
      return circleOutlineLine(xc, yc, rc, x1, y1, x2, y2, thickness);
    };
  }
});

// node_modules/intersects/ellipse-ellipse.js
var require_ellipse_ellipse = __commonJS({
  "node_modules/intersects/ellipse-ellipse.js"(exports, module) {
    var ellipseHelper = require_ellipse_helper();
    module.exports = function ellipseEllipse(x1, y1, r1x, r1y, x2, y2, r2x, r2y) {
      return ellipseHelper.ellipseEllipse(x1, y1, r1x, r1y, x2, y2, r2x, r2y);
    };
  }
});

// node_modules/intersects/ellipse-polygon.js
var require_ellipse_polygon = __commonJS({
  "node_modules/intersects/ellipse-polygon.js"(exports, module) {
    var polygonEllipse = require_polygon_ellipse();
    module.exports = function ellipsePolygon(xe, ye, rex, rey, points) {
      return polygonEllipse(points, xe, ye, rex, rey);
    };
  }
});

// node_modules/intersects/index.js
var require_intersects = __commonJS({
  "node_modules/intersects/index.js"(exports, module) {
    module.exports = {
      circlePoint: require_circle_point(),
      circleCircle: require_circle_circle(),
      circleLine: require_circle_line(),
      circleBox: require_circle_box(),
      circlePolygon: require_circle_polygon(),
      circleEllipse: require_circle_ellipse(),
      // circleCircleOutline: require('./circle-circleOutline'),
      circleOutlineBox: require_circleOutline_box(),
      circleOutlineLine: require_circleOutline_line(),
      circleOutlinePoint: require_circleOutline_point(),
      // circleOutlineCircle: require('./circleOutline-circle'),
      polygonPoint: require_polygon_point(),
      polygonLine: require_polygon_line(),
      polygonPolygon: require_polygon_polygon(),
      polygonBox: require_polygon_box(),
      polygonCircle: require_polygon_circle(),
      polygonEllipse: require_polygon_ellipse(),
      boxPoint: require_box_point(),
      boxBox: require_box_box(),
      boxLine: require_box_line(),
      boxPolygon: require_box_polygon(),
      boxCircle: require_box_circle(),
      boxEllipse: require_box_ellipse(),
      boxCircleOutline: require_box_circleOutline(),
      pointBox: require_point_box(),
      pointPolygon: require_point_polygon(),
      pointCircle: require_point_circle(),
      pointLine: require_point_line(),
      pointEllipse: require_point_ellipse(),
      pointCircleOutline: require_point_circleOutline(),
      lineLine: require_line_line(),
      lineBox: require_line_box(),
      linePolygon: require_line_polygon(),
      lineCircle: require_line_circle(),
      linePoint: require_line_point(),
      lineEllipse: require_line_ellipse(),
      lineCircleOutline: require_line_circleOutline(),
      ellipsePoint: require_ellipse_point(),
      ellipseLine: require_ellipse_line(),
      ellipseBox: require_ellipse_box(),
      ellipseCircle: require_ellipse_circle(),
      ellipseEllipse: require_ellipse_ellipse(),
      ellipsePolygon: require_ellipse_polygon()
    };
  }
});
export default require_intersects();
//# sourceMappingURL=intersects.js.map
