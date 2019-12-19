#pragma once
#include "Polygon2D.h"
#include "Player3D.h"
enum UITYPE
{
	UI_HP00 = 0,
	UI_HP01,
	UI_MP,
	UI_NUMBER,
	UI_SCORE,
	MAXUITYPE,
};
class C_Ui :public Polygon2D
{
private:
	// ƒƒ“ƒo•Ï”
	int nType;
	Player3D* pPlayer;
	int nHP, nMaxHP;
	int nMP, nMaxMP;
	int nScore;
	XMFLOAT2 vScorePos;
public:
	C_Ui();
	C_Ui(const char*, int);
	~C_Ui();

	//ƒƒ“ƒoŠÖ”
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Draw(XMFLOAT2* pPos, unsigned uNumber, int nWidth,
		float fSizeX, float fSizeY);
};

