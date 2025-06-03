import { Select, ConfigProvider, theme } from 'antd';
import { useState } from 'react';
import type { ThemeConfig } from 'antd/es/config-provider/context';
import { ClassicPreset } from "rete";
import * as React from "react";

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
                //onKeyDown={stopEventPropagation}
                //onKeyUp={stopEventPropagation}
                //onKeyPress={stopEventPropagation}
            >
                <Select 
                    showSearch
                    filterOption={(input, option) =>
                    (option?.label ?? '').toLowerCase().includes(input.toLowerCase())
                    }
                    defaultValue="lucy"
                    onChange={(value: string) => {console.log(value)}}
                    options={[
                        {value: 'jack', label: 'Jack'},
                        {value: 'lucy', label: 'Lucy'},
                        {value: 'sonia', label: 'Sonia'}
                    ]}
                    style={{ minWidth: 150 }}
                    searchValue={searchValue}
                    onSearch={handleSearch}
                    onInputKeyDown={handleInputKeyDown}
                    >
                </Select>
            </div>
        </ConfigProvider>
    );
}