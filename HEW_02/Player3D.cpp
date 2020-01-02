#include "Player3D.h"
#include "debugproc.h"
#include "InputManager.h"
#include "input.h"
#include "SceneGame.h"
#include "string.h"
#define PLAYER_SPEED	(5.0f)					// 移動速度
#define JUMP_FORCE 7.0f
#define SCALE_NINJA	  1.0f
#define SCALE_SAMURAI 1.0f
#define SCALE_GEISHA 1.0f
#define MAX_INPUT_TIMER 60
#define MAX_ATTACKS 14
#define INIT_HP 100
#define INIT_STAMINA 30
#define TRANSFORM_ACCELERATION 0.075f
#define ATTACK_HITBOX_SCALE { 3,8,6 }
Player3D* MainPlayer;

enum MOVEMENT_LOCKED
{
	M_LOCKED = 0,
	M_UNLOCKED = 1
};

PLAYER_ATTACK_MOVE stAllMoves[MAX_ATTACKS] =
{
	//忍者
	{"A",   MODEL_NINJA, NINJA_ATTACK_COMBO_A,    false, GROUND_MOVE,	420 },
	{"AA",  MODEL_NINJA, NINJA_ATTACK_COMBO_B,    false, GROUND_MOVE,	520 },
	{"AAA", MODEL_NINJA, NINJA_ATTACK_COMBO_C,    false, GROUND_MOVE,	680 },
	{"AAAA", MODEL_NINJA, NINJA_ATTACK_COMBO_D,   false, GROUND_MOVE,	820 },
	{"AAAAA", MODEL_NINJA, NINJA_ATTACK_COMBO_E,  true,  GROUND_MOVE,	950 },
	{"A",   MODEL_NINJA, NINJA_ATTACK_COMBOAIR_A, false, AIR_MOVE,		1228 },
	{"AA",  MODEL_NINJA, NINJA_ATTACK_COMBOAIR_B, false, AIR_MOVE,		1328 },
	{"AAA", MODEL_NINJA, NINJA_ATTACK_COMBOAIR_C, false,  AIR_MOVE,		1411 },
	{"AAAA", MODEL_NINJA, NINJA_ATTACK_COMBOAIR_D, true,  AIR_MOVE,		1480 },
	{"UA", MODEL_NINJA, NINJA_UPPER_SLASH, true,  GROUND_MOVE,		1480 },
	{"DA", MODEL_NINJA, NINJA_AIR_DOWN, true,  AIR_MOVE,		1480 },
	//侍
	{"A",   MODEL_SAMURAI, SAMURAI_COMBOA,		  false, BOTH_MOVE,		263 },
	{"AA",  MODEL_SAMURAI, SAMURAI_COMBOB,		  true,  BOTH_MOVE,		348 },
	//芸者														  
	{"A",   MODEL_GEISHA, GEISHA_BLOCK,			  true,  BOTH_MOVE,		323 },
};
Player3D::Player3D() :GameObject3D()
{
	Init();
}


Player3D::~Player3D()
{
	Uninit();
}

void Player3D::Init()
{
	strcpy(szInputs, "********");
	MainPlayer = this;
	pCurrentAttackPlaying = nullptr;
	for (int i = 0; i < MAX_HB; i++)
		pVisualHitbox[i] = nullptr;
	for (int i = 0; i < PLAYER_MODELS_MAX; i++)
		pPlayerModels[i] = nullptr;
	pPlayerModels[MODEL_SAMURAI] = new Model3D();
	pPlayerModels[MODEL_SAMURAI]->InitModel("data/model/SamuraiHew.fbx", this);
	pPlayerModels[MODEL_SAMURAI]->SetPosition({ -5,6,0 });
	pPlayerModels[MODEL_NINJA] = new Model3D();
	pPlayerModels[MODEL_NINJA]->InitModel("data/model/NinjaHew.fbx", this);
	pPlayerModels[MODEL_NINJA]->SetPosition({ -5,6,0 });
	pPlayerModels[MODEL_GEISHA] = new Model3D();
	pPlayerModels[MODEL_GEISHA]->InitModel("data/model/GeishaHew.fbx", this);
	pPlayerModels[MODEL_GEISHA]->SetPosition({ -5,6,0 });
	pPlayerModels[MODEL_GEISHA]->SetScale({ 0,SCALE_GEISHA,SCALE_GEISHA });
	pPlayerModels[MODEL_NINJA]->SetScale({ SCALE_NINJA,SCALE_NINJA,SCALE_NINJA });
	pPlayerModels[MODEL_SAMURAI]->SetScale({ 0,SCALE_SAMURAI ,SCALE_SAMURAI });
	f_yForce = 0;
	nInputTimer = 0;
	Hitboxes[HB_FEET] = { 0,9.25f,0,3,3,1.5f };
	Hitboxes[HB_BODY] = { 0,15,0,3,8,6 };
	Hitboxes[HB_HEAD] = { 0,22,0,2,2,5 };
	Hitboxes[HB_RIGHT] = { 5,15,0,3,3,6 };
	Hitboxes[HB_LEFT] = { -5,15,0,1,3,6 };
	Hitboxes[HB_ATTACK] = { 7.5f,15,0,0,0,0 };
	pCurrentFloor = nullptr;
	nNextTransform = nCurrentTransformation = MODEL_NINJA;
	nState = PLAYER_IDLE;
	pPlayerModels[MODEL_SAMURAI]->SetRotationY(90 - (XM_PI*0.65f));
	pPlayerModels[MODEL_GEISHA]->SetRotationY(90 - (XM_PI*0.65f));
	nMaxHP = nHP = INIT_HP;
	nMaxStamina = nStamina = INIT_STAMINA;
	RightWall = nullptr;
	LeftWall = nullptr;
	WallAttachedTo = nullptr;
	LockMovementRight = 1;
	LockMovementLeft = 1;
	nCurrentAnimation = 0;
	bUsingDebugAim = false;
	bDeadAnimation = false;
	pMainCamera = GetMainCamera();
	pDebugAim = new DebugAim();
	pDebugAim->SetPosition(Position);
	bStaminaCoolDown = false;
	Position = { 0,0,0 };
	nDirection = RIGHT_DIR;
	nHP /= 2;//DEL
	
#if SHOW_HITBOX
	for (int i = 0; i < MAX_HB; i++)
	{
		pVisualHitbox[i] = new Cube3D("data/texture/hbox.tga");
		pVisualHitbox[i]->SetPosition({ GetHitBox(i).x, GetHitBox(i).y, GetHitBox(i).z });
		pVisualHitbox[i]->SetScale({ GetHitBox(i).SizeX, GetHitBox(i).SizeY, GetHitBox(i).SizeZ });
	}
#endif
}

