import React from 'react';
import { ClassicPreset, GetSchemes, NodeEditor } from 'rete';
import { OnNodeUpdated, AFSerializeInterface } from './affunclib';

type Schemes = GetSchemes<ClassicPreset.Node, ClassicPreset.Connection<ClassicPreset.Node, ClassicPreset.Node>>;

type Vec3 = { x: number; y: number; z: number };

function fmt1dpMinVec(val: number) {
  const str = String(val);
  return str.includes('.') ? str : str + '.0';
}

function parseMaybeNumber(raw: string): number | null {
  const n = Number(raw.trim());
  return Number.isFinite(n) ? n : null;
}

export class Vector3Control extends ClassicPreset.Control implements AFSerializeInterface {
  value: Vec3;
  onChange?: (val: Vec3) => void;
  node?: ClassicPreset.Node;
  editor: NodeEditor<Schemes>;
  varName: string;

  constructor(
    editor: NodeEditor<Schemes>,
    node: ClassicPreset.Node,
    varName: string,
    initial: Partial<Vec3> | [number, number, number] | number = { x: 0, y: 0, z: 0 }
  ) {
    super();
    this.editor = editor;
    this.node = node;
    this.varName = varName;

    const initVec: Vec3 = Array.isArray(initial)
      ? { x: initial[0] ?? 0, y: initial[1] ?? 0, z: initial[2] ?? 0 }
      : typeof initial === 'number'
      ? { x: initial, y: initial, z: initial }
      : { x: initial.x ?? 0, y: initial.y ?? 0, z: initial.z ?? 0 };

    this.value = initVec;
    this.writeThrough(initVec);
  }

  /** Write-through to node.meta.valuesMap as ONE string "x,y,z" */
  private writeThrough(v: Vec3) {
    const meta: any = (this.node as any).meta ?? ((this.node as any).meta = {});
    const valuesMap: Record<string, string> = (meta.valuesMap ??= {});
    valuesMap[this.varName] = `${v.x},${v.y},${v.z}`;
  }

  /** Replace the whole vector */
  setValue(v: Vec3) {
    this.value = { ...v };
    this.writeThrough(this.value);
    this.onChange?.(this.value);
  }

  /** Update a single axis */
  setAxis(axis: 'x' | 'y' | 'z', val: number) {
    const next = { ...this.value, [axis]: val } as Vec3;
    this.setValue(next);
  }

  /** Load from serialized data: accepts {x,y,z}, [x,y,z], "x,y,z", or number */
  serializeLoad(data: any) {
    let v: Vec3 | null = null;

    if (Array.isArray(data)) {
      v = { x: Number(data[0] ?? 0), y: Number(data[1] ?? 0), z: Number(data[2] ?? 0) };
    } else if (data && typeof data === 'object') {
      v = { x: Number(data.x ?? 0), y: Number(data.y ?? 0), z: Number(data.z ?? 0) };
    } else if (typeof data === 'string') {
      const parts = data.split(',').map((s) => Number(s.trim()));
      v = { x: Number(parts[0] ?? 0), y: Number(parts[1] ?? 0), z: Number(parts[2] ?? 0) };
    } else if (typeof data === 'number') {
      v = { x: data, y: data, z: data };
    }

    if (v) this.setValue(v);
  }

  serializeSave(): string {
    const v = this.value;
    return `${v.x},${v.y},${v.z}`;
  }
}

