#include "Enemy3D.h"
#include "SceneGame.h"
#include "Player3D.h"
#include "C_Ui.h"
#define RED_ONI_MODEL_PATH "data/model/RedOni.fbx"
#define GREEN_ONI_MODEL_PATH "data/model/GreenOni.fbx"
#define BLUE_ONI_MODEL_PATH "data/model/BlueOni.fbx"
#define YELLOW_ONI_MODEL_PATH "data/model/YellowOni.fbx"
#define CANCEL_GRAVITY_FRAMES 30
#define DETECTED_SECONDS 16
SceneGame* pGame;
enum ONI_ANIMATION
{
	ONI_IDLE=1,
	ONI_WALKING,
	ONI_DAMAGEDA,
	ONI_DAMAGEDB, 
	ONI_SENDUP,
	ONI_FALLING,
	ONI_STANDINGUP,
	ONI_PUNCHA,
	ONI_SENDOFF,
	ONI_KICK,
	ONI_KICKJUMP,
	ONI_PUNCHB,
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
	nDamageAgainstPlayer = 0;
	fSendOffAcceleration = 0;
	nFramesSendOff = 0;
	nLastPlayerAttack = -1;
	nDirection == RIGHT_DIR;
	nCancelGravityFrames = 0;
	nPlayerAttackName = -1;
	pCurrentFloor = nullptr;
	pPlayerPointer = nullptr;
	pGame = nullptr;
	bUseGravity = false;
	bUse = true;
	nState = ENEMY_IDLE;
	fSpeed = 1;
	nDelayCounter = nDetectedFrames = 0;
	nDelayFramesBeforeAttack = 0;
	switch (nEnemyType)
	{
	case TYPE_ONI_A:
		bUnlit = true;
		bUseGravity = true;
		nHP = 100;
		InitModel(YELLOW_ONI_MODEL_PATH);
		pModel->SetScale({ 0.75f,0.75f,0.75f });
		pModel->SetPositionZ(-10);
		pModel->SwitchAnimation(ONI_IDLE);
		fSpeed = 0.4f;
		hitbox = { 0,22.5f,0,5,13,10 };
		hbAttack = { 10,22.5f,0,10,13,10 };
		nDelayFramesBeforeAttack = 20;
		nMinAttackFrame = 917;
		nMaxAttackFrame = 939;
		nDamageAgainstPlayer = 10;
		nAnimations[ENEMY_IDLE] = ONI_IDLE;
		nAnimations[ENEMY_DAMAGED] = ONI_DAMAGEDA;
		nAnimations[ENEMY_DAMAGEDALT] = ONI_DAMAGEDB;
		nAnimations[ENEMY_FALLING] = ONI_FALLING;
		nAnimations[ENEMY_MOVING] = ONI_WALKING;
		nAnimations[ENEMY_ATTACKING] = ONI_PUNCHA;
		nAnimations[ENEMY_SENDUP] = ONI_SENDUP;
		nAnimations[ENEMY_SENDOFF] = ONI_SENDOFF;

		fAnimationSpeeds[ENEMY_IDLE] = 2;
		fAnimationSpeeds[ENEMY_DAMAGED] = 5;
		fAnimationSpeeds[ENEMY_DAMAGEDALT] = 5;
		fAnimationSpeeds[ENEMY_FALLING] = 2;
		fAnimationSpeeds[ENEMY_MOVING] = 0.5f * 4;
		fAnimationSpeeds[ENEMY_ATTACKING] = 2;
		fAnimationSpeeds[ENEMY_SENDUP] = 2;
		fAnimationSpeeds[ENEMY_SENDOFF] = 3;

		nTopSendOffFrame = 609;
		nMidSendOffFrame = 593;
		break;
	case TYPE_ONI_B:
		bUnlit = true;
		bUseGravity = true;
		nHP = 135;
		InitModel(BLUE_ONI_MODEL_PATH);
		pModel->SetScale({ 0.75f,0.75f,0.75f });
		pModel->SetPositionZ(-10);
		pModel->SwitchAnimation(ONI_IDLE);
		fSpeed = 0.6f;
		hitbox = { 0,22.5f,0,5,13,10 };
		hbAttack = { 10,22.5f,0,10,13,10 };
		nDelayFramesBeforeAttack = 20;
		nMinAttackFrame = 1502;
		nMaxAttackFrame = 1527;
		nDamageAgainstPlayer = 17;
		nAnimations[ENEMY_IDLE] = ONI_IDLE;
		nAnimations[ENEMY_DAMAGED] = ONI_DAMAGEDA;
		nAnimations[ENEMY_DAMAGEDALT] = ONI_DAMAGEDB;
		nAnimations[ENEMY_FALLING] = ONI_FALLING;
		nAnimations[ENEMY_MOVING] = ONI_WALKING;
		nAnimations[ENEMY_ATTACKING] = ONI_PUNCHB;
		nAnimations[ENEMY_SENDUP] = ONI_SENDUP;
		nAnimations[ENEMY_SENDOFF] = ONI_SENDOFF;

		fAnimationSpeeds[ENEMY_IDLE] = 2;
		fAnimationSpeeds[ENEMY_DAMAGED] = 5;
		fAnimationSpeeds[ENEMY_DAMAGEDALT] = 5;
		fAnimationSpeeds[ENEMY_FALLING] = 2;
		fAnimationSpeeds[ENEMY_MOVING] = 0.5f * 4;
		fAnimationSpeeds[ENEMY_ATTACKING] = 2;
		fAnimationSpeeds[ENEMY_SENDUP] = 2;
		fAnimationSpeeds[ENEMY_SENDOFF] = 3;

		nTopSendOffFrame = 609;
		nMidSendOffFrame = 593;
		break;
	case TYPE_ONI_C:
		bUnlit = true;
		bUseGravity = true;
		nHP = 170;
		InitModel(GREEN_ONI_MODEL_PATH);
		pModel->SetScale({ 0.75f,0.75f,0.75f });
		pModel->SetPositionZ(-10);
		pModel->SwitchAnimation(ONI_IDLE);
		fSpeed = 0.8f;
		hitbox = { 0,22.5f,0,5,13,10 };
		hbAttack = { 10,22.5f,0,10,13,10 };
		nDelayFramesBeforeAttack = 20;
		nMinAttackFrame = 1214;
		nMaxAttackFrame = 1232;
		nDamageAgainstPlayer = 25;
		nAnimations[ENEMY_IDLE] = ONI_IDLE;
		nAnimations[ENEMY_DAMAGED] = ONI_DAMAGEDA;
		nAnimations[ENEMY_DAMAGEDALT] = ONI_DAMAGEDB;
		nAnimations[ENEMY_FALLING] = ONI_FALLING;
		nAnimations[ENEMY_MOVING] = ONI_WALKING;
		nAnimations[ENEMY_ATTACKING] = ONI_KICK;
		nAnimations[ENEMY_SENDUP] = ONI_SENDUP;
		nAnimations[ENEMY_SENDOFF] = ONI_SENDOFF;

		fAnimationSpeeds[ENEMY_IDLE] = 2;
		fAnimationSpeeds[ENEMY_DAMAGED] = 5;
		fAnimationSpeeds[ENEMY_DAMAGEDALT] = 5;
		fAnimationSpeeds[ENEMY_FALLING] = 2;
		fAnimationSpeeds[ENEMY_MOVING] = 0.5f * 4;
		fAnimationSpeeds[ENEMY_ATTACKING] = 2;
		fAnimationSpeeds[ENEMY_SENDUP] = 2;
		fAnimationSpeeds[ENEMY_SENDOFF] = 3;

		nTopSendOffFrame = 609;
		nMidSendOffFrame = 593;
		break;
	case TYPE_ONI_D:
		bUnlit = true;
		bUseGravity = true;
		nHP = 200;
		InitModel(RED_ONI_MODEL_PATH);
		pModel->SetScale({ 0.75f,0.75f,0.75f });
		pModel->SetPositionZ(-10);
		pModel->SwitchAnimation(ONI_IDLE);
		fSpeed = 1.0f;
		hitbox = { 0,22.5f,0,5,13,10 };
		hbAttack = { 10,22.5f,0,10,13,10 };
		nDelayFramesBeforeAttack = 20;
		nMinAttackFrame = 1369;
		nMaxAttackFrame = 1384;
		nDamageAgainstPlayer = 30;
		nAnimations[ENEMY_IDLE] = ONI_IDLE;
		nAnimations[ENEMY_DAMAGED] = ONI_DAMAGEDA;
		nAnimations[ENEMY_DAMAGEDALT] = ONI_DAMAGEDB;
		nAnimations[ENEMY_FALLING] = ONI_FALLING;
		nAnimations[ENEMY_MOVING] = ONI_WALKING;
		nAnimations[ENEMY_ATTACKING] = ONI_KICKJUMP;
		nAnimations[ENEMY_SENDUP] = ONI_SENDUP;
		nAnimations[ENEMY_SENDOFF] = ONI_SENDOFF;

		fAnimationSpeeds[ENEMY_IDLE] = 2;
		fAnimationSpeeds[ENEMY_DAMAGED] = 5;
		fAnimationSpeeds[ENEMY_DAMAGEDALT] = 5;
		fAnimationSpeeds[ENEMY_FALLING] = 2;
		fAnimationSpeeds[ENEMY_MOVING] = 0.5f * 4;
		fAnimationSpeeds[ENEMY_ATTACKING] = 2;
		fAnimationSpeeds[ENEMY_SENDUP] = 2;
		fAnimationSpeeds[ENEMY_SENDOFF] = 3;

		nTopSendOffFrame = 609;
		nMidSendOffFrame = 593;
		break;
	default:
		break;
	}
}

