#include "Spike3D.h"
#include "Player3D.h"
#define MODEL_PATH "data/model/Spike.fbx"


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
	SpikesOnX = 1;
	SpikesOnY = 1;
	hitbox = { 0,15,0,5,5,5 };
}

void Spike3D::Update()
{
	GameObject3D::Update();
	hitbox = { 0,13.5f,0,4.8f * (float)SpikesOnX,6.5f* (float)SpikesOnY,5 };
	if (SpikesOnX > 1 || SpikesOnY > 1) {
		pSpikeModel->SetPositionX(5);
		pSpikeModel->SetPositionY(15);
	}
	if (pSpikeModel)
		pSpikeModel->UpdateModel();
	Player3D* pPlayer = GetMainPlayer();
	if (!pPlayer)
		return;
	if (IsInCollision3D(pPlayer->GetHitBox(HB_BODY), GetHitBox()))
	{
		//TODO: ƒvƒŒƒCƒ„[‚Ì“–‚½‚è”»’è
	}
	
}


void Spike3D::Draw()
{
	GameObject3D::Draw();
	if (SpikesOnX == 1 && SpikesOnY==1)
	{
		if (pSpikeModel)
			pSpikeModel->DrawModel();
		return;
	}
	
	XMFLOAT3 tempPos = Position;
	for (float j = -SpikesOnY *0.5f; j < SpikesOnY *0.5f; j++)
	{
		Position.y = 13 * j;
		for (float i = -SpikesOnX * 0.5f; i < SpikesOnX *0.5f; i++)
		{
			Position.x = 10 * i;
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
