#pragma once
#include "GameObject3D.h"
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
	Model3D* EnemyModel;
public:
	Enemy3D(int enemyType);
	~Enemy3D();
	void Init();
	void Update();
	void Draw();
	void Uninit();
};

