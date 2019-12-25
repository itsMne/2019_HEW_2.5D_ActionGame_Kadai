#include "Enemy3D.h"
#include "SceneGame.h"
#include "Player3D.h"
#define ONI_MODEL_PATH "data/model/Oni.fbx"
SceneGame* pGame;
enum ONI_ANIMATION
{
	ONI_IDLE=1,
	ONI_MAX
};
Enemy3D::Enemy3D(int enemyType) :GameObject3D()
{
	nEnemyType = enemyType;
	Init();
}


Enemy3D::~Enemy3D()
{
	Uninit();
	pGame = nullptr;
}

void Enemy3D::Init()
{
	nType = GO_ENEMY;
	fYForce = 0;
	pCurrentFloor = nullptr;
	pGame = nullptr;
	bUseGravity = false;
	switch (nEnemyType)
	{
	case TYPE_ONI:
		bUnlit = true;
		bUseGravity = true;
		InitModel(ONI_MODEL_PATH);
		pModel->SetScale({ 0.75f,0.75f,0.75f });
		pModel->SetPositionZ(-10);
		pModel->SwitchAnimation(ONI_IDLE);
		hitbox = { 0,22.5f,0,5,13,10 };
		break;
	default:
		break;
	}
}

void Enemy3D::Update()
{
	GameObject3D::Update();
	Player3D* pPlayer = GetMainPlayer();
	if (bUseGravity)
		GravityControl();
	if (!pPlayer)
		return;
	if (pPlayer->IsDebugAimOn())
		return;
	int nPlayerDirection = pPlayer->GetDirection();
	if (IsInCollision3D(pPlayer->GetHitBox(HB_BODY), GetHitBox()) && pPlayer->GetFloor())
	{
		Position.x+=0.5f * nPlayerDirection;
		if (nPlayerDirection == LEFT_DIR) {
			while ((pPlayer->GetState() == PLAYER_WALKING && IsInCollision3D(pPlayer->GetHitBox(HB_BODY), GetHitBox())))//|| (!(pPlayer->GetFloor()) && IsInCollision3D(pPlayer->GetHitBox(HB_FEET), GetHitBox())))
				pPlayer->TranslateX(1);
		}
		else {
			while (pPlayer->GetState() == PLAYER_WALKING && IsInCollision3D(pPlayer->GetHitBox(HB_BODY), GetHitBox()))//|| (!(pPlayer->GetFloor()) && IsInCollision3D(pPlayer->GetHitBox(HB_FEET), GetHitBox())))
				pPlayer->TranslateX(-1);
		}
	}
	if (IsInCollision3D(pPlayer->GetHitBox(HB_BODY), GetHitBox()) && pPlayer->PlayerIsFalling())
	{
		pPlayer->TranslateX((-0.05f * nPlayerDirection));
		pPlayer->SetYForce(0);
		Position.x += 2 * nPlayerDirection;
	}

}

void Enemy3D::GravityControl()
{
	if (!pGame) {
		pGame = GetCurrentGame();
		return;
	}
	if (!pCurrentFloor)
	{
		fYForce += GRAVITY_FORCE;
		Position.y -= fYForce;

		Go_List* Fields = pGame->GetFields();
		if (!Fields)
			return;
		pCurrentFloor = (Field3D*)Fields->CheckCollision(GetHitBox());
		if (pCurrentFloor) {
			while (IsInCollision3D(pCurrentFloor->GetHitBox(), GetHitBox()))
				Position.y += 0.001f;
			Position.y -= 0.001f;
		}
	}
	else {
		fYForce = 0;
		if (!(IsInCollision3D(pCurrentFloor->GetHitBox(), GetHitBox())))
			pCurrentFloor = nullptr;
	}
}

void Enemy3D::Draw()
{
	GameObject3D::Draw();
}

void Enemy3D::Uninit()
{
	GameObject3D::Uninit();
}

int Enemy3D::GetEnemyType()
{
	return nEnemyType;
}
