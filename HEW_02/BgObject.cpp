//*****************************************************************************
// BgObject.cpp
//バックグラウンドのオブジェクトの管理
//*****************************************************************************
#include "BgObject.h"
#include "Texture.h"
#include "SceneGame.h"
#include "Player3D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define KOMA_FRAME_X		5	// コマの横フレーム数
#define KOMA_FRAME_Y		1	// コマの縦フレーム数
#define UTIMIZU_FRAME_X		3	// 打ち水の横フレーム数
#define UTIMIZU_FRAME_Y		1	// 打ち水の縦フレーム数
#define FURIN_FRAME_X		9	// 風鈴の横フレーム数
#define FURIN_FRAME_Y		1	// 風鈴の縦フレーム数
#define FURIN_HANABI_X		7	// 花火の横フレーム数
#define FURIN_HANABI_Y		1	// 風鈴の縦フレーム数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
ID3D11ShaderResourceView* pBgObjectTextures[MAX_BGOBJECTTYPE];
char pBgObjectTexturesPaths[MAX_BGOBJECTTYPE][256] =
{
	"data/texture/higanbana.tga",
	"data/texture/higanbanas.png",
	"data/texture/Hichifukuzin.png",
	"data/texture/Bonsai.png",
	"data/texture/Tree00.png",
	"data/texture/Tree01.png",
	"data/texture/Tree02.png",
	"data/texture/Tree03.png",
	"data/texture/Sasa.png",
	"data/texture/Hanabi.png",
	"data/texture/Bunbukutyagama.png",
	"data/texture/Koma.png",
	"data/texture/Utimizu.png",
	"data/texture/Furin.png",
};

//*****************************************************************************
// コンストラクタ関数
//*****************************************************************************
BgObject::BgObject(int ty): GameObject3D()
{
	nType = GO_BGOBJ;
	nBGType = ty;
	Init();
}


BgObject::~BgObject()
{
	GameObject3D::~GameObject3D();
	SAFE_DELETE(bBgObject);
}