void Player3D::Update()
{
	if (pMainCamera && !IsDebugAimOn()) {
		if (pMainCamera->GetFocalPoint() != this)
			return;
	}
	GameObject3D::Update();
	if (DebugAimControl()) 
		return;//使っている場合に、関数が終わり

	if (nState == PLAYER_DEAD)//プレイヤーが死んだ場合に管理する
	{
		DeadStateControl();
		return;
	}
	if (nState == PLAYER_TELEPORTING)
	{
		TeleportControl();
		return;
	}
	if (nState == PLAYER_TELEPORTING_DAMAGED)//プレイヤーはスパイクみたいな危険からダメージをもらった場合に管理する
	{
		DamagedTeleportingControl();
		return;
	}
	if (nHP <= 0) {//HPは０になったらプレイヤーの状態が変わる
		nState = PLAYER_DEAD;
		return;
	}
	
	WallAttachedTo = nullptr;
	bool bIsLocked = pCurrentAttackPlaying;
	if (pCurrentAttackPlaying)
		bIsLocked = (pPlayerModels[nCurrentTransformation]->GetCurrentFrame() < pCurrentAttackPlaying->UnlockFrame);
	GravityControl();
	PlayerInputsControl(bIsLocked);
	if ((nDirection == RIGHT_DIR && nState!=PLAYER_KICK_WALL_STATE) || ((nDirection == LEFT_DIR && nState == PLAYER_KICK_WALL_STATE))) {
		LockMovementLeft = M_UNLOCKED;
		if (RightWall)
		{
			if (IsInCollision3D(GetHitBox(HB_RIGHT), RightWall->GetHitBox()))
			{
				while (IsInCollision3D(GetHitBox(HB_BODY), RightWall->GetHitBox())) 
				{
					Position.x -= 0.001f;
					LockMovementRight = M_LOCKED;
				}
				Position.x += 0.001f;
				if(!pCurrentFloor)
					WallAttachedTo = RightWall;
			}
			else
			{
				LockMovementRight = M_UNLOCKED;
				RightWall = nullptr;
			}
		}
	}
	else if ((nDirection == LEFT_DIR && nState != PLAYER_KICK_WALL_STATE) || (nDirection == RIGHT_DIR && nState == PLAYER_KICK_WALL_STATE)) {
		LockMovementRight = M_UNLOCKED;
		if (LeftWall)
		{
			if (IsInCollision3D(GetHitBox(HB_LEFT), LeftWall->GetHitBox()))
			{
				while (IsInCollision3D(GetHitBox(HB_BODY), LeftWall->GetHitBox()))
				{
					Position.x += 0.001f;
					LockMovementLeft = M_LOCKED;
				}
				Position.x -= 0.001f;
				if (!pCurrentFloor)
					WallAttachedTo = LeftWall;
			}
			else 
			{
				LockMovementLeft = M_UNLOCKED;
				LeftWall = nullptr;
			}
		}
	}
	pPlayerModels[nCurrentTransformation]->UpdateModel();
	if (pCurrentFloor)
	{
		if (!(pCurrentFloor->IsMoveableObject())) {
			if (++nFrameCountForSafePos > 30)
			{
				nFrameCountForSafePos = 0;
				x3LastSafePos = Position;
			}
		}
	}
	else {
		nFrameCountForSafePos = 0;
	}
	SceneGame* pGame = GetCurrentGame();
	XMFLOAT3 rotCamera;
	if (nCurrentTransformation == MODEL_SAMURAI || nCurrentTransformation == MODEL_GEISHA)
	{
		static int framecontrol=0;
		if (++framecontrol > 30)
		{
			nStamina -= 2;
			framecontrol = 0;
			if (nStamina < 0)
				nStamina = 0;
		}
	}
	else {
		static int framecontrol = 0;
		if (++framecontrol > 20)
		{
			nStamina += 1;
			framecontrol = 0;
		}
		if (nStamina > nMaxStamina)
			nStamina = nMaxStamina;
	}
	if (nStamina <= 0)
	{
		nStamina = 0;
		bStaminaCoolDown = true;
	}
	if (nStamina >= nMaxStamina)
		bStaminaCoolDown = false;
	// カメラの向き取得
	rotCamera = pMainCamera->GetCameraAngle();
	static int NumTest = 2;
	static int nKickWallFrameCounter=0;
	
	if (nState != PLAYER_KICK_WALL_STATE) 
		nKickWallFrameCounter = 0;
	if (nState != PLAYER_ATTACKING)
	{
		Hitboxes[HB_ATTACK].SizeX = 0;
		Hitboxes[HB_ATTACK].SizeY = 0;
		Hitboxes[HB_ATTACK].SizeZ = 0;
	}
	switch (nState)
	{
	case PLAYER_IDLE:
		SwitchAnimationSlowness(0);
		if (!pCurrentFloor) 
		{
			if (LockMovementRight == M_LOCKED || LockMovementLeft == M_LOCKED) 
			{
				if(!GetInput(INPUT_NINJACRAWL_UP) && !GetInput(INPUT_DOWN))
					SwitchAnimation(MODEL_NINJA, NINJA_ON_WALL);
				else
					SwitchAnimation(MODEL_NINJA, NINJA_CRAWLING);
			}
			else
				SwitchAnimation(MODEL_NINJA, NINJA_AIR_IDLE); 
			SwitchAnimation(MODEL_GEISHA, GEISHA_AIR_IDLE);
			SwitchAnimation(MODEL_SAMURAI, SAMURAI_IDLE);
		}
		else {
			SwitchAnimation(MODEL_NINJA, NINJA_IDLE);
			SwitchAnimation(MODEL_GEISHA, GEISHA_IDLE);
			SwitchAnimation(MODEL_SAMURAI, SAMURAI_IDLE);
		}
		if (GetInput(INPUT_RIGHT) || GetInput(INPUT_LEFT))
			nState = PLAYER_WALKING;
		pCurrentAttackPlaying = nullptr;
		break;
	case PLAYER_WALKING:
		SwitchAnimationSlowness(0);
		if (!(!pCurrentFloor && (LockMovementRight == M_LOCKED || LockMovementLeft == M_LOCKED) && nCurrentTransformation==MODEL_NINJA))
		{
			if (GetInput(INPUT_LEFT))
			{
				nDirection = LEFT_DIR;
				Rotation = XMFLOAT3(0.0f, -180.0f, 0.0f);
			}
			if (GetInput(INPUT_RIGHT))
			{
				Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
				nDirection = RIGHT_DIR;
			}
		}
		if (!(GetInput(INPUT_RIGHT) || GetInput(INPUT_LEFT)))
			nState = PLAYER_IDLE;
		if (!pCurrentFloor)
		{
			if (LockMovementRight == M_LOCKED || LockMovementLeft == M_LOCKED)
			{
				if (!GetInput(INPUT_NINJACRAWL_UP))
					SwitchAnimation(MODEL_NINJA, NINJA_ON_WALL);
				else
					SwitchAnimation(MODEL_NINJA, NINJA_CRAWLING);
			}
			else
				SwitchAnimation(MODEL_NINJA, NINJA_AIR_IDLE);
			SwitchAnimation(MODEL_GEISHA, GEISHA_AIR_IDLE);
			SwitchAnimation(MODEL_SAMURAI, SAMURAI_IDLE);
		}
		else {
			SwitchAnimation(MODEL_NINJA, NINJA_WALKING);
			pPlayerModels[MODEL_NINJA]->SwitchAnimationSpeed(2);
			SwitchAnimation(MODEL_GEISHA, GEISHA_WALKING);
			pPlayerModels[MODEL_GEISHA]->SwitchAnimationSpeed(2);
			SwitchAnimation(MODEL_SAMURAI, SAMURAI_WALKING);
			pPlayerModels[MODEL_SAMURAI]->SwitchAnimationSpeed(1.35f);
		}
		if(nCurrentTransformation==MODEL_NINJA)
			Position.x += PLAYER_SPEED * 0.75f * nDirection*LockMovementRight*LockMovementLeft;
		else if (nCurrentTransformation == MODEL_SAMURAI)
			Position.x += PLAYER_SPEED * 0.15f * nDirection*LockMovementRight*LockMovementLeft;
		else if (nCurrentTransformation == MODEL_GEISHA)
			Position.x += PLAYER_SPEED * 0.35f * nDirection*LockMovementRight*LockMovementLeft;
		if (!LeftWall && !RightWall && pGame)
		{
			pGame->GetSkySphere()->RotateOnY(0.00075f*(-nDirection));
		}
		break;
	case PLAYER_TRANSFORMING:
		TransformingStateControl();
		break;
	case PLAYER_ATTACKING:
		AttackingStateControl();
		break;
	case PLAYER_KICK_WALL_STATE:
		SwitchAnimationSlowness(0);
		SwitchAnimationSpeed(2.0f);
		SwitchAnimation(MODEL_NINJA, NINJA_KICKWALL);
		Position.x += PLAYER_SPEED * 0.75f * -nDirection;
		if (pPlayerModels[nCurrentTransformation]->GetLoops() > 0 ||
			(LockMovementRight == M_LOCKED) || (LockMovementLeft == M_LOCKED))
		{
			if (nDirection == RIGHT_DIR)
			{
				nDirection = LEFT_DIR;
				Rotation = XMFLOAT3(0.0f, -180.0f, 0.0f);
				nState = PLAYER_IDLE;
				return;
			}
			if (nDirection == LEFT_DIR)
			{
				Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
				nDirection = RIGHT_DIR;
				nState = PLAYER_IDLE;
			}
			
		}
		if (pCurrentFloor || nCurrentTransformation != MODEL_NINJA)
			nState = PLAYER_IDLE;
		break;
	default:
		break;
	}
	HitboxControl();
}

