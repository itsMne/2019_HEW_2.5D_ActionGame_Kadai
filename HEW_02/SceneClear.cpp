#include "SceneClear.h"
#include "InputManager.h"


SceneClear::SceneClear()
{
	Clear = new C_Ui("data/texture/clear.jpg", UI_CLEAR);
}


SceneClear::~SceneClear()
{
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
}
