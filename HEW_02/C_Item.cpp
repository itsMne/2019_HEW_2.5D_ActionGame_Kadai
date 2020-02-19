//*****************************************************************************
// C_Item.cpp
// アイテムオブジェクトの管理
//*****************************************************************************
#include "C_Item.h"
#include "Player3D.h"
#include "Sound.h"
#include "Texture.h"
#include "SceneGame.h"


//*****************************************************************************
// グローバル変数
//*****************************************************************************
ID3D11ShaderResourceView* pItemTextures[MAX_ITEMTYPE];
char pItemTexturesPaths[MAX_ITEMTYPE][256] = 
{
	"data/texture/Sushi.png",
	"data/texture/Udon.png",
	"data/texture/ODen.png",
	"data/texture/Dango.png",
	"data/texture/Tai.png",
};

//*****************************************************************************
// コンストラクタ関数
//*****************************************************************************
C_Item::C_Item(int Type)
{
	nItemType = Type;
	Init();
}

C_Item::~C_Item()
{
	GameObject3D::~GameObject3D();
	SAFE_DELETE(bbItem);
}

//*****************************************************************************
//Init関数
//初期化関数
//引数：void
//戻：void
//*****************************************************************************
void C_Item::Init()
{
	hitbox = { 0,5,0, 5,5,5 };
	ID3D11Device* pDevice = GetDevice();
	if (!pItemTextures[nItemType])
	{
		printf("%s\n", pItemTexturesPaths[nItemType]);
		CreateTextureFromFile(pDevice,					// デバイスへのポインタ
			pItemTexturesPaths[nItemType],		// ファイルの名前
			&(pItemTextures[nItemType]));
	}
	switch (nItemType)
	{
	case TYPE_SUSHI://寿司
		bbItem = new Billboard2D(pItemTextures[TYPE_SUSHI]);
		bbItem->SetVertex(590 / 40, 366 / 40);
		nType = GO_ITEM;
		bUse = true;
		break;
	case TYPE_UDON://うどん
		bbItem = new Billboard2D(pItemTextures[TYPE_UDON]);
		bbItem->SetVertex(591 / 40, 513 / 40);
		nType = GO_ITEM;
		bUse = true;
		break;
	case TYPE_ODEN://おでん
		bbItem = new Billboard2D(pItemTextures[TYPE_ODEN]);
		bbItem->SetVertex(589 / 40, 519 / 40);
		nType = GO_ITEM;
		bUse = true;
		break;
	case TYPE_DANGO://団子
		bbItem = new Billboard2D(pItemTextures[TYPE_DANGO]);
		bbItem->SetVertex(500 / 40, 550 / 40);
		nType = GO_ITEM;
		bUse = true;
		break;
	case TYPE_TAI://タイ
		bbItem = new Billboard2D(pItemTextures[TYPE_TAI]);
		bbItem->SetVertex(550 / 40, 452 / 40);
		nType = GO_ITEM;
		bUse = true;
		break;
	}

}

//*****************************************************************************
//Update関数
//変更関数
//引数：void
//戻：void
//*****************************************************************************
void C_Item::Update()
{
	GameObject3D::Update();
	Player3D* pPlayer = GetMainPlayer();
#if USE_IN_RENDERZONE
	if (!(GetMainCamera()->IsOnRenderZone(GetHitBox())))
		return;
#endif
	if (!bUse)
		return;
	if (bbItem) {
		bbItem->SetPosition(Position);
		bbItem->SetRotation(Rotation);
	}
	Rotation.y += 0.025f;
	if (Rotation.y > 3.14 * 2)
		Rotation.y = 0;

	SceneGame* pGame= GetCurrentGame();
	if (!pPlayer)
		return;
	if (pPlayer) {
		if (pPlayer->IsDebugAimOn())
			return;
	}
	if (IsInCollision3D(pPlayer->GetHitBox(HB_BODY), GetHitBox()) && pPlayer->GetState()!=PLAYER_TELEPORTING)
	{
		bUse = false;
		PlaySoundGame(SOUND_LABEL_SE_RECOVER);
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

//*****************************************************************************
//Draw関数
//レンダリング関数
//引数：void
//戻：void
//*****************************************************************************
void C_Item::Draw()
{
#if USE_IN_RENDERZONE
	if (!bIgnoreRenderingZone) {
		if (!(GetMainCamera()->IsOnRenderZone(GetHitBox())))
			return;
	}
#endif
	GameObject3D::Draw();
	if (!bUse)
		return;
	SetCullMode(CULLMODE_NONE);
	bbItem->Draw();
}

//*****************************************************************************
//Uninit関数
//終了関数
//引数：void
//戻：void
//*****************************************************************************
void C_Item::Uninit()
{
	SAFE_DELETE(bbItem);
}

//*****************************************************************************
//SetUse関数
//使っていることを設定する
//引数：bool
//戻：void
//*****************************************************************************
void C_Item::SetUse(bool use)
{
	bUse = use;
}

//*****************************************************************************
//GetItemType関数
//アイテムの種類を戻す
//引数：void
//戻：int
//*****************************************************************************
int C_Item::GetItemType()
{
	return nItemType;
}
