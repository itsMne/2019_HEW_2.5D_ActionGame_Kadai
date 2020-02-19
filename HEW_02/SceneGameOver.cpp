//*****************************************************************************
// SceneGameOver.cpp
// �Q�[���I�[�o�[�̃V�[��
//*****************************************************************************
#include "SceneGameOver.h"
#include "Sound.h"
#include "InputManager.h"

//*****************************************************************************
// �R���X�g���N�^�֐�
//*****************************************************************************
SceneGameOver::SceneGameOver()
{
	GameOver = new C_Ui("data/texture/1574411424324.png", UI_GAMEOVER);
	Score = new C_Ui("data/texture/number.png", UI_RESULT_SCORE);
	PlaySoundGame(SOUND_LABEL_BGM_GAMEOVER);
}


SceneGameOver::~SceneGameOver()
{
	Uninit();
}

//*****************************************************************************
//Init�֐�
//�������֐�
//�����Fvoid
//�߁Fvoid
//*****************************************************************************
void SceneGameOver::Init()
{
	//�Ȃ�
}

//*****************************************************************************
//Uninit�֐�
//�I���֐�
//�����Fvoid
//�߁Fvoid
//*****************************************************************************
void SceneGameOver::Uninit()
{
	SAFE_DELETE(GameOver);
	SAFE_DELETE(Score);
	StopSound();
}

//*****************************************************************************
//Update�֐�
//�ύX�֐�
//�����Fvoid
//�߁Fvoid
//*****************************************************************************
int SceneGameOver::Update()
{
	GameOver->Update();

	if (GetInput(INPUT_JUMP)) {
		return SCENE_TITLE;
	}

	return SCENE_GAMEOVER;
}

//*****************************************************************************
//Draw�֐�
//�����_�����O�֐�
//�����Fvoid
//�߁Fvoid
//*****************************************************************************
void SceneGameOver::Draw()
{
	SetZBuffer(false);
	GameOver->Draw();

	Score->Draw();
}
