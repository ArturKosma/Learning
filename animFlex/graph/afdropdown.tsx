import { Select, ConfigProvider, theme } from 'antd';
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

    const stopEventPropagation = (e: React.SyntheticEvent) => {
        e.stopPropagation();
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
                onKeyDown={stopEventPropagation}
                onKeyUp={stopEventPropagation}
                onKeyPress={stopEventPropagation}
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
                    >
                </Select>
            </div>
        </ConfigProvider>
    );
}