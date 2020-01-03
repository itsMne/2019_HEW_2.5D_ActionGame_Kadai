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


	pStart = new C_Ui("data/texture/Menu00.png", UI_MENU_OPTION);
	pRanking = new C_Ui("data/texture/Menu02.png", UI_MENU_OPTION);
	pEnd = new C_Ui("data/texture/Menu01.png", UI_MENU_OPTION);

	pStart->SetPolygonPos(500, 0);
	pRanking->SetPolygonPos(-500, 0);

	pStart->SetRotationY(90);
	pRanking->SetRotationY(90);
	pEnd->SetRotationY(90);
	bOptionSelected = false;
	nNextScene = 0;
}


SceneMenu::~SceneMenu()
{

}

void SceneMenu::Init()
{

}

void SceneMenu::Uninit()
{
	SAFE_DELETE(pStart);
	SAFE_DELETE(pRanking);
	SAFE_DELETE(pEnd);
}

int SceneMenu::Update()
{
	static float fAcceleration = 0;
	if (bOptionSelected)
	{
		if (pStart->GetRotationY() != 90 || pRanking->GetRotationY() != 90 || pEnd->GetRotationY() != 90)
		{
			fAcceleration -= 1;
			if (pStart->GetRotationY() < 90)
				pStart->RotateAroundY(-fAcceleration);
			if (pStart->GetRotationY() > 90) {
				pStart->SetRotationY(90);
				fAcceleration = 0;
			}
			if (pStart->GetRotationY() == 90)
			{
				if (pEnd->GetRotationY() < 90)
					pEnd->RotateAroundY(-fAcceleration);
				if (pEnd->GetRotationY() > 90) {
					pEnd->SetRotationY(90);
					fAcceleration = 0;
				}
			}
			if (pEnd->GetRotationY() == 90)
			{
				if (pRanking->GetRotationY() < 90)
					pRanking->RotateAroundY(-fAcceleration);
				if (pRanking->GetRotationY() > 90) {
					pRanking->SetRotationY(90);
					fAcceleration = 0;
				}
			}
			return SCENE_MENU;
		}
		fAcceleration = 0;
		return nNextScene;
	}
	if (GetInput(INPUT_TRIGGER_RIGHT)) {
		g_nSelectMenu = (MENU)((g_nSelectMenu + 1) % NUM_PAUSE_MENU);
	}
	else if (GetInput(INPUT_TRIGGER_LEFT)) {
		g_nSelectMenu = (MENU)((g_nSelectMenu + NUM_PAUSE_MENU - 1) % NUM_PAUSE_MENU);
	}

	pStart->Update();
	pRanking->Update();
	pEnd->Update();

	
	if (pStart->GetRotationY() != 0 || pRanking->GetRotationY() != 0 || pEnd->GetRotationY() != 0) 
	{
		fAcceleration += 1;
		if (pStart->GetRotationY() > 0)
			pStart->RotateAroundY(-fAcceleration);
		if (pStart->GetRotationY() < 0) {
			pStart->SetRotationY(0);
			fAcceleration = 0;
		}
		if (pStart->GetRotationY() == 0)
		{
			if (pEnd->GetRotationY() > 0)
				pEnd->RotateAroundY(-fAcceleration);
			if (pEnd->GetRotationY() < 0) {
				pEnd->SetRotationY(0);
				fAcceleration = 0;
			}
		}
		if (pEnd->GetRotationY() == 0)
		{
			if (pRanking->GetRotationY() > 0)
				pRanking->RotateAroundY(-fAcceleration);
			if (pRanking->GetRotationY() < 0) {
				pRanking->SetRotationY(0);
				fAcceleration = 0;
				
			}
		}
		return SCENE_MENU;
	}
	fAcceleration = 0;
	switch (g_nSelectMenu) {
	case START:
		pStart->SetAsSelectedOption(true);
		pRanking->SetAsSelectedOption(false);
		pEnd->SetAsSelectedOption(false);
		if (GetInput(INPUT_JUMP)) {
			nNextScene = SCENE_GAME;
			bOptionSelected = true;
		}
		break;
	case RANKING:
		pStart->SetAsSelectedOption(false);
		pRanking->SetAsSelectedOption(true);
		pEnd->SetAsSelectedOption(false);
		if (GetInput(INPUT_JUMP)) {
			nNextScene = SCENE_RANKING;
			bOptionSelected = true;
		}
		break;
	case END:
		pStart->SetAsSelectedOption(false);
		pRanking->SetAsSelectedOption(false);
		pEnd->SetAsSelectedOption(true);
		if (GetInput(INPUT_JUMP)) {
			nNextScene = SCENE_TITLE;
			bOptionSelected = true;
		}
		break;

	}

	return SCENE_MENU;
}

void SceneMenu::Draw()
{
	SetZBuffer(false);
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
