import {
  _toConsumableArray
} from "./chunk-W4S6QAMR.js";
import {
  NodeEditor,
  Scope,
  require_regenerator
} from "./chunk-O6P36EK3.js";
import {
  _assertThisInitialized,
  _asyncToGenerator,
  _classCallCheck,
  _createClass,
  _defineProperty,
  _getPrototypeOf,
  _inherits,
  _possibleConstructorReturn,
  _typeof
} from "./chunk-AQPE7URQ.js";
import {
  __toESM
} from "./chunk-PLDDJCW6.js";

// node_modules/rete-area-plugin/rete-area-plugin.esm.js
var import_regenerator = __toESM(require_regenerator());
var Content = function() {
  function Content2(reordered) {
    _classCallCheck(this, Content2);
    this.reordered = reordered;
    this.holder = document.createElement("div");
    this.holder.style.transformOrigin = "0 0";
  }
  _createClass(Content2, [{
    key: "getPointerFrom",
    value: function getPointerFrom(event) {
      var _this$holder$getBound = this.holder.getBoundingClientRect(), left = _this$holder$getBound.left, top = _this$holder$getBound.top;
      var x = event.clientX - left;
      var y = event.clientY - top;
      return {
        x,
        y
      };
    }
  }, {
    key: "add",
    value: function add(element) {
      this.holder.appendChild(element);
    }
    // eslint-disable-next-line no-undef
  }, {
    key: "reorder",
    value: function() {
      var _reorder = _asyncToGenerator(import_regenerator.default.mark(function _callee(target, next) {
        return import_regenerator.default.wrap(function _callee$(_context) {
          while (1) switch (_context.prev = _context.next) {
            case 0:
              if (this.holder.contains(target)) {
                _context.next = 2;
                break;
              }
              throw new Error("content doesn't have 'target' for reordering");
            case 2:
              if (!(next !== null && !this.holder.contains(next))) {
                _context.next = 4;
                break;
              }
              throw new Error("content doesn't have 'next' for reordering");
            case 4:
              this.holder.insertBefore(target, next);
              _context.next = 7;
              return this.reordered(target);
            case 7:
            case "end":
              return _context.stop();
          }
        }, _callee, this);
      }));
      function reorder(_x, _x2) {
        return _reorder.apply(this, arguments);
      }
      return reorder;
    }()
  }, {
    key: "remove",
    value: function remove(element) {
      this.holder.removeChild(element);
    }
  }]);
  return Content2;
}();
function usePointerListener(element, handlers) {
  var move = function move2(event) {
    handlers.move(event);
  };
  var up = function up2(event) {
    window.removeEventListener("pointermove", move);
    window.removeEventListener("pointerup", up2);
    window.removeEventListener("pointercancel", up2);
    handlers.up(event);
  };
  var down = function down2(event) {
    window.addEventListener("pointermove", move);
    window.addEventListener("pointerup", up);
    window.addEventListener("pointercancel", up);
    handlers.down(event);
  };
  element.addEventListener("pointerdown", down);
  return {
    destroy: function destroy() {
      element.removeEventListener("pointerdown", down);
      window.removeEventListener("pointermove", move);
      window.removeEventListener("pointerup", up);
      window.removeEventListener("pointercancel", up);
    }
  };
}
var min = function min2(arr) {
  return arr.length === 0 ? 0 : Math.min.apply(Math, _toConsumableArray(arr));
};
var max = function max2(arr) {
  return arr.length === 0 ? 0 : Math.max.apply(Math, _toConsumableArray(arr));
};
function getBoundingBox$1(rects) {
  var left = min(rects.map(function(rect) {
    return rect.position.x;
  }));
  var top = min(rects.map(function(rect) {
    return rect.position.y;
  }));
  var right = max(rects.map(function(rect) {
    return rect.position.x + rect.width;
  }));
  var bottom = max(rects.map(function(rect) {
    return rect.position.y + rect.height;
  }));
  return {
    left,
    right,
    top,
    bottom,
    width: Math.abs(left - right),
    height: Math.abs(top - bottom),
    center: {
      x: (left + right) / 2,
      y: (top + bottom) / 2
    }
  };
}
function ownKeys$4(object, enumerableOnly) {
  var keys = Object.keys(object);
  if (Object.getOwnPropertySymbols) {
    var symbols = Object.getOwnPropertySymbols(object);
    enumerableOnly && (symbols = symbols.filter(function(sym) {
      return Object.getOwnPropertyDescriptor(object, sym).enumerable;
    })), keys.push.apply(keys, symbols);
  }
  return keys;
}
function _objectSpread$4(target) {
  for (var i = 1; i < arguments.length; i++) {
    var source = null != arguments[i] ? arguments[i] : {};
    i % 2 ? ownKeys$4(Object(source), true).forEach(function(key) {
      _defineProperty(target, key, source[key]);
    }) : Object.getOwnPropertyDescriptors ? Object.defineProperties(target, Object.getOwnPropertyDescriptors(source)) : ownKeys$4(Object(source)).forEach(function(key) {
      Object.defineProperty(target, key, Object.getOwnPropertyDescriptor(source, key));
    });
  }
  return target;
}
var Drag = function() {
  function Drag2(guards) {
    var _this = this;
    _classCallCheck(this, Drag2);
    _defineProperty(this, "down", function(e) {
      if (!_this.guards.down(e)) return;
      e.stopPropagation();
      _this.pointerStart = {
        x: e.pageX,
        y: e.pageY
      };
      _this.startPosition = _objectSpread$4({}, _this.config.getCurrentPosition());
      _this.events.start(e);
    });
    _defineProperty(this, "move", function(e) {
      if (!_this.pointerStart || !_this.startPosition) return;
      if (!_this.guards.move(e)) return;
      e.preventDefault();
      var delta = {
        x: e.pageX - _this.pointerStart.x,
        y: e.pageY - _this.pointerStart.y
      };
      var zoom = _this.config.getZoom();
      var x = _this.startPosition.x + delta.x / zoom;
      var y = _this.startPosition.y + delta.y / zoom;
      _this.events.translate(x, y, e);
    });
    _defineProperty(this, "up", function(e) {
      if (!_this.pointerStart) return;
      delete _this.pointerStart;
      _this.events.drag(e);
    });
    this.guards = guards || {
      down: function down(e) {
        return !(e.pointerType === "mouse" && e.button !== 0);
      },
      move: function move() {
        return true;
      }
    };
  }
  _createClass(Drag2, [{
    key: "initialize",
    value: function initialize(element, config, events) {
      this.config = config;
      this.events = events;
      element.style.touchAction = "none";
      this.pointerListener = usePointerListener(element, {
        down: this.down,
        move: this.move,
        up: this.up
      });
    }
  }, {
    key: "destroy",
    value: function destroy() {
      this.pointerListener.destroy();
    }
  }]);
  return Drag2;
}();
var Zoom = function() {
  function Zoom2(intensity) {
    var _this = this;
    _classCallCheck(this, Zoom2);
    _defineProperty(this, "previous", null);
    _defineProperty(this, "pointers", []);
    _defineProperty(this, "wheel", function(e) {
      e.preventDefault();
      var _this$element$getBoun = _this.element.getBoundingClientRect(), left = _this$element$getBoun.left, top = _this$element$getBoun.top;
      var isNegative = e.deltaY < 0;
      var delta = isNegative ? _this.intensity : -_this.intensity;
      var ox = (left - e.clientX) * delta;
      var oy = (top - e.clientY) * delta;
      _this.onzoom(delta, ox, oy, "wheel");
    });
    _defineProperty(this, "down", function(e) {
      _this.pointers.push(e);
    });
    _defineProperty(this, "move", function(e) {
      _this.pointers = _this.pointers.map(function(p) {
        return p.pointerId === e.pointerId ? e : p;
      });
      if (!_this.isTranslating()) return;
      var _this$element$getBoun2 = _this.element.getBoundingClientRect(), left = _this$element$getBoun2.left, top = _this$element$getBoun2.top;
      var _this$getTouches = _this.getTouches(), cx = _this$getTouches.cx, cy = _this$getTouches.cy, distance = _this$getTouches.distance;
      if (_this.previous !== null) {
        var _delta = distance / _this.previous.distance - 1;
        var _ox = (left - cx) * _delta;
        var _oy = (top - cy) * _delta;
        _this.onzoom(_delta, _ox - (_this.previous.cx - cx), _oy - (_this.previous.cy - cy), "touch");
      }
      _this.previous = {
        cx,
        cy,
        distance
      };
    });
    _defineProperty(this, "up", function(e) {
      _this.previous = null;
      _this.pointers = _this.pointers.filter(function(p) {
        return p.pointerId !== e.pointerId;
      });
    });
    _defineProperty(this, "dblclick", function(e) {
      e.preventDefault();
      var _this$element$getBoun3 = _this.element.getBoundingClientRect(), left = _this$element$getBoun3.left, top = _this$element$getBoun3.top;
      var delta = 4 * _this.intensity;
      var ox = (left - e.clientX) * delta;
      var oy = (top - e.clientY) * delta;
      _this.onzoom(delta, ox, oy, "dblclick");
    });
    this.intensity = intensity;
  }
  _createClass(Zoom2, [{
    key: "initialize",
    value: function initialize(container, element, onzoom) {
      this.container = container;
      this.element = element;
      this.onzoom = onzoom;
      this.container.addEventListener("wheel", this.wheel);
      this.container.addEventListener("pointerdown", this.down);
      this.container.addEventListener("dblclick", this.dblclick);
      window.addEventListener("pointermove", this.move);
      window.addEventListener("pointerup", this.up);
      window.addEventListener("pointercancel", this.up);
    }
  }, {
    key: "getTouches",
    value: function getTouches() {
      var e = {
        touches: this.pointers
      };
      var _ref = [e.touches[0].clientX, e.touches[0].clientY], x1 = _ref[0], y1 = _ref[1];
      var _ref2 = [e.touches[1].clientX, e.touches[1].clientY], x2 = _ref2[0], y2 = _ref2[1];
      var distance = Math.sqrt(Math.pow(x1 - x2, 2) + Math.pow(y1 - y2, 2));
      return {
        cx: (x1 + x2) / 2,
        cy: (y1 + y2) / 2,
        distance
      };
    }
  }, {
    key: "isTranslating",
    value: function isTranslating() {
      return this.pointers.length >= 2;
    }
  }, {
    key: "destroy",
    value: function destroy() {
      this.container.removeEventListener("wheel", this.wheel);
      this.container.removeEventListener("pointerdown", this.down);
      this.container.removeEventListener("dblclick", this.dblclick);
      window.removeEventListener("pointermove", this.move);
      window.removeEventListener("pointerup", this.up);
      window.removeEventListener("pointercancel", this.up);
    }
  }]);
  return Zoom2;
}();
var Area = function() {
  function Area2(container, events, guards) {
    var _this = this;
    _classCallCheck(this, Area2);
    _defineProperty(this, "transform", {
      k: 1,
      x: 0,
      y: 0
    });
    _defineProperty(this, "pointer", {
      x: 0,
      y: 0
    });
    _defineProperty(this, "zoomHandler", null);
    _defineProperty(this, "dragHandler", null);
    _defineProperty(this, "pointerdown", function(event) {
      _this.setPointerFrom(event);
      _this.events.pointerDown(_this.pointer, event);
    });
    _defineProperty(this, "pointermove", function(event) {
      _this.setPointerFrom(event);
      _this.events.pointerMove(_this.pointer, event);
    });
    _defineProperty(this, "pointerup", function(event) {
      _this.setPointerFrom(event);
      _this.events.pointerUp(_this.pointer, event);
    });
    _defineProperty(this, "resize", function(event) {
      _this.events.resize(event);
    });
    _defineProperty(this, "onTranslate", function(x, y) {
      if (_this.zoomHandler && _this.zoomHandler.isTranslating()) return;
      _this.translate(x, y);
    });
    _defineProperty(this, "onZoom", function(delta, ox, oy, source) {
      _this.zoom(_this.transform.k * (1 + delta), ox, oy, source);
      _this.update();
    });
    this.container = container;
    this.events = events;
    this.guards = guards;
    this.content = new Content(function(element) {
      return _this.events.reordered(element);
    });
    this.content.holder.style.transformOrigin = "0 0";
    this.setZoomHandler(new Zoom(0.1));
    this.setDragHandler(new Drag());
    this.container.addEventListener("pointerdown", this.pointerdown);
    this.container.addEventListener("pointermove", this.pointermove);
    window.addEventListener("pointerup", this.pointerup);
    window.addEventListener("resize", this.resize);
    container.appendChild(this.content.holder);
    this.update();
  }
  _createClass(Area2, [{
    key: "update",
    value: function update() {
      var _this$transform = this.transform, x = _this$transform.x, y = _this$transform.y, k = _this$transform.k;
      this.content.holder.style.transform = "translate(".concat(x, "px, ").concat(y, "px) scale(").concat(k, ")");
    }
    /**
     * Drag handler. Destroy previous drag handler if exists.
     * @param drag drag handler
     * @example area.area.setDragHandler(null) // disable drag
     */
  }, {
    key: "setDragHandler",
    value: function setDragHandler(drag) {
      var _this2 = this;
      if (this.dragHandler) this.dragHandler.destroy();
      this.dragHandler = drag;
      if (this.dragHandler) this.dragHandler.initialize(this.container, {
        getCurrentPosition: function getCurrentPosition() {
          return _this2.transform;
        },
        getZoom: function getZoom() {
          return 1;
        }
      }, {
        start: function start() {
          return null;
        },
        translate: this.onTranslate,
        drag: function drag2() {
          return null;
        }
      });
    }
    /**
     * Set zoom handler. Destroy previous zoom handler if exists.
     * @param zoom zoom handler
     * @example area.area.setZoomHandler(null) // disable zoom
     */
  }, {
    key: "setZoomHandler",
    value: function setZoomHandler(zoom) {
      if (this.zoomHandler) this.zoomHandler.destroy();
      this.zoomHandler = zoom;
      if (this.zoomHandler) this.zoomHandler.initialize(this.container, this.content.holder, this.onZoom);
    }
  }, {
    key: "setPointerFrom",
    value: function setPointerFrom(event) {
      var _this$content$getPoin = this.content.getPointerFrom(event), x = _this$content$getPoin.x, y = _this$content$getPoin.y;
      var k = this.transform.k;
      this.pointer = {
        x: x / k,
        y: y / k
      };
    }
  }, {
    key: "translate",
    value: (
      /**
       * Change position of the area
       * @param x desired x coordinate
       * @param y desired y coordinate
       * @returns true if the translation was successful, false otherwise
       * @emits translate
       * @emits translated
       */
      function() {
        var _translate = _asyncToGenerator(import_regenerator.default.mark(function _callee(x, y) {
          var position, result;
          return import_regenerator.default.wrap(function _callee$(_context) {
            while (1) switch (_context.prev = _context.next) {
              case 0:
                position = {
                  x,
                  y
                };
                _context.next = 3;
                return this.guards.translate({
                  previous: this.transform,
                  position
                });
              case 3:
                result = _context.sent;
                if (result) {
                  _context.next = 6;
                  break;
                }
                return _context.abrupt("return", false);
              case 6:
                this.transform.x = result.data.position.x;
                this.transform.y = result.data.position.y;
                this.update();
                _context.next = 11;
                return this.events.translated(result.data);
              case 11:
                return _context.abrupt("return", true);
              case 12:
              case "end":
                return _context.stop();
            }
          }, _callee, this);
        }));
        function translate(_x, _x2) {
          return _translate.apply(this, arguments);
        }
        return translate;
      }()
    )
    /**
     * Change zoom level of the area
     * @param zoom new zoom level
     * @param ox x coordinate of the origin of the zoom
     * @param oy y coordinate of the origin of the zoom
     * @param source source of the zoom
     * @returns true if the zoom was successful, false otherwise
     * @emits zoom
     * @emits zoomed
     */
  }, {
    key: "zoom",
    value: function() {
      var _zoom2 = _asyncToGenerator(import_regenerator.default.mark(function _callee2(_zoom) {
        var ox, oy, source, k, result, d, _args2 = arguments;
        return import_regenerator.default.wrap(function _callee2$(_context2) {
          while (1) switch (_context2.prev = _context2.next) {
            case 0:
              ox = _args2.length > 1 && _args2[1] !== void 0 ? _args2[1] : 0;
              oy = _args2.length > 2 && _args2[2] !== void 0 ? _args2[2] : 0;
              source = _args2.length > 3 ? _args2[3] : void 0;
              k = this.transform.k;
              _context2.next = 6;
              return this.guards.zoom({
                previous: this.transform,
                zoom: _zoom,
                source
              });
            case 6:
              result = _context2.sent;
              if (result) {
                _context2.next = 9;
                break;
              }
              return _context2.abrupt("return", true);
            case 9:
              d = (k - result.data.zoom) / (k - _zoom || 1);
              this.transform.k = result.data.zoom || 1;
              this.transform.x += ox * d;
              this.transform.y += oy * d;
              this.update();
              _context2.next = 16;
              return this.events.zoomed(result.data);
            case 16:
              return _context2.abrupt("return", false);
            case 17:
            case "end":
              return _context2.stop();
          }
        }, _callee2, this);
      }));
      function zoom(_x3) {
        return _zoom2.apply(this, arguments);
      }
      return zoom;
    }()
  }, {
    key: "destroy",
    value: function destroy() {
      this.container.removeEventListener("pointerdown", this.pointerdown);
      this.container.removeEventListener("pointermove", this.pointermove);
      window.removeEventListener("pointerup", this.pointerup);
      window.removeEventListener("resize", this.resize);
      if (this.dragHandler) this.dragHandler.destroy();
      if (this.zoomHandler) this.zoomHandler.destroy();
      this.content.holder.innerHTML = "";
    }
  }]);
  return Area2;
}();
function _createSuper$1(Derived) {
  var hasNativeReflectConstruct = _isNativeReflectConstruct$1();
  return function _createSuperInternal() {
    var Super = _getPrototypeOf(Derived), result;
    if (hasNativeReflectConstruct) {
      var NewTarget = _getPrototypeOf(this).constructor;
      result = Reflect.construct(Super, arguments, NewTarget);
    } else {
      result = Super.apply(this, arguments);
    }
    return _possibleConstructorReturn(this, result);
  };
}
function _isNativeReflectConstruct$1() {
  if (typeof Reflect === "undefined" || !Reflect.construct) return false;
  if (Reflect.construct.sham) return false;
  if (typeof Proxy === "function") return true;
  try {
    Boolean.prototype.valueOf.call(Reflect.construct(Boolean, [], function() {
    }));
    return true;
  } catch (e) {
    return false;
  }
}
var BaseAreaPlugin = function(_Scope) {
  _inherits(BaseAreaPlugin2, _Scope);
  var _super = _createSuper$1(BaseAreaPlugin2);
  function BaseAreaPlugin2() {
    _classCallCheck(this, BaseAreaPlugin2);
    return _super.apply(this, arguments);
  }
  return _createClass(BaseAreaPlugin2);
}(Scope);
var ConnectionView = _createClass(function ConnectionView2(events) {
  _classCallCheck(this, ConnectionView2);
  this.element = document.createElement("div");
  this.element.style.position = "absolute";
  this.element.style.left = "0";
  this.element.style.top = "0";
  this.element.addEventListener("contextmenu", function(event) {
    return events.contextmenu(event);
  });
});
var ElementsHolder = function() {
  function ElementsHolder2() {
    _classCallCheck(this, ElementsHolder2);
    _defineProperty(this, "views", /* @__PURE__ */ new WeakMap());
    _defineProperty(this, "viewsElements", /* @__PURE__ */ new Map());
  }
  _createClass(ElementsHolder2, [{
    key: "set",
    value: function set(context) {
      var element = context.element, type = context.type, payload = context.payload;
      if (payload !== null && payload !== void 0 && payload.id) {
        this.views.set(element, context);
        this.viewsElements.set("".concat(type, "_").concat(payload.id), element);
      }
    }
  }, {
    key: "get",
    value: function get(type, id) {
      var element = this.viewsElements.get("".concat(type, "_").concat(id));
      return element && this.views.get(element);
    }
  }, {
    key: "delete",
    value: function _delete(element) {
      var _view$payload;
      var view = this.views.get(element);
      if (view && (_view$payload = view.payload) !== null && _view$payload !== void 0 && _view$payload.id) {
        this.views["delete"](element);
        this.viewsElements["delete"]("".concat(view.type, "_").concat(view.payload.id));
      }
    }
  }]);
  return ElementsHolder2;
}();
function ownKeys$3(object, enumerableOnly) {
  var keys = Object.keys(object);
  if (Object.getOwnPropertySymbols) {
    var symbols = Object.getOwnPropertySymbols(object);
    enumerableOnly && (symbols = symbols.filter(function(sym) {
      return Object.getOwnPropertyDescriptor(object, sym).enumerable;
    })), keys.push.apply(keys, symbols);
  }
  return keys;
}
function _objectSpread$3(target) {
  for (var i = 1; i < arguments.length; i++) {
    var source = null != arguments[i] ? arguments[i] : {};
    i % 2 ? ownKeys$3(Object(source), true).forEach(function(key) {
      _defineProperty(target, key, source[key]);
    }) : Object.getOwnPropertyDescriptors ? Object.defineProperties(target, Object.getOwnPropertyDescriptors(source)) : ownKeys$3(Object(source)).forEach(function(key) {
      Object.defineProperty(target, key, Object.getOwnPropertyDescriptor(source, key));
    });
  }
  return target;
}
var NodeView = function() {
  function NodeView2(getZoom, events, guards) {
    var _this = this;
    _classCallCheck(this, NodeView2);
    _defineProperty(this, "translate", function() {
      var _ref = _asyncToGenerator(import_regenerator.default.mark(function _callee(x, y) {
        var previous, translation;
        return import_regenerator.default.wrap(function _callee$(_context) {
          while (1) switch (_context.prev = _context.next) {
            case 0:
              previous = _objectSpread$3({}, _this.position);
              _context.next = 3;
              return _this.guards.translate({
                previous,
                position: {
                  x,
                  y
                }
              });
            case 3:
              translation = _context.sent;
              if (translation) {
                _context.next = 6;
                break;
              }
              return _context.abrupt("return", false);
            case 6:
              _this.position = _objectSpread$3({}, translation.data.position);
              _this.element.style.transform = "translate(".concat(_this.position.x, "px, ").concat(_this.position.y, "px)");
              _context.next = 10;
              return _this.events.translated({
                position: _this.position,
                previous
              });
            case 10:
              return _context.abrupt("return", true);
            case 11:
            case "end":
              return _context.stop();
          }
        }, _callee);
      }));
      return function(_x, _x2) {
        return _ref.apply(this, arguments);
      };
    }());
    _defineProperty(this, "resize", function() {
      var _ref2 = _asyncToGenerator(import_regenerator.default.mark(function _callee2(width, height) {
        var size, el;
        return import_regenerator.default.wrap(function _callee2$(_context2) {
          while (1) switch (_context2.prev = _context2.next) {
            case 0:
              size = {
                width,
                height
              };
              _context2.next = 3;
              return _this.guards.resize({
                size
              });
            case 3:
              if (_context2.sent) {
                _context2.next = 5;
                break;
              }
              return _context2.abrupt("return", false);
            case 5:
              el = _this.element.children.item(0);
              if (!(!el || !(el instanceof HTMLElement))) {
                _context2.next = 8;
                break;
              }
              return _context2.abrupt("return", false);
            case 8:
              el.style.width = "".concat(width, "px");
              el.style.height = "".concat(height, "px");
              _context2.next = 12;
              return _this.events.resized({
                size
              });
            case 12:
              return _context2.abrupt("return", true);
            case 13:
            case "end":
              return _context2.stop();
          }
        }, _callee2);
      }));
      return function(_x3, _x4) {
        return _ref2.apply(this, arguments);
      };
    }());
    this.getZoom = getZoom;
    this.events = events;
    this.guards = guards;
    this.element = document.createElement("div");
    this.element.style.position = "absolute";
    this.position = {
      x: 0,
      y: 0
    };
    this.translate(0, 0);
    this.element.addEventListener("contextmenu", function(event) {
      return _this.events.contextmenu(event);
    });
    this.dragHandler = new Drag();
    this.dragHandler.initialize(this.element, {
      getCurrentPosition: function getCurrentPosition() {
        return _this.position;
      },
      getZoom: function getZoom2() {
        return _this.getZoom();
      }
    }, {
      start: this.events.picked,
      translate: this.translate,
      drag: this.events.dragged
    });
  }
  _createClass(NodeView2, [{
    key: "destroy",
    value: function destroy() {
      this.dragHandler.destroy();
    }
  }]);
  return NodeView2;
}();
function getNodesRect(nodes, views) {
  return nodes.map(function(node) {
    return {
      view: views.get(node.id),
      node
    };
  }).filter(function(item) {
    return item.view;
  }).map(function(_ref) {
    var view = _ref.view, node = _ref.node;
    var width = node.width, height = node.height;
    if (typeof width !== "undefined" && typeof height !== "undefined") {
      return {
        position: view.position,
        width,
        height
      };
    }
    return {
      position: view.position,
      width: view.element.clientWidth,
      height: view.element.clientHeight
    };
  });
}
function getBoundingBox(plugin, nodes) {
  var editor = plugin.parentScope(NodeEditor);
  var list = nodes.map(function(node) {
    return _typeof(node) === "object" ? node : editor.getNode(node);
  });
  var rects = getNodesRect(list, plugin.nodeViews);
  return getBoundingBox$1(rects);
}
function simpleNodesOrder(base) {
  var area = base;
  area.addPipe(function(context) {
    if (!context || _typeof(context) !== "object" || !("type" in context)) return context;
    if (context.type === "nodepicked") {
      var view = area.nodeViews.get(context.data.id);
      var content = area.area.content;
      if (view) {
        content.reorder(view.element, null);
      }
    }
    if (context.type === "connectioncreated") {
      var _view = area.connectionViews.get(context.data.id);
      var _content = area.area.content;
      if (_view) {
        _content.reorder(_view.element, _content.holder.firstChild);
      }
    }
    return context;
  });
}
function ownKeys$2(object, enumerableOnly) {
  var keys = Object.keys(object);
  if (Object.getOwnPropertySymbols) {
    var symbols = Object.getOwnPropertySymbols(object);
    enumerableOnly && (symbols = symbols.filter(function(sym) {
      return Object.getOwnPropertyDescriptor(object, sym).enumerable;
    })), keys.push.apply(keys, symbols);
  }
  return keys;
}
function _objectSpread$2(target) {
  for (var i = 1; i < arguments.length; i++) {
    var source = null != arguments[i] ? arguments[i] : {};
    i % 2 ? ownKeys$2(Object(source), true).forEach(function(key) {
      _defineProperty(target, key, source[key]);
    }) : Object.getOwnPropertyDescriptors ? Object.defineProperties(target, Object.getOwnPropertyDescriptors(source)) : ownKeys$2(Object(source)).forEach(function(key) {
      Object.defineProperty(target, key, Object.getOwnPropertyDescriptor(source, key));
    });
  }
  return target;
}
function restrictor(plugin, params) {
  var scaling = params !== null && params !== void 0 && params.scaling ? params.scaling === true ? {
    min: 0.1,
    max: 1
  } : params.scaling : false;
  var translation = params !== null && params !== void 0 && params.translation ? params.translation === true ? {
    left: 0,
    top: 0,
    right: 1e3,
    bottom: 1e3
  } : params.translation : false;
  function restrictZoom(zoom) {
    if (!scaling) throw new Error("scaling param isnt defined");
    var _ref = typeof scaling === "function" ? scaling() : scaling, min3 = _ref.min, max3 = _ref.max;
    if (zoom < min3) {
      return min3;
    } else if (zoom > max3) {
      return max3;
    }
    return zoom;
  }
  function restrictPosition(position) {
    if (!translation) throw new Error("translation param isnt defined");
    var nextPosition = _objectSpread$2({}, position);
    var _ref2 = typeof translation === "function" ? translation() : translation, left = _ref2.left, top = _ref2.top, right = _ref2.right, bottom = _ref2.bottom;
    if (nextPosition.x < left) {
      nextPosition.x = left;
    }
    if (nextPosition.x > right) {
      nextPosition.x = right;
    }
    if (nextPosition.y < top) {
      nextPosition.y = top;
    }
    if (nextPosition.y > bottom) {
      nextPosition.y = bottom;
    }
    return nextPosition;
  }
  plugin.addPipe(function(context) {
    if (!context || _typeof(context) !== "object" || !("type" in context)) return context;
    if (scaling && context.type === "zoom") {
      return _objectSpread$2(_objectSpread$2({}, context), {}, {
        data: _objectSpread$2(_objectSpread$2({}, context.data), {}, {
          zoom: restrictZoom(context.data.zoom)
        })
      });
    }
    if (translation && context.type === "zoomed") {
      var position = restrictPosition(plugin.area.transform);
      plugin.area.translate(position.x, position.y);
    }
    if (translation && context.type === "translate") {
      return _objectSpread$2(_objectSpread$2({}, context), {}, {
        data: _objectSpread$2(_objectSpread$2({}, context.data), {}, {
          position: restrictPosition(context.data.position)
        })
      });
    }
    return context;
  });
}
function accumulateOnCtrl() {
  var pressed = false;
  function keydown(e) {
    if (e.key === "Control") pressed = true;
  }
  function keyup(e) {
    if (e.key === "Control") pressed = false;
  }
  document.addEventListener("keydown", keydown);
  document.addEventListener("keyup", keyup);
  return {
    active: function active() {
      return pressed;
    },
    destroy: function destroy() {
      document.removeEventListener("keydown", keydown);
      document.removeEventListener("keyup", keyup);
    }
  };
}
var Selector = function() {
  function Selector2() {
    _classCallCheck(this, Selector2);
    _defineProperty(this, "entities", /* @__PURE__ */ new Map());
    _defineProperty(this, "pickId", null);
  }
  _createClass(Selector2, [{
    key: "isSelected",
    value: function isSelected(entity) {
      return this.entities.has("".concat(entity.label, "_").concat(entity.id));
    }
  }, {
    key: "add",
    value: function add(entity, accumulate) {
      if (!accumulate) this.unselectAll();
      this.entities.set("".concat(entity.label, "_").concat(entity.id), entity);
    }
  }, {
    key: "remove",
    value: function remove(entity) {
      var id = "".concat(entity.label, "_").concat(entity.id);
      var item = this.entities.get(id);
      if (item) {
        this.entities["delete"](id);
        item.unselect();
      }
    }
  }, {
    key: "unselectAll",
    value: function unselectAll() {
      var _this = this;
      _toConsumableArray(Array.from(this.entities.values())).forEach(function(item) {
        return _this.remove(item);
      });
    }
  }, {
    key: "translate",
    value: function translate(dx, dy) {
      var _this2 = this;
      this.entities.forEach(function(item) {
        return !_this2.isPicked(item) && item.translate(dx, dy);
      });
    }
  }, {
    key: "pick",
    value: function pick(entity) {
      this.pickId = "".concat(entity.label, "_").concat(entity.id);
    }
  }, {
    key: "release",
    value: function release() {
      this.pickId = null;
    }
  }, {
    key: "isPicked",
    value: function isPicked(entity) {
      return this.pickId === "".concat(entity.label, "_").concat(entity.id);
    }
  }]);
  return Selector2;
}();
function selector() {
  return new Selector();
}
function selectableNodes(base, core, options) {
  var editor = null;
  var area = base;
  var getEditor = function getEditor2() {
    return editor || (editor = area.parentScope(NodeEditor));
  };
  var twitch = 0;
  function selectNode(node) {
    if (!node.selected) {
      node.selected = true;
      area.update("node", node.id);
    }
  }
  function unselectNode(node) {
    if (node.selected) {
      node.selected = false;
      area.update("node", node.id);
    }
  }
  function add(nodeId, accumulate) {
    var node = getEditor().getNode(nodeId);
    if (!node) return;
    core.add({
      label: "node",
      id: node.id,
      translate: function translate(dx, dy) {
        var view = area.nodeViews.get(node.id);
        var current = view === null || view === void 0 ? void 0 : view.position;
        if (current) {
          view.translate(current.x + dx, current.y + dy);
        }
      },
      unselect: function unselect() {
        unselectNode(node);
      }
    }, accumulate);
    selectNode(node);
  }
  function remove(nodeId) {
    core.remove({
      id: nodeId,
      label: "node"
    });
  }
  area.addPipe(function(context) {
    if (!context || _typeof(context) !== "object" || !("type" in context)) return context;
    if (context.type === "nodepicked") {
      var pickedId = context.data.id;
      var accumulate = options.accumulating.active();
      core.pick({
        id: pickedId,
        label: "node"
      });
      twitch = null;
      add(pickedId, accumulate);
    } else if (context.type === "nodetranslated") {
      var _context$data = context.data, id = _context$data.id, position = _context$data.position, previous = _context$data.previous;
      var _dx = position.x - previous.x;
      var _dy = position.y - previous.y;
      if (core.isPicked({
        id,
        label: "node"
      })) core.translate(_dx, _dy);
    } else if (context.type === "pointerdown") {
      twitch = 0;
    } else if (context.type === "pointermove") {
      if (twitch !== null) twitch++;
    } else if (context.type === "pointerup") {
      if (twitch !== null && twitch < 4) {
        core.unselectAll();
      }
      twitch = null;
    }
    return context;
  });
  return {
    select: add,
    unselect: remove
  };
}
function showInputControl(area, visible) {
  var editor = null;
  var getEditor = function getEditor2() {
    return editor || (editor = area.parentScope(NodeEditor));
  };
  function updateInputControlVisibility(target, targetInput) {
    var node = getEditor().getNode(target);
    if (!node) return;
    var input = node.inputs[targetInput];
    if (!input) throw new Error("cannot find input");
    var previous = input.showControl;
    var connections = getEditor().getConnections();
    var hasAnyConnection = Boolean(connections.find(function(connection) {
      return connection.target === target && connection.targetInput === targetInput;
    }));
    input.showControl = visible ? visible({
      hasAnyConnection,
      input
    }) : !hasAnyConnection;
    if (input.showControl !== previous) {
      area.update("node", node.id);
    }
  }
  area.addPipe(function(context) {
    if (context.type === "connectioncreated" || context.type === "connectionremoved") {
      updateInputControlVisibility(context.data.target, context.data.targetInput);
    }
    return context;
  });
}
function ownKeys$1(object, enumerableOnly) {
  var keys = Object.keys(object);
  if (Object.getOwnPropertySymbols) {
    var symbols = Object.getOwnPropertySymbols(object);
    enumerableOnly && (symbols = symbols.filter(function(sym) {
      return Object.getOwnPropertyDescriptor(object, sym).enumerable;
    })), keys.push.apply(keys, symbols);
  }
  return keys;
}
function _objectSpread$1(target) {
  for (var i = 1; i < arguments.length; i++) {
    var source = null != arguments[i] ? arguments[i] : {};
    i % 2 ? ownKeys$1(Object(source), true).forEach(function(key) {
      _defineProperty(target, key, source[key]);
    }) : Object.getOwnPropertyDescriptors ? Object.defineProperties(target, Object.getOwnPropertyDescriptors(source)) : ownKeys$1(Object(source)).forEach(function(key) {
      Object.defineProperty(target, key, Object.getOwnPropertyDescriptor(source, key));
    });
  }
  return target;
}
function snapGrid(base, params) {
  var area = base;
  var size = typeof (params === null || params === void 0 ? void 0 : params.size) === "undefined" ? 16 : params.size;
  var dynamic = typeof (params === null || params === void 0 ? void 0 : params.dynamic) === "undefined" ? true : params.dynamic;
  function snap(value) {
    return Math.round(value / size) * size;
  }
  area.addPipe(function(context) {
    if (!context || _typeof(context) !== "object" || !("type" in context)) return context;
    if (dynamic && context.type === "nodetranslate") {
      var position = context.data.position;
      var x = snap(position.x);
      var y = snap(position.y);
      return _objectSpread$1(_objectSpread$1({}, context), {}, {
        data: _objectSpread$1(_objectSpread$1({}, context.data), {}, {
          position: {
            x,
            y
          }
        })
      });
    }
    if (!dynamic && context.type === "nodedragged") {
      var view = area.nodeViews.get(context.data.id);
      if (view) {
        var _view$position = view.position, _x = _view$position.x, _y = _view$position.y;
        view.translate(snap(_x), snap(_y));
      }
    }
    return context;
  });
}
function zoomAt(_x, _x2, _x3) {
  return _zoomAt.apply(this, arguments);
}
function _zoomAt() {
  _zoomAt = _asyncToGenerator(import_regenerator.default.mark(function _callee(plugin, nodes, params) {
    var _ref, _ref$scale, scale, editor, list, rects, boundingBox, _ref2, w, h, kw, kh, k;
    return import_regenerator.default.wrap(function _callee$(_context) {
      while (1) switch (_context.prev = _context.next) {
        case 0:
          _ref = params || {}, _ref$scale = _ref.scale, scale = _ref$scale === void 0 ? 0.9 : _ref$scale;
          editor = plugin.parentScope(NodeEditor);
          list = nodes.map(function(node) {
            return _typeof(node) === "object" ? node : editor.getNode(node);
          });
          rects = getNodesRect(list, plugin.nodeViews);
          boundingBox = getBoundingBox$1(rects);
          _ref2 = [plugin.container.clientWidth, plugin.container.clientHeight], w = _ref2[0], h = _ref2[1];
          kw = w / boundingBox.width, kh = h / boundingBox.height;
          k = Math.min(kh * scale, kw * scale, 1);
          plugin.area.transform.x = w / 2 - boundingBox.center.x * k;
          plugin.area.transform.y = h / 2 - boundingBox.center.y * k;
          _context.next = 12;
          return plugin.area.zoom(k, 0, 0);
        case 12:
        case "end":
          return _context.stop();
      }
    }, _callee);
  }));
  return _zoomAt.apply(this, arguments);
}
var index = Object.freeze({
  __proto__: null,
  getBoundingBox,
  simpleNodesOrder,
  restrictor,
  accumulateOnCtrl,
  selectableNodes,
  Selector,
  selector,
  showInputControl,
  snapGrid,
  zoomAt
});
function ownKeys(object, enumerableOnly) {
  var keys = Object.keys(object);
  if (Object.getOwnPropertySymbols) {
    var symbols = Object.getOwnPropertySymbols(object);
    enumerableOnly && (symbols = symbols.filter(function(sym) {
      return Object.getOwnPropertyDescriptor(object, sym).enumerable;
    })), keys.push.apply(keys, symbols);
  }
  return keys;
}
function _objectSpread(target) {
  for (var i = 1; i < arguments.length; i++) {
    var source = null != arguments[i] ? arguments[i] : {};
    i % 2 ? ownKeys(Object(source), true).forEach(function(key) {
      _defineProperty(target, key, source[key]);
    }) : Object.getOwnPropertyDescriptors ? Object.defineProperties(target, Object.getOwnPropertyDescriptors(source)) : ownKeys(Object(source)).forEach(function(key) {
      Object.defineProperty(target, key, Object.getOwnPropertyDescriptor(source, key));
    });
  }
  return target;
}
function _createSuper(Derived) {
  var hasNativeReflectConstruct = _isNativeReflectConstruct();
  return function _createSuperInternal() {
    var Super = _getPrototypeOf(Derived), result;
    if (hasNativeReflectConstruct) {
      var NewTarget = _getPrototypeOf(this).constructor;
      result = Reflect.construct(Super, arguments, NewTarget);
    } else {
      result = Super.apply(this, arguments);
    }
    return _possibleConstructorReturn(this, result);
  };
}
function _isNativeReflectConstruct() {
  if (typeof Reflect === "undefined" || !Reflect.construct) return false;
  if (Reflect.construct.sham) return false;
  if (typeof Proxy === "function") return true;
  try {
    Boolean.prototype.valueOf.call(Reflect.construct(Boolean, [], function() {
    }));
    return true;
  } catch (e) {
    return false;
  }
}
var AreaPlugin = function(_BaseAreaPlugin) {
  _inherits(AreaPlugin2, _BaseAreaPlugin);
  var _super = _createSuper(AreaPlugin2);
  function AreaPlugin2(container) {
    var _this;
    _classCallCheck(this, AreaPlugin2);
    _this = _super.call(this, "area");
    _defineProperty(_assertThisInitialized(_this), "nodeViews", /* @__PURE__ */ new Map());
    _defineProperty(_assertThisInitialized(_this), "connectionViews", /* @__PURE__ */ new Map());
    _defineProperty(_assertThisInitialized(_this), "elements", new ElementsHolder());
    _defineProperty(_assertThisInitialized(_this), "onContextMenu", function(event) {
      _this.emit({
        type: "contextmenu",
        data: {
          event,
          context: "root"
        }
      });
    });
    _this.container = container;
    container.style.overflow = "hidden";
    container.addEventListener("contextmenu", _this.onContextMenu);
    _this.addPipe(function(context) {
      if (!context || !(_typeof(context) === "object" && "type" in context)) return context;
      if (context.type === "nodecreated") {
        _this.addNodeView(context.data);
      }
      if (context.type === "noderemoved") {
        _this.removeNodeView(context.data.id);
      }
      if (context.type === "connectioncreated") {
        _this.addConnectionView(context.data);
      }
      if (context.type === "connectionremoved") {
        _this.removeConnectionView(context.data.id);
      }
      if (context.type === "render") {
        _this.elements.set(context.data);
      }
      if (context.type === "unmount") {
        _this.elements["delete"](context.data.element);
      }
      return context;
    });
    _this.area = new Area(container, {
      zoomed: function zoomed(params) {
        return _this.emit({
          type: "zoomed",
          data: params
        });
      },
      pointerDown: function pointerDown(position, event) {
        return _this.emit({
          type: "pointerdown",
          data: {
            position,
            event
          }
        });
      },
      pointerMove: function pointerMove(position, event) {
        return _this.emit({
          type: "pointermove",
          data: {
            position,
            event
          }
        });
      },
      pointerUp: function pointerUp(position, event) {
        return _this.emit({
          type: "pointerup",
          data: {
            position,
            event
          }
        });
      },
      resize: function resize(event) {
        return _this.emit({
          type: "resized",
          data: {
            event
          }
        });
      },
      translated: function translated(params) {
        return _this.emit({
          type: "translated",
          data: params
        });
      },
      reordered: function reordered(element) {
        return _this.emit({
          type: "reordered",
          data: {
            element
          }
        });
      }
    }, {
      translate: function translate(params) {
        return _this.emit({
          type: "translate",
          data: params
        });
      },
      zoom: function zoom(params) {
        return _this.emit({
          type: "zoom",
          data: params
        });
      }
    });
    return _this;
  }
  _createClass(AreaPlugin2, [{
    key: "addNodeView",
    value: function addNodeView(node) {
      var _this2 = this;
      var id = node.id;
      var view = new NodeView(function() {
        return _this2.area.transform.k;
      }, {
        picked: function picked() {
          return _this2.emit({
            type: "nodepicked",
            data: {
              id
            }
          });
        },
        translated: function translated(data) {
          return _this2.emit({
            type: "nodetranslated",
            data: _objectSpread({
              id
            }, data)
          });
        },
        dragged: function dragged() {
          return _this2.emit({
            type: "nodedragged",
            data: node
          });
        },
        contextmenu: function contextmenu(event) {
          return _this2.emit({
            type: "contextmenu",
            data: {
              event,
              context: node
            }
          });
        },
        resized: function resized(_ref) {
          var size = _ref.size;
          return _this2.emit({
            type: "noderesized",
            data: {
              id: node.id,
              size
            }
          });
        }
      }, {
        translate: function translate(data) {
          return _this2.emit({
            type: "nodetranslate",
            data: _objectSpread({
              id
            }, data)
          });
        },
        resize: function resize(_ref2) {
          var size = _ref2.size;
          return _this2.emit({
            type: "noderesize",
            data: {
              id: node.id,
              size
            }
          });
        }
      });
      this.nodeViews.set(id, view);
      this.area.content.add(view.element);
      this.emit({
        type: "render",
        data: {
          element: view.element,
          type: "node",
          payload: node
        }
      });
      return view;
    }
  }, {
    key: "removeNodeView",
    value: function removeNodeView(id) {
      var view = this.nodeViews.get(id);
      if (view) {
        this.emit({
          type: "unmount",
          data: {
            element: view.element
          }
        });
        this.nodeViews["delete"](id);
        this.area.content.remove(view.element);
      }
    }
  }, {
    key: "addConnectionView",
    value: function addConnectionView(connection) {
      var _this3 = this;
      var view = new ConnectionView({
        contextmenu: function contextmenu(event) {
          return _this3.emit({
            type: "contextmenu",
            data: {
              event,
              context: connection
            }
          });
        }
      });
      this.connectionViews.set(connection.id, view);
      this.area.content.add(view.element);
      this.emit({
        type: "render",
        data: {
          element: view.element,
          type: "connection",
          payload: connection
        }
      });
      return view;
    }
  }, {
    key: "removeConnectionView",
    value: function removeConnectionView(id) {
      var view = this.connectionViews.get(id);
      if (view) {
        this.emit({
          type: "unmount",
          data: {
            element: view.element
          }
        });
        this.connectionViews["delete"](id);
        this.area.content.remove(view.element);
      }
    }
    /**
     * Force update rendered element by id (node, connection, etc.)
     * @param type Element type
     * @param id Element id
     * @emits render
     */
  }, {
    key: "update",
    value: function() {
      var _update = _asyncToGenerator(import_regenerator.default.mark(function _callee(type, id) {
        var data;
        return import_regenerator.default.wrap(function _callee$(_context) {
          while (1) switch (_context.prev = _context.next) {
            case 0:
              data = this.elements.get(type, id);
              if (!data) {
                _context.next = 4;
                break;
              }
              _context.next = 4;
              return this.emit({
                type: "render",
                data
              });
            case 4:
            case "end":
              return _context.stop();
          }
        }, _callee, this);
      }));
      function update(_x, _x2) {
        return _update.apply(this, arguments);
      }
      return update;
    }()
    /**
     * Resize node
     * @param id Node id
     * @param width Desired width
     * @param height Desired height
     */
  }, {
    key: "resize",
    value: function() {
      var _resize = _asyncToGenerator(import_regenerator.default.mark(function _callee2(id, width, height) {
        var view;
        return import_regenerator.default.wrap(function _callee2$(_context2) {
          while (1) switch (_context2.prev = _context2.next) {
            case 0:
              view = this.nodeViews.get(id);
              if (!view) {
                _context2.next = 5;
                break;
              }
              _context2.next = 4;
              return view.resize(width, height);
            case 4:
              return _context2.abrupt("return", _context2.sent);
            case 5:
            case "end":
              return _context2.stop();
          }
        }, _callee2, this);
      }));
      function resize(_x3, _x4, _x5) {
        return _resize.apply(this, arguments);
      }
      return resize;
    }()
    /**
     * Translate node to position
     * @param id Node id
     * @param position Position
     */
  }, {
    key: "translate",
    value: function() {
      var _translate = _asyncToGenerator(import_regenerator.default.mark(function _callee3(id, _ref3) {
        var x, y, view;
        return import_regenerator.default.wrap(function _callee3$(_context3) {
          while (1) switch (_context3.prev = _context3.next) {
            case 0:
              x = _ref3.x, y = _ref3.y;
              view = this.nodeViews.get(id);
              if (!view) {
                _context3.next = 6;
                break;
              }
              _context3.next = 5;
              return view.translate(x, y);
            case 5:
              return _context3.abrupt("return", _context3.sent);
            case 6:
            case "end":
              return _context3.stop();
          }
        }, _callee3, this);
      }));
      function translate(_x6, _x7) {
        return _translate.apply(this, arguments);
      }
      return translate;
    }()
    /**
     * Destroy all views and remove all event listeners
     */
  }, {
    key: "destroy",
    value: function destroy() {
      var _this4 = this;
      this.container.removeEventListener("contextmenu", this.onContextMenu);
      Array.from(this.connectionViews.keys()).forEach(function(id) {
        return _this4.removeConnectionView(id);
      });
      Array.from(this.nodeViews.keys()).forEach(function(id) {
        return _this4.removeNodeView(id);
      });
      this.area.destroy();
    }
  }]);
  return AreaPlugin2;
}(BaseAreaPlugin);

export {
  usePointerListener,
  Drag,
  Zoom,
  Area,
  BaseAreaPlugin,
  NodeView,
  index,
  AreaPlugin
};
/*! Bundled license information:

rete-area-plugin/rete-area-plugin.esm.js:
  (*!
  * rete-area-plugin v2.0.0
  * (c) 2023 Vitaliy Stoliarov
  * Released under the MIT license.
  * *)
*/
//# sourceMappingURL=chunk-OM334EPU.js.map
