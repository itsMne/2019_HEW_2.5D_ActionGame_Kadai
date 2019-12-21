#include "Goal3D.h"
#include "Player3D.h"
#include "SceneGame.h"
#define GOAL_MODEL_PATH "data/model/Goal.fbx"


Goal3D::Goal3D(): GameObject3D()
{
	Init();
}


Goal3D::~Goal3D()
{
	Uninit();
}

void Goal3D::Init()
{
	InitModel(GOAL_MODEL_PATH);
	pModel->SetScale(0.025f);
	hitbox = {0,25,0,3,12,3};
	nType = GO_GOAL;
}

void Goal3D::Update()
{
	GameObject3D::Update();
	Player3D* pPlayer = GetMainPlayer();
	if (!pPlayer)
		return;
	if (IsInCollision3D(pPlayer->GetHitBox(HB_BODY), GetHitBox()) && !pPlayer->IsDebugAimOn())
	{
		SceneGame* pGame = GetCurrentGame();
		if (!pGame)
			return;
		pGame->SetGoalReached();
	}
}

void Goal3D::Draw()
{
	GameObject3D::Draw();
}

void Goal3D::Uninit()
{
}
