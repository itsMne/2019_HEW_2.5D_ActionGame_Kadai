#include "SceneGameOver.h"
#include "InputManager.h"


SceneGameOver::SceneGameOver()
{
	GameOver = new C_Ui("data/texture/1574411424324.png", UI_GAMEOVER);
}


SceneGameOver::~SceneGameOver()
{
}

void SceneGameOver::Init()
{
}

void SceneGameOver::Uninit()
{
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
}
