#include <cassert>
#include "DirectInput.h"
#include "manager/window/WindowManager.h"
#include "debugTools/logger/Logger.h"

void DirectInput::Initialize(WindowManager* windowManager) {
	SetWindowManager(windowManager);

	HRESULT result = S_FALSE;
	directInput = nullptr;
	result = DirectInput8Create(
		windowManager_->GetWndClass().hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&directInput, nullptr
	);
	assert(SUCCEEDED(result));

	keybord = nullptr;
	result = directInput->CreateDevice(GUID_SysKeyboard, &keybord, NULL);
	assert(SUCCEEDED(result));

	result = keybord->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));
	result = keybord->SetCooperativeLevel(windowManager_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void DirectInput::Update() {
	keybord->Acquire();
	memcpy(preKey, key, sizeof(key));
	keybord->GetDeviceState(sizeof(key), key);

	for (int i = 0; i < 4; ++i) {
		preGamepadStates[i] = gamepadStates[i];
		DWORD dwResult = XInputGetState(i, &gamepadStates[i]);
		if (dwResult != ERROR_SUCCESS) {
			ZeroMemory(&gamepadStates[i], sizeof(XINPUT_STATE));
		}
	}
}

// キーボードの押下状態の確認
bool DirectInput::PushKey(BYTE keyNumber) {
	return key[keyNumber] != 0;
}

// キーボードのトリガー状態の確認（前フレームでは押されておらず、現在フレームで押された場合）
bool DirectInput::TriggerKey(BYTE keyNumber) {
	return !preKey[keyNumber] && key[keyNumber];
}

// キーボードのホールド状態の確認（前フレームと現在フレームで押され続けている場合）
bool DirectInput::HoldKey(BYTE keyNumber) {
	return preKey[keyNumber] && key[keyNumber];
}

// キーボードのリリース状態の確認（前フレームで押されており、現在フレームで離された場合）
bool DirectInput::ReleaseKey(BYTE keyNumber) {
	return preKey[keyNumber] && !key[keyNumber];
}


bool DirectInput::PushButton(int controllerID, int buttonNumber) {
	return (gamepadStates[controllerID].Gamepad.wButtons & buttonNumber) != 0;
}

bool DirectInput::TriggerButton(int controllerID, int buttonNumber) {
	return !(preGamepadStates[controllerID].Gamepad.wButtons & buttonNumber) &&
		(gamepadStates[controllerID].Gamepad.wButtons & buttonNumber);
}

bool DirectInput::HoldButton(int controllerID, int buttonNumber) {
	return (preGamepadStates[controllerID].Gamepad.wButtons & buttonNumber) &&
		(gamepadStates[controllerID].Gamepad.wButtons & buttonNumber);
}

bool DirectInput::ReleaseButton(int controllerID, int buttonNumber) {
	return (preGamepadStates[controllerID].Gamepad.wButtons & buttonNumber) &&
		!(gamepadStates[controllerID].Gamepad.wButtons & buttonNumber);
}

// デッドゾーンの設定
void DirectInput::SetDeadZone(int deadZone) {
	deadZone_ = (deadZone < 0) ? 0 : (deadZone > 100) ? 100 : deadZone;
}

int DirectInput::GetDeadZone() const {
	return deadZone_;
}

// スティック入力のデッドゾーン処理
int DirectInput::ProcessDeadZone(int value) const {
	int maxValue = 32767; // XInputのスティックの最大値
	int threshold = maxValue * deadZone_ / 100;

	if (abs(value) < threshold) {
		return 0;
	} else {
		return (value > 0) ? (value - threshold) * maxValue / (maxValue - threshold)
			: (value + threshold) * maxValue / (maxValue - threshold);
	}
}


int DirectInput::GetLeftStickX(int controllerID) const {
	return ProcessDeadZone(gamepadStates[controllerID].Gamepad.sThumbLX);
}

int DirectInput::GetLeftStickY(int controllerID) const {
	return ProcessDeadZone(gamepadStates[controllerID].Gamepad.sThumbLY);
}

int DirectInput::GetRightStickX(int controllerID) const {
	return ProcessDeadZone(gamepadStates[controllerID].Gamepad.sThumbRX);
}

int DirectInput::GetRightStickY(int controllerID) const {
	return ProcessDeadZone(gamepadStates[controllerID].Gamepad.sThumbRY);
}

int DirectInput::GetLeftTrigger(int controllerID) const {
	return gamepadStates[controllerID].Gamepad.bLeftTrigger;
}

int DirectInput::GetRightTrigger(int controllerID) const {
	return gamepadStates[controllerID].Gamepad.bRightTrigger;
}

bool DirectInput::IsPadUp(int controllerID) const {
	return (gamepadStates[controllerID].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0;
}

bool DirectInput::IsPadRight(int controllerID) const {
	return (gamepadStates[controllerID].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0;
}

bool DirectInput::IsPadDown(int controllerID) const {
	return (gamepadStates[controllerID].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0;
}

bool DirectInput::IsPadLeft(int controllerID) const {
	return (gamepadStates[controllerID].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0;
}

void DirectInput::SetWindowManager(WindowManager* windowManager) {
	windowManager_ = windowManager;
}
