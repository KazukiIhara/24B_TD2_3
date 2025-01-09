#pragma once

#define DIRECTINPUT_VERSION	0x0800
#include <dinput.h>
#include <XInput.h>
#include "directX/includes/ComPtr.h"
#include "enum/DirectInputEnum.h"

#pragma comment(lib, "xinput.lib")

class WindowManager;

class DirectInput {
public:
	void Initialize(WindowManager* windowManager);
	void Update();

	// キー入力
	bool PushKey(BYTE keyNumber);
	bool TriggerKey(BYTE keyNumber);
	bool HoldKey(BYTE keyNumber);
	bool ReleaseKey(BYTE keyNumber);

	// コントローラのボタン入力
	bool PushButton(int controllerID, int buttonNumber);
	bool TriggerButton(int controllerID, int buttonNumber);
	bool HoldButton(int controllerID, int buttonNumber);
	bool ReleaseButton(int controllerID, int buttonNumber);

	// デッドゾーンの設定
	void SetDeadZone(int deadZone);
	int GetDeadZone() const;
	int ProcessDeadZone(int value) const;

	// スティックやトリガーの位置取得
	int GetLeftStickX(int controllerID) const;
	int GetLeftStickY(int controllerID) const;
	int GetRightStickX(int controllerID) const;
	int GetRightStickY(int controllerID) const;
	int GetLeftTrigger(int controllerID) const;
	int GetRightTrigger(int controllerID) const;

	// 方向キー
	bool IsPadUp(int controllerID) const;
	bool IsPadRight(int controllerID) const;
	bool IsPadDown(int controllerID) const;
	bool IsPadLeft(int controllerID) const;

private:
	void SetWindowManager(WindowManager* windowManager);

private:
	WindowManager* windowManager_ = nullptr;
	ComPtr <IDirectInput8> directInput = nullptr;
	ComPtr <IDirectInputDevice8> keybord = nullptr;

	BYTE key[256];
	BYTE preKey[256];

	// 最大4つのコントローラの状態を管理
	XINPUT_STATE gamepadStates[4]{};
	XINPUT_STATE preGamepadStates[4]{};

	int deadZone_ = 25;
};
