import {
  _slicedToArray
} from "./chunk-37VOTOQP.js";
import {
  BaseAreaPlugin
} from "./chunk-OM334EPU.js";
import {
  _toConsumableArray
} from "./chunk-W4S6QAMR.js";
import {
  require_regenerator
} from "./chunk-O6P36EK3.js";
import {
  _asyncToGenerator,
  _classCallCheck,
  _createClass,
  _defineProperty,
  _getPrototypeOf,
  _inherits,
  _possibleConstructorReturn
} from "./chunk-AQPE7URQ.js";
import {
  __toESM
} from "./chunk-PLDDJCW6.js";

// node_modules/rete-render-utils/rete-render-utils.esm.js
var import_regenerator = __toESM(require_regenerator());
function classicConnectionPath(points, curvature) {
  var _points = _slicedToArray(points, 2), _points$ = _points[0], x1 = _points$.x, y1 = _points$.y, _points$2 = _points[1], x2 = _points$2.x, y2 = _points$2.y;
  var vertical = Math.abs(y1 - y2);
  var hx1 = x1 + Math.max(vertical / 2, Math.abs(x2 - x1)) * curvature;
  var hx2 = x2 - Math.max(vertical / 2, Math.abs(x2 - x1)) * curvature;
  return "M ".concat(x1, " ").concat(y1, " C ").concat(hx1, " ").concat(y1, " ").concat(hx2, " ").concat(y2, " ").concat(x2, " ").concat(y2);
}
function loopConnectionPath(points, curvature, size) {
  var _points2 = _slicedToArray(points, 2), _points2$ = _points2[0], x1 = _points2$.x, y1 = _points2$.y, _points2$2 = _points2[1], x2 = _points2$2.x, y2 = _points2$2.y;
  var k = y2 > y1 ? 1 : -1;
  var scale = size + Math.abs(x1 - x2) / (size / 2);
  var middleX = (x1 + x2) / 2;
  var middleY = y1 - k * scale;
  var vertical = (y2 - y1) * curvature;
  return "\n        M ".concat(x1, " ").concat(y1, "\n        C ").concat(x1 + scale, " ").concat(y1, "\n        ").concat(x1 + scale, " ").concat(middleY - vertical, "\n        ").concat(middleX, " ").concat(middleY, "\n        C ").concat(x2 - scale, " ").concat(middleY + vertical, "\n        ").concat(x2 - scale, " ").concat(y2, "\n        ").concat(x2, " ").concat(y2, "\n    ");
}
function getElementCenter(_x, _x2) {
  return _getElementCenter.apply(this, arguments);
}
function _getElementCenter() {
  _getElementCenter = _asyncToGenerator(import_regenerator.default.mark(function _callee(child, parent) {
    var x, y, currentElement, width, height;
    return import_regenerator.default.wrap(function _callee$(_context) {
      while (1) switch (_context.prev = _context.next) {
        case 0:
          if (child.offsetParent) {
            _context.next = 5;
            break;
          }
          _context.next = 3;
          return new Promise(function(res) {
            return setTimeout(res, 0);
          });
        case 3:
          _context.next = 0;
          break;
        case 5:
          x = child.offsetLeft;
          y = child.offsetTop;
          currentElement = child.offsetParent;
          if (currentElement) {
            _context.next = 10;
            break;
          }
          throw new Error("child has null offsetParent");
        case 10:
          while (currentElement !== null && currentElement !== parent) {
            x += currentElement.offsetLeft + currentElement.clientLeft;
            y += currentElement.offsetTop + currentElement.clientTop;
            currentElement = currentElement.offsetParent;
          }
          width = child.offsetWidth;
          height = child.offsetHeight;
          return _context.abrupt("return", {
            x: x + width / 2,
            y: y + height / 2
          });
        case 14:
        case "end":
          return _context.stop();
      }
    }, _callee);
  }));
  return _getElementCenter.apply(this, arguments);
}
var EventEmitter = function() {
  function EventEmitter2() {
    _classCallCheck(this, EventEmitter2);
    _defineProperty(this, "listeners", /* @__PURE__ */ new Set());
  }
  return _createClass(EventEmitter2, [{
    key: "emit",
    value: function emit(data) {
      this.listeners.forEach(function(listener) {
        listener(data);
      });
    }
  }, {
    key: "listen",
    value: function listen(handler) {
      var _this = this;
      this.listeners.add(handler);
      return function() {
        _this.listeners["delete"](handler);
      };
    }
  }]);
}();
var SocketsPositionsStorage = function() {
  function SocketsPositionsStorage2() {
    _classCallCheck(this, SocketsPositionsStorage2);
    _defineProperty(this, "elements", /* @__PURE__ */ new Map());
  }
  return _createClass(SocketsPositionsStorage2, [{
    key: "getPosition",
    value: function getPosition(data) {
      var _found$pop$position, _found$pop;
      var list = Array.from(this.elements.values()).flat();
      var found = list.filter(function(item) {
        return item.side === data.side && item.nodeId === data.nodeId && item.key === data.key;
      });
      if (found.length > 1) console.warn(["Found more than one element for socket with same key and side.", "Probably it was not unmounted correctly"].join(" "), data);
      return (_found$pop$position = (_found$pop = found.pop()) === null || _found$pop === void 0 ? void 0 : _found$pop.position) !== null && _found$pop$position !== void 0 ? _found$pop$position : null;
    }
  }, {
    key: "add",
    value: function add(data) {
      var existing = this.elements.get(data.element);
      this.elements.set(data.element, existing ? [].concat(_toConsumableArray(existing.filter(function(n) {
        return !(n.nodeId === data.nodeId && n.key === data.key && n.side === data.side);
      })), [data]) : [data]);
    }
  }, {
    key: "remove",
    value: function remove(element) {
      this.elements["delete"](element);
    }
  }, {
    key: "snapshot",
    value: function snapshot() {
      return Array.from(this.elements.values()).flat();
    }
  }]);
}();
var BaseSocketPosition = function() {
  function BaseSocketPosition2() {
    _classCallCheck(this, BaseSocketPosition2);
    _defineProperty(this, "sockets", new SocketsPositionsStorage());
    _defineProperty(this, "emitter", new EventEmitter());
    _defineProperty(this, "area", null);
  }
  return _createClass(BaseSocketPosition2, [{
    key: "attach",
    value: (
      /**
       * Attach the watcher to the area's child scope.
       * @param scope Scope of the watcher that should be a child of `BaseAreaPlugin`
       */
      function attach(scope) {
        var _this = this;
        if (this.area) return;
        if (!scope.hasParent()) return;
        this.area = scope.parentScope(BaseAreaPlugin);
        this.area.addPipe(function() {
          var _ref = _asyncToGenerator(import_regenerator.default.mark(function _callee2(context) {
            var _context$data, _nodeId, _key, _side, _element, position, _nodeId2, _context$data$payload, source, target, _nodeId3;
            return import_regenerator.default.wrap(function _callee2$(_context2) {
              while (1) switch (_context2.prev = _context2.next) {
                case 0:
                  if (!(context.type === "rendered" && context.data.type === "socket")) {
                    _context2.next = 8;
                    break;
                  }
                  _context$data = context.data, _nodeId = _context$data.nodeId, _key = _context$data.key, _side = _context$data.side, _element = _context$data.element;
                  _context2.next = 4;
                  return _this.calculatePosition(_nodeId, _side, _key, _element);
                case 4:
                  position = _context2.sent;
                  if (position) {
                    _this.sockets.add({
                      nodeId: _nodeId,
                      key: _key,
                      side: _side,
                      element: _element,
                      position
                    });
                    _this.emitter.emit({
                      nodeId: _nodeId,
                      key: _key,
                      side: _side
                    });
                  }
                  _context2.next = 24;
                  break;
                case 8:
                  if (!(context.type === "unmount")) {
                    _context2.next = 12;
                    break;
                  }
                  _this.sockets.remove(context.data.element);
                  _context2.next = 24;
                  break;
                case 12:
                  if (!(context.type === "nodetranslated")) {
                    _context2.next = 16;
                    break;
                  }
                  _this.emitter.emit({
                    nodeId: context.data.id
                  });
                  _context2.next = 24;
                  break;
                case 16:
                  if (!(context.type === "noderesized")) {
                    _context2.next = 23;
                    break;
                  }
                  _nodeId2 = context.data.id;
                  _context2.next = 20;
                  return Promise.all(_this.sockets.snapshot().filter(function(item) {
                    return item.nodeId === context.data.id && item.side === "output";
                  }).map(function() {
                    var _ref2 = _asyncToGenerator(import_regenerator.default.mark(function _callee(item) {
                      var side, key, element, position2;
                      return import_regenerator.default.wrap(function _callee$(_context) {
                        while (1) switch (_context.prev = _context.next) {
                          case 0:
                            side = item.side, key = item.key, element = item.element;
                            _context.next = 3;
                            return _this.calculatePosition(_nodeId2, side, key, element);
                          case 3:
                            position2 = _context.sent;
                            if (position2) {
                              item.position = position2;
                            }
                          case 5:
                          case "end":
                            return _context.stop();
                        }
                      }, _callee);
                    }));
                    return function(_x2) {
                      return _ref2.apply(this, arguments);
                    };
                  }()));
                case 20:
                  _this.emitter.emit({
                    nodeId: _nodeId2
                  });
                  _context2.next = 24;
                  break;
                case 23:
                  if (context.type === "render" && context.data.type === "connection") {
                    _context$data$payload = context.data.payload, source = _context$data$payload.source, target = _context$data$payload.target;
                    _nodeId3 = source || target;
                    _this.emitter.emit({
                      nodeId: _nodeId3
                    });
                  }
                case 24:
                  return _context2.abrupt("return", context);
                case 25:
                case "end":
                  return _context2.stop();
              }
            }, _callee2);
          }));
          return function(_x) {
            return _ref.apply(this, arguments);
          };
        }());
      }
    )
    /**
     * Listen to socket position changes. Usually used by rendering plugins to update the start/end of the connection.
     * @internal
     * @param nodeId Node ID
     * @param side Side of the socket, 'input' or 'output'
     * @param key Socket key
     * @param change Callback function that is called when the socket position changes
     */
  }, {
    key: "listen",
    value: function listen(nodeId, side, key, change) {
      var _this2 = this;
      var unlisten = this.emitter.listen(function(data) {
        if (data.nodeId !== nodeId) return;
        if ((!data.key || data.side === side) && (!data.side || data.key === key)) {
          var _this2$area;
          var position = _this2.sockets.getPosition({
            side,
            nodeId,
            key
          });
          if (!position) return;
          var x = position.x, y = position.y;
          var nodeView = (_this2$area = _this2.area) === null || _this2$area === void 0 ? void 0 : _this2$area.nodeViews.get(nodeId);
          if (nodeView) change({
            x: x + nodeView.position.x,
            y: y + nodeView.position.y
          });
        }
      });
      this.sockets.snapshot().forEach(function(data) {
        if (data.nodeId === nodeId) _this2.emitter.emit(data);
      });
      return unlisten;
    }
  }]);
}();
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
var DOMSocketPosition = function(_BaseSocketPosition) {
  function DOMSocketPosition2(props) {
    var _this;
    _classCallCheck(this, DOMSocketPosition2);
    _this = _callSuper(this, DOMSocketPosition2);
    _this.props = props;
    return _this;
  }
  _inherits(DOMSocketPosition2, _BaseSocketPosition);
  return _createClass(DOMSocketPosition2, [{
    key: "calculatePosition",
    value: function() {
      var _calculatePosition = _asyncToGenerator(import_regenerator.default.mark(function _callee(nodeId, side, key, element) {
        var _this$area, _this$props;
        var view, position;
        return import_regenerator.default.wrap(function _callee$(_context) {
          while (1) switch (_context.prev = _context.next) {
            case 0:
              view = (_this$area = this.area) === null || _this$area === void 0 ? void 0 : _this$area.nodeViews.get(nodeId);
              if (view !== null && view !== void 0 && view.element) {
                _context.next = 3;
                break;
              }
              return _context.abrupt("return", null);
            case 3:
              _context.next = 5;
              return getElementCenter(element, view.element);
            case 5:
              position = _context.sent;
              if (!((_this$props = this.props) !== null && _this$props !== void 0 && _this$props.offset)) {
                _context.next = 8;
                break;
              }
              return _context.abrupt("return", this.props.offset(position, nodeId, side, key));
            case 8:
              return _context.abrupt("return", {
                x: position.x + 12 * (side === "input" ? -1 : 1),
                y: position.y
              });
            case 9:
            case "end":
              return _context.stop();
          }
        }, _callee, this);
      }));
      function calculatePosition(_x, _x2, _x3, _x4) {
        return _calculatePosition.apply(this, arguments);
      }
      return calculatePosition;
    }()
  }]);
}(BaseSocketPosition);
function getDOMSocketPosition(props) {
  return new DOMSocketPosition(props);
}

export {
  classicConnectionPath,
  loopConnectionPath,
  getElementCenter,
  BaseSocketPosition,
  DOMSocketPosition,
  getDOMSocketPosition
};
/*! Bundled license information:

rete-render-utils/rete-render-utils.esm.js:
  (*!
  * rete-render-utils v2.0.2
  * (c) 2024 Vitaliy Stoliarov
  * Released under the MIT license.
  * *)
*/
//# sourceMappingURL=chunk-C2PZ3BDC.js.map