void Enemy3D::Update()
{
	if (!bUse)
		return;
	if (!pPlayerPointer)
		pPlayerPointer = GetMainPlayer();
	if (!pPlayerPointer)
		return;
	Player3D* pPlayer = (Player3D*)pPlayerPointer;
	if (pPlayer->IsDebugAimOn())
		return;
	if (nState == ENEMY_DEAD)
	{
		GetMainCamera()->SetFocalPoint(GetMainPlayer());
		pModel->SwitchAnimation(nAnimations[ENEMY_SENDOFF]);
		fSendOffAcceleration += 2.0f;
		Position.x -= fSendOffAcceleration * nDirection;
		Position.y += fSendOffAcceleration;
		if (!(GetMainCamera()->IsOnRenderZone(GetHitBox()))) {
			GetMainCamera()->CancelShake();
			GetMainCamera()->ShakeCamera({ 3.95f,3.95f,2.75f }, 30, 15);
			bUse = false;
		}
		return;
	}
	if (bUseGravity)
		GravityControl();
	Go_List* Spikes = GetCurrentGame()->GetSpikes();
	if (!Spikes)
		return;
	GameObject3D* spikeCol = Spikes->CheckCollision(GetHitBox());
	if (spikeCol && nState != ENEMY_DEAD)
	{
		pGame->CancelZoom();
		pModel->SwitchAnimationSpeed(0.25f);
		pPlayer->SwitchAnimationSpeed(0.25f);
		pModel->SwitchAnimation(nAnimations[ENEMY_SENDOFF]);
		pGame->ZoomPause(70, 60, 3, true, true);
		GetMainCamera()->SetFocalPoint(this);
		nState = ENEMY_DEAD;
		bUseGravity = false;
		return;
	}
	GameObject3D::Update();
#if USE_IN_RENDERZONE
	if (!(GetMainCamera()->IsOnRenderZone(GetHitBox())) && nState != ENEMY_DEAD) {
		nState = ENEMY_IDLE;
		return;
	}
#endif
	if (pPlayer->IsDebugAimOn())
		return;
	if (nDirection == LEFT_DIR)
		pModel->SetRotationY(-90);
	else
		pModel->SetRotationY(90);
	if (nPlayerAttackName!=-1)
	{
		if (pPlayer->GetState() != PLAYER_ATTACKING)
			nPlayerAttackName = -1;
	}
;	RegularCollisionWithPlayer();
	EnemyStatesControl();

}

