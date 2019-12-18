#include "C_Item.h"
#include "Player3D.h"



C_Item::C_Item(): GameObject3D()
{
	Init();
}

C_Item::~C_Item()
{
	Uninit();
}

void C_Item::Init()
{
	bbItem = new Billboard2D("data/texture/Sushi.png");
	bbItem->SetVertex(590/40, 366/40);
	hitbox = { 0,0,0, 3,3,3 };
	nType = GO_ITEM;
	bUse = true;
}

void C_Item::Update()
{
	GameObject3D::Update();
	if (!bUse)
		return;
	bbItem->SetPosition(Position);
	bbItem->SetRotation(Rotation);
	Rotation.y += 0.025f;
	Player3D* pPlayer = GetMainPlayer();
	if (!pPlayer)
		return;
	if (IsInCollision3D(pPlayer->GetHitBox(HB_BODY), GetHitBox())) 
	{
		bUse = false;
		pPlayer->RiseHP(10);
	}
}

void C_Item::Draw()
{
	GameObject3D::Draw();
	if (!bUse)
		return;
	bbItem->Draw();
}

void C_Item::Uninit()
{
	GameObject3D::Uninit();
}
