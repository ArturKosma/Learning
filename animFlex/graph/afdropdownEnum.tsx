// DropdownControlEnum.tsx
import React, { useState, useEffect, useRef } from 'react';
import { ClassicPreset, GetSchemes, NodeEditor } from 'rete';
import { AFSerializeInterface, OnNodeUpdated } from './affunclib';
import { AreaPlugin } from 'rete-area-plugin';
import { ReactPlugin } from 'rete-react-plugin';
import { getManifestEnums } from './afmanager';

type Schemes = GetSchemes<ClassicPreset.Node, ClassicPreset.Connection<ClassicPreset.Node, ClassicPreset.Node>>;

export class DropdownControlEnum extends ClassicPreset.Control implements AFSerializeInterface {
  // Store integer index internally; OnNodeUpdated will stringify it.
  value: number;
  onChange?: (val: number) => void;
  node: ClassicPreset.Node;
  editor: NodeEditor<Schemes>;
  varName: string;

  enumName: string;
  values: string[]; // labels

  constructor(enumMeta: string, editor: NodeEditor<Schemes>, node: ClassicPreset.Node, varName: string) {
    super();
    this.value = 0;
    this.node = node;
    this.editor = editor;
    this.varName = varName;

    this.enumName = enumMeta.replace(/_Enum$/, '');
    const enums = getManifestEnums() || [];
    const match = enums.find(e => e.enum_name === this.enumName);
    this.values = Array.isArray(match?.values) ? match!.values.slice() : [];

    if (!match) {
      console.warn(`[DropdownControlEnum] Enum "${this.enumName}" not found in manifest.`);
    }

    // Initialize from meta.valuesMap if present (prefer numeric index)
    const meta: any = (this.node as any).meta ?? ((this.node as any).meta = {});
    const valuesMap: Record<string, string> = (meta.valuesMap ??= {});
    const existingRaw = valuesMap[this.varName];

    let idx = 0;
    if (typeof existingRaw === 'string') {
      const parsed = Number.parseInt(existingRaw, 10);
      if (!Number.isNaN(parsed) && parsed >= 0 && parsed < this.values.length) {
        idx = parsed;
      } else {
        // Legacy: if a label was stored, map it to index
        const labelIndex = this.values.indexOf(existingRaw);
        if (labelIndex !== -1) {
          idx = labelIndex;
        }
      }
    }
    // Clamp to range if enum list shorter than expected
    if (this.values.length > 0) {
      idx = Math.max(0, Math.min(idx, this.values.length - 1));
    } else {
      idx = 0;
    }

    this.value = idx;
    // Ensure valuesMap stores the integer index as string
    valuesMap[this.varName] = String(this.value);
  }

  // Accept a LABEL from the UI, convert to index, store numeric index
  setValue(label: string) {
    
    const idx = this.values.indexOf(label);
    if (idx === -1) return;

    this.value = idx;
    this.onChange?.(this.value);

    const meta: any = (this.node as any).meta ?? ((this.node as any).meta = {});
    const valuesMap: Record<string, string> = (meta.valuesMap ??= {});
    valuesMap[this.varName] = String(this.value); // <-- store integer as string

    OnNodeUpdated(this.editor, this.node);
  }

  serializeLoad(data: any): void {
    this.setValue(this.values[data]);
  }
}

