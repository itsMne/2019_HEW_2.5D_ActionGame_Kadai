#include "Wall3D.h"
#include "Cube3D.h"
#include "Player3D.h"
#define WALL_MODEL_PATH "data/model/Cube.fbx"


Wall3D::Wall3D() : GameObject3D()
{
	Init();
}


Wall3D::~Wall3D()
{
	Uninit();
}

void Wall3D::Init()
{
	GameObject3D::InitModel(WALL_MODEL_PATH);
	pModel->SetScale({ 11,11,11 });
	pModel->SetRotation({ 0,0,0 });
	Scale = { 1,1,1 };
	pModel->SetPositionY(-2.5f);
	Rotation = { 0,0,0 };
	nType = GO_WALL;

}

void Wall3D::Update()
{
	GameObject3D::Update();
	hitbox = { 0,0,0,6 * Scale.x,5.5f * Scale.y,6 * Scale.z };
	Player3D* pPlayer = GetMainPlayer();
	if (!pPlayer)
		return;
	if (IsInCollision3D(pPlayer->GetHitBox(HB_HEAD), GetHitBox()) && pPlayer->GetYForce()<0)
	{
		pPlayer->SetYForce(0);
		return;
	}
	if (IsInCollision3D(pPlayer->GetHitBox(HB_RIGHT), GetHitBox()))
	{
		pPlayer->SetRightWall(this);
	}
	if (IsInCollision3D(pPlayer->GetHitBox(HB_LEFT), GetHitBox()))
	{
		pPlayer->SetLeftWall(this);
	}
}

void Wall3D::Draw()
{
	GetDeviceContext()->RSSetState(GetMainWindow()->GetRasterizerState(2));
	GameObject3D::Draw();
	GetDeviceContext()->RSSetState(GetMainWindow()->GetRasterizerState(1));
}

void Wall3D::Uninit()
{
	GameObject3D::Uninit();
}
