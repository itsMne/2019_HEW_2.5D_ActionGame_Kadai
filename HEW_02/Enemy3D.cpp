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
		bUnlit = true;
		InitModel(ONI_MODEL_PATH);
		pModel->SetScale({ 0.75f,0.75f,0.75f });
		pModel->SetPositionZ(-10);
		
		break;
	default:
		break;
	}
}

void Enemy3D::Update()
{
	GameObject3D::Update();

}

void Enemy3D::Draw()
{
	GameObject3D::Draw();
}

void Enemy3D::Uninit()
{
	GameObject3D::Uninit();
}
