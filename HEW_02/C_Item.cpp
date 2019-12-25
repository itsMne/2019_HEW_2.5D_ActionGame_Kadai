#include "C_Item.h"
#include "Player3D.h"
#include "SceneGame.h"



C_Item::C_Item(int Type)
{
	nItemType = Type;
	Init();
}

C_Item::~C_Item()
{
}

void C_Item::Init()
{
	hitbox = { 0,5,0, 5,5,5 };
	switch (nItemType)
	{
	case TYPE_SUSHI:
		bbItem = new Billboard2D("data/texture/Sushi.png");
		bbItem->SetVertex(590 / 40, 366 / 40);
		nType = GO_ITEM;
		bUse = true;
		break;
	case TYPE_UDON:
		bbItem = new Billboard2D("data/texture/Udon.png");
		bbItem->SetVertex(591 / 40, 513 / 40);
		nType = GO_ITEM;
		bUse = true;
		break;
	case TYPE_ODEN:
		bbItem = new Billboard2D("data/texture/ODen.png");
		bbItem->SetVertex(589 / 40, 519 / 40);
		nType = GO_ITEM;
		bUse = true;
		break;
	case TYPE_DANGO:
		bbItem = new Billboard2D("data/texture/Dango.png");
		bbItem->SetVertex(500 / 40, 550 / 40);
		nType = GO_ITEM;
		bUse = true;
		break;
	case TYPE_TAI:
		bbItem = new Billboard2D("data/texture/Tai.png");
		bbItem->SetVertex(550 / 40, 452 / 40);
		nType = GO_ITEM;
		bUse = true;
		break;
	}

}

void C_Item::Update()
{
	GameObject3D::Update();
	Player3D* pPlayer = GetMainPlayer();
#if USE_IN_RENDERZONE
	if (!(GetMainCamera()->IsOnRenderZone(GetHitBox())) && !pPlayer->IsDebugAimOn())
		return;
#endif
	if (!bUse)
		return;
	bbItem->SetPosition(Position);
	bbItem->SetRotation(Rotation);
	Rotation.y += 0.025f;
	if (Rotation.y > 3.14 * 2)
		Rotation.y = 0;

	SceneGame* pGame= GetCurrentGame();
	if (!pPlayer)
		return;
	if (IsInCollision3D(pPlayer->GetHitBox(HB_BODY), GetHitBox()))
	{
		bUse = false;
		switch (nItemType)
		{
		case TYPE_SUSHI:
			pPlayer->RiseHP(10);
			if (pGame)
				pGame->RaiseScore(10);
			break;
		case TYPE_UDON:
			pPlayer->RiseHP(30);
			if (pGame)
				pGame->RaiseScore(30);
			break;
		case TYPE_ODEN:
			pPlayer->RiseHP(50);
			if (pGame)
				pGame->RaiseScore(50);
			break;
		case TYPE_DANGO:
			pPlayer->RiseHP(70);
			if (pGame)
				pGame->RaiseScore(70);
			break;
		case TYPE_TAI:
			pPlayer->RiseHP(20);
			if (pGame)
				pGame->RaiseScore(20);
			break;
		}

	}
}

void C_Item::Draw()
{
	Player3D* pPlayer = GetMainPlayer();
#if USE_IN_RENDERZONE
	if (!(GetMainCamera()->IsOnRenderZone(GetHitBox())) && !pPlayer->IsDebugAimOn())
		return;
#endif
	GameObject3D::Draw();
	if (!bUse)
		return;
	SetCullMode(CULLMODE_NONE);
	bbItem->Draw();
}

void C_Item::Uninit()
{
}

void C_Item::SetUse(bool use)
{
	bUse = use;
}

int C_Item::GetItemType()
{
	return nItemType;
}
