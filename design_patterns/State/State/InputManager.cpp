#include "InputManager.h"
#include <iostream>
#include <algorithm>

InputManager* InputManager::GetInstance()
{
	static InputManager* instance = new InputManager();
	return instance;
}

void InputManager::UpdateInput(unsigned InKey, bool bInPressed)
{
	auto it = InputState.find(InKey);
	if (it != InputState.end())
	{
		if (it->second != bInPressed)
		{
			if (bInPressed)
			{
				it->second = true;
				FindAndCallInputAction(EInputActionType::INPUTACTIONTYPE_Pressed, InKey);
			}
			else
			{
				it->second = false;
				FindAndCallInputAction(EInputActionType::INPUTACTIONTYPE_Released, InKey);
			}
		}
	}
}

void InputManager::BindInput(const wchar_t* InInputActionName, EInputActionType InInputActionType, std::function<void(void)> InCallback)
{
	auto it = std::find_if(InputActions.begin(), InputActions.end(),
		[&InInputActionName](const InputAction& InInputAction) {return InInputAction.InputActionName == InInputActionName; });
	if (it != InputActions.end())
	{
		InInputActionType == EInputActionType::INPUTACTIONTYPE_Pressed ? (*it).BoundFunction_Pressed = InCallback : (*it).BoundFunction_Released = InCallback;
	}
}

void InputManager::FindAndCallInputAction(EInputActionType InInputActionType, unsigned InKey)
{
	std::for_each(InputActions.begin(), InputActions.end(),
		[&InInputActionType, &InKey](const InputAction& InInputAction)
		{
			if (InInputAction == InKey)
			{
				InInputAction.Broadcast(InInputActionType);
			}
		});
}

InputManager::InputManager()
{

}

bool InputManager::InputAction::operator==(unsigned InKey) const
{
	return InputActionKey == InKey;
}

void InputManager::InputAction::Broadcast(EInputActionType InInputActionType) const
{
	if (InInputActionType == EInputActionType::INPUTACTIONTYPE_Pressed && BoundFunction_Pressed)
	{
		BoundFunction_Pressed();
	}
	else if (InInputActionType == EInputActionType::INPUTACTIONTYPE_Released && BoundFunction_Released)
	{
		BoundFunction_Released();
	}
}

InputManager::InputAction::InputAction(const wchar_t* InInputActionName, unsigned InInputActionKey) :
	InputActionName(InInputActionName),
	InputActionKey(InInputActionKey)
{

}
