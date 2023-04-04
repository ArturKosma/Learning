#pragma once
#include "InputDefines.h"
#include <unordered_map>
#include <functional>

enum class EInputActionType
{
	INPUTACTIONTYPE_Pressed,
	INPUTACTIONTYPE_Released
};

class InputManager
{
private:

	struct InputAction
	{
		const wchar_t* InputActionName;
		const unsigned InputActionKey;

		std::function<void(void)> BoundFunction_Pressed;
		std::function<void(void)> BoundFunction_Released;

		bool operator==(unsigned InKey) const;
		void Broadcast(EInputActionType InInputActionType) const;

		InputAction(const wchar_t* InInputActionName, unsigned InInputActionKey);
	};

public:

	static InputManager* GetInstance();
	void UpdateInput(unsigned InKey, bool bInPressed);
	void BindInput(const wchar_t* InInputActionName, EInputActionType InInputActionType, std::function<void(void)>);

private:

	std::unordered_map<unsigned, bool> InputState =
	{
		{INPUTKEY_A, false},{INPUTKEY_B, false},{INPUTKEY_C, false},{INPUTKEY_D, false},
		{INPUTKEY_E, false},{INPUTKEY_F, false},{INPUTKEY_G, false},{INPUTKEY_H, false},
		{INPUTKEY_I, false},{INPUTKEY_J, false},{INPUTKEY_K, false},{INPUTKEY_L, false},
		{INPUTKEY_M, false},{INPUTKEY_N, false},{INPUTKEY_O, false},{INPUTKEY_P, false},
		{INPUTKEY_Q, false},{INPUTKEY_R, false},{INPUTKEY_S, false},{INPUTKEY_T, false},
		{INPUTKEY_U, false},{INPUTKEY_V, false},{INPUTKEY_W, false},{INPUTKEY_X, false},
		{INPUTKEY_Y, false},{INPUTKEY_Z, false}
	};

	std::vector<InputAction> InputActions =
	{
		InputAction(L"Forward", INPUTKEY_W),
		InputAction(L"Back", INPUTKEY_S),
		InputAction(L"Left", INPUTKEY_A),
		InputAction(L"Right", INPUTKEY_D)
	};

	void FindAndCallInputAction(EInputActionType, unsigned);

	InputManager();
};

