import React, { useState, useEffect, useRef } from 'react';
import { ClassicPreset, GetSchemes, NodeEditor } from 'rete';
import { AFSerializeInterface, OnNodeUpdated } from './affunclib';
import { AreaPlugin } from 'rete-area-plugin';
import { ReactPlugin } from 'rete-react-plugin';

// Dropdown loading.
const animsUrl = `https://cdn.jsdelivr.net/gh/ArturKosma/assets@main/anims/manifest.json`;
const curvesUrl = `https://cdn.jsdelivr.net/gh/ArturKosma/assets@main/curves/manifest.json`;

const manifestCache = new Map<string, { name: string }[]>();

async function loadManifest(url: string): Promise<{ name: string }[]> {
  if (!manifestCache.has(url)) {
    const res = await fetch(url);
    if (!res.ok) throw new Error(`Failed to load manifest: ${url}`);
    const data = (await res.json()) as { name: string }[];
    manifestCache.set(url, data);
  }
  return manifestCache.get(url)!;
}

type Schemes = GetSchemes<ClassicPreset.Node, ClassicPreset.Connection<ClassicPreset.Node, ClassicPreset.Node>>;

export class DropdownControl extends ClassicPreset.Control implements AFSerializeInterface {
  type: string
  value: string;
  onChange?: (val: string) => void;
  node: ClassicPreset.Node;
  editor: NodeEditor<Schemes>;
  varName: string;

  constructor(type: string, editor: NodeEditor<Schemes>, node: ClassicPreset.Node, varName: string, defaultValue: string = "") {
    super();
    this.type = type;
    this.value = defaultValue;
    this.node = node;
    this.editor = editor;
    this.varName = varName;
  }

  setValue(val: string) {
    this.value = val;
    this.onChange?.(val);

    // Write-through to valuesMap.
    const meta: any = (this.node as any).meta ?? ((this.node as any).meta = {});
    const valuesMap: Record<string, string> = (meta.valuesMap ??= {});
    valuesMap[this.varName] = String(val);

    OnNodeUpdated(this.editor, this.node)
  }

  serializeLoad(data: any): void {
    this.setValue(data);
  }
}

