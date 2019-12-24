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
public:
	Enemy3D(int enemyType);
	~Enemy3D();
	void Init();
	void Update();
	void GravityControl();
	void Draw();
	void Uninit();
	int GetEnemyType();
};

