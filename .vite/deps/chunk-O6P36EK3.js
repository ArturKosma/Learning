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
  __commonJS,
  __toESM
} from "./chunk-PLDDJCW6.js";

// node_modules/@babel/runtime/helpers/typeof.js
var require_typeof = __commonJS({
  "node_modules/@babel/runtime/helpers/typeof.js"(exports, module) {
    function _typeof(o) {
      "@babel/helpers - typeof";
      return module.exports = _typeof = "function" == typeof Symbol && "symbol" == typeof Symbol.iterator ? function(o2) {
        return typeof o2;
      } : function(o2) {
        return o2 && "function" == typeof Symbol && o2.constructor === Symbol && o2 !== Symbol.prototype ? "symbol" : typeof o2;
      }, module.exports.__esModule = true, module.exports["default"] = module.exports, _typeof(o);
    }
    module.exports = _typeof, module.exports.__esModule = true, module.exports["default"] = module.exports;
  }
});

// node_modules/@babel/runtime/helpers/regeneratorRuntime.js
var require_regeneratorRuntime = __commonJS({
  "node_modules/@babel/runtime/helpers/regeneratorRuntime.js"(exports, module) {
    var _typeof = require_typeof()["default"];
    function _regeneratorRuntime2() {
      "use strict";
      module.exports = _regeneratorRuntime2 = function _regeneratorRuntime3() {
        return r;
      }, module.exports.__esModule = true, module.exports["default"] = module.exports;
      var t, r = {}, e = Object.prototype, n = e.hasOwnProperty, o = "function" == typeof Symbol ? Symbol : {}, i = o.iterator || "@@iterator", a = o.asyncIterator || "@@asyncIterator", u = o.toStringTag || "@@toStringTag";
      function c(t2, r2, e2, n2) {
        return Object.defineProperty(t2, r2, {
          value: e2,
          enumerable: !n2,
          configurable: !n2,
          writable: !n2
        });
      }
      try {
        c({}, "");
      } catch (t2) {
        c = function c2(t3, r2, e2) {
          return t3[r2] = e2;
        };
      }
      function h(r2, e2, n2, o2) {
        var i2 = e2 && e2.prototype instanceof Generator ? e2 : Generator, a2 = Object.create(i2.prototype);
        return c(a2, "_invoke", /* @__PURE__ */ function(r3, e3, n3) {
          var o3 = 1;
          return function(i3, a3) {
            if (3 === o3) throw Error("Generator is already running");
            if (4 === o3) {
              if ("throw" === i3) throw a3;
              return {
                value: t,
                done: true
              };
            }
            for (n3.method = i3, n3.arg = a3; ; ) {
              var u2 = n3.delegate;
              if (u2) {
                var c2 = d(u2, n3);
                if (c2) {
                  if (c2 === f) continue;
                  return c2;
                }
              }
              if ("next" === n3.method) n3.sent = n3._sent = n3.arg;
              else if ("throw" === n3.method) {
                if (1 === o3) throw o3 = 4, n3.arg;
                n3.dispatchException(n3.arg);
              } else "return" === n3.method && n3.abrupt("return", n3.arg);
              o3 = 3;
              var h2 = s(r3, e3, n3);
              if ("normal" === h2.type) {
                if (o3 = n3.done ? 4 : 2, h2.arg === f) continue;
                return {
                  value: h2.arg,
                  done: n3.done
                };
              }
              "throw" === h2.type && (o3 = 4, n3.method = "throw", n3.arg = h2.arg);
            }
          };
        }(r2, n2, new Context(o2 || [])), true), a2;
      }
      function s(t2, r2, e2) {
        try {
          return {
            type: "normal",
            arg: t2.call(r2, e2)
          };
        } catch (t3) {
          return {
            type: "throw",
            arg: t3
          };
        }
      }
      r.wrap = h;
      var f = {};
      function Generator() {
      }
      function GeneratorFunction() {
      }
      function GeneratorFunctionPrototype() {
      }
      var l = {};
      c(l, i, function() {
        return this;
      });
      var p = Object.getPrototypeOf, y = p && p(p(x([])));
      y && y !== e && n.call(y, i) && (l = y);
      var v = GeneratorFunctionPrototype.prototype = Generator.prototype = Object.create(l);
      function g(t2) {
        ["next", "throw", "return"].forEach(function(r2) {
          c(t2, r2, function(t3) {
            return this._invoke(r2, t3);
          });
        });
      }
      function AsyncIterator(t2, r2) {
        function e2(o3, i2, a2, u2) {
          var c2 = s(t2[o3], t2, i2);
          if ("throw" !== c2.type) {
            var h2 = c2.arg, f2 = h2.value;
            return f2 && "object" == _typeof(f2) && n.call(f2, "__await") ? r2.resolve(f2.__await).then(function(t3) {
              e2("next", t3, a2, u2);
            }, function(t3) {
              e2("throw", t3, a2, u2);
            }) : r2.resolve(f2).then(function(t3) {
              h2.value = t3, a2(h2);
            }, function(t3) {
              return e2("throw", t3, a2, u2);
            });
          }
          u2(c2.arg);
        }
        var o2;
        c(this, "_invoke", function(t3, n2) {
          function i2() {
            return new r2(function(r3, o3) {
              e2(t3, n2, r3, o3);
            });
          }
          return o2 = o2 ? o2.then(i2, i2) : i2();
        }, true);
      }
      function d(r2, e2) {
        var n2 = e2.method, o2 = r2.i[n2];
        if (o2 === t) return e2.delegate = null, "throw" === n2 && r2.i["return"] && (e2.method = "return", e2.arg = t, d(r2, e2), "throw" === e2.method) || "return" !== n2 && (e2.method = "throw", e2.arg = new TypeError("The iterator does not provide a '" + n2 + "' method")), f;
        var i2 = s(o2, r2.i, e2.arg);
        if ("throw" === i2.type) return e2.method = "throw", e2.arg = i2.arg, e2.delegate = null, f;
        var a2 = i2.arg;
        return a2 ? a2.done ? (e2[r2.r] = a2.value, e2.next = r2.n, "return" !== e2.method && (e2.method = "next", e2.arg = t), e2.delegate = null, f) : a2 : (e2.method = "throw", e2.arg = new TypeError("iterator result is not an object"), e2.delegate = null, f);
      }
      function w(t2) {
        this.tryEntries.push(t2);
      }
      function m(r2) {
        var e2 = r2[4] || {};
        e2.type = "normal", e2.arg = t, r2[4] = e2;
      }
      function Context(t2) {
        this.tryEntries = [[-1]], t2.forEach(w, this), this.reset(true);
      }
      function x(r2) {
        if (null != r2) {
          var e2 = r2[i];
          if (e2) return e2.call(r2);
          if ("function" == typeof r2.next) return r2;
          if (!isNaN(r2.length)) {
            var o2 = -1, a2 = function e3() {
              for (; ++o2 < r2.length; ) if (n.call(r2, o2)) return e3.value = r2[o2], e3.done = false, e3;
              return e3.value = t, e3.done = true, e3;
            };
            return a2.next = a2;
          }
        }
        throw new TypeError(_typeof(r2) + " is not iterable");
      }
      return GeneratorFunction.prototype = GeneratorFunctionPrototype, c(v, "constructor", GeneratorFunctionPrototype), c(GeneratorFunctionPrototype, "constructor", GeneratorFunction), GeneratorFunction.displayName = c(GeneratorFunctionPrototype, u, "GeneratorFunction"), r.isGeneratorFunction = function(t2) {
        var r2 = "function" == typeof t2 && t2.constructor;
        return !!r2 && (r2 === GeneratorFunction || "GeneratorFunction" === (r2.displayName || r2.name));
      }, r.mark = function(t2) {
        return Object.setPrototypeOf ? Object.setPrototypeOf(t2, GeneratorFunctionPrototype) : (t2.__proto__ = GeneratorFunctionPrototype, c(t2, u, "GeneratorFunction")), t2.prototype = Object.create(v), t2;
      }, r.awrap = function(t2) {
        return {
          __await: t2
        };
      }, g(AsyncIterator.prototype), c(AsyncIterator.prototype, a, function() {
        return this;
      }), r.AsyncIterator = AsyncIterator, r.async = function(t2, e2, n2, o2, i2) {
        void 0 === i2 && (i2 = Promise);
        var a2 = new AsyncIterator(h(t2, e2, n2, o2), i2);
        return r.isGeneratorFunction(e2) ? a2 : a2.next().then(function(t3) {
          return t3.done ? t3.value : a2.next();
        });
      }, g(v), c(v, u, "Generator"), c(v, i, function() {
        return this;
      }), c(v, "toString", function() {
        return "[object Generator]";
      }), r.keys = function(t2) {
        var r2 = Object(t2), e2 = [];
        for (var n2 in r2) e2.unshift(n2);
        return function t3() {
          for (; e2.length; ) if ((n2 = e2.pop()) in r2) return t3.value = n2, t3.done = false, t3;
          return t3.done = true, t3;
        };
      }, r.values = x, Context.prototype = {
        constructor: Context,
        reset: function reset(r2) {
          if (this.prev = this.next = 0, this.sent = this._sent = t, this.done = false, this.delegate = null, this.method = "next", this.arg = t, this.tryEntries.forEach(m), !r2) for (var e2 in this) "t" === e2.charAt(0) && n.call(this, e2) && !isNaN(+e2.slice(1)) && (this[e2] = t);
        },
        stop: function stop() {
          this.done = true;
          var t2 = this.tryEntries[0][4];
          if ("throw" === t2.type) throw t2.arg;
          return this.rval;
        },
        dispatchException: function dispatchException(r2) {
          if (this.done) throw r2;
          var e2 = this;
          function n2(t2) {
            a2.type = "throw", a2.arg = r2, e2.next = t2;
          }
          for (var o2 = e2.tryEntries.length - 1; o2 >= 0; --o2) {
            var i2 = this.tryEntries[o2], a2 = i2[4], u2 = this.prev, c2 = i2[1], h2 = i2[2];
            if (-1 === i2[0]) return n2("end"), false;
            if (!c2 && !h2) throw Error("try statement without catch or finally");
            if (null != i2[0] && i2[0] <= u2) {
              if (u2 < c2) return this.method = "next", this.arg = t, n2(c2), true;
              if (u2 < h2) return n2(h2), false;
            }
          }
        },
        abrupt: function abrupt(t2, r2) {
          for (var e2 = this.tryEntries.length - 1; e2 >= 0; --e2) {
            var n2 = this.tryEntries[e2];
            if (n2[0] > -1 && n2[0] <= this.prev && this.prev < n2[2]) {
              var o2 = n2;
              break;
            }
          }
          o2 && ("break" === t2 || "continue" === t2) && o2[0] <= r2 && r2 <= o2[2] && (o2 = null);
          var i2 = o2 ? o2[4] : {};
          return i2.type = t2, i2.arg = r2, o2 ? (this.method = "next", this.next = o2[2], f) : this.complete(i2);
        },
        complete: function complete(t2, r2) {
          if ("throw" === t2.type) throw t2.arg;
          return "break" === t2.type || "continue" === t2.type ? this.next = t2.arg : "return" === t2.type ? (this.rval = this.arg = t2.arg, this.method = "return", this.next = "end") : "normal" === t2.type && r2 && (this.next = r2), f;
        },
        finish: function finish(t2) {
          for (var r2 = this.tryEntries.length - 1; r2 >= 0; --r2) {
            var e2 = this.tryEntries[r2];
            if (e2[2] === t2) return this.complete(e2[4], e2[3]), m(e2), f;
          }
        },
        "catch": function _catch(t2) {
          for (var r2 = this.tryEntries.length - 1; r2 >= 0; --r2) {
            var e2 = this.tryEntries[r2];
            if (e2[0] === t2) {
              var n2 = e2[4];
              if ("throw" === n2.type) {
                var o2 = n2.arg;
                m(e2);
              }
              return o2;
            }
          }
          throw Error("illegal catch attempt");
        },
        delegateYield: function delegateYield(r2, e2, n2) {
          return this.delegate = {
            i: x(r2),
            r: e2,
            n: n2
          }, "next" === this.method && (this.arg = t), f;
        }
      }, r;
    }
    module.exports = _regeneratorRuntime2, module.exports.__esModule = true, module.exports["default"] = module.exports;
  }
});

