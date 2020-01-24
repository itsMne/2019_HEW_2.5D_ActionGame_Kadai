#pragma once
#include "GameObject3D.h"
#include "Billboard2D.h"

#define MAX_KOMA			256
#define MAX_UTIMIZU			256
#define MAX_FURIN			256

enum BgObjectType
{
	HIGANBANA = 0,
	HIGANBANAS,
	HICHIFUKUZIN,
	BONSAI,
	TREE00,
	TREE01,
	TREE02,
	TREE03,
	SASA,
	HANABI,
	BUMBUKUTYAGAMA,
	KOMA,
	UTIMIZU,
	FURIN,
	MAX_BGOBJECTTYPE
};

typedef struct TKOMA {
	int			nAnimeIdx;	// フレームNo.
	int			nAinmCnt;	// カウンタ
};

typedef struct TUTIMIZU {
	int			nAnimeIdx;	// フレームNo.
	int			nAinmCnt;	// カウンタ
};

typedef struct TFURIN {
	int			nAnimeIdx;	// フレームNo.
	int			nAinmCnt;	// カウンタ
};

class BgObject :
	public GameObject3D
{
private:
	// メンバ変数
	Billboard2D * bBgObject;
	int nBGType;
	bool bUse;
	TKOMA g_koma[MAX_KOMA];
	TUTIMIZU g_utimizu[MAX_UTIMIZU];
	TFURIN g_furin[MAX_FURIN];
public:
	BgObject(int ty);
	~BgObject();
	void Init();
	void Update();
	void Draw();
	void Uninit();
	void SetUse(bool);
	int GetBgObjectType();
};

