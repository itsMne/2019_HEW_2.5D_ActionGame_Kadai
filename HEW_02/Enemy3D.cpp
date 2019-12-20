#include "Enemy3D.h"
#define ONI_MODEL_PATH "data/model/Oni.fbx"


Enemy3D::Enemy3D(int enemyType) :GameObject3D()
{
	nEnemyType = enemyType;
	Init();
}


Enemy3D::~Enemy3D()
{
	Uninit();
}

void Enemy3D::Init()
{
	switch (nEnemyType)
	{
	case TYPE_ONI:
		//InitModel(ONI_MODEL_PATH);
		EnemyModel = new Model3D();
		EnemyModel->InitModel(ONI_MODEL_PATH, this);
		EnemyModel->SetScale({ 1,1,1 });
		break;
	default:
		break;
	}
}

void Enemy3D::Update()
{
	GameObject3D::Update();
	//EnemyModel->UpdateModel();
}

void Enemy3D::Draw()
{
	GameObject3D::Draw();
	EnemyModel->DrawModel();
}

void Enemy3D::Uninit()
{
	GameObject3D::Uninit();
}
