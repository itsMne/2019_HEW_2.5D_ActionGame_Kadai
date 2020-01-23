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
	GameObject3D::~GameObject3D();
	Uninit();
}

void Goal3D::Init()
{
	InitModel(GOAL_MODEL_PATH);
	pModel->SetScale(0.025f);
	pModel->SetRotationY(180);
	hitbox = {0,25,0,3,12,3};
	nType = GO_GOAL;
}

void Goal3D::Update()
{
	GameObject3D::Update();
	Player3D* pPlayer = GetMainPlayer();
#if USE_IN_RENDERZONE
	if (pPlayer) {
		if (!(GetMainCamera()->IsOnRenderZone(GetHitBox())) && pPlayer->GetState()!=PLAYER_OVER)
			return;
		if (pPlayer->IsDebugAimOn())
			return;
	}
#endif
	if (!pPlayer)
		return;
	if (IsInCollision3D(pPlayer->GetHitBox(HB_BODY), GetHitBox()))
	{
		pPlayer->SetPlayerState(PLAYER_OVER);
		//SceneGame* pGame = GetCurrentGame();
		//if (!pGame)
		//	return;
		//pGame->SetGoalReached();
	}
}

void Goal3D::Draw()
{
	Player3D* pPlayer = GetMainPlayer();
#if USE_IN_RENDERZONE
	if (pPlayer) {
		if (!(GetMainCamera()->IsOnRenderZone(GetHitBox())))
			return;
	}
#endif
	GameObject3D::Draw();
}

void Goal3D::Uninit()
{
}