//*****************************************************************************
//Init関数
//初期化関数
//引数：void
//戻：void
//*****************************************************************************
void BgObject::Init()
{
	if (!pBgObjectTextures[nBGType])
	{
		printf("%s\n", pBgObjectTexturesPaths[nBGType]);
		CreateTextureFromFile(GetDevice(),					// デバイスへのポインタ
			pBgObjectTexturesPaths[nBGType],		// ファイルの名前
			&(pBgObjectTextures[nBGType]));
	}

	switch (nBGType)
	{
	case HIGANBANA:
		bBgObject = new Billboard2D(pBgObjectTextures[HIGANBANA]);
		bBgObject->SetVertex(374 / 40, 359 / 40);
		nType = GO_BGOBJ;
		bUse = true;
		hitbox = { 0,3,0,5,5,5 };
		break;
	case HIGANBANAS:
		bBgObject = new Billboard2D(pBgObjectTextures[HIGANBANAS]);
		bBgObject->SetVertex(1920 / 40, 717 / 40);
		nType = GO_BGOBJ;
		bUse = true;
		hitbox = { 0,8,0,20,7,5 };
		break;
	case HICHIFUKUZIN:
		bBgObject = new Billboard2D(pBgObjectTextures[HICHIFUKUZIN]);
		bBgObject->SetVertex(1500 / 40, 1500 / 40);
		nType = GO_BGOBJ;
		bUse = true;
		hitbox = { 0,10,0,10,20,5 };
		break;
	case BONSAI:
		bBgObject = new Billboard2D(pBgObjectTextures[BONSAI]);
		bBgObject->SetVertex(900 / 40, 600 / 40);
		nType = GO_BGOBJ;
		bUse = true;
		hitbox = { 2,8,0,5,5,5 };
		break;
	case TREE00:
		bBgObject = new Billboard2D(pBgObjectTextures[TREE00]);
		bBgObject->SetVertex(1840 / 40, 3400 / 40);
		nType = GO_BGOBJ;
		bUse = true;
		hitbox = { 0,37,0,20,30,5 };
		break;
	case TREE01:
		bBgObject = new Billboard2D(pBgObjectTextures[TREE01]);
		bBgObject->SetVertex(3400 / 40, 3400 / 40);
		nType = GO_BGOBJ;
		bUse = true;
		hitbox = { 0,40,0,30,30,5 };
		break;
	case TREE02:
		bBgObject = new Billboard2D(pBgObjectTextures[TREE02]);
		bBgObject->SetVertex(3400 / 40, 3400 / 40);
		nType = GO_BGOBJ;
		bUse = true;
		hitbox = { 0,40,0,30,30,5 };
		break;
	case TREE03:
		bBgObject = new Billboard2D(pBgObjectTextures[TREE03]);
		bBgObject->SetVertex(3400 / 40, 3400 / 40);
		nType = GO_BGOBJ;
		bUse = true;
		hitbox = { 0,40,0,30,30,5 };
		break;
	case SASA:
		bBgObject = new Billboard2D(pBgObjectTextures[SASA]);
		bBgObject->SetVertex(3400 / 40, 3400 / 40);
		nType = GO_BGOBJ;
		bUse = true;
		hitbox = { 0,40,0,35,30,5 };
		break;
	case HANABI:
		bBgObject = new Billboard2D(pBgObjectTextures[HANABI]);
		bBgObject->SetVertex(1228 / 40, 1400 / 40);
		nType = GO_BGOBJ;
		bUse = true;
		hitbox = { 0,15,0,5,10,5 };
		break;
	case BUMBUKUTYAGAMA:
		bBgObject = new Billboard2D(pBgObjectTextures[BUMBUKUTYAGAMA]);
		bBgObject->SetVertex(728 / 40, 900 / 40);
		nType = GO_BGOBJ;
		bUse = true;
		hitbox = { 0,12,0,5,5,5 };
		break;
	case KOMA:
		bBgObject = new Billboard2D(pBgObjectTextures[KOMA]);
		bBgObject->SetVertex(328 / 40, 450/ 40);
		nType = GO_BGOBJ;
		bUse = true;
		for (int i = 0; i < MAX_KOMA; ++i) {
			g_koma[i].nAnimeIdx = 0;
			g_koma[i].nAinmCnt = 5;
			continue;
		}
		hitbox = { 0,5,0,5,5,5 };
		break;
	case UTIMIZU:
		bBgObject = new Billboard2D(pBgObjectTextures[UTIMIZU]);
		bBgObject->SetVertex(1400 / 40, 1159/ 40);
		nType = GO_BGOBJ;
		bUse = true;
		for (int i = 0; i < MAX_UTIMIZU; ++i) {
			g_utimizu[i].nAnimeIdx = 0;
			g_utimizu[i].nAinmCnt = 20;
			continue;
		}
		hitbox = { 0,15,0,5,10,5 };
		break;
	case FURIN:
		bBgObject = new Billboard2D(pBgObjectTextures[FURIN]);
		bBgObject->SetVertex(311 / 40, 355/ 40);
		nType = GO_BGOBJ;
		bUse = true;
		for (int i = 0; i < MAX_FURIN; ++i) {
			g_furin[i].nAnimeIdx = 0;
			g_furin[i].nAinmCnt = 10;
			continue;
		}
		hitbox = { 0,0,0,5,5,5 };
		break;
	}
}

