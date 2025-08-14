import React from 'react';
import { ClassicPreset, GetSchemes, NodeEditor } from 'rete';
import { OnNodeUpdated, AFSerializeInterface } from './affunclib';

type Schemes = GetSchemes<ClassicPreset.Node, ClassicPreset.Connection<ClassicPreset.Node, ClassicPreset.Node>>;

export class FloatControl extends ClassicPreset.Control implements AFSerializeInterface {
  value: number;
  onChange?: (val: number) => void;
  node?: ClassicPreset.Node;
  editor: NodeEditor<Schemes>;
  varName: string;

  constructor(editor: NodeEditor<Schemes>, node: ClassicPreset.Node, varName: string, initial: number) {
    super();
    this.value = initial;
    this.node = node;
    this.editor = editor;
    this.varName = varName;
  }

  setValue(val: number) {
    this.value = val;

    // Write-through to valuesMap.
    const meta: any = (this.node as any).meta ?? ((this.node as any).meta = {});
    const valuesMap: Record<string, string> = (meta.valuesMap ??= {});
    valuesMap[this.varName] = String(val);
  }

  serializeLoad(data: any) {
    this.setValue(data);
  }
}

function formatOneDecimalMin(val: number) {
  const str = String(val);
  return str.includes('.') ? str : str + '.0';
}

export function CustomFloatField(props: { data: FloatControl }) {
  const [text, setText] = React.useState<string>(() => formatOneDecimalMin(props.data.value));
  const inputRef = React.useRef<HTMLInputElement>(null);

  // If value changes externally, reflect it without rounding
  React.useEffect(() => {
    setText(formatOneDecimalMin(props.data.value));
  }, [props.data.value]);

  const handleBlur = () => {
    const raw = text.trim();

    // Accept numbers incl. scientific: -1.23e-4, .5, 5., +3
    // If you want to be looser, just use parseFloat and isNaN check.
    const num = Number(raw);
    if (Number.isFinite(num)) {
      props.data.setValue(num);
      setText(formatOneDecimalMin(num));
      if (props.data.node) {
        OnNodeUpdated(props.data.editor, props.data.node);
      }
    } else {
      setText(formatOneDecimalMin(props.data.value));
    }
  };

  const handleFocus = (e: React.FocusEvent<HTMLInputElement>) => {
    e.target.select();
  };

  const handleKeyDown = (e: React.KeyboardEvent<HTMLInputElement>) => {
    if (e.key === 'Enter') {
      e.preventDefault();
      inputRef.current?.blur();
    }
    e.stopPropagation();
  };

  return (
    <div
        onPointerDown={(e) => e.stopPropagation()}
        onPointerUp={(e) => e.stopPropagation()}
        onClick={(e) => e.stopPropagation()}
        onDoubleClick={(e) => e.stopPropagation()}
        onMouseDown={(e) => e.stopPropagation()}
        onMouseUp={(e) => e.stopPropagation()}
        onKeyDown={(e) => e.stopPropagation()}
      >
        <input
          ref={inputRef}
          type="text"
          value={text}
          onChange={(e) => setText(e.target.value)}
          onFocus={handleFocus}
          onKeyDown={handleKeyDown}
          onBlur={handleBlur}
          className="custom-number-input"
          style={{
            width: `${Math.max(text.length * 1.4, 4)}ch`,
            textAlign: 'center'
          }}
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

          .custom-number-input:hover {
            border: 1px solid rgb(67, 150, 238);
          }

          .custom-number-input:focus {
            border: 1px solid rgb(67, 150, 238);
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