//*****************************************************************************
// Mirror3D.cpp
// 鏡（テレポート）の管理
//*****************************************************************************
#include "Mirror3D.h"
#include "Player3D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_MIRROR "data/model/Mirror.fbx"

//*****************************************************************************
// コンストラクタ関数
//*****************************************************************************
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
	GameObject3D::~GameObject3D();
	Uninit();
}

//*****************************************************************************
//Init関数
//初期化関数
//引数：void
//戻：void
//*****************************************************************************
void Mirror3D::Init(XMFLOAT3 Destination)
{
	x3DestinationPos = Destination;
	InitModel(MODEL_MIRROR);
	pModel->SetScale({ 1, 1, 1 });
	hitbox = { 0,10,0,7,7,5 };
	Position.x += 20;
	nType = GO_MIRROR;
}

//*****************************************************************************
//Update関数
//変更関数
//引数：void
//戻：void
//*****************************************************************************
void Mirror3D::Update()
{
	GameObject3D::Update();
	Player3D* pPlayer = GetMainPlayer();
#if USE_IN_RENDERZONE
	if (!(GetMainCamera()->IsOnRenderZone(GetHitBox())))
		return;
#endif
	if (!pPlayer)
		return;
	if (IsInCollision3D(pPlayer->GetHitBox(HB_BODY), GetHitBox()) && !(pPlayer->IsDebugAimOn())) {
		PLAYER_ATTACK_MOVE* pAttack = pPlayer->GetPlayerAttack();
		if (pAttack)
		{
			if (pAttack->Animation == SAMURAI_STINGER)
				pPlayer->CancelAttack();
		}
		pPlayer->SetPlayerTeleporting(x3DestinationPos);
	}
}

//*****************************************************************************
//Draw関数
//レンダリング関数
//引数：void
//戻：void
//*****************************************************************************
void Mirror3D::Draw()
{
	Player3D* pPlayer = GetMainPlayer();
#if USE_IN_RENDERZONE
	if (!(GetMainCamera()->IsOnRenderZone(GetHitBox())))
		return;
#endif
	GameObject3D::Draw();
}

//*****************************************************************************
//Uninit関数
//終了関数
//引数：void
//戻：void
//*****************************************************************************
void Mirror3D::Uninit()
{
	//なし
}

//*****************************************************************************
//SetDestination関数
//プレイヤーと当たり判定した後で、プレイヤーの拠点を設定する
//引数：XMFLOAT3
//戻：void
//*****************************************************************************
void Mirror3D::SetDestination(XMFLOAT3 newDest)
{
	x3DestinationPos = newDest;
}

//*****************************************************************************
//GetDestination関数
//プレイヤーと当たり判定した後で、プレイヤーの拠点を戻す
//引数：void
//戻：XMFLOAT3
//*****************************************************************************
XMFLOAT3 Mirror3D::GetDestination()
{
	return x3DestinationPos;
}