//*****************************************************************************
//Update関数
//変更関数
//引数：void
//戻：void
//*****************************************************************************
void BgObject::Update()
{
	GameObject3D::Update();
#if USE_IN_RENDERZONE
	if (!(GetMainCamera()->IsOnRenderZone(GetHitBox())))
		return;
#endif
	if (!bUse)
		return;
	bBgObject->SetPosition(XMFLOAT3(Position.x, Position.y, Position.z + 10));
	bBgObject->SetRotation(Rotation);

	switch (nBGType)
	{
	case KOMA:
		for (int i = 0; i < MAX_KOMA; ++i) {
			if (!bUse) {
				continue;
			}
			--g_koma[i].nAinmCnt;
			if (g_koma[i].nAinmCnt <= 0) {
				++g_koma[i].nAnimeIdx;
				if (g_koma[i].nAnimeIdx >= KOMA_FRAME_X) {
					g_koma[i].nAnimeIdx = 0;
					continue;
				}
				g_koma[i].nAinmCnt = 5;
			}
		}
		break;
	case UTIMIZU:
		for (int i = 0; i < MAX_UTIMIZU; ++i) {
			if (!bUse) {
				continue;
			}
			--g_utimizu[i].nAinmCnt;
			if (g_utimizu[i].nAinmCnt <= 0) {
				++g_utimizu[i].nAnimeIdx;
				if (g_utimizu[i].nAnimeIdx >= UTIMIZU_FRAME_X) {
					g_utimizu[i].nAnimeIdx = 0;
					continue;
				}
				g_utimizu[i].nAinmCnt = 20;
			}
		}
		break;
	case FURIN:
		for (int i = 0; i < MAX_FURIN; ++i) {
			if (!bUse) {
				continue;
			}
			--g_furin[i].nAinmCnt;
			if (g_furin[i].nAinmCnt <= 0) {
				++g_furin[i].nAnimeIdx;
				if (g_furin[i].nAnimeIdx >= FURIN_FRAME_X) {
					g_furin[i].nAnimeIdx = 0;
					continue;
				}
				g_furin[i].nAinmCnt = 10;
			}
		}
		break;
	case HANABI:
		for (int i = 0; i < MAX_HANABI; ++i) {
			if (!bUse) {
				continue;
			}
			--g_hanabi[i].nAinmCnt;
			if (g_hanabi[i].nAinmCnt <= 0) {
				++g_hanabi[i].nAnimeIdx;
				if (g_hanabi[i].nAnimeIdx >= FURIN_HANABI_X) {
					g_hanabi[i].nAnimeIdx = 0;
					continue;
				}
				g_hanabi[i].nAinmCnt = 10;
			}
		}
		break;

	}
}

//*****************************************************************************
//Draw関数
//レンダリング関数
//引数：void
//戻：void
//*****************************************************************************
void BgObject::Draw()
{
#if USE_IN_RENDERZONE
	if (!bIgnoreRenderingZone) {
		if (!(GetMainCamera()->IsOnRenderZone(GetHitBox())))
			return;
	}
#endif

	switch (nBGType)
	{
	case KOMA:
		for (int i = 0; i < MAX_KOMA; ++i) {
			if (!bUse) {
				continue;
			}
			bBgObject->SetUVFrames(KOMA_FRAME_X, KOMA_FRAME_Y);
			bBgObject->SetUV(g_koma[i].nAnimeIdx % KOMA_FRAME_X, g_koma[i].nAnimeIdx / KOMA_FRAME_X);
		}
		break;
	case UTIMIZU:
		for (int i = 0; i < MAX_UTIMIZU; ++i) {
			if (!bUse) {
				continue;
			}
			bBgObject->SetUVFrames(UTIMIZU_FRAME_X, UTIMIZU_FRAME_Y);
			bBgObject->SetUV(g_utimizu[i].nAnimeIdx % UTIMIZU_FRAME_X, g_utimizu[i].nAnimeIdx / UTIMIZU_FRAME_X);
		}
		break;
	case FURIN:
		for (int i = 0; i < MAX_FURIN; ++i) {
			if (!bUse) {
				continue;
			}
			bBgObject->SetUVFrames(FURIN_FRAME_X, FURIN_FRAME_Y);
			bBgObject->SetUV(g_furin[i].nAnimeIdx % FURIN_FRAME_X, g_furin[i].nAnimeIdx / FURIN_FRAME_X);
		}
		break;
	case HANABI:
		for (int i = 0; i < MAX_HANABI; ++i) {
			if (!bUse) {
				continue;
			}
			bBgObject->SetUVFrames(FURIN_HANABI_X, FURIN_HANABI_Y);
			bBgObject->SetUV(g_hanabi[i].nAnimeIdx % FURIN_HANABI_X, g_hanabi[i].nAnimeIdx / FURIN_HANABI_X);
		}
		break;
	}

	GameObject3D::Draw();
	if (!bUse)
		return;
	SetCullMode(CULLMODE_NONE);
	bBgObject->Draw();
}

//*****************************************************************************
//Uninit関数
//終了関数
//引数：void
//戻：void
//*****************************************************************************
void BgObject::Uninit()
{
	SAFE_DELETE(bBgObject);
}

//*****************************************************************************
//SetUse関数
//使うことを設定する
//引数：bool
//戻：void
//*****************************************************************************
void BgObject::SetUse(bool use)
{
	bUse = use;
}

//*****************************************************************************
//GetBgObjectType関数
//バックグラウンドオブジェクトの種類を戻す
//引数：void
//戻：int
//*****************************************************************************
int BgObject::GetBgObjectType()
{
	return nBGType;
}

