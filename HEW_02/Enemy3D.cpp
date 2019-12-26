#include "Enemy3D.h"
#include "SceneGame.h"
#include "Player3D.h"
#define ONI_MODEL_PATH "data/model/Oni.fbx"
#define CANCEL_GRAVITY_FRAMES 30
SceneGame* pGame;
enum ONI_ANIMATION
{
	ONI_IDLE=1,
	ONI_WALKING,
	ONI_DAMAGEDA,
	ONI_DAMAGEDB, 
	ONI_SENDUP,
	ONI_FALLING,
	ONI_MAX
};
enum ENEMY_STATES
{
	ENEMY_IDLE = 0,
	ENEMY_DAMAGED,
	ENEMY_SENDUP,
	ENEMY_FALLING,
	MAX_ENEMY_STATES
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
	nLastPlayerAttack = -1;
	nDirection == RIGHT_DIR;
	nCancelGravityFrames = 0;
	pCurrentFloor = nullptr;
	pPlayerPointer = nullptr;
	pGame = nullptr;
	bUseGravity = false;
	nState = ENEMY_IDLE;
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
#if USE_IN_RENDERZONE
	if (!(GetMainCamera()->IsOnRenderZone(GetHitBox())))
		return;
#endif
	if(!pPlayerPointer)
		pPlayerPointer = GetMainPlayer();
	if (bUseGravity)
		GravityControl();
	if (!pPlayerPointer)
		return;
	Player3D* pPlayer = (Player3D*)pPlayerPointer;
	if (pPlayer->IsDebugAimOn())
		return;
	if (nDirection == LEFT_DIR)
		pModel->SetRotationY(-90);
	else
		pModel->SetRotationY(90);
	RegularCollisionWithPlayer();
	OniStatesControl();
}

void Enemy3D::OniStatesControl()
{
	DamageControl();
	switch (nState)
	{
	case ENEMY_IDLE:
		pModel->SwitchAnimation(ONI_IDLE);
		break;
	case ENEMY_DAMAGED:
		if (pModel->GetLoops() > 0)
			nState = ENEMY_IDLE;
		break;
	case ENEMY_SENDUP:
		if (nCancelGravityFrames==0)
			nState = ENEMY_FALLING;
		break;
	case ENEMY_FALLING:
		pModel->SwitchAnimation(ONI_FALLING);
		if(pCurrentFloor)
			nState = ENEMY_IDLE;
		break;
	default:
		break;
	}
}

