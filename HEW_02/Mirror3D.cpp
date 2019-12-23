#include "Mirror3D.h"
#include "Player3D.h"
#define MODEL_MIRROR "data/model/Mirror.fbx"

Mirror3D::Mirror3D(): GameObject3D()
{
	Init({0,0,0});
}

Mirror3D::Mirror3D(XMFLOAT3 Destination)
{
	Init(Destination);
}

Mirror3D::~Mirror3D()
{
	Uninit();
}

void Mirror3D::Init(XMFLOAT3 Destination)
{
	x3DestinationPos = Destination;
	InitModel(MODEL_MIRROR);
	pModel->SetScale({ 1, 1, 1 });
	hitbox = { 0,10,0,7,7,5 };
	Position.x += 20;
}

void Mirror3D::Update()
{
	GameObject3D::Update();

	Player3D* pPlayer = GetMainPlayer();
	if (!pPlayer)
		return;
	if (IsInCollision3D(pPlayer->GetHitBox(HB_BODY), GetHitBox()))
		pPlayer->SetPlayerTeleporting(x3DestinationPos);
}

void Mirror3D::Draw()
{
	GameObject3D::Draw();
}

void Mirror3D::Uninit()
{
}

void Mirror3D::SetDestination(XMFLOAT3 newDest)
{
	x3DestinationPos = newDest;
}
