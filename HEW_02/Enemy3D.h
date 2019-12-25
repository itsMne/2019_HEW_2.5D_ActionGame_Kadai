#pragma once
#include "GameObject3D.h"
#include "Field3D.h"
//#include "SceneGame.h"
enum EnemyType
{
	TYPE_ONI,
	MAX_ENEMY_TYPE
};
class Enemy3D :
	public GameObject3D
{
private:
	int nEnemyType;
	float fYForce;
	Field3D* pCurrentFloor;
	bool bUseGravity;
	int nState;
	void* pPlayerPointer;
	int nCancelGravityFrames;
	int nDirection;
	int nLastPlayerAttack;
	bool bUseDamageA;
public:
	Enemy3D(int enemyType);
	~Enemy3D();
	void Init();
	void Update();
	void OniStatesControl();
	void DamageControl();
	void RegularCollisionWithPlayer();
	void GravityControl();
	void Draw();
	void Uninit();
	int GetEnemyType();
};

