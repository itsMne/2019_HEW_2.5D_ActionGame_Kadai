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
	UI_LEVEL_EDITOR,
	UI_LEVEL_EDITOR_MOVEABLEMODE,
	UI_LEVEL_EDITOR_STATICMODE,
	UI_LEVEL_EDITOR_OBJSPEED,
	UI_LEVEL_EDITOR_DELAY,
	UI_TITLE,
	UI_GAMEOVER,
	UI_CLEAR,
	UI_RANKING,
	MAXUITYPE,
};
class C_Ui :public Polygon2D
{
private:
	// ÉÅÉìÉoïœêî
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
	//ÉÅÉìÉoä÷êî
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Draw(XMFLOAT2* pPos, unsigned uNumber, int nWidth,
		float fSizeX, float fSizeY);
};

