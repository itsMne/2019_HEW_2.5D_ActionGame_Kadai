#include "SceneTitle.h"
#include "InputManager.h"



SceneTitle::SceneTitle()
{
	Title = new C_Ui("data/texture/haikei.png", UI_TITLE);
}


SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
}

void SceneTitle::Uninit()
{
}

int SceneTitle::Update()
{
	Title->Update();

	if (GetInput(INPUT_JUMP)) {
		return SCENE_GAME;
	}

	return SCENE_TITLE;
}

void SceneTitle::Draw()
{
	SetZBuffer(false);
	Title->Draw();
}