void Player3D::AttackingStateControl()
{
	SwitchAnimationSpeed(2.5f);
	Hitboxes[HB_ATTACK].x = 7.5f*nDirection;
	XMFLOAT3 hbAttackScale = ATTACK_HITBOX_SCALE;
	bool bIsAttacking = false;
	bool bTwiceTheHitbox = false;
	int nAttackFrame = pPlayerModels[nCurrentTransformation]->GetCurrentFrame();
	int nCurrentAnim = 0;
	//printf("%d\n", nAttackFrame);
	if (pCurrentAttackPlaying) {
		nCurrentAnim = pCurrentAttackPlaying->Animation;
		if (nCurrentAnim != NINJA_UPPER_SLASH && nCurrentAnim != NINJA_AIR_DOWN)
			f_yForce = 0;
		switch (pCurrentAttackPlaying->Animation)
		{
		case NINJA_ATTACK_COMBO_A:
			if (nAttackFrame > 415 && nAttackFrame < 436) {
				bIsAttacking = true;
				Position.x += 1 * nDirection;
			}
			break;
		case NINJA_ATTACK_COMBO_B:
			if (nAttackFrame > 521 && nAttackFrame < 546) {
				bIsAttacking = true;
				Position.x += 1 * nDirection;
			}
			break;
		case NINJA_ATTACK_COMBO_C:
			if (nAttackFrame > 673 && nAttackFrame < 688) {
				Position.x += 1 * nDirection;
				bIsAttacking = true;
			}
			break;
		case NINJA_ATTACK_COMBO_D:
			if (nAttackFrame > 820 && nAttackFrame < 826) {
				Position.x += 1 * nDirection;
				bIsAttacking = true;
			}
			break;
		case NINJA_ATTACK_COMBO_E:
			if (nAttackFrame > 920 && nAttackFrame < 931) {
				Position.x += 3 * nDirection;
				bIsAttacking = true;
				bTwiceTheHitbox = true;
			}
			break;
		case NINJA_ATTACK_COMBOAIR_A:
			bTwiceTheHitbox = true;
			if (nAttackFrame > 1222 && nAttackFrame < 1233)
				bIsAttacking = true;
			break;
		case NINJA_ATTACK_COMBOAIR_B:
			bTwiceTheHitbox = true;
			if (nAttackFrame > 1311 && nAttackFrame < 1331)
				bIsAttacking = true;
			break;
		case NINJA_ATTACK_COMBOAIR_C:
			bTwiceTheHitbox = true;
			if (nAttackFrame > 1394 && nAttackFrame < 1408)
				bIsAttacking = true;
			break;
		case NINJA_ATTACK_COMBOAIR_D:
			bTwiceTheHitbox = true;
			if (nAttackFrame > 1473 && nAttackFrame < 1491)
				bIsAttacking = true;
			break;
		case NINJA_AIR_DOWN:
			bIsAttacking = true;
			if (nAttackFrame >= 1746)
				pPlayerModels[MODEL_NINJA]->SetFrame(1746);
			if (!pCurrentFloor) {
				f_yForce += GRAVITY_FORCE*2.0f;
				Position.y -= f_yForce;
			}
			else {
				pCurrentAttackPlaying = nullptr;
			}
			break;
		case NINJA_UPPER_SLASH:
			if (pCurrentFloor && nAttackFrame>997) {
				Jump(JUMP_FORCE);
			}
			if(nAttackFrame > 997)
				bIsAttacking = true;
			f_yForce += GRAVITY_FORCE;
			if (f_yForce > 0)
				f_yForce = 0;
			Position.y -= f_yForce;
			break;
		case SAMURAI_COMBOA:
			if (nAttackFrame > 241 && nAttackFrame < 261)
				bIsAttacking = true;
			break;
		case SAMURAI_COMBOB:
			if (nAttackFrame > 303 && nAttackFrame < 331)
				bIsAttacking = true;
			break;
		case GEISHA_BLOCK:
			if (nAttackFrame > 261 && nAttackFrame < 318)
				bIsAttacking = true;
			break;
		default:
			break;
		}
	}
	if (bIsAttacking)
	{
		Hitboxes[HB_ATTACK].SizeX = hbAttackScale.x;
		Hitboxes[HB_ATTACK].SizeY = hbAttackScale.y;
		Hitboxes[HB_ATTACK].SizeZ = hbAttackScale.z;
		if (bTwiceTheHitbox)
		{
			Hitboxes[HB_ATTACK].x = 11.5f*nDirection;
			Hitboxes[HB_ATTACK].SizeX = hbAttackScale.x * 2;
			Hitboxes[HB_ATTACK].SizeY = hbAttackScale.y;
			Hitboxes[HB_ATTACK].SizeZ = hbAttackScale.z * 2;
		}
		if (nCurrentAnim == GEISHA_BLOCK)
		{
			Hitboxes[HB_ATTACK].SizeX = hbAttackScale.x * 2;
			Hitboxes[HB_ATTACK].SizeY = hbAttackScale.y;
			Hitboxes[HB_ATTACK].SizeZ = hbAttackScale.z * 2;
		}
	}
	else {
		Hitboxes[HB_ATTACK].SizeX = 0;
		Hitboxes[HB_ATTACK].SizeY = 0;
		Hitboxes[HB_ATTACK].SizeZ = 0;
	}

	if (pPlayerModels[nCurrentTransformation]->GetLoops() > 0 && nCurrentAnim != NINJA_AIR_DOWN)//
		pCurrentAttackPlaying = nullptr;
	if (!pCurrentAttackPlaying)
		nState = PLAYER_IDLE;
}

