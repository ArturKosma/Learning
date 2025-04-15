#pragma once

class IAFPickerInterface
{
public:

	virtual ~IAFPickerInterface() = default;

	virtual void OnClickPressed() = 0;
	virtual void OnClickReleased() = 0;
};