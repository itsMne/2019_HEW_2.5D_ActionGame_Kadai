#pragma once
#include "Polygon2D.h"
#include "Player3D.h"
enum UITYPE
{
	UI_HP00=0,
	UI_HP01,
	UI_MP,
	UI_Menu,
	MAXUITYPE,
};
class C_Ui:public Polygon2D
{
private:
	// ƒƒ“ƒo•Ï”
	int nType;
	Player3D* pPlayer;
	int nHP, nMaxHP;
	int nMP, nMaxMP;
public:
	C_Ui();
	C_Ui(const char*, int);
	~C_Ui();

	//ƒƒ“ƒoŠÖ”
	void Init();
	void Uninit();
	void Update();
	void Draw();
};

