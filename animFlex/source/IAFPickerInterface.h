#pragma once

class IAFPickerInterface
{
public:

	virtual ~IAFPickerInterface() = default;

	virtual void OnClickPressed(uint8_t elementId) = 0;

	virtual void OnHoverBegin(uint8_t elementId) = 0;
	virtual void OnHoverEnd(uint8_t elementId) = 0;

	virtual std::vector<uint8_t> GetDisabledElements() = 0;
};