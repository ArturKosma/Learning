import React from 'react';
import { ClassicPreset, GetSchemes, NodeEditor } from 'rete';
import { AFSerializeInterface, OnNodeUpdated } from './affunclib';

type Schemes = GetSchemes<ClassicPreset.Node, ClassicPreset.Connection<ClassicPreset.Node, ClassicPreset.Node>>;

export class BoolControl extends ClassicPreset.Control implements AFSerializeInterface {
  value: boolean;
  onChange?: (val: boolean) => void;
  node: ClassicPreset.Node;
  editor: NodeEditor<Schemes>;
  varName: string;

  constructor(editor: NodeEditor<Schemes>, node: ClassicPreset.Node, varName: string, initial: boolean) {
    super();
    this.value = initial;
    this.node = node;
    this.editor = editor;
    this.varName = varName;
  }

  setValue(val: boolean) {
    this.value = val;

    // Write-through to valuesMap.
    const meta: any = (this.node as any).meta ?? ((this.node as any).meta = {});
    const valuesMap: Record<string, string> = (meta.valuesMap ??= {});
    valuesMap[this.varName] = String(val);

    this.onChange?.(val);
    OnNodeUpdated(this.editor, this.node);
  }

  serializeLoad(data: any): void {
    this.setValue(data);
  }
}

export function CustomChecker(props: { data: BoolControl }) {
  const [checked, setChecked] = React.useState<boolean>(props.data.value);

  React.useEffect(() => {
    setChecked(props.data.value);
  }, [props.data.value]);

  const handleChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    const newValue = e.target.checked;
    setChecked(newValue);
    props.data.setValue(newValue);
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
        type="checkbox"
        checked={checked}
        onChange={handleChange}
        className="custom-checkbox-input"
      />
      <style>{`
        .custom-checkbox-input {
          appearance: none;
          width: 20px;
          height: 20px;
          background-color: #141414;
          border: 1px solid #434343;
          border-radius: 4px;
          position: relative;
          transition: background 0.2s ease, border 0.2s ease;
          cursor: pointer;
          vertical-align: middle;
          box-sizing: border-box;
          padding: 0;
        }

        .custom-checkbox-input:hover {
          border-color: rgb(67, 150, 238);
        }

        .custom-checkbox-input:checked {
          background-color: rgb(67, 150, 238);
          border-color: rgb(67, 150, 238);
        }

        .custom-checkbox-input:checked::after {
          content: "";
          position: absolute;
          width: 4px;
          height: 8px;
          top: 50%;
          left: 50%;
          border: solid white;
          border-width: 0 2px 2px 0;
          transform: translate(-50%, -50%) rotate(45deg);
          transform-origin: center;
        }
      `}</style>
    </div>
  );
}