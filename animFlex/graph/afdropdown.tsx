import { Select, ConfigProvider, theme } from 'antd';
import { useState } from 'react';
import type { ThemeConfig } from 'antd/es/config-provider/context';
import { ClassicPreset } from "rete";
import * as React from "react";
import { GraphUpdate } from './affunclib';

const url = `https://cdn.jsdelivr.net/gh/ArturKosma/assets@main/anims/manifest.json`;
let animNamesManifest: { name: string }[];
async function loadManifest() : Promise<{ name: string }[]> {
    if(!animNamesManifest) {
        const res = await fetch(url);
        animNamesManifest = await res.json();
        console.log('Loaded manifest:', animNamesManifest);
    }
    return animNamesManifest;
}

const themeConfig: ThemeConfig = {
  algorithm: theme.darkAlgorithm,
  token: {
    fontSize: 10,
  }
};

export class DropdownControl extends ClassicPreset.Control {
    constructor() {
        super();
    }
}

export function CustomDropdown(props: {data: DropdownControl}) {

    const [searchValue, setSearchValue] = useState('');
    const [manifestOptions, setManifestOptions] = useState<{value: String, label: string}[]>([]);

    // Load manifest with names on mount.
    React.useEffect(() => {
        loadManifest().then(entries => {
            const options = entries.map(entry => ({
                value: entry.name,
                label: entry.name
            }));
            setManifestOptions(options);
        });
    }, []);

    const stopEventPropagation = (e: React.SyntheticEvent) => {
        e.stopPropagation();
    };

    const handleSearch = (value: string) => {
    setSearchValue(value);
    };

    const handleInputKeyDown = (e: React.KeyboardEvent<HTMLInputElement>) => {
    if (e.key === 'Backspace') {
      e.preventDefault();
      setSearchValue((prev) => prev.slice(0, -1));
    }
    };

    return (
        <ConfigProvider theme={themeConfig}>
            <div
                onPointerDown={stopEventPropagation}
                onPointerUp={stopEventPropagation}
                onClick={stopEventPropagation}
                onDoubleClick={stopEventPropagation}
                onMouseDown={stopEventPropagation}
                onMouseUp={stopEventPropagation}
            >
                <Select 
                    showSearch
                    popupMatchSelectWidth={false}
                    filterOption={(input, option) => {
                        const normalize = (str: string) =>
                            str.toLowerCase().replace(/[\s_\-]+/g, ' ').split(' ').filter(Boolean);

                        const inputTokens = normalize(input);
                        const labelTokens = normalize(option?.label ?? '');

                        return inputTokens.every(token =>
                            labelTokens.some(labelToken => labelToken.includes(token))
                        );
                    }}
                    defaultValue="Choose Anim"
                    onChange={(value: string) => {GraphUpdate()}}
                    options={manifestOptions}
                    style={{ width: 150 }}
                    searchValue={searchValue}
                    onSearch={handleSearch}
                    onInputKeyDown={handleInputKeyDown}
                    >
                </Select>
            </div>
        </ConfigProvider>
    );
}