void Enemy3D::EnemyStatesControl()
{
	Player3D* pPlayer = (Player3D*)pPlayerPointer;
	GameObject3D* pWall = nullptr;
	DamageControl();
	switch (nState)
	{
	case ENEMY_IDLE:
		pModel->SwitchAnimationSpeed(fAnimationSpeeds[ENEMY_IDLE]);
		pModel->SwitchAnimation(nAnimations[ENEMY_IDLE]);
		if (pPlayer->GetFloor() == pCurrentFloor && pPlayer->GetFloor()!=nullptr) {
			nState = ENEMY_MOVING;
			nDetectedFrames = 60 * DETECTED_SECONDS;
		}
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
		pModel->SwitchAnimationSpeed(fAnimationSpeeds[ENEMY_FALLING]);
		pModel->SwitchAnimation(nAnimations[ENEMY_FALLING]);
		if(pCurrentFloor)
			nState = ENEMY_IDLE;
		break;
	case ENEMY_MOVING:
		EnemyMovingControl();
		break;
	case ENEMY_ATTACKING:
		if (++nDelayCounter < nDelayFramesBeforeAttack) {
			pModel->SwitchAnimationSpeed(fAnimationSpeeds[ENEMY_ATTACKING]);
			pModel->SwitchAnimation(nAnimations[ENEMY_IDLE]);
			return;
		}
		if (pModel->GetCurrentFrame() > nMinAttackFrame && pModel->GetCurrentFrame() < nMaxAttackFrame) {
			if (IsInCollision3D(pPlayer->GetHitBox(HB_BODY), GetAttackHitbox()))
			{
				pPlayer->SetDamage(nDamageAgainstPlayer);
			}
		}
		pModel->SwitchAnimationSpeed(fAnimationSpeeds[ENEMY_ATTACKING]);
		pModel->SwitchAnimation(nAnimations[ENEMY_ATTACKING]);
		if (pModel->GetLoops() > 0)
			nState = ENEMY_IDLE;
		//printf("a");
		break;
	case ENEMY_SENDOFF:
		if (pGame)
			pWall = pGame->GetWalls()->CheckCollision(GetHitBox());
		if (--nFramesSendOff<=0 || pWall)
		{
			if (pWall)
			{
				if (fSendOffAcceleration == 0)
					fSendOffAcceleration = 1.0f;
				while(IsInCollision3D(pWall->GetHitBox(), GetHitBox()))
					Position.x += fSendOffAcceleration * nDirection;
			}
			nState = ENEMY_IDLE;
			fSendOffAcceleration = 0.0f;
			break;
		}
		pModel->SwitchAnimationSpeed(fAnimationSpeeds[ENEMY_SENDOFF]);
		pModel->SwitchAnimation(nAnimations[ENEMY_SENDOFF]);
		fSendOffAcceleration += 1.0f;
		Position.x -= fSendOffAcceleration * nDirection;
		break;
	default:
		break;
	}
}

