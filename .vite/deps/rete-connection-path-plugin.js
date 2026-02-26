import {
  _get
} from "./chunk-XQ3X4JL4.js";
import {
  bundle_default,
  line_default
} from "./chunk-3LB3UQTO.js";
import {
  classicConnectionPath
} from "./chunk-C2PZ3BDC.js";
import {
  _slicedToArray
} from "./chunk-37VOTOQP.js";
import "./chunk-OM334EPU.js";
import "./chunk-W4S6QAMR.js";
import {
  Scope
} from "./chunk-O6P36EK3.js";
import {
  _classCallCheck,
  _createClass,
  _defineProperty,
  _getPrototypeOf,
  _inherits,
  _possibleConstructorReturn,
  _typeof
} from "./chunk-AQPE7URQ.js";
import "./chunk-PLDDJCW6.js";

// node_modules/rete-connection-path-plugin/rete-connection-path-plugin.esm.js
var PathFactory = function() {
  function PathFactory2(curve) {
    _classCallCheck(this, PathFactory2);
    this.curve = curve;
  }
  return _createClass(PathFactory2, [{
    key: "getData",
    value: function getData(points) {
      var getPath = line_default().x(function(d) {
        return d[0];
      }).y(function(d) {
        return d[1];
      }).curve(this.curve);
      return getPath(points.map(function(p) {
        return [p.x, p.y];
      }));
    }
  }]);
}();
var linear = function linear2() {
  return function(points) {
    if (points.length !== 2) throw new Error("number of points should be equal to 2");
    var _points = _slicedToArray(points, 2), start = _points[0], end = _points[1];
    return [start, end];
  };
};
var classic = function classic2(options) {
  var _options$vertical = options.vertical, vertical = _options$vertical === void 0 ? false : _options$vertical, _options$curvature = options.curvature, curvature = _options$curvature === void 0 ? 0.3 : _options$curvature;
  function add(a, b) {
    return {
      x: a.x + b.x,
      y: a.y + b.y
    };
  }
  return function(points) {
    if (points.length !== 2) throw new Error("number of points should be equal to 2");
    var _points2 = _slicedToArray(points, 2), start = _points2[0], end = _points2[1];
    var xDistance = Math.abs(start.x - end.x);
    var yDistance = Math.abs(start.y - end.y);
    var crossDistance = vertical ? xDistance : yDistance;
    var alongDistance = vertical ? yDistance : xDistance;
    var offset = Math.max(crossDistance / 2, alongDistance) * curvature;
    var startOffset = vertical ? {
      x: 0,
      y: offset
    } : {
      y: 0,
      x: offset
    };
    var endOffset = vertical ? {
      x: 0,
      y: -offset
    } : {
      y: 0,
      x: -offset
    };
    return [start, add(start, startOffset), add(end, endOffset), end];
  };
};
var transformers = Object.freeze({
  __proto__: null,
  linear,
  classic
});
function getAngle(_ref, _ref2) {
  var x1 = _ref.x, y1 = _ref.y;
  var x2 = _ref2.x, y2 = _ref2.y;
  var dx = x1 - x2;
  var dy = y1 - y2;
  return 180 * Math.atan2(dy, dx) / Math.PI;
}
function getTransformAlong(path, offset) {
  var delta = arguments.length > 2 && arguments[2] !== void 0 ? arguments[2] : 1;
  var needRotate = arguments.length > 3 && arguments[3] !== void 0 ? arguments[3] : true;
  var length = path.getTotalLength() * delta;
  var p1 = path.getPointAtLength(length + offset);
  var p2 = path.getPointAtLength(length);
  var angle = 180 + (needRotate ? getAngle(p1, p2) : 0);
  return "translate(".concat(p1.x, ", ").concat(p1.y, ") rotate(").concat(angle, ")");
}
function ownKeys(e, r) {
  var t = Object.keys(e);
  if (Object.getOwnPropertySymbols) {
    var o = Object.getOwnPropertySymbols(e);
    r && (o = o.filter(function(r2) {
      return Object.getOwnPropertyDescriptor(e, r2).enumerable;
    })), t.push.apply(t, o);
  }
  return t;
}
function _objectSpread(e) {
  for (var r = 1; r < arguments.length; r++) {
    var t = null != arguments[r] ? arguments[r] : {};
    r % 2 ? ownKeys(Object(t), true).forEach(function(r2) {
      _defineProperty(e, r2, t[r2]);
    }) : Object.getOwnPropertyDescriptors ? Object.defineProperties(e, Object.getOwnPropertyDescriptors(t)) : ownKeys(Object(t)).forEach(function(r2) {
      Object.defineProperty(e, r2, Object.getOwnPropertyDescriptor(t, r2));
    });
  }
  return e;
}
function _callSuper(t, o, e) {
  return o = _getPrototypeOf(o), _possibleConstructorReturn(t, _isNativeReflectConstruct() ? Reflect.construct(o, e || [], _getPrototypeOf(t).constructor) : o.apply(t, e));
}
function _isNativeReflectConstruct() {
  try {
    var t = !Boolean.prototype.valueOf.call(Reflect.construct(Boolean, [], function() {
    }));
  } catch (t2) {
  }
  return (_isNativeReflectConstruct = function _isNativeReflectConstruct2() {
    return !!t;
  })();
}
function _superPropGet(t, e, o, r) {
  var p = _get(_getPrototypeOf(1 & r ? t.prototype : t), e, o);
  return 2 & r && "function" == typeof p ? function(t2) {
    return p.apply(o, t2);
  } : p;
}
var ConnectionPathPlugin = function(_Scope) {
  function ConnectionPathPlugin2(props) {
    var _this;
    _classCallCheck(this, ConnectionPathPlugin2);
    _this = _callSuper(this, ConnectionPathPlugin2, ["connection-path"]);
    _defineProperty(_this, "arrows", /* @__PURE__ */ new Map());
    _defineProperty(_this, "transforms", /* @__PURE__ */ new Map());
    _this.props = props;
    return _this;
  }
  _inherits(ConnectionPathPlugin2, _Scope);
  return _createClass(ConnectionPathPlugin2, [{
    key: "setParent",
    value: function setParent(scope) {
      var _this2 = this;
      _superPropGet(ConnectionPathPlugin2, "setParent", this, 3)([scope]);
      scope.addPipe(function(context) {
        if (!context || _typeof(context) !== "object" || !("type" in context)) return context;
        if (context.type === "connectionpath") {
          var _this2$props, _this2$props2;
          var _context$data = context.data, points = _context$data.points, payload = _context$data.payload;
          var curve = (_this2$props = _this2.props) !== null && _this2$props !== void 0 && _this2$props.curve ? _this2.props.curve(payload) : bundle_default.beta(0.9);
          var transformer = (_this2$props2 = _this2.props) !== null && _this2$props2 !== void 0 && _this2$props2.transformer ? _this2.props.transformer(payload) : classic({});
          var factory = new PathFactory(curve);
          var transformedPoints = transformer(points);
          var path = factory.getData(transformedPoints);
          var p = document.createElementNS("http://www.w3.org/2000/svg", "path");
          p.setAttribute("d", path !== null && path !== void 0 ? path : classicConnectionPath(transformedPoints, 0.3));
          _this2.transforms.set(payload.id, getTransformAlong(p, -15));
          _this2.updateArrow(payload);
          return path ? _objectSpread(_objectSpread({}, context), {}, {
            data: _objectSpread(_objectSpread({}, context.data), {}, {
              path
            })
          }) : context;
        }
        if (context.type === "connectionremoved") {
          var id = context.data.id;
          _this2.arrows["delete"](id);
          _this2.transforms["delete"](id);
        }
        if (context.type === "rendered" && context.data.type === "connection") {
          var _this2$props3;
          if (!((_this2$props3 = _this2.props) !== null && _this2$props3 !== void 0 && _this2$props3.arrow) || context.data.type !== "connection") return;
          var _payload = context.data.payload;
          var _id = _payload.id;
          var arrowData = _this2.getArrowData(_payload);
          if (_this2.arrows.has(_id) || !arrowData) return;
          var el = context.data.element;
          var svgEl = document.createElementNS("http://www.w3.org/2000/svg", "svg");
          var marker = document.createElementNS("http://www.w3.org/2000/svg", "path");
          svgEl.setAttribute("style", "position: absolute; overflow: visible !important; pointer-events: none");
          svgEl.appendChild(marker);
          el.appendChild(svgEl);
          marker.classList.add("marker");
          _this2.arrows.set(_id, {
            marker
          });
          _this2.updateArrow(_payload);
        }
        return context;
      });
    }
  }, {
    key: "getArrowData",
    value: function getArrowData(c) {
      var _this$props;
      if (!((_this$props = this.props) !== null && _this$props !== void 0 && _this$props.arrow)) return null;
      var data = this.props.arrow(c);
      if (!data) return null;
      var _ref = data === true ? {} : data, _ref$color = _ref.color, color = _ref$color === void 0 ? "steelblue" : _ref$color, _ref$marker = _ref.marker, marker = _ref$marker === void 0 ? "M-5,-10 L-5,10 L20,0 z" : _ref$marker;
      return {
        color,
        marker
      };
    }
  }, {
    key: "updateArrow",
    value: function updateArrow(c) {
      var _this$transforms$get;
      var data = this.arrows.get(c.id);
      var arrowData = this.getArrowData(c);
      if (!data || !arrowData) return;
      var color = arrowData.color, marker = arrowData.marker;
      data.marker.setAttribute("fill", color);
      data.marker.setAttribute("d", marker);
      data.marker.setAttribute("transform", (_this$transforms$get = this.transforms.get(c.id)) !== null && _this$transforms$get !== void 0 ? _this$transforms$get : "");
    }
  }]);
}(Scope);
export {
  ConnectionPathPlugin,
  transformers as Transformers
};
/*! Bundled license information:

rete-connection-path-plugin/rete-connection-path-plugin.esm.js:
  (*!
  * rete-connection-path-plugin v2.0.3
  * (c) 2024 Vitaliy Stoliarov
  * Released under the MIT license.
  * *)
*/
//# sourceMappingURL=rete-connection-path-plugin.js.map
