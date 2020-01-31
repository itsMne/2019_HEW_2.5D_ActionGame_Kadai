#include "Enemy3D.h"
#include "SceneGame.h"
#include "Player3D.h"
#include "RankManager.h"
#include "InputManager.h"
#include "Sound.h"
#include "C_Ui.h"

#define CANCEL_GRAVITY_FRAMES 30
#define DETECTED_SECONDS 16
#define PAUSE_FRAMES_PER_ATTACK 6
#define UNLIT_FRAMES_PER_ATTACK 9

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

enum WARRIOR_ANIMATION
{
	WARRIOR_WALK,
	WARRIOR_IDLE,
	WARRIOR_ATTACK,
	WARRIOR_DAMAGEDA,
	WARRIOR_DAMAGEDB,
	WARRIOR_SENDUP,
	WARRIOR_SENDOFF,
	WARRIOR_FALLING,
	WARRIOR_MAX
};

enum ONI_BOSS_ANIMATION
{
	ONI_BOSS_IDLE,
	ONI_BOSS_ATTACKA,
	ONI_BOSS_ATTACKB,
	ONI_BOSS_DAMAGEA,
	ONI_BOSS_DAMAGEB,
	ONI_BOSS_DAMAGEUP,
	ONI_BOSS_DAMAGEFALL,
	ONI_BOSS_SENDOFF,
	ONI_BOSS_WALK,
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
	nEnragedFrames = nEnragedMeter = nEnragedHitCountMax = nEnragedCounter = 0;
	nUnlitFrames = 0;
	nDamageAgainstPlayer = 0;
	fSendOffAcceleration = 0;
	nStuntFrames = 0;
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
	bEndStageAfterDeath = false;
	bIsBoss = false;
	nState = ENEMY_IDLE;
	fSpeed = 1;
	nDelayCounter = nDetectedFrames = 0;
	nDelayFramesBeforeAttack = 0;
	bUnlit = false;
	nAttackHitSound = SOUND_LABEL_SE_ENEMYATTACK1;
	nSuperArmor = 0;
	RecoverySuperArmorFramesCounter=RecoverySuperArmorFrames=0;
	switch (nEnemyType)
	{
	case TYPE_ONI_A:
		bUseGravity = true;
		nHP = 100;
		InitModel(ENEMY_ONI_MODEL_YELLOW);
		pModel->SetScale({ 0.75f,0.75f,0.75f });
		pModel->SetPositionZ(-10);
		pModel->SwitchAnimation(ONI_IDLE);
		fSpeed = 0.4f;
		hitbox = { 0,22.5f,0,5,10,10 };
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
		bUseGravity = true;
		nHP = 135;
		InitModel(ENEMY_ONI_MODEL_BLUE);
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
		bUseGravity = true;
		nHP = 170;
		InitModel(ENEMY_ONI_MODEL_GREEN);
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
		bUseGravity = true;
		nHP = 200;
		InitModel(ENEMY_ONI_MODEL_RED);
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

		nEnragedHitCountMax = 15;
		nTopSendOffFrame = 609;
		nMidSendOffFrame = 593;
		nEnragedFrames = 120;
		break;
	case TYPE_WARRIOR:
		bUseGravity = true;
		nHP = 150;
		InitModel(ENEMY_WARRIOR);
		pModel->SetScale({ 1.25f,1.25f,1.25f });
		pModel->SetPositionZ(-10);
		pModel->SwitchAnimation(WARRIOR_IDLE);
		fSpeed = 1.0f;
		hitbox = { 0,22.5f,0,5,13,10 };
		hbAttack = { 10,22.5f,0,10,13,10 };
		nDelayFramesBeforeAttack = 20;
		nMinAttackFrame = 350;
		nMaxAttackFrame = 395;
		nDamageAgainstPlayer = 30;
		nAnimations[ENEMY_IDLE] = WARRIOR_IDLE;
		nAnimations[ENEMY_DAMAGED] = WARRIOR_DAMAGEDA;
		nAnimations[ENEMY_DAMAGEDALT] = WARRIOR_DAMAGEDB;
		nAnimations[ENEMY_FALLING] = WARRIOR_FALLING;
		nAnimations[ENEMY_MOVING] = WARRIOR_WALK;
		nAnimations[ENEMY_ATTACKING] = WARRIOR_ATTACK;
		nAnimations[ENEMY_SENDUP] = WARRIOR_SENDUP;
		nAnimations[ENEMY_SENDOFF] = WARRIOR_SENDOFF;

		fAnimationSpeeds[ENEMY_IDLE] = 2;
		fAnimationSpeeds[ENEMY_DAMAGED] = 5;
		fAnimationSpeeds[ENEMY_DAMAGEDALT] = 5;
		fAnimationSpeeds[ENEMY_FALLING] = 2;
		fAnimationSpeeds[ENEMY_MOVING] = 0.5f * 4;
		fAnimationSpeeds[ENEMY_ATTACKING] = 1.2f;
		fAnimationSpeeds[ENEMY_SENDUP] = 4;
		fAnimationSpeeds[ENEMY_SENDOFF] = 3;
		nEnragedHitCountMax = 5;
		nTopSendOffFrame = 828;
		nMidSendOffFrame = 897;
		nEnragedFrames = 180;
		break;
	case TYPE_BOSS_ONI:
		bIsBoss = true;
		bUseGravity = true;
		nHP = 600;
		nSuperArmorMax=nSuperArmor = 300;
		RecoverySuperArmorFrames = 60*7;
		InitModel(ENEMY_BOSS_MODEL_PATH);
		pModel->SetScale({ 0.25f,0.25f,0.25f });
		pModel->SetPositionZ(-10);
		pModel->SwitchAnimation(WARRIOR_IDLE);
		fSpeed = 1.0f;
		fSpeed = 0.15f;//DEL
		hitbox = { 0,22.5f,0,15,29,10 };
		hbAttack = { 40,22.5f,0,40,29,10 };
		nDelayFramesBeforeAttack = 30;
		nMinAttackFrame = 215;
		nMaxAttackFrame = 235;
		nDamageAgainstPlayer = 15;
		nAnimations[ENEMY_IDLE] = ONI_BOSS_IDLE;
		nAnimations[ENEMY_DAMAGED] = ONI_BOSS_DAMAGEA;
		nAnimations[ENEMY_DAMAGEDALT] = ONI_BOSS_DAMAGEB;
		nAnimations[ENEMY_FALLING] = ONI_BOSS_DAMAGEFALL;
		nAnimations[ENEMY_MOVING] = ONI_BOSS_WALK;
		nAnimations[ENEMY_ATTACKING] = ONI_BOSS_ATTACKA;
		nAnimations[ENEMY_SENDUP] = WARRIOR_SENDUP;
		nAnimations[ENEMY_SENDOFF] = WARRIOR_SENDOFF;
		fAnimationSpeeds[ENEMY_IDLE] = 2;
		fAnimationSpeeds[ENEMY_DAMAGED] = 5;
		fAnimationSpeeds[ENEMY_DAMAGEDALT] = 5;
		fAnimationSpeeds[ENEMY_FALLING] = 2;
		fAnimationSpeeds[ENEMY_MOVING] = 0.5f * 4;
		fAnimationSpeeds[ENEMY_ATTACKING] = 0.7f;
		fAnimationSpeeds[ENEMY_SENDUP] = 4;
		fAnimationSpeeds[ENEMY_SENDOFF] = 3;
		nEnragedHitCountMax = 0;
		nTopSendOffFrame = 828;
		nMidSendOffFrame = 897;
		nEnragedFrames = 180;
		bEndStageAfterDeath = true;
		break;
	default:
		break;
	}
	pModel->SwitchAnimationSpeed(fAnimationSpeeds[ENEMY_IDLE]);
	pModel->SwitchAnimation(nAnimations[ENEMY_IDLE]);
}