void Enemy3D::EnemyMovingControl()
{
	Player3D* pPlayer = (Player3D*)pPlayerPointer;
	GameObject3D* pWall = nullptr;
	if (pGame)
		pWall = pGame->GetWalls()->CheckCollision(GetHitBox());
	if (!pPlayer) {
		nState = ENEMY_IDLE;
		return;
	}
	if (IsInCollision3D(GetHitBox(), pPlayer->GetHitBox(HB_LEFT)) || IsInCollision3D(GetHitBox(), pPlayer->GetHitBox(HB_RIGHT))) {
		nState = ENEMY_ATTACKING;
		nDelayCounter = 0;
		return;
	}
	if(--nDetectedFrames<=0 && pPlayer->GetFloor() != pCurrentFloor){
		nState = ENEMY_IDLE;
		return;
	}
	if(pPlayer->GetFloor() == pCurrentFloor)
		nDetectedFrames = DETECTED_SECONDS * 5;
	pModel->SwitchAnimation(nAnimations[ENEMY_MOVING]);
	XMFLOAT3 PlayerPos = pPlayer->GetPosition();
	if (!pWall) {
		if (PlayerPos.x < Position.x)
			nDirection = LEFT_DIR;
		else if (PlayerPos.x > Position.x)
			nDirection = RIGHT_DIR;
	}


	Position.x += nDirection * fSpeed;
	if (pWall) {
		if(!((pPlayer->GetPosition().x < pWall->GetHitBox().x && nDirection == LEFT_DIR)
			|| (pPlayer->GetPosition().x > pWall->GetHitBox().x+ pWall->GetHitBox().SizeX && nDirection == RIGHT_DIR)))
			Position.x -= nDirection * fSpeed;
		pModel->SwitchAnimation(nAnimations[ENEMY_IDLE]);
	}
	pModel->SwitchAnimationSpeed(fAnimationSpeeds[ENEMY_MOVING]);
}