void Player3D::TeleportControl()
{
	static float acceleration;
	static bool reachedDestination = false;
	if (reachedDestination)
	{
		acceleration += 0.005f;
		Scale.x += acceleration;
		Scale.y += acceleration;
		Scale.z += acceleration;
		if (Scale.x > 1)
			Scale.x = 1;
		if (Scale.y > 1)
			Scale.y = 1;
		if (Scale.z > 1)
			Scale.z = 1;
		if (CompVector(Scale, { 1,1,1 }))
		{
			acceleration = 0;
			nState = PLAYER_IDLE;
			reachedDestination = false;
		}
		return;
	}
	if (IsVectorZero(Scale))
	{
		static float f_Acceleration = 0;
		f_Acceleration+=0.5f;
		if (Position.x < x3TeleportDestination.x) {
			Position.x += f_Acceleration;
			if (Position.x >= x3TeleportDestination.x)
				Position.x = x3TeleportDestination.x;
		}
		else if (Position.x > x3TeleportDestination.x) {
			Position.x -= f_Acceleration;
			if (Position.x <= x3TeleportDestination.x)
				Position.x = x3TeleportDestination.x;
		}

		if (Position.y < x3TeleportDestination.y) {
			Position.y += f_Acceleration;
			if (Position.y >= x3TeleportDestination.y)
				Position.y = x3TeleportDestination.y;
		}
		else if (Position.y > x3TeleportDestination.y) {
			Position.y -= f_Acceleration;
			if (Position.y <= x3TeleportDestination.y)
				Position.y = x3TeleportDestination.y;
		}
		if (CompVector(Position, x3TeleportDestination)) {
			f_Acceleration = 0;
			reachedDestination = true;
		}
	}

	acceleration += 0.005f;
	Scale.x -= acceleration;
	Scale.y -= acceleration;
	Scale.z -= acceleration;
	if (Scale.x < 0)
		Scale.x = 0;
	if (Scale.y < 0)
		Scale.y = 0;
	if (Scale.z < 0)
		Scale.z = 0;
	if (IsVectorZero(Scale))
		acceleration = 0;
}

