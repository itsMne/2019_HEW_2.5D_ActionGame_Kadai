#include "SceneGameOver.h"
#include "Sound.h"
#include "InputManager.h"


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

void SceneGameOver::Init()
{
}

void SceneGameOver::Uninit()
{
	SAFE_DELETE(GameOver);
	SAFE_DELETE(Score);
	StopSound();
}

int SceneGameOver::Update()
{
	GameOver->Update();

	if (GetInput(INPUT_JUMP)) {
		return SCENE_TITLE;
	}

	return SCENE_GAMEOVER;
}

void SceneGameOver::Draw()
{
	SetZBuffer(false);
	GameOver->Draw();

	Score->Draw();
}
