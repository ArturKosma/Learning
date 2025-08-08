import {
  _extends,
  _objectWithoutProperties
} from "./chunk-HCAUJSZP.js";
import {
  dt,
  lt
} from "./chunk-KTQQQ4GN.js";
import {
  _get
} from "./chunk-XQ3X4JL4.js";
import {
  classicConnectionPath,
  getDOMSocketPosition,
  loopConnectionPath
} from "./chunk-C2PZ3BDC.js";
import {
  _slicedToArray
} from "./chunk-37VOTOQP.js";
import {
  BaseAreaPlugin
} from "./chunk-OM334EPU.js";
import "./chunk-W4S6QAMR.js";
import {
  Scope,
  classic,
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
  require_react_dom
} from "./chunk-XB4SY35X.js";
import {
  require_react
} from "./chunk-4LE5KZDT.js";
import {
  __toESM
} from "./chunk-PLDDJCW6.js";

// node_modules/rete-react-plugin/rete-react-plugin.esm.js
var React = __toESM(require_react());
var import_react = __toESM(require_react());
var ReactDOM = __toESM(require_react_dom());
var import_react_dom = __toESM(require_react_dom());
var import_regenerator = __toESM(require_regenerator());

// node_modules/@babel/runtime/helpers/esm/taggedTemplateLiteral.js
function _taggedTemplateLiteral(e, t) {
  return t || (t = e.slice(0)), Object.freeze(Object.defineProperties(e, {
    raw: {
      value: Object.freeze(t)
    }
  }));
}