// node_modules/@babel/runtime/regenerator/index.js
var require_regenerator = __commonJS({
  "node_modules/@babel/runtime/regenerator/index.js"(exports, module) {
    var runtime = require_regeneratorRuntime()();
    module.exports = runtime;
    try {
      regeneratorRuntime = runtime;
    } catch (accidentalStrictMode) {
      if (typeof globalThis === "object") {
        globalThis.regeneratorRuntime = runtime;
      } else {
        Function("r", "regeneratorRuntime = r")(runtime);
      }
    }
  }
});

// node_modules/rete/rete.esm.js
var import_regenerator = __toESM(require_regenerator());
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
function useHelper() {
  return {
    debug: function debug(_f) {
    }
  };
}
var Signal = function() {
  function Signal2() {
    _classCallCheck(this, Signal2);
    _defineProperty(this, "pipes", []);
  }
  return _createClass(Signal2, [{
    key: "addPipe",
    value: function addPipe(pipe) {
      this.pipes.push(pipe);
    }
  }, {
    key: "emit",
    value: function() {
      var _emit = _asyncToGenerator(import_regenerator.default.mark(function _callee(context) {
        var current, _iterator, _step, pipe;
        return import_regenerator.default.wrap(function _callee$(_context) {
          while (1) switch (_context.prev = _context.next) {
            case 0:
              current = context;
              _iterator = _createForOfIteratorHelper$1(this.pipes);
              _context.prev = 2;
              _iterator.s();
            case 4:
              if ((_step = _iterator.n()).done) {
                _context.next = 13;
                break;
              }
              pipe = _step.value;
              _context.next = 8;
              return pipe(current);
            case 8:
              current = _context.sent;
              if (!(typeof current === "undefined")) {
                _context.next = 11;
                break;
              }
              return _context.abrupt("return");
            case 11:
              _context.next = 4;
              break;
            case 13:
              _context.next = 18;
              break;
            case 15:
              _context.prev = 15;
              _context.t0 = _context["catch"](2);
              _iterator.e(_context.t0);
            case 18:
              _context.prev = 18;
              _iterator.f();
              return _context.finish(18);
            case 21:
              return _context.abrupt("return", current);
            case 22:
            case "end":
              return _context.stop();
          }
        }, _callee, this, [[2, 15, 18, 21]]);
      }));
      function emit(_x) {
        return _emit.apply(this, arguments);
      }
      return emit;
    }()
  }]);
}();
var Scope = function() {
  function Scope2(name) {
    _classCallCheck(this, Scope2);
    _defineProperty(this, "signal", new Signal());
    this.name = name;
  }
  return _createClass(Scope2, [{
    key: "addPipe",
    value: function addPipe(middleware) {
      this.signal.addPipe(middleware);
    }
  }, {
    key: "use",
    value: function use(scope) {
      if (!(scope instanceof Scope2)) throw new Error("cannot use non-Scope instance");
      scope.setParent(this);
      this.addPipe(function(context) {
        return scope.signal.emit(context);
      });
      return useHelper();
    }
  }, {
    key: "setParent",
    value: function setParent(scope) {
      this.parent = scope;
    }
  }, {
    key: "emit",
    value: function emit(context) {
      return this.signal.emit(context);
    }
  }, {
    key: "hasParent",
    value: function hasParent() {
      return Boolean(this.parent);
    }
  }, {
    key: "parentScope",
    value: function parentScope(type) {
      if (!this.parent) throw new Error("cannot find parent");
      if (type && this.parent instanceof type) return this.parent;
      if (type) throw new Error("actual parent is not instance of type");
      return this.parent;
    }
  }]);
}();
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
function _callSuper$1(t, o, e) {
  return o = _getPrototypeOf(o), _possibleConstructorReturn(t, _isNativeReflectConstruct$1() ? Reflect.construct(o, e || [], _getPrototypeOf(t).constructor) : o.apply(t, e));
}
function _isNativeReflectConstruct$1() {
  try {
    var t = !Boolean.prototype.valueOf.call(Reflect.construct(Boolean, [], function() {
    }));
  } catch (t2) {
  }
  return (_isNativeReflectConstruct$1 = function _isNativeReflectConstruct2() {
    return !!t;
  })();
}
var NodeEditor = function(_Scope) {
  function NodeEditor2() {
    var _this;
    _classCallCheck(this, NodeEditor2);
    _this = _callSuper$1(this, NodeEditor2, ["NodeEditor"]);
    _defineProperty(_this, "nodes", []);
    _defineProperty(_this, "connections", []);
    return _this;
  }
  _inherits(NodeEditor2, _Scope);
  return _createClass(NodeEditor2, [{
    key: "getNode",
    value: function getNode(id) {
      return this.nodes.find(function(node) {
        return node.id === id;
      });
    }
    /**
     * Get all nodes
     * @returns Copy of array with nodes
     */
  }, {
    key: "getNodes",
    value: function getNodes() {
      return this.nodes.slice();
    }
    /**
     * Get all connections
     * @returns Copy of array with onnections
     */
  }, {
    key: "getConnections",
    value: function getConnections() {
      return this.connections.slice();
    }
    /**
     * Get a connection by id
     * @param id - The connection id
     * @returns The connection or undefined
     */
  }, {
    key: "getConnection",
    value: function getConnection(id) {
      return this.connections.find(function(connection) {
        return connection.id === id;
      });
    }
    /**
     * Add a node
     * @param data - The node data
     * @returns Whether the node was added
     * @throws If the node has already been added
     * @emits nodecreate
     * @emits nodecreated
     */
  }, {
    key: "addNode",
    value: function() {
      var _addNode = _asyncToGenerator(import_regenerator.default.mark(function _callee(data) {
        return import_regenerator.default.wrap(function _callee$(_context) {
          while (1) switch (_context.prev = _context.next) {
            case 0:
              if (!this.getNode(data.id)) {
                _context.next = 2;
                break;
              }
              throw new Error("node has already been added");
            case 2:
              _context.next = 4;
              return this.emit({
                type: "nodecreate",
                data
              });
            case 4:
              if (_context.sent) {
                _context.next = 6;
                break;
              }
              return _context.abrupt("return", false);
            case 6:
              this.nodes.push(data);
              _context.next = 9;
              return this.emit({
                type: "nodecreated",
                data
              });
            case 9:
              return _context.abrupt("return", true);
            case 10:
            case "end":
              return _context.stop();
          }
        }, _callee, this);
      }));
      function addNode(_x) {
        return _addNode.apply(this, arguments);
      }
      return addNode;
    }()
  }, {
    key: "addConnection",
    value: function() {
      var _addConnection = _asyncToGenerator(import_regenerator.default.mark(function _callee2(data) {
        return import_regenerator.default.wrap(function _callee2$(_context2) {
          while (1) switch (_context2.prev = _context2.next) {
            case 0:
              if (!this.getConnection(data.id)) {
                _context2.next = 2;
                break;
              }
              throw new Error("connection has already been added");
            case 2:
              _context2.next = 4;
              return this.emit({
                type: "connectioncreate",
                data
              });
            case 4:
              if (_context2.sent) {
                _context2.next = 6;
                break;
              }
              return _context2.abrupt("return", false);
            case 6:
              this.connections.push(data);
              _context2.next = 9;
              return this.emit({
                type: "connectioncreated",
                data
              });
            case 9:
              return _context2.abrupt("return", true);
            case 10:
            case "end":
              return _context2.stop();
          }
        }, _callee2, this);
      }));
      function addConnection(_x2) {
        return _addConnection.apply(this, arguments);
      }
      return addConnection;
    }()
  }, {
    key: "removeNode",
    value: function() {
      var _removeNode = _asyncToGenerator(import_regenerator.default.mark(function _callee3(id) {
        var index, node;
        return import_regenerator.default.wrap(function _callee3$(_context3) {
          while (1) switch (_context3.prev = _context3.next) {
            case 0:
              index = this.nodes.findIndex(function(n) {
                return n.id === id;
              });
              node = this.nodes[index];
              if (!(index < 0)) {
                _context3.next = 4;
                break;
              }
              throw new Error("cannot find node");
            case 4:
              _context3.next = 6;
              return this.emit({
                type: "noderemove",
                data: node
              });
            case 6:
              if (_context3.sent) {
                _context3.next = 8;
                break;
              }
              return _context3.abrupt("return", false);
            case 8:
              this.nodes.splice(index, 1);
              _context3.next = 11;
              return this.emit({
                type: "noderemoved",
                data: node
              });
            case 11:
              return _context3.abrupt("return", true);
            case 12:
            case "end":
              return _context3.stop();
          }
        }, _callee3, this);
      }));
      function removeNode(_x3) {
        return _removeNode.apply(this, arguments);
      }
      return removeNode;
    }()
  }, {
    key: "removeConnection",
    value: function() {
      var _removeConnection = _asyncToGenerator(import_regenerator.default.mark(function _callee4(id) {
        var index, connection;
        return import_regenerator.default.wrap(function _callee4$(_context4) {
          while (1) switch (_context4.prev = _context4.next) {
            case 0:
              index = this.connections.findIndex(function(n) {
                return n.id === id;
              });
              connection = this.connections[index];
              if (!(index < 0)) {
                _context4.next = 4;
                break;
              }
              throw new Error("cannot find connection");
            case 4:
              _context4.next = 6;
              return this.emit({
                type: "connectionremove",
                data: connection
              });
            case 6:
              if (_context4.sent) {
                _context4.next = 8;
                break;
              }
              return _context4.abrupt("return", false);
            case 8:
              this.connections.splice(index, 1);
              _context4.next = 11;
              return this.emit({
                type: "connectionremoved",
                data: connection
              });
            case 11:
              return _context4.abrupt("return", true);
            case 12:
            case "end":
              return _context4.stop();
          }
        }, _callee4, this);
      }));
      function removeConnection(_x4) {
        return _removeConnection.apply(this, arguments);
      }
      return removeConnection;
    }()
  }, {
    key: "clear",
    value: function() {
      var _clear = _asyncToGenerator(import_regenerator.default.mark(function _callee5() {
        var _iterator, _step, connection, _iterator2, _step2, node;
        return import_regenerator.default.wrap(function _callee5$(_context5) {
          while (1) switch (_context5.prev = _context5.next) {
            case 0:
              _context5.next = 2;
              return this.emit({
                type: "clear"
              });
            case 2:
              if (_context5.sent) {
                _context5.next = 6;
                break;
              }
              _context5.next = 5;
              return this.emit({
                type: "clearcancelled"
              });
            case 5:
              return _context5.abrupt("return", false);
            case 6:
              _iterator = _createForOfIteratorHelper(this.connections.slice());
              _context5.prev = 7;
              _iterator.s();
            case 9:
              if ((_step = _iterator.n()).done) {
                _context5.next = 15;
                break;
              }
              connection = _step.value;
              _context5.next = 13;
              return this.removeConnection(connection.id);
            case 13:
              _context5.next = 9;
              break;
            case 15:
              _context5.next = 20;
              break;
            case 17:
              _context5.prev = 17;
              _context5.t0 = _context5["catch"](7);
              _iterator.e(_context5.t0);
            case 20:
              _context5.prev = 20;
              _iterator.f();
              return _context5.finish(20);
            case 23:
              _iterator2 = _createForOfIteratorHelper(this.nodes.slice());
              _context5.prev = 24;
              _iterator2.s();
            case 26:
              if ((_step2 = _iterator2.n()).done) {
                _context5.next = 32;
                break;
              }
              node = _step2.value;
              _context5.next = 30;
              return this.removeNode(node.id);
            case 30:
              _context5.next = 26;
              break;
            case 32:
              _context5.next = 37;
              break;
            case 34:
              _context5.prev = 34;
              _context5.t1 = _context5["catch"](24);
              _iterator2.e(_context5.t1);
            case 37:
              _context5.prev = 37;
              _iterator2.f();
              return _context5.finish(37);
            case 40:
              _context5.next = 42;
              return this.emit({
                type: "cleared"
              });
            case 42:
              return _context5.abrupt("return", true);
            case 43:
            case "end":
              return _context5.stop();
          }
        }, _callee5, this, [[7, 17, 20, 23], [24, 34, 37, 40]]);
      }));
      function clear() {
        return _clear.apply(this, arguments);
      }
      return clear;
    }()
  }]);
}(Scope);
var crypto = globalThis.crypto;
function getUID() {
  if ("randomBytes" in crypto) {
    return crypto.randomBytes(8).toString("hex");
  }
  var bytes = crypto.getRandomValues(new Uint8Array(8));
  var array = Array.from(bytes);
  var hexPairs = array.map(function(b) {
    return b.toString(16).padStart(2, "0");
  });
  return hexPairs.join("");
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
var Socket = _createClass(
  /**
   * @constructor
   * @param name Name of the socket
   */
  function Socket2(name) {
    _classCallCheck(this, Socket2);
    this.name = name;
  }
);
var Port = _createClass(
  /**
   * Port id, unique string generated by `getUID` function
   */
  /**
   * Port index, used for sorting ports. Default is `0`
   */
  /**
   * @constructor
   * @param socket Socket instance
   * @param label Label of the port
   * @param multipleConnections Whether the output port can have multiple connections
   */
  function Port2(socket, label, multipleConnections) {
    _classCallCheck(this, Port2);
    this.socket = socket;
    this.label = label;
    this.multipleConnections = multipleConnections;
    this.id = getUID();
  }
);
var Input = function(_Port) {
  function Input2(socket, label, multipleConnections) {
    var _this;
    _classCallCheck(this, Input2);
    _this = _callSuper(this, Input2, [socket, label, multipleConnections]);
    _defineProperty(_this, "control", null);
    _defineProperty(_this, "showControl", true);
    _this.socket = socket;
    _this.label = label;
    _this.multipleConnections = multipleConnections;
    return _this;
  }
  _inherits(Input2, _Port);
  return _createClass(Input2, [{
    key: "addControl",
    value: function addControl(control) {
      if (this.control) throw new Error("control already added for this input");
      this.control = control;
    }
    /**
     * Remove control from the input port
     */
  }, {
    key: "removeControl",
    value: function removeControl() {
      this.control = null;
    }
  }]);
}(Port);
var Output = function(_Port2) {
  function Output2(socket, label, multipleConnections) {
    _classCallCheck(this, Output2);
    return _callSuper(this, Output2, [socket, label, multipleConnections !== false]);
  }
  _inherits(Output2, _Port2);
  return _createClass(Output2);
}(Port);
var Control = _createClass(
  /**
   * Control id, unique string generated by `getUID` function
   */
  /**
   * Control index, used for sorting controls. Default is `0`
   */
  function Control2() {
    _classCallCheck(this, Control2);
    this.id = getUID();
  }
);
var InputControl = function(_Control) {
  function InputControl2(type, options) {
    var _options$readonly;
    var _this2;
    _classCallCheck(this, InputControl2);
    _this2 = _callSuper(this, InputControl2);
    _this2.type = type;
    _this2.options = options;
    _this2.id = getUID();
    _this2.readonly = (_options$readonly = options === null || options === void 0 ? void 0 : options.readonly) !== null && _options$readonly !== void 0 ? _options$readonly : false;
    if (typeof (options === null || options === void 0 ? void 0 : options.initial) !== "undefined") _this2.value = options.initial;
    return _this2;
  }
  _inherits(InputControl2, _Control);
  return _createClass(InputControl2, [{
    key: "setValue",
    value: function setValue(value) {
      var _this$options;
      this.value = value;
      if ((_this$options = this.options) !== null && _this$options !== void 0 && _this$options.change) this.options.change(value);
    }
  }]);
}(Control);
var Node = function() {
  function Node2(label) {
    _classCallCheck(this, Node2);
    _defineProperty(this, "inputs", {});
    _defineProperty(this, "outputs", {});
    _defineProperty(this, "controls", {});
    this.label = label;
    this.id = getUID();
  }
  return _createClass(Node2, [{
    key: "hasInput",
    value: function hasInput(key) {
      return Object.prototype.hasOwnProperty.call(this.inputs, key);
    }
  }, {
    key: "addInput",
    value: function addInput(key, input) {
      if (this.hasInput(key)) throw new Error("input with key '".concat(String(key), "' already added"));
      Object.defineProperty(this.inputs, key, {
        value: input,
        enumerable: true,
        configurable: true
      });
    }
  }, {
    key: "removeInput",
    value: function removeInput(key) {
      delete this.inputs[key];
    }
  }, {
    key: "hasOutput",
    value: function hasOutput(key) {
      return Object.prototype.hasOwnProperty.call(this.outputs, key);
    }
  }, {
    key: "addOutput",
    value: function addOutput(key, output) {
      if (this.hasOutput(key)) throw new Error("output with key '".concat(String(key), "' already added"));
      Object.defineProperty(this.outputs, key, {
        value: output,
        enumerable: true,
        configurable: true
      });
    }
  }, {
    key: "removeOutput",
    value: function removeOutput(key) {
      delete this.outputs[key];
    }
  }, {
    key: "hasControl",
    value: function hasControl(key) {
      return Object.prototype.hasOwnProperty.call(this.controls, key);
    }
  }, {
    key: "addControl",
    value: function addControl(key, control) {
      if (this.hasControl(key)) throw new Error("control with key '".concat(String(key), "' already added"));
      Object.defineProperty(this.controls, key, {
        value: control,
        enumerable: true,
        configurable: true
      });
    }
  }, {
    key: "removeControl",
    value: function removeControl(key) {
      delete this.controls[key];
    }
  }]);
}();
var Connection = _createClass(
  /**
   * Connection id, unique string generated by `getUID` function
   */
  /**
   * Source node id
   */
  /**
   * Target node id
   */
  /**
   * @constructor
   * @param source Source node instance
   * @param sourceOutput Source node output key
   * @param target Target node instance
   * @param targetInput Target node input key
   */
  function Connection2(source, sourceOutput, target, targetInput) {
    _classCallCheck(this, Connection2);
    this.sourceOutput = sourceOutput;
    this.targetInput = targetInput;
    if (!source.outputs[sourceOutput]) {
      throw new Error("source node doesn't have output with a key ".concat(String(sourceOutput)));
    }
    if (!target.inputs[targetInput]) {
      throw new Error("target node doesn't have input with a key ".concat(String(targetInput)));
    }
    this.id = getUID();
    this.source = source.id;
    this.target = target.id;
  }
);
var classic = Object.freeze({
  __proto__: null,
  Socket,
  Port,
  Input,
  Output,
  Control,
  InputControl,
  Node,
  Connection
});

export {
  require_regenerator,
  Signal,
  Scope,
  NodeEditor,
  getUID,
  classic
};
/*! Bundled license information:

@babel/runtime/helpers/regeneratorRuntime.js:
  (*! regenerator-runtime -- Copyright (c) 2014-present, Facebook, Inc. -- license (MIT): https://github.com/babel/babel/blob/main/packages/babel-helpers/LICENSE *)

rete/rete.esm.js:
  (*!
  * rete v2.0.4
  * (c) 2024 Vitaliy Stoliarov
  * Released under the MIT license.
  * *)
*/
//# sourceMappingURL=chunk-O6P36EK3.js.map
