#include "Wall3D.h"
#include "Cube3D.h"
#include "Player3D.h"



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
	GameObject3D::InitModel(WALL_MODEL);
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
#if USE_IN_RENDERZONE
	if (pPlayer) {
		if (!(GetMainCamera()->IsOnRenderZone(GetHitBox())))
			return;
	}
#endif
	
	if (!pPlayer)
		return;

	PLAYER_ATTACK_MOVE* pAttack = pPlayer->GetPlayerAttack();
	if (pAttack && pAttack->Animation == SAMURAI_STINGER)
	{
		while (IsInCollision3D(pPlayer->GetHitBox(HB_ATTACK), GetHitBox())) {
			pPlayer->TranslateX(-(pPlayer->GetDirection())*0.01f);
			pPlayer->CancelAttack();
		}
	}
	if (IsInCollision3D(pPlayer->GetHitBox(HB_HEAD), GetHitBox()))
	{
		pPlayer->SetYForce(0);
		return;
	}
	if (IsInCollision3D(pPlayer->GetHitBox(HB_RIGHT), GetHitBox()))
	{
		pPlayer->SetRightWall(this);
		if (pPlayer->GetPlayerAttack())
			if (pPlayer->GetPlayerAttack()->Animation == SAMURAI_STINGER)
				pPlayer->CancelAttack();
	}
	if (IsInCollision3D(pPlayer->GetHitBox(HB_LEFT), GetHitBox()))
	{
		pPlayer->SetLeftWall(this);
		if (pPlayer->GetPlayerAttack())
			if (pPlayer->GetPlayerAttack()->Animation == SAMURAI_STINGER)
				pPlayer->CancelAttack();
	}
}

void Wall3D::Draw()
{
#if USE_IN_RENDERZONE
	if (!bIgnoreRenderingZone) {
		if (!(GetMainCamera()->IsOnRenderZone(GetHitBox())))
			return;
	}
#endif
	GetDeviceContext()->RSSetState(GetMainWindow()->GetRasterizerState(2));
	GameObject3D::Draw();
	GetDeviceContext()->RSSetState(GetMainWindow()->GetRasterizerState(1));
}

void Wall3D::Uninit()
{
	GameObject3D::Uninit();
}