bool Player3D::DebugAimControl()
{
	if (bUsingDebugAim)
	{
		if (pDebugAim)
			pDebugAim->Update();

		if (GetInput(INPUT_DEBUGAIM)) {
			GetMainCamera()->SetFocalPoint(this);
			bUsingDebugAim = false;
		}
		return true;
	}
	if (pDebugAim)
		pDebugAim->SetPosition(Position);
	if (GetInput(INPUT_DEBUGAIM)) {
		GetMainCamera()->SetFocalPoint(pDebugAim);
		bUsingDebugAim = true;
	}
	return false;
}

void Player3D::DeadStateControl()
{
	pPlayerModels[MODEL_NINJA]->SetCanLoop(false);
	if (!pCurrentFloor) {
		GravityControl();
		return;
	}
	nNextTransform = MODEL_NINJA;
	TransformingStateControl();
	if (nCurrentTransformation != MODEL_NINJA)
		return;
	
	SwitchAnimation(MODEL_NINJA, NINJA_DEAD);
	pPlayerModels[MODEL_NINJA]->SwitchAnimationSpeed(1);
	pPlayerModels[MODEL_NINJA]->UpdateModel();
	bDeadAnimation = true;
	printf("%d\n", pPlayerModels[nCurrentTransformation]->GetCurrentFrame());
}

void Player3D::DamagedTeleportingControl()
{
	nNextTransform = MODEL_NINJA;
	TransformingStateControl();
	if (nCurrentTransformation != MODEL_NINJA)
		return;
	pPlayerModels[MODEL_NINJA]->SwitchAnimationSpeed(0.01f);
	SwitchAnimation(MODEL_NINJA, NINJA_DAMAGEDALT);
	pPlayerModels[MODEL_NINJA]->UpdateModel();
	static int fAcceleration = 0;
	fAcceleration++;
	if (Position.x < x3LastSafePos.x) {
		Position.x += fAcceleration;
		if (Position.x >= x3LastSafePos.x)
			Position.x = x3LastSafePos.x;
	}
	else if (Position.x > x3LastSafePos.x) {
		Position.x -= fAcceleration;
		if (Position.x <= x3LastSafePos.x)
			Position.x = x3LastSafePos.x;
	}

	if (Position.y < x3LastSafePos.y) {
		Position.y += fAcceleration;
		if (Position.y >= x3LastSafePos.y)
			Position.y = x3LastSafePos.y;
	}
	else if (Position.y > x3LastSafePos.y) {
		Position.y -= fAcceleration;
		if (Position.y <= x3LastSafePos.y)
			Position.y = x3LastSafePos.y;
	}
	if (Position.x == x3LastSafePos.x && Position.y == x3LastSafePos.y)
	{
		fAcceleration = 0;
		nState = PLAYER_IDLE;
	}
}

