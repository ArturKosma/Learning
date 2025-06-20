import React from 'react';
import { ClassicPreset } from 'rete';
import { OnNodeUpdated } from './affunclib';

export class FloatControl extends ClassicPreset.Control {
  value: number;
  onChange?: (val: number) => void;
  node?: ClassicPreset.Node;

  constructor(node: ClassicPreset.Node) {
    super();
    this.value = 0.0;
    this.node = node;
  }

  setValue(val: number) {
    this.value = val;
  }
}

export function CustomFloatField(props: { data: FloatControl }) {
  const [text, setText] = React.useState<string>(() => props.data.value.toFixed(1));
  const inputRef = React.useRef<HTMLInputElement>(null);

  React.useEffect(() => {
    setText(props.data.value.toFixed(1));
  }, [props.data.value]);

  const handleBlur = () => {
    const parsed = parseFloat(text);
    if (isNaN(parsed)) {
      props.data.setValue(0.0);
      setText("0.0");
      return;
    }

    const rounded = parseFloat(parsed.toFixed(3));
    props.data.setValue(rounded);

    const formatted =
      Number.isInteger(rounded) ? `${rounded.toFixed(1)}` : `${rounded}`;
    setText(formatted);

    if (props.data.node) {
      OnNodeUpdated(props.data.node);
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
            min-width: 30px;
            height: 30px;
            font-size: 10px;
            background-color: #141414;
            color: #fff;
            border: 1px solid #434343;
            border-radius: 6px;
            padding: 0 6px;
            transition: background 0.2s ease, border 0.2s ease;
            outline: none;
            width: auto
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