void Enemy3D::Update()
{
	if (nUnlitFrames > 0) {
		bUnlit = true;
		nUnlitFrames--;
	}
	else
	{
		nUnlitFrames = 0;
		bUnlit = false;
	}
	if (!bUse)
		return;
	if (!pPlayerPointer)
		pPlayerPointer = GetMainPlayer();
	if (!pPlayerPointer)
		return;
	if (nEnragedCounter > 0) {
		nEnragedCounter--;
		if (nEnragedCounter < 0)
			nEnragedCounter = 0;
	}
	Player3D* pPlayer = (Player3D*)pPlayerPointer;
	if (pPlayer->IsDebugAimOn())
		return;
	if (pPlayer->PlayerIsTransforming())
		return;
	if (pPlayer->GetState() == PLAYER_GEISHA_DODGE)
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
			if (bEndStageAfterDeath) {
				pPlayer->SetPlayerState(PLAYER_OVER);
				StopSound();
				PlaySoundGame(SOUND_LABEL_SE_END_REACHED);
			}
			PlaySoundGame(SOUND_LABEL_SE_DISSAPPEARED);
		}
		return;
	}
	if (bUseGravity)
		GravityControl();
	if (nSuperArmorMax > 0)
	{
		if (nSuperArmor <= 0 && !pPlayer->PlayerIsTransforming())
		{
			if (++RecoverySuperArmorFramesCounter > RecoverySuperArmorFrames)
			{
				nSuperArmor = nSuperArmorMax;
				RecoverySuperArmorFramesCounter = 0;

			}
		}
	}
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
		PlaySoundGame(SOUND_LABEL_SE_KO);
		bUseGravity = false;
		return;
	}
	GameObject3D::Update();