void Enemy3D::DamageControl()
{
	Player3D* pPlayer = (Player3D*)pPlayerPointer;
	PLAYER_ATTACK_MOVE* pPlayerAttack = nullptr;
	SceneGame* pGame = GetCurrentGame();
	Hitbox3D AttackHitbox;
	int nPlayerDirection = pPlayer->GetDirection();
	if (IsInCollision3D(pPlayer->GetHitBox(HB_ATTACK), GetHitBox()))
	{
		pPlayerAttack = pPlayer->GetPlayerAttack();
		AttackHitbox = pPlayer->GetHitBox(HB_ATTACK);
	}
	else {
		return;
	}
	if (!pPlayerAttack)
		return;
	if (!pGame)
		return;
	nState = ENEMY_DAMAGED;
	GameObject3D* pWall = nullptr;
	nDirection = -1*pPlayer->GetDirection();
	pGame->ZoomPause(60, 30, 3, false, true);
	switch (pPlayerAttack->Animation)
	{
	case NINJA_UPPER_SLASH:
		pModel->SwitchAnimationSpeed(2);
		Position.x = AttackHitbox.x;
		Position.y = AttackHitbox.y-20;
		fYForce = 0;
		nCancelGravityFrames = CANCEL_GRAVITY_FRAMES;
		if (pModel->GetAnimation() == ONI_SENDUP) {
			if (pModel->GetCurrentFrame() >= 609) {
				pModel->SetFrame(593);
				pModel->SwitchAnimationSpeed(0.5f);
			}
		}
		else {
			pModel->SwitchAnimation(ONI_SENDUP);
		}
		nState = ENEMY_SENDUP;
		pCurrentFloor = nullptr;
		break;
	case NINJA_AIR_DOWN:
		Position.x = pPlayer->GetPosition().x+(-5* nDirection);
		Position.y = pPlayer->GetPosition().y;
		GetMainCamera()->ShakeCamera({ 2.95f,2.95f,2.75f }, 25, 15);
		nState = ENEMY_FALLING;
		break;
	default:
		pModel->SwitchAnimationSpeed(5);
		Position.x = AttackHitbox.x;
		GetMainCamera()->ShakeCamera({ 0.85f,0.85f,0.75f }, 25, 10);
		if (!(pPlayer->GetFloor()) && pPlayer->GetState()!=PLAYER_TELEPORTING) {
			pPlayer->TranslateX(1.5f* nPlayerDirection);
			if(!pCurrentFloor)
				Position.y = AttackHitbox.y - 20;
			nCancelGravityFrames = CANCEL_GRAVITY_FRAMES;
		}
		pWall = pGame->GetWalls()->CheckCollision(GetHitBox());
		if (pWall) {
			while (IsInCollision3D(GetHitBox(), pWall->GetHitBox()))
			{
				pPlayer->TranslateX(1 * -nPlayerDirection);
				Position.x -= 1;
			}
		}
		if (nLastPlayerAttack != pPlayerAttack->Animation)
		{
			if (pModel->GetAnimation() == ONI_DAMAGEDA || pModel->GetAnimation() == ONI_DAMAGEDB) {
				if (bUseDamageA && pModel->GetCurrentFrame() > 538)
				{
					pModel->SwitchAnimation(ONI_DAMAGEDA);
					bUseDamageA = false;
				}
				else if (!bUseDamageA && pModel->GetCurrentFrame() > 382)
				{
					pModel->SwitchAnimation(ONI_DAMAGEDB);
					bUseDamageA = true;
				}
			}
			else {
				if (bUseDamageA)
				{
					pModel->SwitchAnimation(ONI_DAMAGEDA);
					bUseDamageA = false;
				}
				else if (!bUseDamageA)
				{
					pModel->SwitchAnimation(ONI_DAMAGEDB);
					bUseDamageA = true;
				}
			}
		}
		
		fYForce = 0;
		break;
	}
}

void Enemy3D::RegularCollisionWithPlayer()
{
	Player3D* pPlayer = (Player3D*)pPlayerPointer;
	int nPlayerDirection = pPlayer->GetDirection();
	if (IsInCollision3D(pPlayer->GetHitBox(HB_BODY), GetHitBox()) && pPlayer->GetFloor())
	{
		Position.x += 0.5f * nPlayerDirection;
		if (nPlayerDirection == LEFT_DIR) {
			while ((pPlayer->GetState() == PLAYER_WALKING && IsInCollision3D(pPlayer->GetHitBox(HB_BODY), GetHitBox())))//|| (!(pPlayer->GetFloor()) && IsInCollision3D(pPlayer->GetHitBox(HB_FEET), GetHitBox())))
				pPlayer->TranslateX(1);
		}
		else {
			while (pPlayer->GetState() == PLAYER_WALKING && IsInCollision3D(pPlayer->GetHitBox(HB_BODY), GetHitBox()))//|| (!(pPlayer->GetFloor()) && IsInCollision3D(pPlayer->GetHitBox(HB_FEET), GetHitBox())))
				pPlayer->TranslateX(-1);
		}
	}
	if (IsInCollision3D(pPlayer->GetHitBox(HB_BODY), GetHitBox()) && pPlayer->PlayerIsFalling() && nState!=ENEMY_FALLING)
	{
		pPlayer->TranslateX((-0.05f * nPlayerDirection));
		pPlayer->SetYForce(0);
		Position.x += 2 * nPlayerDirection;
	}
}

void Enemy3D::GravityControl()
{
	if (nCancelGravityFrames > 0)
	{
		nCancelGravityFrames--;
		return;
	}
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
#if USE_IN_RENDERZONE
	if (!bIgnoreRenderingZone) {
		if (!(GetMainCamera()->IsOnRenderZone(GetHitBox())))
			return;
	}
#endif
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
