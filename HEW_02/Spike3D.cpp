#include "Spike3D.h"
#include "Player3D.h"
#define MODEL_PATH "data/model/Spike.fbx"
#define SPIKE_DAMAGE 10

Spike3D::Spike3D() : GameObject3D()
{
	pSpikeModel = nullptr;
	Init();
}


Spike3D::~Spike3D()
{
	Uninit();
}

void Spike3D::Init()
{
	if (!pSpikeModel) {
		pSpikeModel = new Model3D();
		pSpikeModel->InitModel(MODEL_PATH, this);
	}
	nSpikesOnX = 1;
	nSpikesOnY = 1;
	bInvisible = false;
	nType = GO_SPIKE;
	hitbox = { 0,15,0,5,5,5 };
}

void Spike3D::Update()
{
	GameObject3D::Update();
	Player3D* pPlayer = GetMainPlayer();
	hitbox = { 0,13.5f,0,4.8f * (float)nSpikesOnX,6.5f* (float)nSpikesOnY,5 };
#if USE_IN_RENDERZONE
	if (pPlayer) {
		if (!(GetMainCamera()->IsOnRenderZone(GetHitBox())))
			return;
		if (pPlayer->IsDebugAimOn())
			return;
	}
#endif
	if (nSpikesOnX > 1 || nSpikesOnY > 1) {
		pSpikeModel->SetPositionX(-3.75f);
		pSpikeModel->SetPositionY(2.5f);
	}
	if (pSpikeModel)
		pSpikeModel->UpdateModel();
	if (!pPlayer)
		return;
	if (IsInCollision3D(pPlayer->GetHitBox(HB_BODY), GetHitBox()) && pPlayer->GetState()!=PLAYER_TELEPORTING)
	{
		pPlayer->SetDamageTeleport(SPIKE_DAMAGE);
	}
	
}


void Spike3D::Draw()
{
	if (bInvisible)
		return;
#if USE_IN_RENDERZONE
	if (!bIgnoreRenderingZone) {
		if (!(GetMainCamera()->IsOnRenderZone(GetHitBox())))
			return;
	}
#endif
	GameObject3D::Draw();
	if (nSpikesOnX == 1 && nSpikesOnY==1)
	{
		if (pSpikeModel)
			pSpikeModel->DrawModel();
		return;
	}
	
	XMFLOAT3 tempPos = Position;
	for (float j = -nSpikesOnY *0.5f; j < nSpikesOnY *0.5f; j++)
	{
		Position.y = tempPos.y-13 * j;
		for (float i = -nSpikesOnX * 0.5f; i < nSpikesOnX *0.5f; i++)
		{
			Position.x = tempPos.x-10 * i;
			if (pSpikeModel)
				pSpikeModel->DrawModel();

		}
	}
	Position = tempPos;
}

void Spike3D::Uninit()
{
	GameObject3D::Uninit();
}

void Spike3D::RaiseSpikesX(int rise)
{
	nSpikesOnX += rise;
	if (nSpikesOnX < 1)
		nSpikesOnX = 1;
	hitbox = { 0,13.5f,0,4.8f * (float)nSpikesOnX,6.5f* (float)nSpikesOnY,5 };
}

void Spike3D::RaiseSpikesY(int rise)
{
	nSpikesOnY += rise;
	if (nSpikesOnY < 1)
		nSpikesOnY = 1;
	hitbox = { 0,13.5f,0,4.8f * (float)nSpikesOnX,6.5f* (float)nSpikesOnY,5 };
}

void Spike3D::SetSpikesNum(int x, int y)
{
	nSpikesOnX = x;
	nSpikesOnY = y;
	hitbox = { 0,13.5f,0,4.8f * (float)nSpikesOnX,6.5f* (float)nSpikesOnY,5 };
}

void Spike3D::SetInvisibility(bool invisible)
{
	bInvisible = invisible;
}

XMFLOAT2 Spike3D::GetSpikesNum()
{
	return { (float)nSpikesOnX, (float)nSpikesOnY };
}
