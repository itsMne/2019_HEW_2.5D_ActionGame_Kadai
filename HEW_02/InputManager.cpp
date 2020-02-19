//*****************************************************************************
// GameInputManager.cpp
// �C���v�b�g�̊Ǘ�
//*****************************************************************************
#include "InputManager.h"
#include "input.h"
#include "C_XInput.h"
#include "stdio.h"

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
bool bInputs[MAX_INPUTS];
C_XInput* Player1 = nullptr;
bool bXinputConnected;
bool bHoldingXinput[MAX_INPUTS];
void XinputTriggerControl(bool BeforeInputs);

//*****************************************************************************
//InitInputManager�֐�
//�������̊֐�
//�����Fvoid
//�߁Fvoid
//*****************************************************************************
void InitInputManager()
{
	InitInput();
	Player1 = new C_XInput(1);
	for (int i = 0; i < MAX_INPUTS; bInputs[i] = false, i++);
	for (int i = 0; i < MAX_INPUTS; bHoldingXinput[i] = false, i++);
}

//*****************************************************************************
//UpdateInputManager�֐�
//�A�b�v�f�[�g�̊֐�
//�����Fvoid
//�߁Fvoid
//*****************************************************************************
void UpdateInputManager()
{
	UpdateInput();
	Player1->UpdateXinput();
	XinputTriggerControl(true);
	bXinputConnected = Player1->IsConnected();

	bInputs[INPUT_PAUSE] = GetKeyTrigger(VK_RETURN) || ((Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_START && !bHoldingXinput[INPUT_PAUSE]));
	bInputs[INPUT_LEFT] = GetKeyPress(VK_A) || (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
	bInputs[INPUT_TRIGGER_LEFT] = GetKeyTrigger(VK_A) || ((Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) && !bHoldingXinput[INPUT_TRIGGER_LEFT]);
	bInputs[INPUT_DOWN] = (GetKeyPress(VK_S) && !GetKeyPress(VK_LCONTROL)) || (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
	bInputs[INPUT_RIGHT] = GetKeyPress(VK_D) || (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
	bInputs[INPUT_TRIGGER_RIGHT] = GetKeyTrigger(VK_D) || ((Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) && !bHoldingXinput[INPUT_TRIGGER_RIGHT]);
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
	bInputs[INPUT_SPACE] = GetKeyPress(VK_SPACE);
	bInputs[INPUT_SWITCH_DEBUG_TYPE_UP] = GetKeyTrigger(VK_O);
	bInputs[INPUT_SWITCH_DEBUG_TYPE_DOWN] = GetKeyTrigger(VK_U);
	bInputs[INPUT_SAVE_LEVEL] = GetKeyPress(VK_LCONTROL) && GetKeyTrigger(VK_S);
	bInputs[INPUT_DEBUGAIM_DELETE] = GetKeyTrigger(VK_BACK);
	bInputs[INPUT_SHIFT] = GetKeyPress(VK_LSHIFT);
	bInputs[INPUT_SWITCH_STATIC_OBJECT] = GetKeyTrigger(VK_2);
	bInputs[INPUT_MOVEABLE_OBJECT_SPEED_UP] = GetKeyTrigger(VK_UP);
	bInputs[INPUT_MOVEABLE_OBJECT_SPEED_DOWN] = GetKeyTrigger(VK_DOWN);

	bInputs[INPUT_MOVEABLE_OBJECT_DELAY_UP] = GetKeyTrigger(VK_RIGHT);
	bInputs[INPUT_MOVEABLE_OBJECT_DELAY_DOWN] = GetKeyTrigger(VK_LEFT);
	XinputTriggerControl(false);
}

//*****************************************************************************
//EndInputManager�֐�
//�I���̊֐�
//�����Fvoid
//�߁Fvoid
//*****************************************************************************
void EndInputManager()
{
	UninitInput();

}

//*****************************************************************************
//GetInput�֐�
//�C���v�b�g�̏�Ԃ�߂�
//�����Fint
//�߁Fbool
//*****************************************************************************
bool GetInput(int nInput)
{
	return bInputs[nInput];
}

//*****************************************************************************
//IsXInputConnected�֐�
//XBOX�̃R���g���[���[���g���Ă��邱�Ƃ��m�F����
//�����Fvoid
//�߁Fbool
//*****************************************************************************
void XinputTriggerControl(bool BeforeInputs)
{
	if (BeforeInputs) {
		if (bHoldingXinput[INPUT_ATTACK] && !(Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_Y)) bHoldingXinput[INPUT_ATTACK] = false;
		if (bHoldingXinput[INPUT_TRIGGER_LEFT] && !(Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)) bHoldingXinput[INPUT_TRIGGER_LEFT] = false;
		if (bHoldingXinput[INPUT_TRIGGER_RIGHT] && !(Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)) bHoldingXinput[INPUT_TRIGGER_RIGHT] = false;
		if (bHoldingXinput[INPUT_JUMP] && !(Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A)) bHoldingXinput[INPUT_JUMP] = false;
		if (bHoldingXinput[INPUT_DEBUGAIM_ACCEPT] && !(Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A)) bHoldingXinput[INPUT_DEBUGAIM_ACCEPT] = false;
		if (bHoldingXinput[INPUT_JUMP] && !(Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A)) bHoldingXinput[INPUT_DEBUGAIM] = false;
		if (bHoldingXinput[INPUT_PAUSE] && !(Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_START)) bHoldingXinput[INPUT_PAUSE] = false;

	}
	else {
		if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_Y) bHoldingXinput[INPUT_ATTACK] = true;
		if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) bHoldingXinput[INPUT_TRIGGER_LEFT] = true;
		if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) bHoldingXinput[INPUT_TRIGGER_RIGHT] = true;
		if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A) bHoldingXinput[INPUT_JUMP] = true;
		if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A) bHoldingXinput[INPUT_DEBUGAIM_ACCEPT] = true;
		if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A) bHoldingXinput[INPUT_DEBUGAIM] = true;
		if (Player1->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_START) bHoldingXinput[INPUT_PAUSE] = true;
	}
}

//*****************************************************************************
//VibrateXinput�֐�
//XBOX�̃R���g���[���[�ɗh�点��
//�����Ffloat, float, int
//�߁Fvoid
//*****************************************************************************
void VibrateXinput(float vivbrationL, float VibrationR, int FrameNumbers)
{
	if (!bXinputConnected)
		return;
	Player1->Vibrate(vivbrationL, VibrationR, FrameNumbers);
}
