#include "InputManager.h"
#include "input.h"
#include "C_XInput.h"
#include "stdio.h"
bool bInputs[MAX_INPUTS];
C_XInput* Player1 = nullptr;
bool bXinputConnected;
bool bHoldingXinput[MAX_INPUTS];
void XinputTriggerControl(bool BeforeInputs);
void InitInputManager()
{
	InitInput();
	Player1 = new C_XInput(1);
	for (int i = 0; i < MAX_INPUTS; bInputs[i] = false, i++);
	for (int i = 0; i < MAX_INPUTS; bHoldingXinput[i] = false, i++);
}

void UpdateInputManager()
{
	UpdateInput();
	Player1->UpdateXinput();
	XinputTriggerControl(true);
	bXinputConnected = Player1->IsConnected();

	bInputs[INPUT_LEFT] = GetKeyPress(VK_A) || (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
	bInputs[INPUT_DOWN] = (GetKeyPress(VK_S) && !GetKeyPress(VK_LCONTROL)) || (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
	bInputs[INPUT_RIGHT] = GetKeyPress(VK_D) || (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
	bInputs[INPUT_DEBUGAIM] = GetKeyTrigger(VK_1) || ((Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) && !bHoldingXinput[INPUT_DEBUGAIM]);
	bInputs[INPUT_NINJACRAWL_UP] = GetKeyPress(VK_W) || (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
	bInputs[INPUT_UP] = GetKeyPress(VK_W) || (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
	bInputs[TRANSFORM_GEISHA] = GetKeyPress(VK_LSHIFT) || (bXinputConnected && Player1->GetState().Gamepad.bRightTrigger > 0);
	bInputs[INPUT_JUMP] = GetKeyTrigger(VK_K) || (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A && !bHoldingXinput[INPUT_JUMP]);
	bInputs[TRANSFORM_SAMURAI] = GetKeyPress(VK_TAB) || (bXinputConnected && Player1->GetState().Gamepad.bLeftTrigger > 0);
	bInputs[INPUT_ATTACK] = GetKeyTrigger(VK_I) || (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_Y && !bHoldingXinput[INPUT_ATTACK]);
	bInputs[INPUT_DEBUGAIM_SPEEDDOWN] = GetKeyPress(VK_TAB) || (bXinputConnected && Player1->GetState().Gamepad.bLeftTrigger > 0);
	bInputs[INPUT_DEBUGAIM_SPEEDUP] = GetKeyPress(VK_LCONTROL) || (bXinputConnected && Player1->GetState().Gamepad.bRightTrigger > 0);
	bInputs[INPUT_DEBUGAIM_ACCEPT] = GetKeyTrigger(VK_E) || (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A && !bHoldingXinput[INPUT_DEBUGAIM_ACCEPT]);
	bInputs[INPUT_SCALE_DOWN_X] = GetKeyPress(VK_J);
	bInputs[INPUT_SCALE_UP_X] = GetKeyPress(VK_L);
	bInputs[INPUT_SCALE_DOWN_Z] = GetKeyPress(VK_K);
	bInputs[INPUT_SCALE_UP_Z] = GetKeyPress(VK_I);
	bInputs[INPUT_SWITCH_DEBUG_TYPE_UP] = GetKeyTrigger(VK_O);
	bInputs[INPUT_SWITCH_DEBUG_TYPE_DOWN] = GetKeyTrigger(VK_U);
	bInputs[INPUT_SAVE_LEVEL] = GetKeyPress(VK_LCONTROL) && GetKeyTrigger(VK_S);
	bInputs[INPUT_DEBUGAIM_DELETE] = GetKeyTrigger(VK_BACK);
	bInputs[INPUT_SHIFT] = GetKeyPress(VK_LSHIFT);
	bInputs[INPUT_SWITCH_STATIC_OBJECT] = GetKeyTrigger(VK_2);
	XinputTriggerControl(false);
}

void EndInputManager()
{
	UninitInput();

}

bool GetInput(int nInput)
{
	return bInputs[nInput];
}


void XinputTriggerControl(bool BeforeInputs)
{
	if (BeforeInputs) {
		if (bHoldingXinput[INPUT_ATTACK] && !(Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_Y)) bHoldingXinput[INPUT_ATTACK] = false;
		if (bHoldingXinput[INPUT_JUMP] && !(Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A)) bHoldingXinput[INPUT_JUMP] = false;
		if (bHoldingXinput[INPUT_DEBUGAIM_ACCEPT] && !(Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A)) bHoldingXinput[INPUT_DEBUGAIM_ACCEPT] = false;
		if (bHoldingXinput[INPUT_JUMP] && !(Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A)) bHoldingXinput[INPUT_DEBUGAIM] = false;

	}
	else {
		if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_Y) bHoldingXinput[INPUT_ATTACK] = true;
		if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A) bHoldingXinput[INPUT_JUMP] = true;
		if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A) bHoldingXinput[INPUT_DEBUGAIM_ACCEPT] = true;
		if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A) bHoldingXinput[INPUT_DEBUGAIM] = true;
	}
}