import React, { useState, useEffect, useRef } from 'react';
import { ClassicPreset } from 'rete';
import { GraphUpdate } from './affunclib';
import { AreaPlugin } from 'rete-area-plugin';
import { ReactPlugin } from 'rete-react-plugin';

// Anims dropdown loading.
const url = `https://cdn.jsdelivr.net/gh/ArturKosma/assets@main/anims/manifest.json`;
let animNamesManifest: { name: string }[];
async function loadManifest(): Promise<{ name: string }[]> {
  if (!animNamesManifest) {
    const res = await fetch(url);
    animNamesManifest = await res.json();
  }
  return animNamesManifest;
}

export class DropdownControl extends ClassicPreset.Control {

  value: string;
  onChange?: (val: string) => void;

  constructor(type: string) {
    super();
    this.value = ''
  }

  setValue(val: string) {
    this.value = val;
    this.onChange?.(val);
  }
}

export function CustomDropdown(props: { data: DropdownControl, area: AreaPlugin<any>, render: ReactPlugin<any> }) {
  const [searchValue, setSearchValue] = useState(() => props.data.value || '');
  const [manifestOptions, setManifestOptions] = useState<string[]>([]);
  const [filteredOptions, setFilteredOptions] = useState<string[]>([]);
  const [isOpen, setIsOpen] = useState(false);
  const dropdownRef = useRef<HTMLDivElement>(null);
  const inputRef = useRef<HTMLInputElement>(null);

  useEffect(() => {
    loadManifest().then(entries => {
      const names = entries.map(entry => entry.name);
      setManifestOptions(names);
      setFilteredOptions(names);
    });
  }, []);

  useEffect(() => {
    const handleClickOutside = (e: MouseEvent) => {
      if (dropdownRef.current && !dropdownRef.current.contains(e.target as Node)) {
        setIsOpen(false);
      }
    };
    document.addEventListener('mousedown', handleClickOutside);
    return () => document.removeEventListener('mousedown', handleClickOutside);
  }, []);

  const normalize = (str: string) =>
    str.toLowerCase().replace(/[\s_\-]+/g, ' ').split(' ').filter(Boolean);

  const handleSearch = (value: string) => {
    setSearchValue(value);
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
    setSearchValue(value);
    setIsOpen(false);
    inputRef.current?.blur();
    props.data.setValue(value);
  };

    const handleFocus = (e: React.FocusEvent<HTMLInputElement>) => {
        e.target.select();
        setIsOpen(true);
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
      ref={dropdownRef}
      className="custom-dropdown-container"
      onClick={(e) => e.stopPropagation()}
      onDoubleClick={(e) => e.stopPropagation()}
      onKeyDown={(e) => e.stopPropagation()}
    >
      <input
        ref={inputRef}
        className="custom-dropdown-input"
        value={searchValue}
        onChange={(e) => handleSearch(e.target.value)}
        onFocus={handleFocus}
        onKeyDown={handleKeyDown}
        placeholder="Choose Anim"
        title={searchValue}
      />
      {isOpen && (
        <ul className="custom-dropdown-list"
            onPointerDown={(e) => e.stopPropagation()}
            onPointerUp={(e) => e.stopPropagation()}
        >
          {filteredOptions.map(option => (
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

        .custom-dropdown-input:hover {
          border: 1px solid rgb(67, 150, 238);
        }

        .custom-dropdown-input:focus {
          border: 1px solid rgb(67, 150, 238);
        }

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