void Player3D::PlayerInputsControl(bool bIsLocked)
{
	if (nState == PLAYER_KICK_WALL_STATE)
		return;
	if ((GetInput(INPUT_JUMP) && pCurrentFloor) || (GetInput(INPUT_JUMP) && (LockMovementLeft == M_LOCKED || LockMovementRight == M_LOCKED)))
	{
		pCurrentAttackPlaying = nullptr;
		if (pCurrentFloor) {
			if (pCurrentFloor->IsMoveableObject())
			{
				pCurrentFloor->PauseObject(15);
			}
			Jump(JUMP_FORCE);
		}
		else if ((!pCurrentFloor && (LockMovementRight == M_LOCKED || LockMovementLeft == M_LOCKED) && nCurrentTransformation == MODEL_NINJA))
		{
			Jump(JUMP_FORCE);
			
			ResetInputs();
			nState = PLAYER_KICK_WALL_STATE;
			LeftWall = RightWall = nullptr;
			return;
		}
		ResetInputs();
	}
	if (nState == PLAYER_TRANSFORMING || bIsLocked)
		return;
	if (GetInput(TRANSFORM_GEISHA) && nStamina>0 && !bStaminaCoolDown)
	{
		if (nNextTransform != nCurrentTransformation)
			nState = PLAYER_TRANSFORMING;
		nNextTransform = MODEL_GEISHA;
	}
	else if (GetInput(TRANSFORM_SAMURAI) && nStamina > 0 && !bStaminaCoolDown)
	{
		if (nNextTransform != nCurrentTransformation)
			nState = PLAYER_TRANSFORMING;
		nNextTransform = MODEL_SAMURAI;
	}
	else if(!GetInput(TRANSFORM_GEISHA) || !GetInput(TRANSFORM_SAMURAI) || nStamina <= 0 || bStaminaCoolDown){
		nNextTransform = MODEL_NINJA;
		if (nNextTransform != nCurrentTransformation)
			nState = PLAYER_TRANSFORMING;
	}
	if (strcmp(szInputs, "********")) {
		if (++nInputTimer > MAX_INPUT_TIMER)
		{
			if (GetLastInputInserted() != 'P')
			{
				nInputTimer = (MAX_INPUT_TIMER / 2);
				AddInput('P');
			}
			else {
				ResetInputs();
			}
			nInputTimer = 0;
		}
	}
	if (GetInput(INPUT_ATTACK) && !bIsLocked)
	{
		nInputTimer = 0;
		AddInput('A');
		Attack(szInputs);
	}
}

void Player3D::TransformingStateControl()
{
	if (nCurrentTransformation == nNextTransform) {
		return;
	}
	static float fTransformAcceleration = 0;
	static bool bCurrentModelFinished = false;
	static int nCooldownInbetween=0;
	if (!bCurrentModelFinished) {
		if (pPlayerModels[nCurrentTransformation]->GetScale().x > 0)
			pPlayerModels[nCurrentTransformation]->ReduceScaleOnX(0.035f);
		if (pPlayerModels[nCurrentTransformation]->GetRotation().y > 90 - (XM_PI*0.65f)) {
			pPlayerModels[nCurrentTransformation]->RotateAroundY(fTransformAcceleration);
			fTransformAcceleration -= TRANSFORM_ACCELERATION;
		}
		else if (pPlayerModels[nCurrentTransformation]->GetRotation().y <= 90 - (XM_PI*0.65f)) {
			pPlayerModels[nCurrentTransformation]->SetRotationY(90 - (XM_PI*0.65f));
			fTransformAcceleration = 0;
			bCurrentModelFinished = pPlayerModels[nCurrentTransformation]->GetRotation().y == 90 - (XM_PI*0.65f) && fTransformAcceleration == 0;
		}
	}
	else {
		pPlayerModels[nNextTransform]->UpdateModel();
		if (++nCooldownInbetween > 2) {
			float fModelScale = 0;
			if (nNextTransform == MODEL_NINJA)
				fModelScale = SCALE_NINJA;
			if (nNextTransform == MODEL_SAMURAI)
				fModelScale = SCALE_SAMURAI;
			if (nNextTransform == MODEL_GEISHA)
				fModelScale = SCALE_GEISHA;
			if (pPlayerModels[nNextTransform]->GetScale().x < fModelScale)
				pPlayerModels[nNextTransform]->ReduceScaleOnX(-0.035f);
			else if (pPlayerModels[nNextTransform]->GetScale().x >= fModelScale)
				pPlayerModels[nNextTransform]->SetScale(fModelScale);
			if (pPlayerModels[nNextTransform]->GetRotation().y < 90) {
				fTransformAcceleration += 0.035f;
				pPlayerModels[nNextTransform]->RotateAroundY(fTransformAcceleration);
			}
			else if (pPlayerModels[nNextTransform]->GetRotation().y >= 90 && pPlayerModels[nNextTransform]->GetScale().x == fModelScale) {
				pPlayerModels[nNextTransform]->SetRotationY(90);
				fTransformAcceleration = 0;
				nCooldownInbetween = 0;
				bCurrentModelFinished = false;
				nCurrentTransformation = nNextTransform;
			}
		}
	}
	if (nCurrentTransformation == nNextTransform && nState != PLAYER_TELEPORTING_DAMAGED) 
		nState = PLAYER_IDLE;
	else if(nCurrentTransformation == nNextTransform && nState == PLAYER_TELEPORTING_DAMAGED)
		nState = PLAYER_TELEPORTING_DAMAGED;
}

void Player3D::Jump(float fJumpForce)
{
	if (pCurrentFloor) {
		if (!(pCurrentFloor->IsMoveableObject()))
			x3LastSafePos = Position;
		while (IsInCollision3D(pCurrentFloor->GetHitBox(), GetHitBox(HB_FEET)))
			Position.y += 0.01;
	}
	f_yForce = -fJumpForce;
	pCurrentFloor = nullptr;
}

void Player3D::GravityControl()
{
	if (nState == PLAYER_ATTACKING) {
		return;
	}
	if (nState == PLAYER_TRANSFORMING)
		return;
	if (pCurrentFloor)
	{
		f_yForce = 0;
		if (!IsInCollision3D(pCurrentFloor->GetHitBox(), GetHitBox(HB_FEET)))
			pCurrentFloor = nullptr;
	}
	else {
		if (nCurrentTransformation == MODEL_GEISHA) 
		{
			f_yForce += GRAVITY_FORCE;
			if(f_yForce<0)
				Position.y -= f_yForce;
			else
				Position.y -= f_yForce * 0.025f;
			if (f_yForce > 7)
				f_yForce = 7;
		}else {
			if ((LockMovementRight == M_LOCKED || LockMovementLeft == M_LOCKED) && nCurrentTransformation == MODEL_NINJA)
			{
				if (GetInput(INPUT_NINJACRAWL_UP)) {
					Position.y++;
					f_yForce = 0;
				}
				else if (GetInput(INPUT_DOWN))
				{
					Position.y--;
					f_yForce = 0;
				}
				return;
			}
			else {
				if (f_yForce > 7)
					f_yForce = 7;
				f_yForce += GRAVITY_FORCE;
				Position.y -= f_yForce;
			}
		}
	}
}

