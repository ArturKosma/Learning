#pragma once

class IAFPickerInterface
{
public:

	virtual ~IAFPickerInterface() = default;

	virtual void OnClickPressed(uint8_t elementId) = 0;
};