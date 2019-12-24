#pragma once
#include "GameObject3D.h"
#include "UniversalStructures.h"
#include "Cube3D.h"
#include "Wall3D.h"
#include "Field3D.h"
#include "DebugAim.h"
#define MAX_PLAYER_INPUT 8
enum PlayerModels
{
	MODEL_NINJA,
	MODEL_GEISHA,
	MODEL_SAMURAI,
	PLAYER_MODELS_MAX
};
enum PLAYER_HITBOXES
{
	HB_FEET=0,
	HB_BODY,
	HB_RIGHT,
	HB_LEFT,
	HB_HEAD,
	MAX_HB
};
enum PLAYER_STATES
{
	PLAYER_IDLE=0,
	PLAYER_WALKING,
	PLAYER_TRANSFORMING,
	PLAYER_ATTACKING,
	PLAYER_KICK_WALL_STATE,
	PLAYER_TELEPORTING,
	PLAYER_TELEPORTING_DAMAGED,
	PLAYER_DEAD,
	PLAYER_MAX
};
enum AirMove
{
	BOTH_MOVE = 0,
	GROUND_MOVE = 1,
	AIR_MOVE = -1
};

typedef struct PLAYER_ATTACK_MOVE
{
	char Input[MAX_PLAYER_INPUT];
	int Transformation;
	int Animation;
	bool ResetInputs;
	AirMove eAirMove;
	int UnlockFrame;
};
class Player3D :
	public GameObject3D
{
private:
	Model3D* pPlayerModels[PLAYER_MODELS_MAX];
	Camera3D* pMainCamera = nullptr;
	int nCurrentTransformation;//現在の変化
	float f_yForce;
	Hitbox3D Hitboxes[MAX_HB];
	Cube3D* pVisualHitbox[MAX_HB];
	Field3D* pCurrentFloor;
	char szInputs[MAX_PLAYER_INPUT + 1];
	int nState;//プレイヤーの状況
	int nDirection;
	int nNextTransform;
	int nInputTimer;
	int nHP;//体力
	int nMaxHP;//マックス体力
	int nStamina;//スタミナ
	int nMaxStamina;//スタミナ
	PLAYER_ATTACK_MOVE* pCurrentAttackPlaying;
	Wall3D* RightWall;
	Wall3D* LeftWall;
	Wall3D* WallAttachedTo;
	float LockMovementRight;
	float LockMovementLeft;
	int nCurrentAnimation;
	bool bUsingDebugAim;
	DebugAim* pDebugAim;
	bool bStaminaCoolDown;
	int nFrameCountForSafePos;
	XMFLOAT3 x3LastSafePos;
	XMFLOAT3 x3TeleportDestination;
	bool bDeadAnimation;
public:
	Player3D();
	~Player3D();
	void Init();
	void Update();
	void TeleportControl();
	bool DebugAimControl();
	void DeadStateControl();
	void DamagedTeleportingControl();
	void PlayerInputsControl(bool bIsLocked);
	void TransformingStateControl();
	void Jump(float fJumpForce);
	void GravityControl();
	void HitboxControl();
	void Draw();
	void DrawHitboxes();
	void Uninit();
	void SwitchAnimation(int Model, int Animation);
	void SwitchAnimation(int Animation);
	void SwitchAnimationSlowness(int Model, float nSpeed);
	void SwitchAnimationSlowness(float nSpeed);
	void SwitchAnimationSpeed(float nSpeed);
	void SwitchAnimationSpeed(int Model, float nSpeed);
	Hitbox3D GetHitBox(int hitbox);
	Field3D* GetFloor();
	int GetDirection();
	void TranslateX(float x);
	void TranslateY(float);
	void SetFloor(Field3D* floor);
	void AddInput(char A);
	char GetLastInputInserted();
	void ResetInputs();
	void Attack(const char* atkInput);
	void Attack(const char* atkInput, int recursions);
	void SetLeftWall(Wall3D*);
	void SetRightWall(Wall3D*);
	int GetPlayerHp();
	int GetPlayerMaxHp();
	int GetPlayerMp();
	int GetPlayerMaxMp();
	void RiseHP(int nhprise);
	bool IsStaminaCooldownOn();
	void SetDamageTeleport(int Damage);
	float GetYForce();
	void SetYForce(float fyforce);
	int GetState();
	DebugAim* GetDebugAim();
	bool IsDebugAimOn();
	GameObject3D* GetWallCrawling();
	void SetPlayerTeleporting(XMFLOAT3 Destination);
	bool PlayerGameOver();
};

Player3D* GetMainPlayer();