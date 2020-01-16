#pragma once
#include "SceneBase.h"
#include "C_Ui.h"

enum MENU {
	TUTORIAL=0,
	START,			// スタート
	ALT,			
	RANKING,			// ランキング
	MENU_MAX
};
class SceneMenu :
	public SceneBase
{
private:
	C_Ui* pStart;
	C_Ui* pRanking;
	C_Ui* pHard;
	C_Ui* pTutorial;
	bool bOptionSelected;
	MENU g_nSelectMenu;
	int nNextScene;
public:
	SceneMenu();
	~SceneMenu();
	void Init();
	void Uninit();
	int Update();
	void Draw();

	MENU GetPauseMenu();
	void ResetPauseMenu();
};

SceneMenu* GetMenu();