export function CustomVector3Field(props: { data: Vector3Control; labels?: [string, string, string] }) {
  const { data } = props;
  const [tx, setTx] = React.useState<string>(() => fmt1dpMinVec(data.value.x));
  const [ty, setTy] = React.useState<string>(() => fmt1dpMinVec(data.value.y));
  const [tz, setTz] = React.useState<string>(() => fmt1dpMinVec(data.value.z));

  const rx = React.useRef<HTMLInputElement>(null);
  const ry = React.useRef<HTMLInputElement>(null);
  const rz = React.useRef<HTMLInputElement>(null);

  // Reflect external changes
  React.useEffect(() => setTx(fmt1dpMinVec(data.value.x)), [data.value.x]);
  React.useEffect(() => setTy(fmt1dpMinVec(data.value.y)), [data.value.y]);
  React.useEffect(() => setTz(fmt1dpMinVec(data.value.z)), [data.value.z]);

  const commitAxis = (axis: 'x' | 'y' | 'z', rawText: string) => {
    const num = parseMaybeNumber(rawText);
    if (num !== null) {
      data.setAxis(axis, num);
      if (data.node) OnNodeUpdated(data.editor, data.node);
      if (axis === 'x') setTx(fmt1dpMinVec(num));
      if (axis === 'y') setTy(fmt1dpMinVec(num));
      if (axis === 'z') setTz(fmt1dpMinVec(num));
    } else {
      // Revert
      if (axis === 'x') setTx(fmt1dpMinVec(data.value.x));
      if (axis === 'y') setTy(fmt1dpMinVec(data.value.y));
      if (axis === 'z') setTz(fmt1dpMinVec(data.value.z));
    }
  };

  const handleFocus = (e: React.FocusEvent<HTMLInputElement>) => e.target.select();
  const stopAll = (e: any) => e.stopPropagation();

  const onKeyDown = (ref: React.RefObject<HTMLInputElement>) => (e: React.KeyboardEvent<HTMLInputElement>) => {
    if (e.key === 'Enter') {
      e.preventDefault();
      ref.current?.blur();
    }
    e.stopPropagation();
  };

  const labels = props.labels ?? ['X', 'Y', 'Z'];

  return (
    <div
      onPointerDown={stopAll}
      onPointerUp={stopAll}
      onClick={stopAll}
      onDoubleClick={stopAll}
      onMouseDown={stopAll}
      onMouseUp={stopAll}
      onKeyDown={stopAll}
      style={{ display: 'flex', alignItems: 'center', gap: 6 }}
    >
      <AxisField
        refEl={rx}
        label={labels[0]}
        text={tx}
        setText={setTx}
        onCommit={(t) => commitAxis('x', t)}
        onFocus={handleFocus}
        onKeyDown={onKeyDown(rx)}
        variant="x"
      />
      <AxisField
        refEl={ry}
        label={labels[1]}
        text={ty}
        setText={setTy}
        onCommit={(t) => commitAxis('y', t)}
        onFocus={handleFocus}
        onKeyDown={onKeyDown(ry)}
        variant="y"
      />
      <AxisField
        refEl={rz}
        label={labels[2]}
        text={tz}
        setText={setTz}
        onCommit={(t) => commitAxis('z', t)}
        onFocus={handleFocus}
        onKeyDown={onKeyDown(rz)}
        variant="z"
      />

      <style>{`
        .custom-number-input {
          height: 24px;
          font-size: 10px;
          background-color: #141414;
          color: #fff;
          border: 1px solid #434343;
          border-radius: 4px;
          transition: background 0.2s ease, border 0.2s ease;
          line-height: 24px;
          vertical-align: middle;
          width: auto;
          box-sizing: border-box;
          padding: 0;
        }
        .custom-number-input:hover,
        .custom-number-input:focus {
          border: 1px solid rgb(67, 150, 238);
        }
        .axis-wrap {
          display: inline-flex,
          align-items: center;
          gap: 4px;
        }

        /* Font & size for XYZ labels, plus per-axis colors */
        .playrate-flex-number {
          font-family: 'Roboto', sans-serif;
          font-weight: 400;
          font-size: 12px;
          letter-spacing: 0.5px;
          color: #dadada9c;
        }
        .axis-x { color: red; }
        .axis-y { color: green; }
        .axis-z { color: blue; }

        .axis-label {
          user-select: none;
          width: 12px;
          text-align: right;
        }

        .custom-number-input::-webkit-outer-spin-button,
        .custom-number-input::-webkit-inner-spin-button {
          -webkit-appearance: none;
          margin: 0;
        }
        .custom-number-input[type=number] {
          -moz-appearance: textfield;
        }
      `}</style>
    </div>
  );
}

function AxisField(props: {
  refEl: React.RefObject<HTMLInputElement>;
  label: string;
  text: string;
  setText: (v: string) => void;
  onCommit: (raw: string) => void;
  onFocus: (e: React.FocusEvent<HTMLInputElement>) => void;
  onKeyDown: (e: React.KeyboardEvent<HTMLInputElement>) => void;
  variant: 'x' | 'y' | 'z';
}) {
  return (
    <span className="axis-wrap">
      <span className={`axis-label playrate-flex-number axis-${props.variant}`}>{props.label}</span>
      <input
        ref={props.refEl}
        type="text"
        value={props.text}
        onChange={(e) => props.setText(e.target.value)}
        onFocus={props.onFocus}
        onKeyDown={props.onKeyDown}
        onBlur={() => props.onCommit(props.text)}
        className="custom-number-input"
        style={{
          width: `${Math.max(props.text.length * 1.4, 4)}ch`,
          textAlign: 'center'
        }}
      />
    </span>
  );
}