// node_modules/rete-react-plugin/rete-react-plugin.esm.js
function getRenderer(props) {
  var createRoot = props === null || props === void 0 ? void 0 : props.createRoot;
  var wrappers = /* @__PURE__ */ new WeakMap();
  function getWrapper(container) {
    var wrapper = wrappers.get(container);
    if (wrapper) return wrapper;
    var span = document.createElement("span");
    container.appendChild(span);
    return wrappers.set(container, span).get(container);
  }
  function removeWrapper(container) {
    var wrapper = wrappers.get(container);
    if (wrapper) wrapper.remove();
    wrappers["delete"](container);
  }
  if (createRoot) {
    var roots = /* @__PURE__ */ new WeakMap();
    return {
      mount: function(element, container) {
        var wrapper = getWrapper(container);
        if (!roots.has(wrapper)) {
          roots.set(wrapper, createRoot(wrapper));
        }
        var root = roots.get(wrapper);
        return root.render(element);
      },
      unmount: function unmount(container) {
        var wrapper = getWrapper(container);
        var root = roots.get(wrapper);
        if (root) {
          root.unmount();
          roots["delete"](wrapper);
          removeWrapper(container);
        }
      }
    };
  }
  return {
    mount: function(element, container) {
      return ReactDOM.render(element, getWrapper(container));
    },
    unmount: function unmount(container) {
      ReactDOM.unmountComponentAtNode(getWrapper(container));
      removeWrapper(container);
    }
  };
}
function Root(_ref) {
  var children = _ref.children, rendered = _ref.rendered;
  (0, import_react.useEffect)(function() {
    rendered();
  });
  return children;
}
function syncFlush() {
  var ready = (0, import_react.useRef)(false);
  (0, import_react.useEffect)(function() {
    ready.current = true;
  }, []);
  return {
    apply: function apply(f) {
      if (ready.current) {
        queueMicrotask(function() {
          (0, import_react_dom.flushSync)(f);
        });
      } else {
        f();
      }
    }
  };
}
function useRete(create) {
  var _useState = (0, import_react.useState)(null), _useState2 = _slicedToArray(_useState, 2), container = _useState2[0], setContainer = _useState2[1];
  var editorRef = (0, import_react.useRef)();
  var _useState3 = (0, import_react.useState)(null), _useState4 = _slicedToArray(_useState3, 2), editor = _useState4[0], setEditor = _useState4[1];
  var ref = (0, import_react.useRef)(null);
  (0, import_react.useEffect)(function() {
    if (container) {
      if (editorRef.current) {
        editorRef.current.destroy();
        container.innerHTML = "";
      }
      void create(container).then(function(value) {
        editorRef.current = value;
        setEditor(value);
      });
    }
  }, [container, create]);
  (0, import_react.useEffect)(function() {
    return function() {
      if (editorRef.current) {
        editorRef.current.destroy();
      }
    };
  }, []);
  (0, import_react.useEffect)(function() {
    if (ref.current) {
      setContainer(ref.current);
    }
  }, [ref.current]);
  return [ref, editor];
}
var ConnectionContext = (0, import_react.createContext)({
  start: null,
  end: null,
  path: null
});
function ConnectionWrapper(props) {
  var children = props.children;
  var _useState = (0, import_react.useState)(null), _useState2 = _slicedToArray(_useState, 2), computedStart = _useState2[0], setStart = _useState2[1];
  var _useState3 = (0, import_react.useState)(null), _useState4 = _slicedToArray(_useState3, 2), computedEnd = _useState4[0], setEnd = _useState4[1];
  var _useState5 = (0, import_react.useState)(null), _useState6 = _slicedToArray(_useState5, 2), path = _useState6[0], setPath = _useState6[1];
  var start = "x" in props.start ? props.start : computedStart;
  var end = "x" in props.end ? props.end : computedEnd;
  var flush = syncFlush();
  (0, import_react.useEffect)(function() {
    var unwatch1 = typeof props.start === "function" && props.start(function(s) {
      flush.apply(function() {
        setStart(s);
      });
    });
    var unwatch2 = typeof props.end === "function" && props.end(function(s) {
      flush.apply(function() {
        setEnd(s);
      });
    });
    return function() {
      if (unwatch1) unwatch1();
      if (unwatch2) unwatch2();
    };
  }, []);
  (0, import_react.useEffect)(function() {
    if (start && end) void props.path(start, end).then(function(p) {
      flush.apply(function() {
        setPath(p);
      });
    });
  }, [start, end]);
  return React.createElement(ConnectionContext.Provider, {
    value: {
      start,
      end,
      path
    }
  }, children);
}
function useConnection() {
  return (0, import_react.useContext)(ConnectionContext);
}
var _templateObject$b;
var _templateObject2$3;
var Svg = dt.svg(_templateObject$b || (_templateObject$b = _taggedTemplateLiteral(["\n    overflow: visible !important;\n    position: absolute;\n    pointer-events: none;\n    width: 9999px;\n    height: 9999px;\n"])));
var Path = dt.path(_templateObject2$3 || (_templateObject2$3 = _taggedTemplateLiteral(["\n    fill: none;\n    stroke-width: 5px;\n    stroke: steelblue;\n    pointer-events: auto;\n    ", "\n"])), function(props) {
  var _props$styles;
  return (_props$styles = props.styles) === null || _props$styles === void 0 ? void 0 : _props$styles.call(props, props);
});
function Connection(props) {
  var _useConnection = useConnection(), path = _useConnection.path;
  if (!path) return null;
  return React.createElement(Svg, {
    "data-testid": "connection"
  }, React.createElement(Path, {
    styles: props.styles,
    d: path
  }));
}
function _createForOfIteratorHelper$1(r, e) {
  var t = "undefined" != typeof Symbol && r[Symbol.iterator] || r["@@iterator"];
  if (!t) {
    if (Array.isArray(r) || (t = _unsupportedIterableToArray$1(r)) || e && r && "number" == typeof r.length) {
      t && (r = t);
      var _n = 0, F = function F2() {
      };
      return { s: F, n: function n() {
        return _n >= r.length ? { done: true } : { done: false, value: r[_n++] };
      }, e: function e2(r2) {
        throw r2;
      }, f: F };
    }
    throw new TypeError("Invalid attempt to iterate non-iterable instance.\nIn order to be iterable, non-array objects must have a [Symbol.iterator]() method.");
  }
  var o, a = true, u = false;
  return { s: function s() {
    t = t.call(r);
  }, n: function n() {
    var r2 = t.next();
    return a = r2.done, r2;
  }, e: function e2(r2) {
    u = true, o = r2;
  }, f: function f() {
    try {
      a || null == t["return"] || t["return"]();
    } finally {
      if (u) throw o;
    }
  } };
}
function _unsupportedIterableToArray$1(r, a) {
  if (r) {
    if ("string" == typeof r) return _arrayLikeToArray$1(r, a);
    var t = {}.toString.call(r).slice(8, -1);
    return "Object" === t && r.constructor && (t = r.constructor.name), "Map" === t || "Set" === t ? Array.from(r) : "Arguments" === t || /^(?:Ui|I)nt(?:8|16|32)(?:Clamped)?Array$/.test(t) ? _arrayLikeToArray$1(r, a) : void 0;
  }
}
function _arrayLikeToArray$1(r, a) {
  (null == a || a > r.length) && (a = r.length);
  for (var e = 0, n = Array(a); e < a; e++) n[e] = r[e];
  return n;
}
function copyEvent(e) {
  var newEvent = new e.constructor(e.type);
  var current = newEvent;
  while (current = Object.getPrototypeOf(current)) {
    var keys = Object.getOwnPropertyNames(current);
    var _iterator = _createForOfIteratorHelper$1(keys), _step;
    try {
      for (_iterator.s(); !(_step = _iterator.n()).done; ) {
        var k = _step.value;
        var item = newEvent[k];
        if (typeof item === "function") continue;
        Object.defineProperty(newEvent, k, {
          value: e[k]
        });
      }
    } catch (err) {
      _iterator.e(err);
    } finally {
      _iterator.f();
    }
  }
  return newEvent;
}
var rootPrefix = "__reactContainer$";
function findReactRoot(element) {
  var current = element;
  while (current) {
    if (current._reactRootContainer || Object.keys(current).some(function(key) {
      return key.startsWith(rootPrefix);
    })) return current;
    current = current.parentElement;
  }
}
function ownKeys$1(e, r) {
  var t = Object.keys(e);
  if (Object.getOwnPropertySymbols) {
    var o = Object.getOwnPropertySymbols(e);
    r && (o = o.filter(function(r2) {
      return Object.getOwnPropertyDescriptor(e, r2).enumerable;
    })), t.push.apply(t, o);
  }
  return t;
}
function _objectSpread$1(e) {
  for (var r = 1; r < arguments.length; r++) {
    var t = null != arguments[r] ? arguments[r] : {};
    r % 2 ? ownKeys$1(Object(t), true).forEach(function(r2) {
      _defineProperty(e, r2, t[r2]);
    }) : Object.getOwnPropertyDescriptors ? Object.defineProperties(e, Object.getOwnPropertyDescriptors(t)) : ownKeys$1(Object(t)).forEach(function(r2) {
      Object.defineProperty(e, r2, Object.getOwnPropertyDescriptor(t, r2));
    });
  }
  return e;
}
function useDrag(translate, getPointer) {
  return {
    start: function start(e) {
      var previous = _objectSpread$1({}, getPointer(e));
      function move(moveEvent) {
        var current = _objectSpread$1({}, getPointer(moveEvent));
        var dx = current.x - previous.x;
        var dy = current.y - previous.y;
        previous = current;
        translate(dx, dy);
      }
      function up() {
        window.removeEventListener("pointermove", move);
        window.removeEventListener("pointerup", up);
        window.removeEventListener("pointercancel", up);
      }
      window.addEventListener("pointermove", move);
      window.addEventListener("pointerup", up);
      window.addEventListener("pointercancel", up);
    }
  };
}
function useNoDrag(ref, disabled) {
  React.useEffect(function() {
    var handleClick = function handleClick2(e) {
      if (disabled) return;
      var root = findReactRoot(e.target);
      var target = React.version.startsWith("16") ? document : root;
      if (target) {
        e.stopPropagation();
        target.dispatchEvent(copyEvent(e));
      }
    };
    var el = ref.current;
    el === null || el === void 0 ? void 0 : el.addEventListener("pointerdown", handleClick);
    return function() {
      el === null || el === void 0 ? void 0 : el.removeEventListener("pointerdown", handleClick);
    };
  }, [ref, disabled]);
}
function NoDrag(props) {
  var ref = React.useRef(null);
  useNoDrag(ref, props.disabled);
  return React.createElement("span", {
    ref
  }, props.children);
}
var drag = Object.freeze({
  __proto__: null,
  useDrag,
  useNoDrag,
  NoDrag
});
var _templateObject$a;
var Input = dt.input(_templateObject$a || (_templateObject$a = _taggedTemplateLiteral(["\n  width: 100%;\n  border-radius: 30px;\n  background-color: white;\n  padding: 2px 6px;\n  border: 1px solid #999;\n  font-size: 110%;\n  box-sizing: border-box;\n  ", "\n"])), function(props) {
  var _props$styles;
  return (_props$styles = props.styles) === null || _props$styles === void 0 ? void 0 : _props$styles.call(props, props);
});
function Control(props) {
  var _React$useState = React.useState(props.data.value), _React$useState2 = _slicedToArray(_React$useState, 2), value = _React$useState2[0], setValue = _React$useState2[1];
  var ref = React.useRef(null);
  useNoDrag(ref);
  React.useEffect(function() {
    setValue(props.data.value);
  }, [props.data.value]);
  return React.createElement(Input, {
    value,
    type: props.data.type,
    ref,
    readOnly: props.data.readonly,
    onChange: function onChange(e) {
      var val = props.data.type === "number" ? +e.target.value : e.target.value;
      setValue(val);
      props.data.setValue(val);
    },
    styles: props.styles
  });
}
var $nodecolor = "rgba(110,136,255,0.8)";
var $nodecolorselected = "#ffd92c";
var $socketsize = 24;
var $socketmargin = 6;
var $socketcolor = "#96b38a";
var $nodewidth = 180;
var vars = Object.freeze({
  __proto__: null,
  $nodecolor,
  $nodecolorselected,
  $socketsize,
  $socketmargin,
  $socketcolor,
  $nodewidth
});
var _excluded$2 = ["init", "unmount"];
function RefComponent(_ref) {
  var init = _ref.init, unmount = _ref.unmount, props = _objectWithoutProperties(_ref, _excluded$2);
  var ref = React.useRef(null);
  React.useEffect(function() {
    var element = ref.current;
    return function() {
      if (element) unmount(element);
    };
  }, []);
  React.useEffect(function() {
    if (ref.current) init(ref.current);
  });
  return React.createElement("span", _extends({}, props, {
    ref
  }));
}
var _excluded$1 = ["name", "emit", "payload"];
function RefControl(_ref) {
  var name = _ref.name, emit = _ref.emit, payload = _ref.payload, props = _objectWithoutProperties(_ref, _excluded$1);
  return React.createElement(RefComponent, _extends({}, props, {
    className: name,
    init: function init(ref) {
      emit({
        type: "render",
        data: {
          type: "control",
          element: ref,
          payload
        }
      });
    },
    unmount: function unmount(ref) {
      emit({
        type: "unmount",
        data: {
          element: ref
        }
      });
    }
  }));
}
var _excluded = ["name", "emit", "nodeId", "side", "socketKey", "payload"];
function RefSocket(_ref) {
  var name = _ref.name, emit = _ref.emit, nodeId = _ref.nodeId, side = _ref.side, socketKey = _ref.socketKey, payload = _ref.payload, props = _objectWithoutProperties(_ref, _excluded);
  return React.createElement(RefComponent, _extends({}, props, {
    className: name,
    init: function init(ref) {
      emit({
        type: "render",
        data: {
          type: "socket",
          side,
          key: socketKey,
          nodeId,
          element: ref,
          payload
        }
      });
    },
    unmount: function unmount(ref) {
      emit({
        type: "unmount",
        data: {
          element: ref
        }
      });
    }
  }));
}
var _templateObject$9;
var _templateObject2$2;
var NodeStyles = dt.div(_templateObject$9 || (_templateObject$9 = _taggedTemplateLiteral(["\n    background: ", ";\n    border: 2px solid #4e58bf;\n    border-radius: 10px;\n    cursor: pointer;\n    box-sizing: border-box;\n    width: ", ";\n    height: ", ";\n    padding-bottom: 6px;\n    position: relative;\n    user-select: none;\n    line-height: initial;\n    font-family: Arial;\n\n    &:hover {\n        background: lighten(", ",4%);\n    }\n    ", "\n    .title {\n        color: white;\n        font-family: sans-serif;\n        font-size: 18px;\n        padding: 8px;\n    }\n    .output {\n        text-align: right;\n    }\n    .input {\n        text-align: left;\n    }\n    .output-socket {\n        text-align: right;\n        margin-right: -", "px;\n        display: inline-block;\n    }\n    .input-socket {\n        text-align: left;\n        margin-left: -", "px;\n        display: inline-block;\n    }\n    .input-title,.output-title {\n        vertical-align: middle;\n        color: white;\n        display: inline-block;\n        font-family: sans-serif;\n        font-size: 14px;\n        margin: ", "px;\n        line-height: ", "px;\n    }\n    .input-control {\n        z-index: 1;\n        width: calc(100% - ", "px);\n        vertical-align: middle;\n        display: inline-block;\n    }\n    .control {\n        display: block;\n        padding: ", "px ", "px;\n    }\n    ", "\n"])), $nodecolor, function(props) {
  return Number.isFinite(props.width) ? "".concat(props.width, "px") : "".concat($nodewidth, "px");
}, function(props) {
  return Number.isFinite(props.height) ? "".concat(props.height, "px") : "auto";
}, $nodecolor, function(props) {
  return props.selected && lt(_templateObject2$2 || (_templateObject2$2 = _taggedTemplateLiteral(["\n        background: ", ";\n        border-color: #e3c000;\n    "])), $nodecolorselected);
}, $socketsize / 2 + $socketmargin, $socketsize / 2 + $socketmargin, $socketmargin, $socketsize, $socketsize + 2 * $socketmargin, $socketmargin, $socketsize / 2 + $socketmargin, function(props) {
  var _props$styles;
  return (_props$styles = props.styles) === null || _props$styles === void 0 ? void 0 : _props$styles.call(props, props);
});
function sortByIndex(entries) {
  entries.sort(function(a, b) {
    var _a$, _b$;
    var ai = ((_a$ = a[1]) === null || _a$ === void 0 ? void 0 : _a$.index) || 0;
    var bi = ((_b$ = b[1]) === null || _b$ === void 0 ? void 0 : _b$.index) || 0;
    return ai - bi;
  });
}
function Node(props) {
  var inputs = Object.entries(props.data.inputs);
  var outputs = Object.entries(props.data.outputs);
  var controls = Object.entries(props.data.controls);
  var selected = props.data.selected || false;
  var _props$data = props.data, id = _props$data.id, label = _props$data.label, width = _props$data.width, height = _props$data.height;
  sortByIndex(inputs);
  sortByIndex(outputs);
  sortByIndex(controls);
  return React.createElement(NodeStyles, {
    selected,
    width,
    height,
    styles: props.styles,
    "data-testid": "node"
  }, React.createElement("div", {
    className: "title",
    "data-testid": "title"
  }, label), outputs.map(function(_ref) {
    var _ref2 = _slicedToArray(_ref, 2), key = _ref2[0], output = _ref2[1];
    return output && React.createElement("div", {
      className: "output",
      key,
      "data-testid": "output-".concat(key)
    }, React.createElement("div", {
      className: "output-title",
      "data-testid": "output-title"
    }, output.label), React.createElement(RefSocket, {
      name: "output-socket",
      side: "output",
      socketKey: key,
      nodeId: id,
      emit: props.emit,
      payload: output.socket,
      "data-testid": "output-socket"
    }));
  }), controls.map(function(_ref3) {
    var _ref4 = _slicedToArray(_ref3, 2), key = _ref4[0], control = _ref4[1];
    return control ? React.createElement(RefControl, {
      key,
      name: "control",
      emit: props.emit,
      payload: control,
      "data-testid": "control-".concat(key)
    }) : null;
  }), inputs.map(function(_ref5) {
    var _ref6 = _slicedToArray(_ref5, 2), key = _ref6[0], input = _ref6[1];
    return input && React.createElement("div", {
      className: "input",
      key,
      "data-testid": "input-".concat(key)
    }, React.createElement(RefSocket, {
      name: "input-socket",
      side: "input",
      socketKey: key,
      nodeId: id,
      emit: props.emit,
      payload: input.socket,
      "data-testid": "input-socket"
    }), input && (!input.control || !input.showControl) && React.createElement("div", {
      className: "input-title",
      "data-testid": "input-title"
    }, input.label), input.control && input.showControl && React.createElement(RefControl, {
      key,
      name: "input-control",
      emit: props.emit,
      payload: input.control,
      "data-testid": "input-control"
    }));
  }));
}
var _templateObject$8;
var _templateObject2$1;
var Styles$4 = dt.div(_templateObject$8 || (_templateObject$8 = _taggedTemplateLiteral(["\n    display: inline-block;\n    cursor: pointer;\n    border: 1px solid white;\n    border-radius: ", "px;\n    width: ", "px;\n    height: ", "px;\n    vertical-align: middle;\n    background: ", ";\n    z-index: 2;\n    box-sizing: border-box;\n    &:hover {\n      border-width: 4px;\n    }\n    &.multiple {\n      border-color: yellow;\n    }\n"])), $socketsize / 2, $socketsize, $socketsize, $socketcolor);
var Hoverable = dt.div(_templateObject2$1 || (_templateObject2$1 = _taggedTemplateLiteral(["\n    border-radius: ", "px;\n    padding: ", "px;\n    &:hover ", " {\n      border-width: 4px;\n    }\n"])), ($socketsize + $socketmargin * 2) / 2, $socketmargin, Styles$4);
function Socket(props) {
  return React.createElement(Hoverable, null, React.createElement(Styles$4, {
    title: props.data.name
  }));
}
function setup$3(props) {
  var positionWatcher = typeof (props === null || props === void 0 ? void 0 : props.socketPositionWatcher) === "undefined" ? getDOMSocketPosition() : props.socketPositionWatcher;
  var _ref = (props === null || props === void 0 ? void 0 : props.customize) || {}, node = _ref.node, connection = _ref.connection, socket = _ref.socket, control = _ref.control;
  return {
    attach: function attach(plugin) {
      positionWatcher.attach(plugin);
    },
    // eslint-disable-next-line complexity
    render: function render2(context, plugin) {
      if (context.data.type === "node") {
        var parent = plugin.parentScope();
        var Component = node ? node(context.data) : Node;
        return Component && React.createElement(Component, {
          data: context.data.payload,
          emit: function emit(data) {
            return void parent.emit(data);
          }
        });
      } else if (context.data.type === "connection") {
        var _Component = connection ? connection(context.data) : Connection;
        var payload = context.data.payload;
        var sourceOutput = payload.sourceOutput, targetInput = payload.targetInput, source = payload.source, target = payload.target;
        return _Component && React.createElement(ConnectionWrapper, {
          start: context.data.start || function(change) {
            return positionWatcher.listen(source, "output", sourceOutput, change);
          },
          end: context.data.end || function(change) {
            return positionWatcher.listen(target, "input", targetInput, change);
          },
          path: function() {
            var _ref2 = _asyncToGenerator(import_regenerator.default.mark(function _callee(start, end) {
              var response, _response$data, path, points, curvature;
              return import_regenerator.default.wrap(function _callee$(_context) {
                while (1) switch (_context.prev = _context.next) {
                  case 0:
                    _context.next = 2;
                    return plugin.emit({
                      type: "connectionpath",
                      data: {
                        payload,
                        points: [start, end]
                      }
                    });
                  case 2:
                    response = _context.sent;
                    if (response) {
                      _context.next = 5;
                      break;
                    }
                    return _context.abrupt("return", "");
                  case 5:
                    _response$data = response.data, path = _response$data.path, points = _response$data.points;
                    curvature = 0.3;
                    if (!(!path && points.length !== 2)) {
                      _context.next = 9;
                      break;
                    }
                    throw new Error("cannot render connection with a custom number of points");
                  case 9:
                    if (path) {
                      _context.next = 11;
                      break;
                    }
                    return _context.abrupt("return", payload.isLoop ? loopConnectionPath(points, curvature, 120) : classicConnectionPath(points, curvature));
                  case 11:
                    return _context.abrupt("return", path);
                  case 12:
                  case "end":
                    return _context.stop();
                }
              }, _callee);
            }));
            return function(_x, _x2) {
              return _ref2.apply(this, arguments);
            };
          }()
        }, React.createElement(_Component, {
          data: context.data.payload
        }));
      } else if (context.data.type === "socket") {
        var _Component2 = socket ? socket(context.data) : Socket;
        return _Component2 && context.data.payload && React.createElement(_Component2, {
          data: context.data.payload
        });
      } else if (context.data.type === "control") {
        var _Component3 = control && context.data.payload ? control(context.data) : context.data.payload instanceof classic.InputControl ? Control : null;
        return _Component3 && React.createElement(_Component3, {
          data: context.data.payload
        });
      }
    }
  };
}
var index$4 = Object.freeze({
  __proto__: null,
  vars,
  setup: setup$3,
  Connection,
  useConnection,
  Control,
  InputControl: Control,
  Node,
  NodeStyles,
  RefControl,
  RefSocket,
  Socket
});
function useDebounce(cb, timeout) {
  var ref = (0, import_react.useRef)();
  function cancel() {
    if (ref.current) {
      clearTimeout(ref.current);
    }
  }
  var func = function func2() {
    cancel();
    ref.current = setTimeout(function() {
      cb();
    }, timeout);
  };
  (0, import_react.useEffect)(function() {
    return cancel;
  }, []);
  return [func, cancel];
}
var $contextColor = "rgba(110,136,255,0.8)";
var $contextColorLight = "rgba(130, 153, 255, 0.8)";
var $contextColorDark = "rgba(69, 103, 255, 0.8)";
var $contextMenuRound = "5px";
var $width = 120;
var _templateObject$7;
var CommonStyle = dt.div(_templateObject$7 || (_templateObject$7 = _taggedTemplateLiteral(["\n  color: #fff;\n  padding: 4px;\n  border-bottom: 1px solid ", ";\n  background-color: ", ";\n  cursor: pointer;\n  width: 100%;\n  position: relative;\n  &:first-child {\n    border-top-left-radius: ", ";\n    border-top-right-radius: ", ";\n  }\n  &:last-child {\n    border-bottom-left-radius: ", ";\n    border-bottom-right-radius: ", ";\n  }\n  &:hover {\n    background-color: ", ";\n  }\n"])), $contextColorDark, $contextColor, $contextMenuRound, $contextMenuRound, $contextMenuRound, $contextMenuRound, $contextColorLight);
var _templateObject$6;
var _templateObject2;
var _templateObject3;
var ItemStyle = dt(CommonStyle)(_templateObject$6 || (_templateObject$6 = _taggedTemplateLiteral(["\n    ", "\n"])), function(props) {
  return props.hasSubitems && lt(_templateObject2 || (_templateObject2 = _taggedTemplateLiteral(["&:after {\n    content: '►';\n    position: absolute;\n    opacity: 0.6;\n    right: 5px;\n    top: 5px;\n    }"])));
});
var SubitemStyles = dt.div(_templateObject3 || (_templateObject3 = _taggedTemplateLiteral(["\n    position: absolute;\n    top: 0;\n    left: 100%;\n    width: ", "px;\n"])), $width);
function ItemElement(props) {
  var _props$components, _props$components$ite, _props$components2, _props$components2$su;
  var _React$useState = React.useState(false), _React$useState2 = _slicedToArray(_React$useState, 2), visibleSubitems = _React$useState2[0], setVisibleSubitems = _React$useState2[1];
  var setInvisibile = React.useCallback(function() {
    setVisibleSubitems(false);
  }, [setVisibleSubitems]);
  var _useDebounce = useDebounce(setInvisibile, props.delay), _useDebounce2 = _slicedToArray(_useDebounce, 2), hide = _useDebounce2[0], cancelHide = _useDebounce2[1];
  var Component = ((_props$components = props.components) === null || _props$components === void 0 ? void 0 : (_props$components$ite = _props$components.item) === null || _props$components$ite === void 0 ? void 0 : _props$components$ite.call(_props$components, props.data)) || ItemStyle;
  var Subitems = ((_props$components2 = props.components) === null || _props$components2 === void 0 ? void 0 : (_props$components2$su = _props$components2.subitems) === null || _props$components2$su === void 0 ? void 0 : _props$components2$su.call(_props$components2, props.data)) || SubitemStyles;
  return React.createElement(Component, {
    onClick: function onClick(e) {
      e.stopPropagation();
      props.data.handler();
      props.hide();
    },
    hasSubitems: Boolean(props.data.subitems),
    onPointerDown: function onPointerDown(e) {
      e.stopPropagation();
    },
    onPointerOver: function onPointerOver() {
      cancelHide();
      setVisibleSubitems(true);
    },
    onPointerLeave: function onPointerLeave() {
      if (hide) hide();
    },
    "data-testid": "context-menu-item"
  }, props.children, props.data.subitems && visibleSubitems && React.createElement(Subitems, null, props.data.subitems.map(function(item) {
    return React.createElement(ItemElement, {
      key: item.key,
      data: item,
      delay: props.delay,
      hide: props.hide,
      components: props.components
    }, item.label);
  })));
}
var _templateObject$5;
var SearchInput = dt.input(_templateObject$5 || (_templateObject$5 = _taggedTemplateLiteral(["\n  color: white;\n  padding: 1px 8px;\n  border: 1px solid white;\n  border-radius: 10px;\n  font-size: 16px;\n  font-family: serif;\n  width: 100%;\n  box-sizing: border-box;\n  background: transparent;\n"])));
function Search(props) {
  var Component = props.component || SearchInput;
  return React.createElement(Component, {
    value: props.value,
    onInput: function onInput(e) {
      props.onChange(e.target.value);
    },
    onPointerDown: function onPointerDown(e) {
      e.stopPropagation();
    },
    "data-testid": "context-menu-search-input"
  });
}
var _templateObject$4;
var Styles$3 = dt.div(_templateObject$4 || (_templateObject$4 = _taggedTemplateLiteral(["\n  padding: 10px;\n  width: ", "px;\n  margin-top: -20px;\n  margin-left: -", "px;\n"])), $width, $width / 2);
function Menu(props) {
  var _props$components, _props$components$mai, _props$components2, _props$components2$co, _props$components3, _props$components3$se;
  var _useDebounce = useDebounce(props.onHide, props.delay), _useDebounce2 = _slicedToArray(_useDebounce, 2), hide = _useDebounce2[0], cancelHide = _useDebounce2[1];
  var _React$useState = React.useState(""), _React$useState2 = _slicedToArray(_React$useState, 2), filter = _React$useState2[0], setFilter = _React$useState2[1];
  var filterRegexp = new RegExp(filter, "i");
  var filteredList = props.items.filter(function(item) {
    return item.label.match(filterRegexp);
  });
  var Component = ((_props$components = props.components) === null || _props$components === void 0 ? void 0 : (_props$components$mai = _props$components.main) === null || _props$components$mai === void 0 ? void 0 : _props$components$mai.call(_props$components)) || Styles$3;
  var Common = ((_props$components2 = props.components) === null || _props$components2 === void 0 ? void 0 : (_props$components2$co = _props$components2.common) === null || _props$components2$co === void 0 ? void 0 : _props$components2$co.call(_props$components2)) || CommonStyle;
  return React.createElement(Component, {
    onMouseOver: function onMouseOver() {
      cancelHide();
    },
    onMouseLeave: function onMouseLeave() {
      return hide && hide();
    },
    onWheel: function onWheel(e) {
      e.stopPropagation();
    },
    "data-testid": "context-menu"
  }, props.searchBar && React.createElement(Common, null, React.createElement(Search, {
    value: filter,
    onChange: setFilter,
    component: (_props$components3 = props.components) === null || _props$components3 === void 0 ? void 0 : (_props$components3$se = _props$components3.search) === null || _props$components3$se === void 0 ? void 0 : _props$components3$se.call(_props$components3)
  })), filteredList.map(function(item) {
    return React.createElement(ItemElement, {
      key: item.key,
      data: item,
      delay: props.delay,
      hide: props.onHide,
      components: props.components
    }, item.label);
  }));
}
function setup$2(props) {
  var delay = typeof (props === null || props === void 0 ? void 0 : props.delay) === "undefined" ? 1e3 : props.delay;
  return {
    render: function render2(context) {
      if (context.data.type === "contextmenu") {
        return React.createElement(Menu, {
          items: context.data.items,
          delay,
          searchBar: context.data.searchBar,
          onHide: context.data.onHide,
          components: props === null || props === void 0 ? void 0 : props.customize
        });
      }
    }
  };
}
var index$3 = Object.freeze({
  __proto__: null,
  setup: setup$2,
  Item: ItemStyle,
  Subitems: SubitemStyles,
  Menu: Styles$3,
  Search: SearchInput,
  Common: CommonStyle
});
var commonjsGlobal = typeof globalThis !== "undefined" ? globalThis : typeof window !== "undefined" ? window : typeof global !== "undefined" ? global : typeof self !== "undefined" ? self : {};
var freeGlobal = typeof commonjsGlobal == "object" && commonjsGlobal && commonjsGlobal.Object === Object && commonjsGlobal;
var freeSelf = typeof self == "object" && self && self.Object === Object && self;
freeGlobal || freeSelf || Function("return this")();
function useIsMounted() {
  const isMounted = (0, import_react.useRef)(false);
  (0, import_react.useEffect)(() => {
    isMounted.current = true;
    return () => {
      isMounted.current = false;
    };
  }, []);
  return (0, import_react.useCallback)(() => isMounted.current, []);
}
var initialSize = {
  width: void 0,
  height: void 0
};
function useResizeObserver(options) {
  const { ref, box = "content-box" } = options;
  const [{ width, height }, setSize] = (0, import_react.useState)(initialSize);
  const isMounted = useIsMounted();
  const previousSize = (0, import_react.useRef)({ ...initialSize });
  const onResize = (0, import_react.useRef)(void 0);
  onResize.current = options.onResize;
  (0, import_react.useEffect)(() => {
    if (!ref.current)
      return;
    if (typeof window === "undefined" || !("ResizeObserver" in window))
      return;
    const observer = new ResizeObserver(([entry]) => {
      const boxProp = box === "border-box" ? "borderBoxSize" : box === "device-pixel-content-box" ? "devicePixelContentBoxSize" : "contentBoxSize";
      const newWidth = extractSize(entry, boxProp, "inlineSize");
      const newHeight = extractSize(entry, boxProp, "blockSize");
      const hasChanged = previousSize.current.width !== newWidth || previousSize.current.height !== newHeight;
      if (hasChanged) {
        const newSize = { width: newWidth, height: newHeight };
        previousSize.current.width = newWidth;
        previousSize.current.height = newHeight;
        if (onResize.current) {
          onResize.current(newSize);
        } else {
          if (isMounted()) {
            setSize(newSize);
          }
        }
      }
    });
    observer.observe(ref.current, { box });
    return () => {
      observer.disconnect();
    };
  }, [box, ref, isMounted]);
  return { width, height };
}
function extractSize(entry, box, sizeType) {
  if (!entry[box]) {
    if (box === "contentBoxSize") {
      return entry.contentRect[sizeType === "inlineSize" ? "width" : "height"];
    }
    return void 0;
  }
  return Array.isArray(entry[box]) ? entry[box][0][sizeType] : (
    // @ts-ignore Support Firefox's non-standard behavior
    entry[box][sizeType]
  );
}
function px(value) {
  return "".concat(value, "px");
}
var _templateObject$3;
var Styles$2 = dt.div(_templateObject$3 || (_templateObject$3 = _taggedTemplateLiteral(["\n    position: absolute;\n    background: rgba(110, 136, 255, 0.8);\n    border: 1px solid rgb(192 206 212 / 60%);\n"])));
function MiniNode(props) {
  return React.createElement(Styles$2, {
    style: {
      left: px(props.left),
      top: px(props.top),
      width: px(props.width),
      height: px(props.height)
    },
    "data-testid": "minimap-node"
  });
}
var _templateObject$2;
var MiniViewportStyles = dt.div(_templateObject$2 || (_templateObject$2 = _taggedTemplateLiteral(["\n  position: absolute;\n  background: rgba(255, 251, 128, 0.32);\n  border: 1px solid #ffe52b;\n"])));
function MiniViewport(props) {
  var scale = function scale2(v) {
    return v * props.containerWidth;
  };
  var invert = function invert2(v) {
    return v / props.containerWidth;
  };
  var drag2 = useDrag(function(dx, dy) {
    props.translate(invert(-dx), invert(-dy));
  }, function(e) {
    return {
      x: e.pageX,
      y: e.pageY
    };
  });
  return React.createElement(MiniViewportStyles, {
    onPointerDown: drag2.start,
    style: {
      left: px(scale(props.left)),
      top: px(scale(props.top)),
      width: px(scale(props.width)),
      height: px(scale(props.height))
    },
    "data-testid": "minimap-viewport"
  });
}
var _templateObject$1;
var Styles$1 = dt.div(_templateObject$1 || (_templateObject$1 = _taggedTemplateLiteral(["\n    position: absolute;\n    right: 24px;\n    bottom: 24px;\n    background: rgba(229, 234, 239, 0.65);\n    padding: 20px;\n    overflow: hidden;\n    border: 1px solid #b1b7ff;\n    border-radius: 8px;\n    box-sizing: border-box;\n"])));
function Minimap(props) {
  var _ref$current;
  var ref = (0, import_react.useRef)(null);
  var _useResizeObserver = useResizeObserver({
    ref
  }), _useResizeObserver$wi = _useResizeObserver.width, width = _useResizeObserver$wi === void 0 ? 0 : _useResizeObserver$wi;
  var containerWidth = ((_ref$current = ref.current) === null || _ref$current === void 0 ? void 0 : _ref$current.clientWidth) || width;
  var scale = (0, import_react.useCallback)(function(v) {
    return v * containerWidth;
  }, [containerWidth]);
  return React.createElement(Styles$1, {
    size: props.size,
    style: {
      width: px(props.size * props.ratio),
      height: px(props.size)
    },
    onPointerDown: function onPointerDown(e) {
      e.stopPropagation();
      e.preventDefault();
    },
    onDoubleClick: function onDoubleClick(e) {
      e.stopPropagation();
      e.preventDefault();
      if (!ref.current) return;
      var box = ref.current.getBoundingClientRect();
      var x = (e.clientX - box.left) / (props.size * props.ratio);
      var y = (e.clientY - box.top) / (props.size * props.ratio);
      props.point(x, y);
    },
    ref,
    "data-testid": "minimap"
  }, containerWidth ? props.nodes.map(function(node, i) {
    return React.createElement(MiniNode, {
      key: i,
      left: scale(node.left),
      top: scale(node.top),
      width: scale(node.width),
      height: scale(node.height)
    });
  }) : null, React.createElement(MiniViewport, _extends({}, props.viewport, {
    start: props.start,
    containerWidth,
    translate: props.translate
  })));
}
function setup$1(props) {
  return {
    render: function render2(context) {
      if (context.data.type === "minimap") {
        return React.createElement(Minimap, {
          nodes: context.data.nodes,
          size: (props === null || props === void 0 ? void 0 : props.size) || 200,
          ratio: context.data.ratio,
          viewport: context.data.viewport,
          start: context.data.start,
          translate: context.data.translate,
          point: context.data.point
        });
      }
    }
  };
}
var index$2 = Object.freeze({
  __proto__: null,
  setup: setup$1
});
var _templateObject;
var pinSize = 20;
var Styles = dt.div(_templateObject || (_templateObject = _taggedTemplateLiteral(["\n  width: ", "px;\n  height: ", "px;\n  box-sizing: border-box;\n  background: ", ";\n  border: 2px solid white;\n  border-radius: ", "px;\n"])), pinSize, pinSize, function(props) {
  return props.selected ? "#ffd92c" : "steelblue";
}, pinSize);
function Pin(props) {
  var drag2 = useDrag(function(dx, dy) {
    props.translate(dx, dy);
  }, props.pointer);
  var _props$position = props.position, x = _props$position.x, y = _props$position.y;
  return React.createElement(Styles, {
    onPointerDown: function onPointerDown(e) {
      e.stopPropagation();
      e.preventDefault();
      drag2.start(e);
      props.pointerdown();
    },
    onContextMenu: function onContextMenu(e) {
      e.stopPropagation();
      e.preventDefault();
      props.contextMenu();
    },
    selected: props.selected,
    style: {
      position: "absolute",
      top: "".concat(y - pinSize / 2, "px"),
      left: "".concat(x - pinSize / 2, "px")
    },
    "data-testid": "pin"
  });
}
function setup(props) {
  function renderPins(data, pointer) {
    return React.createElement(React.Fragment, null, data.pins.map(function(pin) {
      return React.createElement(Pin, _extends({}, pin, {
        key: pin.id,
        contextMenu: function contextMenu() {
          return (props === null || props === void 0 ? void 0 : props.contextMenu) && props.contextMenu(pin.id);
        },
        translate: function translate(dx, dy) {
          return (props === null || props === void 0 ? void 0 : props.translate) && props.translate(pin.id, dx, dy);
        },
        pointerdown: function pointerdown() {
          return (props === null || props === void 0 ? void 0 : props.pointerdown) && props.pointerdown(pin.id);
        },
        pointer
      }));
    }));
  }
  return {
    render: function render2(context, plugin) {
      var data = context.data;
      var area = plugin.parentScope(BaseAreaPlugin);
      if (data.type === "reroute-pins") {
        return renderPins(data.data, function() {
          return area.area.pointer;
        });
      }
    }
  };
}
var index$1 = Object.freeze({
  __proto__: null,
  setup
});
var index = Object.freeze({
  __proto__: null,
  classic: index$4,
  contextMenu: index$3,
  minimap: index$2,
  reroute: index$1
});
function _createForOfIteratorHelper(r, e) {
  var t = "undefined" != typeof Symbol && r[Symbol.iterator] || r["@@iterator"];
  if (!t) {
    if (Array.isArray(r) || (t = _unsupportedIterableToArray(r)) || e && r && "number" == typeof r.length) {
      t && (r = t);
      var _n = 0, F = function F2() {
      };
      return { s: F, n: function n() {
        return _n >= r.length ? { done: true } : { done: false, value: r[_n++] };
      }, e: function e2(r2) {
        throw r2;
      }, f: F };
    }
    throw new TypeError("Invalid attempt to iterate non-iterable instance.\nIn order to be iterable, non-array objects must have a [Symbol.iterator]() method.");
  }
  var o, a = true, u = false;
  return { s: function s() {
    t = t.call(r);
  }, n: function n() {
    var r2 = t.next();
    return a = r2.done, r2;
  }, e: function e2(r2) {
    u = true, o = r2;
  }, f: function f() {
    try {
      a || null == t["return"] || t["return"]();
    } finally {
      if (u) throw o;
    }
  } };
}
function _unsupportedIterableToArray(r, a) {
  if (r) {
    if ("string" == typeof r) return _arrayLikeToArray(r, a);
    var t = {}.toString.call(r).slice(8, -1);
    return "Object" === t && r.constructor && (t = r.constructor.name), "Map" === t || "Set" === t ? Array.from(r) : "Arguments" === t || /^(?:Ui|I)nt(?:8|16|32)(?:Clamped)?Array$/.test(t) ? _arrayLikeToArray(r, a) : void 0;
  }
}
function _arrayLikeToArray(r, a) {
  (null == a || a > r.length) && (a = r.length);
  for (var e = 0, n = Array(a); e < a; e++) n[e] = r[e];
  return n;
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
var ReactPlugin = function(_Scope) {
  function ReactPlugin2(props) {
    var _this;
    _classCallCheck(this, ReactPlugin2);
    _this = _callSuper(this, ReactPlugin2, ["react-render"]);
    _defineProperty(_this, "presets", []);
    _this.renderer = getRenderer({
      createRoot: props === null || props === void 0 ? void 0 : props.createRoot
    });
    _this.addPipe(function(context) {
      if (!context || _typeof(context) !== "object" || !("type" in context)) return context;
      if (context.type === "unmount") {
        _this.unmount(context.data.element);
      } else if (context.type === "render") {
        if ("filled" in context.data && context.data.filled) {
          return context;
        }
        if (_this.mount(context.data.element, context)) {
          return _objectSpread(_objectSpread({}, context), {}, {
            data: _objectSpread(_objectSpread({}, context.data), {}, {
              filled: true
            })
          });
        }
      }
      return context;
    });
    return _this;
  }
  _inherits(ReactPlugin2, _Scope);
  return _createClass(ReactPlugin2, [{
    key: "setParent",
    value: function setParent(scope) {
      var _this2 = this;
      _superPropGet(ReactPlugin2, "setParent", this, 3)([scope]);
      this.presets.forEach(function(preset) {
        if (preset.attach) preset.attach(_this2);
      });
    }
  }, {
    key: "mount",
    value: function mount(element, context) {
      var parent = this.parentScope();
      var _iterator = _createForOfIteratorHelper(this.presets), _step;
      try {
        for (_iterator.s(); !(_step = _iterator.n()).done; ) {
          var preset = _step.value;
          var result = preset.render(context, this);
          if (!result) continue;
          var reactElement = React.createElement(Root, {
            rendered: function rendered() {
              return void parent.emit({
                type: "rendered",
                data: context.data
              });
            }
          }, result);
          this.renderer.mount(reactElement, element);
          return true;
        }
      } catch (err) {
        _iterator.e(err);
      } finally {
        _iterator.f();
      }
    }
  }, {
    key: "unmount",
    value: function unmount(element) {
      this.renderer.unmount(element);
    }
    /**
     * Adds a preset to the plugin.
     * @param preset Preset that can render nodes, connections and other elements.
     */
  }, {
    key: "addPreset",
    value: function addPreset(preset) {
      var local = preset;
      if (local.attach) local.attach(this);
      this.presets.push(local);
    }
  }]);
}(Scope);
export {
  drag as Drag,
  index as Presets,
  ReactPlugin,
  RefComponent,
  useRete
};
/*! Bundled license information:

rete-react-plugin/rete-react-plugin.esm.js:
  (*!
  * rete-react-plugin v2.0.6
  * (c) 2024 Vitaliy Stoliarov
  * Released under the MIT license.
  * *)
*/
//# sourceMappingURL=rete-react-plugin.js.map