#if USE_IN_RENDERZONE
	if (!(GetMainCamera()->IsOnRenderZone(GetHitBox())) && nState != ENEMY_DEAD) {
		nState = ENEMY_IDLE;
		if (pModel)
		{
			pModel->SwitchAnimationSpeed(fAnimationSpeeds[ENEMY_IDLE]);
			pModel->SwitchAnimation(nAnimations[ENEMY_IDLE]);
		}
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
	if (nStuntFrames > 0) {
		nStuntFrames--;
		if (nStuntFrames <= 0)
			nState = ENEMY_IDLE;
	}
	switch (nState)
	{
	case ENEMY_IDLE:
		if (nStuntFrames > 0) {
			nState = ENEMY_STUNT;
			break;
		}
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
		if (nStuntFrames > 0) {
			nState = ENEMY_STUNT;
			break;
		}
		EnemyMovingControl();
		break;
	case ENEMY_STUNT:
		pModel->SwitchAnimationSpeed(fAnimationSpeeds[ENEMY_SENDUP]);
		pModel->SwitchAnimation(nAnimations[ENEMY_SENDUP]);
		break;
	case ENEMY_ATTACKING:
		if (nStuntFrames > 0) {
			nState = ENEMY_STUNT;
			break;
		}
		if (++nDelayCounter < nDelayFramesBeforeAttack) {
			pModel->SwitchAnimationSpeed(fAnimationSpeeds[ENEMY_ATTACKING]);
			pModel->SwitchAnimation(nAnimations[ENEMY_IDLE]);
			return;
		}
		if (pModel->GetCurrentFrame() > nMinAttackFrame && pModel->GetCurrentFrame() < nMaxAttackFrame) {
			if (IsInCollision3D(pPlayer->GetHitBox(HB_BODY), GetAttackHitbox()))
			{
				if (pPlayer->GetCurrentTransformation() == MODEL_GEISHA)
				{
					if (IsInCollision3D(pPlayer->GetHitBox(HB_ATTACK), GetAttackHitbox()))
					{
						nState = ENEMY_STUNT;
						PlaySoundGame(SOUND_LABEL_SE_BLOCKGEISHA);
						PlaySoundGame(SOUND_LABEL_SE_GEISHADODGED);
						VibrateXinput(65535, 65535, 30);
						nStuntFrames = 240;
						nSuperArmor = 0;
						pGame->ZoomPause(70, 120, 3, false, true);
						pGame->SetPetalsFrames(240);
						pGame->SetHitEffect();
						pPlayer->SetPlayerState(PLAYER_GEISHA_DODGE);
						pPlayer->ReduceStamina((int)(pPlayer->GetPlayerMaxMp() / 1.5f));
						AddMoveToRankMeter(GEISHA_BLOCK+100, 200);
					}
					else {
						nState = ENEMY_STUNT;
						PlaySoundGame(SOUND_LABEL_SE_GEISHADODGED);
						VibrateXinput(65535/3, 65535/3, 30);
						nStuntFrames = 120;
						nSuperArmor = 0;
						RecoverySuperArmorFramesCounter = RecoverySuperArmorFrames / 2;
						pGame->ZoomPause(70, 120, 3, false, true);
						pGame->SetPetalsFrames(120);
						pPlayer->SetPlayerState(PLAYER_GEISHA_DODGE);
						pPlayer->ReduceStamina((int)(pPlayer->GetPlayerMaxMp() / 1.5f));
						AddMoveToRankMeter(GEISHA_DODGE_RIGHT + 100, 50);
					}
					pModel->SwitchAnimationSpeed(fAnimationSpeeds[ENEMY_SENDUP]);
					pModel->SwitchAnimation(nAnimations[ENEMY_SENDUP]);
					return;
				}
				bool DamagedDeal=pPlayer->SetDamage(nDamageAgainstPlayer);
				if (DamagedDeal)
					PlaySoundGame((SOUND_LABEL)nAttackHitSound);
			}
		}
		pModel->SwitchAnimationSpeed(fAnimationSpeeds[ENEMY_ATTACKING]);
		pModel->SwitchAnimation(nAnimations[ENEMY_ATTACKING]);
		if (pModel->GetLoops() > 0)
			nState = ENEMY_IDLE;
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
		fSendOffAcceleration += 1.5f;
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
		if(pPlayer->GetPosition().x>Position.x)
			nDirection = 1;
		else
			nDirection = -1;
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
	if (pPlayerAttack->Animation == GEISHA_BLOCK)
		return;
	GameObject3D* pWall = nullptr;
	if(!bIsBoss || (bIsBoss && nSuperArmor==0))
		nDirection = -1*pPlayer->GetDirection();
	pGame->ZoomPause(60, 30, 3, false, true);
	if (bDoDamage && pPlayerAttack->Animation == SAMURAI_STINGER)
		pPlayer->ReduceStamina(2);
	SetFramesForZoomUse(35);
	if ((pCurrentFloor && nEnragedCounter != 0 && nEnragedHitCountMax!=0 && 
		pPlayerAttack->Animation!= NINJA_AIR_DOWN && pPlayerAttack->Animation != NINJA_UPPER_SLASH
		&& pPlayerAttack->Animation != SAMURAI_STINGER && !bIsBoss) || (bIsBoss && nSuperArmor>0))
	{
		if (bDoDamage) {

			if (pPlayerAttack->Animation == SAMURAI_STINGER || 
				pPlayerAttack->Animation == NINJA_ATTACK_COMBOAIR_D ||
				pPlayerAttack->Animation == NINJA_ATTACK_COMBO_E ||
				pPlayerAttack->Animation == NINJA_AIR_DOWN)
				PlaySoundGame(SOUND_LABEL_SE_BIGSLASH2);
			else
				PlaySoundGame(SOUND_LABEL_SE_NINJA_REGULARSLASH);

			AddMoveToRankMeter(NINJA_ATTACK_COMBOAIR_A, 30);
			GetMainCamera()->ShakeCamera({ 1.85f,1.85f,0.75f }, 25, 15);
			pGame->SetPauseFrames(PAUSE_FRAMES_PER_ATTACK*1.5f);
			SetUnlitForFrames(UNLIT_FRAMES_PER_ATTACK);
			nHP -= 5;
			
		}
		if (!bIsBoss) {
			Position.x = AttackHitbox.x;
			if (!(pPlayer->GetFloor()) && pPlayer->GetState() != PLAYER_TELEPORTING) {
				pPlayer->TranslateX(1.5f* nPlayerDirection);
				if (!pCurrentFloor)
					Position.y = AttackHitbox.y - 20;
				nCancelGravityFrames = CANCEL_GRAVITY_FRAMES;
			}
			pWall = pGame->GetWalls()->CheckCollision(GetHitBox());
			if (pWall) {
				while (IsInCollision3D(GetHitBox(), pWall->GetHitBox()))
				{
					pPlayer->TranslateX(1 * -nPlayerDirection);
					Position.x += 1 * nDirection;
				}
			}
		}
		if (pPlayer->GetCurrentTransformation() == MODEL_SAMURAI)
			nSuperArmor -= 10;
		else if (pPlayer->GetCurrentTransformation() == MODEL_NINJA)
			nSuperArmor -= 2;
		if (bDoDamage)
			pGame->RaiseScoreWithRank(5); 
		return;
	}

	switch (pPlayerAttack->Animation)
	{
	case NINJA_UPPER_SLASH:
		pModel->SwitchAnimationSpeed(fAnimationSpeeds[ENEMY_SENDUP]);
		if (!bIsBoss) {
			Position.x = AttackHitbox.x;
			Position.y = AttackHitbox.y - 20;
		}
		if(bIsBoss && nSuperArmor==0)
			Position.y = AttackHitbox.y - 20;
		if(nEnragedMeter>0)
			nEnragedMeter = 0;
		fYForce = 0;
		if (bDoDamage) {
			PlaySoundGame(SOUND_LABEL_SE_BIGSLASH3);
			VibrateXinput(65535, 65535, 30);
			GetMainCamera()->ShakeCamera({ 2.95f,2.95f,2.75f }, 25, 15);
			pGame->SetPauseFrames(4);
			SetUnlitForFrames(12);
			nHP -= 10;
			pGame->SetHitEffect();
			AddMoveToRankMeter(pPlayerAttack->Animation, 40);
			if (bIsBoss)
				nHP -= 5;
		}
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
		nState = ENEMY_SENDOFF;
		if(!pCurrentFloor)
			nState = ENEMY_FALLING;
		if (!bIsBoss)
			nFramesSendOff = 5;
		if (bDoDamage) {
			PlaySoundGame(SOUND_LABEL_SE_BIGSLASH);
			VibrateXinput(65535, 65535, 30);
			AddMoveToRankMeter(pPlayerAttack->Animation, 40);
			nHP -= 10;
			GetMainCamera()->ShakeCamera({ 2.95f,2.95f,2.75f }, 25, 15);
			pGame->SetPauseFrames(PAUSE_FRAMES_PER_ATTACK);
			SetUnlitForFrames(UNLIT_FRAMES_PER_ATTACK);
			pGame->SetHitEffect();
			if (nEnragedHitCountMax > 0 && nEnragedCounter == 0)
				nEnragedMeter++;
		}
		break;
	case NINJA_AIR_DOWN:
		if (!bIsBoss) {
			Position.x = pPlayer->GetPosition().x + (-5 * nDirection);
			Position.y = pPlayer->GetPosition().y;
		}
		if (nEnragedMeter > 0)
			nEnragedMeter = 0;
		nState = ENEMY_FALLING;
		if (bDoDamage) {
			PlaySoundGame(SOUND_LABEL_SE_BIGSLASH2);
			VibrateXinput(65535, 65535, 30);
			AddMoveToRankMeter(pPlayerAttack->Animation, 40);
			GetMainCamera()->ShakeCamera({ 2.95f,2.95f,2.75f }, 25, 15);
			pGame->SetPauseFrames(PAUSE_FRAMES_PER_ATTACK);
			SetUnlitForFrames(UNLIT_FRAMES_PER_ATTACK);
			nHP -= 10;
			pGame->SetHitEffect();
			if (nEnragedHitCountMax > 0 && nEnragedCounter==0)
				nEnragedMeter++;
			if (bIsBoss)
				nHP -= 5;
		}
		break;
	default:
		
		pModel->SwitchAnimationSpeed(fAnimationSpeeds[ENEMY_DAMAGED]);
		if (bDoDamage) {
			if (bIsBoss)
				nHP -= 5;
			if(pPlayerAttack->Animation != SAMURAI_STINGER)
				PlaySoundGame(SOUND_LABEL_SE_NINJA_REGULARSLASH);
			else
				PlaySoundGame(SOUND_LABEL_SE_BIGSLASH2);
			VibrateXinput(65535/2, 65535/2, 30);
			if (pPlayerAttack->Animation == NINJA_ATTACK_COMBOAIR_A || pPlayerAttack->Animation == NINJA_ATTACK_COMBOAIR_B ||
				pPlayerAttack->Animation == NINJA_ATTACK_COMBOAIR_C)
				AddMoveToRankMeter(NINJA_ATTACK_COMBOAIR_A, 40);
			else if (pPlayerAttack->Animation == NINJA_ATTACK_COMBO_A || pPlayerAttack->Animation == NINJA_ATTACK_COMBO_B ||
				pPlayerAttack->Animation == NINJA_ATTACK_COMBO_C)
				AddMoveToRankMeter(NINJA_ATTACK_COMBO_A, 40);
			else
				AddMoveToRankMeter(pPlayerAttack->Animation, 40);
			GetMainCamera()->ShakeCamera({ 1.85f,1.85f,0.75f }, 25, 15);
			pGame->SetPauseFrames(PAUSE_FRAMES_PER_ATTACK);
			SetUnlitForFrames(UNLIT_FRAMES_PER_ATTACK);
			nHP -= 5;
			pGame->SetHitEffect();
			if (nEnragedHitCountMax > 0 && nEnragedCounter == 0)
				nEnragedMeter++;
		}
		if (!bIsBoss) {
			if (nState != ENEMY_SENDOFF && pPlayerAttack->Animation != SAMURAI_STINGER)
				Position.x = AttackHitbox.x;
			if (!(pPlayer->GetFloor()) && pPlayer->GetState() != PLAYER_TELEPORTING) {
				pPlayer->TranslateX(1.5f* nPlayerDirection);
				if (!pCurrentFloor)
					Position.y = AttackHitbox.y - 20;
				nCancelGravityFrames = CANCEL_GRAVITY_FRAMES;
			}
			pWall = pGame->GetWalls()->CheckCollision(GetHitBox());
			if (pWall) {
				while (IsInCollision3D(GetHitBox(), pWall->GetHitBox()) && !IsInCollision3D(pPlayer->GetHitBox(HB_HEAD), pWall->GetHitBox()))
				{
					pPlayer->TranslateX(1 * -nPlayerDirection);
					Position.x += 1 * nDirection;
				}
			}
		}
		if (nLastPlayerAttack != pPlayerAttack->Animation && nEnragedCounter==0)
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
		if (bDoDamage)
			pGame->RaiseScoreWithRank(5);
		fYForce = 0;
		if (pPlayerAttack->Animation == SAMURAI_STINGER)
			pPlayer->CancelAttack();
		break;
	}
	if ((nEnragedCounter == 0 || nEnragedHitCountMax == 0) && nState != ENEMY_SENDOFF && nState != ENEMY_FALLING) {
		nState = ENEMY_DAMAGED;
	}
	if (nEnragedHitCountMax <= nEnragedMeter && nEnragedHitCountMax!=0) {
		nEnragedCounter = nEnragedFrames;
		nEnragedMeter = 0;
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
		PlaySoundGame(SOUND_LABEL_SE_KO);
	}
}

void Enemy3D::RegularCollisionWithPlayer()
{
	Player3D* pPlayer = (Player3D*)pPlayerPointer;
	int nPlayerDirection = pPlayer->GetDirection();
	if (IsInCollision3D(pPlayer->GetHitBox(HB_BODY), GetHitBox()) && pPlayer->GetFloor())
	{
		if (!bIsBoss)
			Position.x += 0.5f * nPlayerDirection;
		if (nPlayerDirection == LEFT_DIR) {
			while (((pPlayer->GetState() == PLAYER_WALKING || bIsBoss) && IsInCollision3D(pPlayer->GetHitBox(HB_BODY), GetHitBox())))//|| (!(pPlayer->GetFloor()) && IsInCollision3D(pPlayer->GetHitBox(HB_FEET), GetHitBox())))
				pPlayer->TranslateX(1);
		}
		else {
			while ((pPlayer->GetState() == PLAYER_WALKING || bIsBoss) && IsInCollision3D(pPlayer->GetHitBox(HB_BODY), GetHitBox()))//|| (!(pPlayer->GetFloor()) && IsInCollision3D(pPlayer->GetHitBox(HB_FEET), GetHitBox())))
				pPlayer->TranslateX(-1);
		}
	}
	if (IsInCollision3D(pPlayer->GetHitBox(HB_BODY), GetHitBox()) && pPlayer->PlayerIsFalling() && nState != ENEMY_FALLING && nState != ENEMY_DAMAGED && nState != ENEMY_DAMAGEDALT)
	{
		pPlayer->TranslateX((-0.05f * nPlayerDirection));
		pPlayer->SetYForce(0);
		if (!bIsBoss)
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

void Enemy3D::SetUnlitForFrames(int frames)
{
	bUnlit = true;
	nUnlitFrames = frames;
}

Hitbox3D Enemy3D::GetAttackHitbox()
{
	return { (hbAttack.x*nDirection) + Position.x, hbAttack.y + Position.y, hbAttack.z + Position.z, 
		hbAttack.SizeX, hbAttack.SizeY, hbAttack.SizeZ };
}
