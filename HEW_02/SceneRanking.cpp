#include "SceneRanking.h"
#include "InputManager.h"


SceneRanking::SceneRanking()
{
	Ranking = new C_Ui("data/texture/ranking.jpg", UI_RANKING);
}


SceneRanking::~SceneRanking()
{
	Uninit();
}

void SceneRanking::Init()
{
}

void SceneRanking::Uninit()
{
	SAFE_DELETE(Ranking);
}

int SceneRanking::Update()
{
	Ranking->Update();

	if (GetInput(INPUT_JUMP)) {
		return SCENE_TITLE;
	}

	return SCENE_RANKING;
}

void SceneRanking::Draw()
{
	SetZBuffer(false);
	Ranking->Draw();
}
