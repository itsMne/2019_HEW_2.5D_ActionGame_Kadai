#pragma once
#include "GameObject3D.h"
#include "Field3D.h"
//#include "SceneGame.h"
enum EnemyType
{
	TYPE_ONI_A = 0,
	TYPE_ONI_B,
	TYPE_ONI_C,
	TYPE_ONI_D,
	MAX_ENEMY_TYPE
};
enum ENEMY_STATES
{
	ENEMY_IDLE = 0,
	ENEMY_DAMAGED,
	ENEMY_DAMAGEDALT,
	ENEMY_SENDUP,
	ENEMY_FALLING,
	ENEMY_MOVING,
	ENEMY_ATTACKING,
	ENEMY_SENDOFF,
	ENEMY_DEAD,
	MAX_ENEMY_STATES
};
class Enemy3D :
	public GameObject3D
{
private:
	int nEnemyType;
	float fYForce;
	Field3D* pCurrentFloor;
	bool bUseGravity; 
	bool bUse;
	int nState;
	void* pPlayerPointer;
	int nCancelGravityFrames;
	int nDirection;
	int nLastPlayerAttack;
	bool bUseDamageA;
	float fSpeed;
	int nDetectedFrames;
	int nDelayFramesBeforeAttack;
	int nDelayCounter;
	Hitbox3D hbAttack;
	int nPlayerAttackName;
	int nAnimations[MAX_ENEMY_STATES];
	float fAnimationSpeeds[MAX_ENEMY_STATES];
	int nFramesSendOff;
	float fSendOffAcceleration;
	int nMinAttackFrame;
	int nMaxAttackFrame;
	int nDamageAgainstPlayer;
	int nHP;
	int nTopSendOffFrame;
	int nMidSendOffFrame;
	int nUnlitFrames;
public:
	Enemy3D(int enemyType);
	~Enemy3D();
	void Init();
	void Update();
	void EnemyStatesControl();
	void EnemyMovingControl();
	void DamageControl();
	void RegularCollisionWithPlayer();
	void GravityControl();
	void Draw();
	void Uninit();
	int GetEnemyType();
	void SetUnlitForFrames(int frames);
	Hitbox3D GetAttackHitbox();
};