export function CustomDropdownEnum(props: { data: DropdownControlEnum}) {
  const [displayValue, setDisplayValue] = useState<string>(() => {
    const idx = props.data.value ?? 0;
    return props.data.values?.[idx] ?? '';
  });
  const [options, setOptions] = useState<string[]>(() => props.data.values || []);
  const [isOpen, setIsOpen] = useState(false);
  const dropdownRef = useRef<HTMLDivElement>(null);
  const inputRef = useRef<HTMLInputElement>(null);

  const stop = (e: any) => e.stopPropagation();

  // Sync from control (index -> label)
  useEffect(() => {
    const vals = props.data.values || [];
    setOptions(vals);
    const idx = props.data.value ?? 0;
    setDisplayValue(vals[idx] ?? '');
  }, [props.data.values, props.data.value]);

  // Close when clicking outside
  useEffect(() => {
    const handleClickOutside = (e: MouseEvent) => {
      if (dropdownRef.current && !dropdownRef.current.contains(e.target as Node)) {
        setIsOpen(false);
      }
    };
    document.addEventListener('mousedown', handleClickOutside);
    return () => document.removeEventListener('mousedown', handleClickOutside);
  }, []);

  const openList = () => {
    if (!options.length) return;
    setIsOpen(true);
    inputRef.current?.focus();
  };

  const handleSelect = (value: string) => {
    setDisplayValue(value);
    setIsOpen(false);
    inputRef.current?.blur();
    props.data.setValue(value); // label -> index stored
  };

  const handleFocus = () => openList();

  const handleClick = (e: React.MouseEvent<HTMLInputElement>) => {
    e.stopPropagation();
    openList();
  };

  const handleKeyDown = (e: React.KeyboardEvent<HTMLInputElement>) => {
    // prevent typing (readOnly); Enter just blurs/closes
    if (e.key === 'Enter') {
      e.preventDefault();
      inputRef.current?.blur();
      setIsOpen(false);
    }
    e.stopPropagation();
  };

  return (
    <div
      ref={dropdownRef}
      className="custom-dropdown-container"
      onClick={(e) => e.stopPropagation()}
      onDoubleClick={(e) => e.stopPropagation()}
      onKeyDown={(e) => e.stopPropagation()}
      onPointerDownCapture={stop}
      onDoubleClickCapture={stop}
      onContextMenuCapture={stop}
      onWheelCapture={stop}
    >
      <input
        ref={inputRef}
        className="custom-dropdown-input"
        value={displayValue}
        readOnly
        onFocus={handleFocus}
        onKeyDown={handleKeyDown}
        onPointerDown={stop}
        onMouseDown={stop}
        onDoubleClick={stop}
        onContextMenu={stop}
        placeholder={props.data.enumName || 'Choose value'}
        title={displayValue}
        style={{ cursor: options.length ? 'pointer' : 'not-allowed' }}
      />
      {isOpen && (
        <ul
          className="custom-dropdown-list"
          onPointerDownCapture={stop}
          onWheelCapture={stop}
        >
          {options.map(option => (
            <li
              key={option}
              onClick={() => handleSelect(option)}
              className="custom-dropdown-item"
              onMouseDown={(e) => e.preventDefault()}
              onWheel={(e) => e.stopPropagation()}
            >
              {option}
            </li>
          ))}
        </ul>
      )}
      <style>{`
        .custom-dropdown-input {
          display: inline-block; /* shrink to fit text */
          width: auto;           /* was 100% */
          min-width: 50px;       /* optional: adjust to taste */
          height: 30px;
          font-size: 10px;
          font-family: inherit;
          background-color: #141414;
          color: #fff;
          border: 1px solid #434343;
          border-radius: 6px;
          padding: 0 6px;
          transition: background 0.2s ease, border 0.2s ease;
          outline: none;
          box-sizing: border-box;
        }

        .custom-dropdown-input:hover {
          border: 1px solid rgb(67, 150, 238);
        }

        .custom-dropdown-input:focus {
          border: 1px solid rgb(67, 150, 238);
        }

        .custom-dropdown-container {
          position: relative;
          display: inline-block; /* shrink-wrap */
          font-size: 10px;
          font-family: 'Inter', 'Segoe UI', 'Roboto', 'Helvetica Neue', sans-serif;
        }

        .custom-dropdown-list {
          position: absolute;
          top: 100%;
          left: 0;
          background: #1f1f1f;
          border: 1px solid #434343;
          max-height: 200px;
          overflow-y: auto;
          margin: 0;
          padding: 0;
          list-style: none;
          z-index: 1000;
          border-radius: 4px;
          white-space: nowrap;
          width: max-content;
          min-width: 100%;
        }

        .custom-dropdown-item {
          min-width: 100%;
          height: 30px;
          line-height: 30px;
          padding: 0 6px;
          font-size: 10px;
          color: #fff;
          cursor: pointer;
          border-bottom: 1px solid #333;
          transition: background 0.2s ease;
          overflow: hidden;
          text-overflow: ellipsis;
        }

        .custom-dropdown-item:hover {
          background-color: #2a2a2a;
        }

        .custom-dropdown-list::-webkit-scrollbar {
          width: 8px;
        }

        .custom-dropdown-list::-webkit-scrollbar-track {
          background: #1f1f1f;
          border-radius: 4px;
        }

        .custom-dropdown-list::-webkit-scrollbar-thumb {
          background-color: #444;
          border-radius: 4px;
          border: 2px solid #1f1f1f;
        }

        .custom-dropdown-list::-webkit-scrollbar-thumb:hover {
          background-color: #666;
        }

        /* Firefox */
        .custom-dropdown-list {
          scrollbar-width: thin;
          scrollbar-color: #444 #1f1f1f;
        }
      `}</style>
    </div>
  );
}
