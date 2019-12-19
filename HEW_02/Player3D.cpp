#include "Player3D.h"
#include "debugproc.h"
#include "InputManager.h"
#include "input.h"
#include "string.h"
#define PLAYER_SPEED	(5.0f)					// ˆÚ“®‘¬“x
#define GRAVITY_FORCE 0.4f
#define JUMP_FORCE 7.0f
#define SCALE_NINJA	  1.0f
#define SCALE_SAMURAI 1.0f
#define SCALE_GEISHA 1.0f
#define MAX_INPUT_TIMER 60
#define MAX_ATTACKS 12
#define INIT_HP 100
#define INIT_STAMINA 100
Player3D* MainPlayer;
enum ANIMATION_NINJA
{
	NINJA_WALKING = 1,
	NINJA_DEAD,
	NINJA_DAMAGED,
	NINJA_IDLE,
	NINJA_ATTACK_COMBO_A,
	NINJA_ATTACK_COMBO_B,
	NINJA_ATTACK_COMBO_C,
	NINJA_ATTACK_COMBO_D,
	NINJA_ATTACK_COMBO_E,
	NINJA_UPPER_SLASH,
	NINJA_AIR_IDLE,
	NINJA_ATTACK_COMBOAIR_A,
	NINJA_ATTACK_COMBOAIR_B,
	NINJA_ATTACK_COMBOAIR_C,
	NINJA_ATTACK_COMBOAIR_D,
	NINJA_AIR_DOWN_FORWARD,
	NINJA_AIR_DASH,
	NINJA_SPIN_DASH,
	NINJA_SHURIKEN,
	NINJA_SHURIKEN_AIR,
	NINJA_ON_WALL=23,
	NINJA_KICKWALL,
	NINJA_CRAWLING,
};
enum ANIMATION_GEISHA
{
	GEISHA_IDLE=0,
	GEISHA_BLOCK,
	GEISHA_DODGE_LEFT,
	GEISHA_DODGE_RIGHT,
	GEISHA_WALKING,
	GEISHA_AIR_IDLE,
};
enum ANIMATION_SAMURAI
{
	SAMURAI_WALKING = 1,
	SAMURAI_IDLE,
	SAMURAI_COMBOA,
	SAMURAI_COMBOB,
};
enum MOVEMENT_LOCKED
{
	M_LOCKED = 0,
	M_UNLOCKED = 1
};

