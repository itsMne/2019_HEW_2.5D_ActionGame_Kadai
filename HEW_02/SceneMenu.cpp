#include "SceneMenu.h"
#include "C_Ui.h"
#include "input.h"
#include "InputManager.h"

#define NUM_PAUSE_MENU		3

SceneMenu* Menu;

SceneMenu::SceneMenu()
{
	Menu = this;

	g_nSelectMenu = START;

	pMenuBg = new C_Ui("data/texture/haikei1.png", UI_MENU);
	pMenuFrame = new C_Ui("data/texture/MenuFrame.png", UI_MENUFRAME);
	pStart = new C_Ui("data/texture/Menu00.png", UI_MSTART);
	pRanking = new C_Ui("data/texture/Menu02.png", UI_MEND);
	pEnd = new C_Ui("data/texture/Menu01.png", UI_MRANKING);
}


SceneMenu::~SceneMenu()
{
}

void SceneMenu::Init()
{

}

void SceneMenu::Uninit()
{
}

int SceneMenu::Update()
{
	if (GetKeyRepeat(VK_D) || GetKeyRepeat(VK_RIGHT)) {
		g_nSelectMenu = (MENU)((g_nSelectMenu + NUM_PAUSE_MENU - 1) % NUM_PAUSE_MENU);
	}
	else if (GetKeyRepeat(VK_A) || GetKeyRepeat(VK_LEFT)) {
		g_nSelectMenu = (MENU)((g_nSelectMenu + 1) % NUM_PAUSE_MENU);
	}

	pMenuBg->Update();
	pMenuFrame->Update();
	pStart->Update();
	pRanking->Update();
	pEnd->Update();


	switch (g_nSelectMenu) {
	case START:
		pStart->SetPolygonSize(MSTART_SIZE_X + 100, MSTART_SIZE_Y + 50);
		pRanking->SetPolygonSize(MRANKING_SIZE_X, MRANKING_SIZE_Y);
		pEnd->SetPolygonSize(MEND_SIZE_X, MEND_SIZE_Y);
		if (GetInput(INPUT_JUMP)) {
			return SCENE_GAME;
		}
		break;
	case RANKING:
		pStart->SetPolygonSize(MSTART_SIZE_X, MSTART_SIZE_Y);
		pRanking->SetPolygonSize(MRANKING_SIZE_X + 100, MRANKING_SIZE_Y + 50);
		pEnd->SetPolygonSize(MEND_SIZE_X, MEND_SIZE_Y);
		if (GetInput(INPUT_JUMP)) {
			return SCENE_RANKING;
		}
		break;
	case END:
		pStart->SetPolygonSize(MSTART_SIZE_X, MSTART_SIZE_Y);
		pRanking->SetPolygonSize(MRANKING_SIZE_X, MRANKING_SIZE_Y);
		pEnd->SetPolygonSize(MEND_SIZE_X + 100, MEND_SIZE_Y + 50);
		if (GetInput(INPUT_JUMP)) {
			return SCENE_TITLE;
		}
		break;

	}

	return SCENE_MENU;
}

void SceneMenu::Draw()
{
	SetZBuffer(false);
	pMenuBg->Draw();
	pMenuFrame->Draw();
	pStart->Draw();
	pRanking->Draw();
	pEnd->Draw();
}

//=============================================================================
// ポーズメニューの取得
//=============================================================================
MENU SceneMenu::GetPauseMenu()
{
	return g_nSelectMenu;
}

//=============================================================================
// ポーズメニューのリセット
//=============================================================================
void SceneMenu::ResetPauseMenu()
{
	g_nSelectMenu = START;
}

SceneMenu * GetMenu()
{
	return Menu;
}