void Player3D::HitboxControl()
{
	if(pCurrentFloor)
		pPlayerModels[MODEL_NINJA]->SetPositionX(-2.0f * nDirection);
	else
		pPlayerModels[MODEL_NINJA]->SetPositionX(-6.0f * nDirection);
	pPlayerModels[MODEL_GEISHA]->SetPositionX(-2.0f * nDirection);
	pPlayerModels[MODEL_SAMURAI]->SetPositionX(-2.0f * nDirection);

#if SHOW_HITBOX
	for (int i = 0; i < MAX_HB; i++)
	{
		if (pVisualHitbox[i]) {
			pVisualHitbox[i]->SetPosition({ GetHitBox(i).x, GetHitBox(i).y, GetHitBox(i).z });
			pVisualHitbox[i]->SetScale({ GetHitBox(i).SizeX, GetHitBox(i).SizeY, GetHitBox(i).SizeZ });
			pVisualHitbox[i]->Update();
		}
	}
#endif // USE_HITBOX
}

void Player3D::Draw()
{
	if (bUsingDebugAim && pDebugAim)
	{
		pDebugAim->Draw();
	}
	DrawHitboxes();
	GameObject3D::Draw();
	
	for (int i = 0; i < PLAYER_MODELS_MAX; i++) {
		
	}
	SetCullMode(CULLMODE_NONE);
	if (pPlayerModels[nCurrentTransformation] && pPlayerModels[nCurrentTransformation]->GetRotation().y != 90 - (XM_PI*0.65f))
		pPlayerModels[nCurrentTransformation]->DrawModel();
	else if (nCurrentTransformation != nNextTransform)
	{
		if (pPlayerModels[nNextTransform] && pPlayerModels[nNextTransform]->GetRotation().y != 90 - (XM_PI*0.65f))
			pPlayerModels[nNextTransform]->DrawModel();
	}
	SetCullMode(CULLMODE_CCW);
	
}

void Player3D::DrawHitboxes()
{
	if (pVisualHitbox) {
		GetDeviceContext()->RSSetState(GetMainWindow()->GetRasterizerState(2));
		SetCullMode(CULLMODE_NONE);
		GetMainLight()->SetLightEnable(false);
		for (int i = 0; i < MAX_HB; i++)
		{
			if (pVisualHitbox[i])
				pVisualHitbox[i]->Draw();
		}
		GetMainLight()->SetLightEnable(true);
		SetCullMode(CULLMODE_CCW);
		GetDeviceContext()->RSSetState(GetMainWindow()->GetRasterizerState(1));
	}
}

void Player3D::Uninit()
{
	GameObject3D::Uninit();
	for (int i = 0; i < PLAYER_MODELS_MAX; i++)
		SAFE_DELETE(pPlayerModels[i]);
	for (int i = 0; i < MAX_HB; i++)
		SAFE_DELETE(pVisualHitbox[i]);
	SAFE_DELETE(pDebugAim);
	
}

void Player3D::SwitchAnimation(int nModel, int nAnimation)
{
	pPlayerModels[nModel]->SwitchAnimation(nAnimation);
	nCurrentAnimation = nAnimation;
}

void Player3D::SwitchAnimation(int nAnimation)
{
	for (int i = 0; i < PLAYER_MODELS_MAX; i++)
		pPlayerModels[i]->SwitchAnimation(nAnimation);
	nCurrentAnimation = nAnimation;
}

void Player3D::SwitchAnimationSlowness(int nModel, float fSpeed)
{
	pPlayerModels[nModel]->SwitchAnimationSlowness(fSpeed);
}

void Player3D::SwitchAnimationSlowness(float fSpeed)
{
	for (int i = 0; i < PLAYER_MODELS_MAX; i++)
		pPlayerModels[i]->SwitchAnimationSlowness(fSpeed);
}

void Player3D::SwitchAnimationSpeed(float fSpeed)
{
	for (int i = 0; i < PLAYER_MODELS_MAX; i++)
		pPlayerModels[i]->SwitchAnimationSpeed(fSpeed);
}

void Player3D::SwitchAnimationSpeed(int Model, float fSpeed)
{
	pPlayerModels[Model]->SwitchAnimationSpeed(fSpeed);
}

Hitbox3D Player3D::GetHitBox(int hitbox)
{
	return { Hitboxes[hitbox].x + Position.x, Hitboxes[hitbox].y +Position.y, Hitboxes[hitbox].z + Position.z, Hitboxes[hitbox].SizeX, Hitboxes[hitbox].SizeY, Hitboxes[hitbox].SizeZ };
}

Field3D * Player3D::GetFloor()
{
	return pCurrentFloor;
}

int Player3D::GetDirection()
{
	return nDirection;
}

void Player3D::TranslateX(float x)
{
	Position.x += x;
}

void Player3D::TranslateY(float y)
{
	Position.y += y;
}

void Player3D::SetFloor(Field3D * floor)
{
	pCurrentFloor = floor;
}

Player3D * GetMainPlayer()
{
	return MainPlayer;
}

void Player3D::AddInput(char A)
{
	if (A == 'S' && szInputs[0] == '*')
		return;
	for (int i = 0; i < MAX_PLAYER_INPUT; i++)
	{
		if (szInputs[i] == '*') {
			szInputs[i] = A;
			return;
		}
	}
	for (int i = 0; i < MAX_PLAYER_INPUT; i++)
	{
		if (i != MAX_PLAYER_INPUT - 1)
		{
			szInputs[i] = szInputs[i + 1];
		}
		else {
			szInputs[i] = A;
		}
	}
}