PLAYER_ATTACK_MOVE stAllMoves[MAX_ATTACKS] =
{
	//”EŽÒ
	{"A",   MODEL_NINJA, NINJA_ATTACK_COMBO_A,    false, GROUND_MOVE,	200 },
	{"AA",  MODEL_NINJA, NINJA_ATTACK_COMBO_B,    false, GROUND_MOVE,	270 },
	{"AAA", MODEL_NINJA, NINJA_ATTACK_COMBO_C,    false, GROUND_MOVE,	374 },
	{"AAAA", MODEL_NINJA, NINJA_ATTACK_COMBO_D,   false, GROUND_MOVE,	374 },
	{"AAAAA", MODEL_NINJA, NINJA_ATTACK_COMBO_E,  true,  GROUND_MOVE,	374 },
	{"A",   MODEL_NINJA, NINJA_ATTACK_COMBOAIR_A, false, AIR_MOVE,		440 },
	{"AA",  MODEL_NINJA, NINJA_ATTACK_COMBOAIR_B, false, AIR_MOVE,		540 },
	{"AAA", MODEL_NINJA, NINJA_ATTACK_COMBOAIR_C, false,  AIR_MOVE,		620 },
	{"AAAA", MODEL_NINJA, NINJA_ATTACK_COMBOAIR_D, true,  AIR_MOVE,		620 },
	//Ž˜
	{"A",   MODEL_SAMURAI, SAMURAI_COMBOA,		  false, BOTH_MOVE,		139 },
	{"AA",  MODEL_SAMURAI, SAMURAI_COMBOB,		  true,  BOTH_MOVE,		280 },
	//Œ|ŽÒ														  
	{"A",   MODEL_GEISHA, GEISHA_BLOCK,			  true,  BOTH_MOVE,		200 },
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
	GameObject3D::Init();
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
	Hitboxes[HB_FEET] = { 0,10,0,3,3,1.5f };
	Hitboxes[HB_BODY] = { 0,15,0,3,8,6 };
	Hitboxes[HB_RIGHT] = { 5,15,0,3,3,6 };
	Hitboxes[HB_LEFT] = { -5,15,0,1,3,6 };
	pCurrentFloor = nullptr;
	nNextTransform = nCurrentTransformation = MODEL_NINJA;
	nState = PLAYER_IDLE;
	pPlayerModels[MODEL_SAMURAI]->SetRotationY(90 - (XM_PI*0.65f));
	pPlayerModels[MODEL_GEISHA]->SetRotationY(90 - (XM_PI*0.65f));
	nMaxHP = nHP = INIT_HP;
	nMaxStamina = nStamina = INIT_STAMINA;
	RightWall = nullptr;
	LeftWall = nullptr;
	LockMovementRight = 1;
	LockMovementLeft = 1;
	nCurrentAnimation = 0;
	bUsingDebugAim = false;
	pMainCamera = GetMainCamera();
	pDebugAim = new DebugAim();
	pDebugAim->SetPosition(Position);
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
	GameObject3D::Update();
	if (bUsingDebugAim)
	{
		if(pDebugAim)
			pDebugAim->Update();

		if (GetInput(INPUT_DEBUGAIM)) {
			GetMainCamera()->SetFocalPoint(this);
			bUsingDebugAim = false;
		}
		return;
	}
	if(pDebugAim)
		pDebugAim->SetPosition(Position);
	if (GetInput(INPUT_DEBUGAIM)) {
		GetMainCamera()->SetFocalPoint(pDebugAim);
		bUsingDebugAim = true;
	}
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
			}
			else 
			{
				LockMovementLeft = M_UNLOCKED;
				LeftWall = nullptr;
			}
		}
	}
	for (int i = 0; i < PLAYER_MODELS_MAX; i++)
		pPlayerModels[i]->UpdateModel();
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
	// ƒJƒƒ‰‚ÌŒü‚«Žæ“¾
	rotCamera = pMainCamera->GetCameraAngle();
	static int NumTest = 2;
	static int nKickWallFrameCounter=0;
	if (nState != PLAYER_KICK_WALL_STATE) 
		nKickWallFrameCounter = 0;
	switch (nState)
	{
	case PLAYER_IDLE:
		SwitchAnimationSpeed(5);
		if (!pCurrentFloor) 
		{
			if (LockMovementRight == M_LOCKED || LockMovementLeft == M_LOCKED) 
			{
				if(!GetInput(INPUT_NINJACRAWL_UP))
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
		break;
	case PLAYER_WALKING:
		SwitchAnimationSpeed(5);
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
			SwitchAnimation(MODEL_GEISHA, GEISHA_WALKING);
			SwitchAnimation(MODEL_SAMURAI, SAMURAI_WALKING);
		}
		if(nCurrentTransformation==MODEL_NINJA)
			Position.x += PLAYER_SPEED * 0.75f * nDirection*LockMovementRight*LockMovementLeft;
		else if (nCurrentTransformation == MODEL_SAMURAI)
			Position.x += PLAYER_SPEED * 0.15f * nDirection*LockMovementRight*LockMovementLeft;
		else if (nCurrentTransformation == MODEL_GEISHA)
			Position.x += PLAYER_SPEED * 0.35f * nDirection*LockMovementRight*LockMovementLeft;
	
		break;
	case PLAYER_TRANSFORMING:
		TrasnformingStateControl();
		break;
	case PLAYER_ATTACKING:
		//printf("%d\n", pPlayerModels[nCurrentTransformation]->GetCurrentFrame());
		if (pPlayerModels[nCurrentTransformation]->GetLoops() > 0)
			pCurrentAttackPlaying = nullptr;
		if (!pCurrentAttackPlaying)
			nState = PLAYER_IDLE;
		break;
	case PLAYER_KICK_WALL_STATE:
		SwitchAnimationSpeed(3);
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

void Player3D::PlayerInputsControl(bool bIsLocked)
{
	if (nState == PLAYER_KICK_WALL_STATE)
		return;
	if ((GetInput(INPUT_JUMP) && pCurrentFloor) || (GetInput(INPUT_JUMP) && (LockMovementLeft == M_LOCKED || LockMovementRight == M_LOCKED)))
	{
		pCurrentAttackPlaying = nullptr;
		if(pCurrentFloor)
			Jump(JUMP_FORCE);
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
	if (GetInput(TRANSFORM_GEISHA))
	{
		if (nNextTransform != nCurrentTransformation)
			nState = PLAYER_TRANSFORMING;
		nNextTransform = MODEL_GEISHA;
	}
	else if (GetInput(TRANSFORM_SAMURAI))
	{
		if (nNextTransform != nCurrentTransformation)
			nState = PLAYER_TRANSFORMING;
		nNextTransform = MODEL_SAMURAI;
	}
	else {
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

void Player3D::TrasnformingStateControl()
{
	static float fTransformAcceleration = 0;
	static bool bCurrentModelFinished = false;
	static int nCooldownInbetween=0;
	if (!bCurrentModelFinished) {
		if (pPlayerModels[nCurrentTransformation]->GetScale().x > 0)
			pPlayerModels[nCurrentTransformation]->ReduceScaleOnX(0.035f);
		if (pPlayerModels[nCurrentTransformation]->GetRotation().y > 90 - (XM_PI*0.65f)) {
			pPlayerModels[nCurrentTransformation]->RotateAroundY(fTransformAcceleration);
			fTransformAcceleration -= 0.035f;
		}
		else if (pPlayerModels[nCurrentTransformation]->GetRotation().y <= 90 - (XM_PI*0.65f)) {
			pPlayerModels[nCurrentTransformation]->SetRotationY(90 - (XM_PI*0.65f));
			fTransformAcceleration = 0;
			bCurrentModelFinished = pPlayerModels[nCurrentTransformation]->GetRotation().y == 90 - (XM_PI*0.65f) && fTransformAcceleration == 0;
		}
	}
	else {
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
	if (nCurrentTransformation == nNextTransform)
		nState = PLAYER_IDLE;
}

void Player3D::Jump(float fJumpForce)
{
	if (pCurrentFloor) {
		while (IsInCollision3D(pCurrentFloor->GetHitBox(), GetHitBox(HB_FEET)))
			Position.y += 0.01;
	}
	f_yForce = -fJumpForce;
	pCurrentFloor = nullptr;
}

void Player3D::GravityControl()
{
	if (nState == PLAYER_TRANSFORMING || nState == PLAYER_ATTACKING)
		return;
	if (pCurrentFloor)
	{
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
			if ((LockMovementRight == M_LOCKED || LockMovementLeft == M_LOCKED) && nCurrentTransformation == MODEL_NINJA && !GetInput(INPUT_DOWN))
			{
				if (GetInput(INPUT_NINJACRAWL_UP))
					Position.y++;
					return;
				if (f_yForce > 5)
					f_yForce = 5;
				f_yForce += GRAVITY_FORCE;
				if (f_yForce < 0)
					Position.y -= f_yForce;
				else
					Position.y -= f_yForce * 0.025f;
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

void Player3D::SwitchAnimationSpeed(int nModel, int nSpeed)
{
	pPlayerModels[nModel]->SwitchAnimationSpeed(nSpeed);
}

void Player3D::SwitchAnimationSpeed(int nSpeed)
{
	for (int i = 0; i < PLAYER_MODELS_MAX; i++)
		pPlayerModels[i]->SwitchAnimationSpeed(nSpeed);
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
	Attack(szAtkInput, MAX_PLAYER_INPUT);
}

void Player3D::Attack(const char * atkInput, int recursions)
{
	if (recursions <= 0)
		return;
	for (int i = 0; i < MAX_ATTACKS; i++)
	{
		if (nCurrentTransformation != stAllMoves[i].Transformation)continue;
		if (stAllMoves[i].eAirMove == AIR_MOVE && pCurrentFloor)continue;
		if (stAllMoves[i].eAirMove == GROUND_MOVE && !pCurrentFloor)continue;
		if (!strcmp(stAllMoves[i].Input, atkInput))//UŒ‚“®ìŒ©‚Â‚¯‚½
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
	for (i = 1; i < MAX_PLAYER_INPUT && atkInput[i] != '\0'; szAtkInput[i-1] = atkInput[i], i++);
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
