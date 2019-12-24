#include "Enemy3D.h"
#include "Player3D.h"
#define ONI_MODEL_PATH "data/model/Oni.fbx"


Enemy3D::Enemy3D(int enemyType) :GameObject3D()
{
	nEnemyType = enemyType;
	Init();
}


Enemy3D::~Enemy3D()
{
	Uninit();
}

void Enemy3D::Init()
{
	nType = GO_ENEMY;
	switch (nEnemyType)
	{
	case TYPE_ONI:
		bUnlit = true;
		InitModel(ONI_MODEL_PATH);
		pModel->SetScale({ 0.75f,0.75f,0.75f });
		pModel->SetPositionZ(-10);
		hitbox = { 0,0,0,5,10,10 };
		break;
	default:
		break;
	}
}

void Enemy3D::Update()
{
	GameObject3D::Update();
	Player3D* pPlayer = GetMainPlayer();
	if (!pPlayer)
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

void Enemy3D::Draw()
{
	GameObject3D::Draw();
}

void Enemy3D::Uninit()
{
	GameObject3D::Uninit();
}