char Player3D::GetLastInputInserted()
{
	for (int i = 0; i < MAX_PLAYER_INPUT; i++)
	{
		if (szInputs[i] == '*')
			return szInputs[i-1];
	}
	return szInputs[MAX_PLAYER_INPUT-1];
}

void Player3D::ResetInputs()
{
	strcpy(szInputs, "********");
	
}

void Player3D::Attack(const char * atkInput)
{
	char szAtkInput[MAX_PLAYER_INPUT + 1];
	int i = 0;
	for (i = 0; i < MAX_PLAYER_INPUT && atkInput[i] != '*'; szAtkInput[i] = atkInput[i], i++);
	szAtkInput[i]='\0';
	if (GetInput(INPUT_UP) && pCurrentFloor)
		strcpy(szAtkInput, "UA");
	if (GetInput(INPUT_DOWN) && !pCurrentFloor)
		strcpy(szAtkInput, "DA");
	Attack(szAtkInput, MAX_PLAYER_INPUT);
}

void Player3D::Attack(const char * atkInput, int recursions)
{
	if (recursions <= 0)
		return;
	char inputToCheck[MAX_PLAYER_INPUT + 1];
	strcpy(inputToCheck, atkInput);
	for (int i = 0; i < MAX_ATTACKS; i++)
	{
		if (nCurrentTransformation != stAllMoves[i].Transformation)continue;
		if (stAllMoves[i].eAirMove == AIR_MOVE && pCurrentFloor)continue;
		if (stAllMoves[i].eAirMove == GROUND_MOVE && !pCurrentFloor)continue;
		if (!strcmp(stAllMoves[i].Input, inputToCheck))//攻撃動作見つけた
		{
			pCurrentAttackPlaying = &stAllMoves[i];
			SwitchAnimation(stAllMoves[i].Transformation, stAllMoves[i].Animation);
			nState = PLAYER_ATTACKING;
			if (stAllMoves[i].ResetInputs)
				ResetInputs();
			return;
		}
	}
	char szAtkInput[MAX_PLAYER_INPUT + 1];
	int i = 0;
	for (i = 1; i < MAX_PLAYER_INPUT && inputToCheck[i] != '\0'; szAtkInput[i-1] = inputToCheck[i], i++);
	szAtkInput[i-1] = '\0';
	Attack(szAtkInput, recursions - 1);
}

void Player3D::SetLeftWall(Wall3D* wall)
{
	LeftWall = wall;
}

void Player3D::SetRightWall(Wall3D* wall)
{
	RightWall = wall;
}

int Player3D::GetPlayerHp()
{
	return nHP;
}

int Player3D::GetPlayerMaxHp()
{
	return nMaxHP;
}

int Player3D::GetPlayerMp()
{
	return nStamina;
}

int Player3D::GetPlayerMaxMp()
{
	return nMaxStamina;
}

void Player3D::RiseHP(int nhprise)
{
	nHP += nhprise;
	if (nHP >= nMaxHP)
		nHP = nMaxHP;
}

bool Player3D::IsStaminaCooldownOn()
{
	return bStaminaCoolDown;
}

void Player3D::SetDamage(int Damage)
{
	if (nHP == 0)
		return;
	nHP -= Damage;
	if (nHP < 0)
		nHP = 0;
	SwitchAnimationSpeed(1);
	SwitchAnimation(MODEL_NINJA, NINJA_DAMAGED);
	GetCurrentGame()->ZoomPause(80, 30, 3, true, false);
	GetMainCamera()->ShakeCamera({ 2.85f,2.85f,1.75f }, 30, 10);
}

void Player3D::SetDamageTeleport(int Damage)
{
	if (nState == PLAYER_TELEPORTING_DAMAGED)
		return;
	nState = PLAYER_TELEPORTING_DAMAGED;
	nHP -= Damage;
	if (nHP < 0)
		nHP = 0;
	SwitchAnimation(MODEL_NINJA, NINJA_DAMAGEDALT);
	GetCurrentGame()->ZoomPause(80, 30, 3, true, false);
	GetMainCamera()->ShakeCamera({ 2.85f,2.85f,1.75f }, 30, 10);
}

float Player3D::GetYForce()
{
	return f_yForce;
}

void Player3D::SetYForce(float fyforce)
{
	f_yForce = fyforce;
}

int Player3D::GetState()
{
	return nState;
}

DebugAim * Player3D::GetDebugAim()
{
	return pDebugAim;
}

bool Player3D::IsDebugAimOn()
{
	return bUsingDebugAim;
}

GameObject3D * Player3D::GetWallCrawling()
{
	return WallAttachedTo;
}

void Player3D::SetPlayerTeleporting(XMFLOAT3 Destination)
{
	if (nState == PLAYER_TELEPORTING || nState == PLAYER_TELEPORTING_DAMAGED)
		return;
	nState = PLAYER_TELEPORTING;
	x3TeleportDestination = Destination;
	x3TeleportDestination.z = Position.z;
}

bool Player3D::PlayerGameOver()
{
	if (pPlayerModels[nCurrentTransformation]->GetCurrentFrame() >= 226 && nState == PLAYER_DEAD && bDeadAnimation)
		return true;
	return false;
}

bool Player3D::PlayerIsFalling()
{
	return f_yForce>0;
}

PLAYER_ATTACK_MOVE * Player3D::GetPlayerAttack()
{
	return pCurrentAttackPlaying;
}
