#include "SceneClear.h"
#include "Sound.h"
#include "InputManager.h"


SceneClear::SceneClear()
{
	Clear = new C_Ui("data/texture/clear.jpg", UI_CLEAR);
	Score = new C_Ui("data/texture/number.png", UI_RESULT_SCORE); 
	PlaySoundGame(SOUND_LABEL_BGM_CLEAR);
}


SceneClear::~SceneClear()
{
	StopSound();
	Uninit();
}

void SceneClear::Init()
{
}

void SceneClear::Uninit()
{
	SAFE_DELETE(Clear);
}

int SceneClear::Update()
{
	Clear->Update();

	if (GetInput(INPUT_JUMP)) {
		return SCENE_RANKING;
	}

	return SCENE_CLEAR;
}

void SceneClear::Draw()
{
	SetZBuffer(false);
	Clear->Draw();

	Score->Draw();
}