export function CustomDropdown(props: { data: DropdownControl }) {
  const [searchValue, setSearchValue] = useState(() => props.data.value || '');
  const [manifestOptions, setManifestOptions] = useState<string[]>([]);
  const [filteredOptions, setFilteredOptions] = useState<string[]>([]);
  const [isOpen, setIsOpen] = useState(false);
  const dropdownRef = useRef<HTMLDivElement>(null);
  const inputRef = useRef<HTMLInputElement>(null);

  const stop = (e: any) => e.stopPropagation();

  const normalize = (str: string) =>
    str.toLowerCase().replace(/[\s_\-]+/g, ' ').split(' ').filter(Boolean);

  const filterNames = (names: string[], inputTokens: string[]) =>
    names.filter((name) => {
      const labelTokens = normalize(name);
      return inputTokens.every((token) => labelTokens.some((lt) => lt.includes(token)));
    });

  // Enable dropdown only for known manifest-backed types
  const dropdownEnabled =
    props.data.type === 'Dropdown_Anims' || props.data.type === 'Dropdown_Curves';

  useEffect(() => {
    if (!dropdownEnabled) {
      // No manifest => behave like a simple text field
      setManifestOptions([]);
      setFilteredOptions([]);
      setIsOpen(false);
      return;
    }

    const url =
      props.data.type === 'Dropdown_Anims'
        ? animsUrl
        : curvesUrl;

    let alive = true;
    loadManifest(url)
      .then((entries) => {
        if (!alive) return;
        const names = entries.map((e) => e.name);
        setManifestOptions(names);

        if (searchValue) {
          const inputTokens = normalize(searchValue);
          setFilteredOptions(filterNames(names, inputTokens));
        } else {
          setFilteredOptions(names);
        }
      })
      .catch(() => {
        if (!alive) return;
        setManifestOptions([]);
        setFilteredOptions([]);
      });

    return () => {
      alive = false;
    };
  }, [props.data.type, dropdownEnabled]); // minimal change: keep deps tight

  useEffect(() => {
    const handleClickOutside = (e: MouseEvent) => {
      if (dropdownRef.current && !dropdownRef.current.contains(e.target as Node)) {
        setIsOpen(false);
      }
    };
    document.addEventListener('mousedown', handleClickOutside);
    return () => document.removeEventListener('mousedown', handleClickOutside);
  }, []);

  // --- commit only after typing finished & dedupe ---
  const lastCommittedRef = useRef<string>(props.data.value || '');
  const suppressBlurCommitRef = useRef(false);

  const commitValue = (val: string) => {
    if (lastCommittedRef.current !== val) {
      props.data.setValue(val);
      lastCommittedRef.current = val;
    }
  };
  // --------------------------------------------------

  const handleSearch = (value: string) => {
    setSearchValue(value);
    if (!dropdownEnabled) return; // simple text field mode
    const inputTokens = normalize(value);
    const matches = manifestOptions.filter(name => {
      const labelTokens = normalize(name);
      return inputTokens.every(token =>
        labelTokens.some(labelToken => labelToken.includes(token))
      );
    });
    setFilteredOptions(matches);
  };

  const handleSelect = (value: string) => {
    suppressBlurCommitRef.current = true; // skip imminent blur commit
    setSearchValue(value);
    setIsOpen(false);
    commitValue(value); // single update
    inputRef.current?.blur();
    setTimeout(() => {
      suppressBlurCommitRef.current = false; // re-enable blur commits next tick
    }, 0);
  };

  const handleKeyDown = (e: React.KeyboardEvent<HTMLInputElement>) => {
    if (e.key === 'Enter') {
      e.preventDefault();
      inputRef.current?.blur(); // onBlur will commit (if not suppressed)
    }
    e.stopPropagation();
  };

  const handleChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    handleSearch(e.target.value);
    // intentionally NOT committing here anymore
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
        value={searchValue}
        onChange={handleChange}
        onFocus={(e) => { e.target.select(); if (dropdownEnabled) setIsOpen(true); }}
        onKeyDown={handleKeyDown}
        onBlur={() => {
          if (!suppressBlurCommitRef.current) {
            commitValue(searchValue); // commit after typing finished
          }
        }}
        onPointerDown={stop}
        onMouseDown={stop}
        onDoubleClick={stop}
        onContextMenu={stop}
        placeholder={
          dropdownEnabled
            ? (props.data.type === "Dropdown_Anims" ? "Choose Anim" : "Choose Curve")
            : "Type value…"
        }
        title={searchValue}
      />
      {dropdownEnabled && isOpen && (
        <ul className="custom-dropdown-list"
            onPointerDownCapture={stop}
            onWheelCapture={stop}
        >
          {filteredOptions.map(option => (
            <li
              key={option}
              onClick={(e) => { e.stopPropagation(); handleSelect(option); }}
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
          width: 100%;
          height: 30px;
          font-size: 10px;
          font-family: 'Inter', 'Segoe UI', 'Roboto', 'Helvetica Neue', sans-serif;
          background-color: #141414;
          color: #fff;
          border: 1px solid #434343;
          border-radius: 6px;
          padding: 0 6px;
          transition: background 0.2s ease, border 0.2s ease;
          outline: none;
          box-sizing: border-box;
        }
        .custom-dropdown-input:hover { border: 1px solid rgb(67, 150, 238); }
        .custom-dropdown-input:focus { border: 1px solid rgb(67, 150, 238); }

        .custom-dropdown-container {
          position: relative;
          min-width: 150px;
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

        .custom-dropdown-item:hover { background-color: #2a2a2a; }

        .custom-dropdown-list::-webkit-scrollbar { width: 8px; }
        .custom-dropdown-list::-webkit-scrollbar-track { background: #1f1f1f; border-radius: 4px; }
        .custom-dropdown-list::-webkit-scrollbar-thumb {
          background-color: #444; border-radius: 4px; border: 2px solid #1f1f1f;
        }
        .custom-dropdown-list::-webkit-scrollbar-thumb:hover { background-color: #666; }

        /* Firefox */
        .custom-dropdown-list { scrollbar-width: thin; scrollbar-color: #444 #1f1f1f; }
      `}</style>
    </div>
  );
}