void Enemy3D::DamageControl()
{
	Player3D* pPlayer = (Player3D*)pPlayerPointer;
	PLAYER_ATTACK_MOVE* pPlayerAttack = nullptr;
	SceneGame* pGame = GetCurrentGame();
	Hitbox3D AttackHitbox;
	int nPlayerDirection = pPlayer->GetDirection();
	bool bDoDamage = false;
	if (IsInCollision3D(pPlayer->GetHitBox(HB_ATTACK), GetHitBox()))
	{
		pPlayerAttack = pPlayer->GetPlayerAttack();
		AttackHitbox = pPlayer->GetHitBox(HB_ATTACK);
		if (pPlayerAttack) {
			if (pPlayerAttack->Animation != nPlayerAttackName) {
				nPlayerAttackName = pPlayerAttack->Animation;
				bDoDamage = true;
			}
		}
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
	SetFramesForZoomUse(35);
	switch (pPlayerAttack->Animation)
	{
	case NINJA_UPPER_SLASH:
		pModel->SwitchAnimationSpeed(fAnimationSpeeds[ENEMY_SENDUP]);
		Position.x = AttackHitbox.x;
		Position.y = AttackHitbox.y-20;
		fYForce = 0;
		if (bDoDamage)
			nHP -= 10;
		nCancelGravityFrames = CANCEL_GRAVITY_FRAMES;
		if (pModel->GetAnimation() == nAnimations[ENEMY_SENDUP]) {
			if (pModel->GetCurrentFrame() >= nTopSendOffFrame) {
				pModel->SetFrame(nMidSendOffFrame);
				pModel->SwitchAnimationSpeed(0.5f);
			}
		}
		else {
			pModel->SwitchAnimation(nAnimations[ENEMY_SENDUP]);
		}
		nState = ENEMY_SENDUP;
		pCurrentFloor = nullptr;
		break;
	case NINJA_ATTACK_COMBOAIR_D: case NINJA_ATTACK_COMBO_E:
		GetMainCamera()->ShakeCamera({ 2.95f,2.95f,2.75f }, 25, 15);
		nState = ENEMY_SENDOFF;
		if(!pCurrentFloor)
			nState = ENEMY_FALLING;
		nFramesSendOff = 5;
		if (bDoDamage)
			nHP -= 10;
		break;
	case NINJA_AIR_DOWN:
		Position.x = pPlayer->GetPosition().x+(-5* nDirection);
		Position.y = pPlayer->GetPosition().y;
		GetMainCamera()->ShakeCamera({ 2.95f,2.95f,2.75f }, 25, 15);
		nState = ENEMY_FALLING;
		if (bDoDamage)
			nHP -= 10;
		break;
	default:
		pModel->SwitchAnimationSpeed(fAnimationSpeeds[ENEMY_DAMAGED]);
		if (bDoDamage)
			nHP -= 5;
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
				Position.x += 1*nDirection;
			}
		}
		if (nLastPlayerAttack != pPlayerAttack->Animation)
		{
			if (pModel->GetAnimation() == nAnimations[ENEMY_DAMAGED] || pModel->GetAnimation() == nAnimations[ENEMY_DAMAGEDALT]) {
				if (bUseDamageA && pModel->GetCurrentFrame() > 538)
				{
					pModel->SwitchAnimation(nAnimations[ENEMY_DAMAGED]);
					bUseDamageA = false;
				}
				else if (!bUseDamageA && pModel->GetCurrentFrame() > 382)
				{
					pModel->SwitchAnimation(nAnimations[ENEMY_DAMAGEDALT]);
					bUseDamageA = true;
				}
			}
			else {
				if (bUseDamageA)
				{
					pModel->SwitchAnimation(nAnimations[ENEMY_DAMAGED]);
					bUseDamageA = false;
				}
				else if (!bUseDamageA)
				{
					pModel->SwitchAnimation(nAnimations[ENEMY_DAMAGEDALT]);
					bUseDamageA = true;
				}
			}
		}
		
		fYForce = 0;
		break;
	}
	if (nHP <= 0)
	{
		pGame->CancelZoom();
		pModel->SwitchAnimationSpeed(0.25f);
		pPlayer->SwitchAnimationSpeed(0.25f);
		pModel->SwitchAnimation(nAnimations[ENEMY_SENDOFF]);
		pGame->ZoomPause(70, 60, 3, true, true);
		GetMainCamera()->SetFocalPoint(this);
		nState = ENEMY_DEAD;
		bUseGravity = false;
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
		if (fYForce > 7)
			fYForce = 7;
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
	if (!bUse)
		return;
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

Hitbox3D Enemy3D::GetAttackHitbox()
{
	return { (hbAttack.x*nDirection) + Position.x, hbAttack.y + Position.y, hbAttack.z + Position.z, 
		hbAttack.SizeX, hbAttack.SizeY, hbAttack.SizeZ };
}
