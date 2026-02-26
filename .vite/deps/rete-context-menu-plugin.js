import {
  _get
} from "./chunk-XQ3X4JL4.js";
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
  NodeEditor,
  Scope,
  require_regenerator
} from "./chunk-O6P36EK3.js";
import {
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

// node_modules/rete-context-menu-plugin/rete-context-menu-plugin.esm.js
var import_regenerator = __toESM(require_regenerator());
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
function createItem(_ref, key, context) {
  var _ref2 = _slicedToArray(_ref, 2), label = _ref2[0], factory = _ref2[1];
  var item = {
    label,
    key: String(key)
  };
  if (typeof factory === "function") {
    return _objectSpread(_objectSpread({}, item), {}, {
      handler: function handler() {
        return _asyncToGenerator(import_regenerator.default.mark(function _callee() {
          var node;
          return import_regenerator.default.wrap(function _callee$(_context) {
            while (1) switch (_context.prev = _context.next) {
              case 0:
                _context.next = 2;
                return factory();
              case 2:
                node = _context.sent;
                _context.next = 5;
                return context.editor.addNode(node);
              case 5:
                context.area.translate(node.id, context.area.area.pointer);
              case 6:
              case "end":
                return _context.stop();
            }
          }, _callee);
        }))();
      }
    });
  }
  return _objectSpread(_objectSpread({}, item), {}, {
    handler: function handler() {
    },
    subitems: factory.map(function(data, i) {
      return createItem(data, i, context);
    })
  });
}
function _createForOfIteratorHelper(o, allowArrayLike) {
  var it = typeof Symbol !== "undefined" && o[Symbol.iterator] || o["@@iterator"];
  if (!it) {
    if (Array.isArray(o) || (it = _unsupportedIterableToArray(o)) || allowArrayLike && o && typeof o.length === "number") {
      if (it) o = it;
      var i = 0;
      var F = function F2() {
      };
      return { s: F, n: function n() {
        if (i >= o.length) return { done: true };
        return { done: false, value: o[i++] };
      }, e: function e(_e) {
        throw _e;
      }, f: F };
    }
    throw new TypeError("Invalid attempt to iterate non-iterable instance.\nIn order to be iterable, non-array objects must have a [Symbol.iterator]() method.");
  }
  var normalCompletion = true, didErr = false, err;
  return { s: function s() {
    it = it.call(o);
  }, n: function n() {
    var step = it.next();
    normalCompletion = step.done;
    return step;
  }, e: function e(_e2) {
    didErr = true;
    err = _e2;
  }, f: function f() {
    try {
      if (!normalCompletion && it["return"] != null) it["return"]();
    } finally {
      if (didErr) throw err;
    }
  } };
}
function _unsupportedIterableToArray(o, minLen) {
  if (!o) return;
  if (typeof o === "string") return _arrayLikeToArray(o, minLen);
  var n = Object.prototype.toString.call(o).slice(8, -1);
  if (n === "Object" && o.constructor) n = o.constructor.name;
  if (n === "Map" || n === "Set") return Array.from(o);
  if (n === "Arguments" || /^(?:Ui|I)nt(?:8|16|32)(?:Clamped)?Array$/.test(n)) return _arrayLikeToArray(o, minLen);
}
function _arrayLikeToArray(arr, len) {
  if (len == null || len > arr.length) len = arr.length;
  for (var i = 0, arr2 = new Array(len); i < len; i++) arr2[i] = arr[i];
  return arr2;
}
function setup(nodes) {
  return function(context, plugin) {
    var area = plugin.parentScope(BaseAreaPlugin);
    var editor = area.parentScope(NodeEditor);
    if (context === "root") {
      return {
        searchBar: true,
        list: nodes.map(function(item, i) {
          return createItem(item, i, {
            editor,
            area
          });
        })
      };
    }
    var deleteItem = {
      label: "Delete",
      key: "delete",
      handler: function handler() {
        return _asyncToGenerator(import_regenerator.default.mark(function _callee() {
          var connectionId, nodeId, connections, _iterator, _step, connection;
          return import_regenerator.default.wrap(function _callee$(_context) {
            while (1) switch (_context.prev = _context.next) {
              case 0:
                if (!("source" in context && "target" in context)) {
                  _context.next = 6;
                  break;
                }
                connectionId = context.id;
                _context.next = 4;
                return editor.removeConnection(connectionId);
              case 4:
                _context.next = 27;
                break;
              case 6:
                nodeId = context.id;
                connections = editor.getConnections().filter(function(c) {
                  return c.source === nodeId || c.target === nodeId;
                });
                _iterator = _createForOfIteratorHelper(connections);
                _context.prev = 9;
                _iterator.s();
              case 11:
                if ((_step = _iterator.n()).done) {
                  _context.next = 17;
                  break;
                }
                connection = _step.value;
                _context.next = 15;
                return editor.removeConnection(connection.id);
              case 15:
                _context.next = 11;
                break;
              case 17:
                _context.next = 22;
                break;
              case 19:
                _context.prev = 19;
                _context.t0 = _context["catch"](9);
                _iterator.e(_context.t0);
              case 22:
                _context.prev = 22;
                _iterator.f();
                return _context.finish(22);
              case 25:
                _context.next = 27;
                return editor.removeNode(nodeId);
              case 27:
              case "end":
                return _context.stop();
            }
          }, _callee, null, [[9, 19, 22, 25]]);
        }))();
      }
    };
    var clone = context.clone;
    var cloneItem = clone && {
      label: "Clone",
      key: "clone",
      handler: function handler() {
        return _asyncToGenerator(import_regenerator.default.mark(function _callee2() {
          var node;
          return import_regenerator.default.wrap(function _callee2$(_context2) {
            while (1) switch (_context2.prev = _context2.next) {
              case 0:
                node = clone();
                _context2.next = 3;
                return editor.addNode(node);
              case 3:
                area.translate(node.id, area.area.pointer);
              case 4:
              case "end":
                return _context2.stop();
            }
          }, _callee2);
        }))();
      }
    };
    return {
      searchBar: false,
      list: [deleteItem].concat(_toConsumableArray(cloneItem ? [cloneItem] : []))
    };
  };
}
var index$1 = Object.freeze({
  __proto__: null,
  setup
});
var index = Object.freeze({
  __proto__: null,
  classic: index$1
});
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
var ContextMenuPlugin = function(_Scope) {
  _inherits(ContextMenuPlugin2, _Scope);
  var _super = _createSuper(ContextMenuPlugin2);
  function ContextMenuPlugin2(props) {
    var _this;
    _classCallCheck(this, ContextMenuPlugin2);
    _this = _super.call(this, "context-menu");
    _this.props = props;
    return _this;
  }
  _createClass(ContextMenuPlugin2, [{
    key: "setParent",
    value: function setParent(scope) {
      var _this2 = this;
      _get(_getPrototypeOf(ContextMenuPlugin2.prototype), "setParent", this).call(this, scope);
      var area = this.parentScope(BaseAreaPlugin);
      var container = area.container;
      if (!container || !(container instanceof HTMLElement)) throw new Error("container expected");
      var element = document.createElement("div");
      element.style.display = "none";
      element.style.position = "fixed";
      this.addPipe(function(context) {
        var parent = _this2.parentScope();
        if (!context || _typeof(context) !== "object" || !("type" in context)) return context;
        if (context.type === "unmount") {
          if (context.data.element === element) {
            element.style.display = "none";
          }
        } else if (context.type === "contextmenu") {
          context.data.event.preventDefault();
          context.data.event.stopPropagation();
          var _this2$props$items = _this2.props.items(context.data.context, _this2), searchBar = _this2$props$items.searchBar, list = _this2$props$items.list;
          container.appendChild(element);
          element.style.left = "".concat(context.data.event.clientX, "px");
          element.style.top = "".concat(context.data.event.clientY, "px");
          element.style.display = "";
          parent.emit({
            type: "render",
            data: {
              type: "contextmenu",
              element,
              searchBar,
              onHide: function onHide() {
                parent.emit({
                  type: "unmount",
                  data: {
                    element
                  }
                });
              },
              items: list
            }
          });
        } else if (context.type === "pointerdown") {
          if (!context.data.event.composedPath().includes(element)) {
            parent.emit({
              type: "unmount",
              data: {
                element
              }
            });
          }
        }
        return context;
      });
    }
  }]);
  return ContextMenuPlugin2;
}(Scope);
export {
  ContextMenuPlugin,
  index as Presets
};
/*! Bundled license information:

rete-context-menu-plugin/rete-context-menu-plugin.esm.js:
  (*!
  * rete-context-menu-plugin v2.0.2
  * (c) 2024 Vitaliy Stoliarov
  * Released under the MIT license.
  * *)
*/
//# sourceMappingURL=rete-context-menu-plugin.js.map
