#include "SceneMenu.h"
#include "C_Ui.h"
#include "input.h"
#include "Sound.h"
#include "InputManager.h"

#define NUM_PAUSE_MENU		4

SceneMenu* Menu;

SceneMenu::SceneMenu()
{
	Menu = this;

	g_nSelectMenu = START;


	pStart = new C_Ui("data/texture/Menu00.png", UI_MENU_OPTION);
	pRanking = new C_Ui("data/texture/Menu02.png", UI_MENU_OPTION);
	pHard = new C_Ui("data/texture/Menu01.png", UI_MENU_OPTION);
	pTutorial = new  C_Ui("data/texture/Menu04.png", UI_MENU_OPTION);
	pStart->SetPolygonPos(150, 0);
	pHard->SetPolygonPos(-150, 0);
	pRanking->SetPolygonPos(-500, 0);
	pTutorial->SetPolygonPos(500, 0);

	pStart->SetRotationY(90);
	pRanking->SetRotationY(90);
	pHard->SetRotationY(90);
	pTutorial->SetRotationY(90);
	bOptionSelected = false;
	nNextScene = 0;
	PlaySoundGame(SOUND_LABEL_BGM_MENU);
}


SceneMenu::~SceneMenu()
{
	Uninit();
}

void SceneMenu::Init()
{

}

void SceneMenu::Uninit()
{
	SAFE_DELETE(pStart);
	SAFE_DELETE(pRanking);
	SAFE_DELETE(pHard);
	SAFE_DELETE(pTutorial);
}

int SceneMenu::Update()
{
	static float fAcceleration = 0;
	if (bOptionSelected)
	{
		if (pStart->GetRotationY() != 90 || pRanking->GetRotationY() != 90 || pHard->GetRotationY() != 90 || pTutorial->GetRotationY() != 90)
		{
			fAcceleration -= 1;

			if (pTutorial->GetRotationY() < 90)
				pTutorial->RotateAroundY(-fAcceleration);
			if (pTutorial->GetRotationY() > 90) {
				pTutorial->SetRotationY(90);
				fAcceleration = 0;
				PlaySoundGame(SOUND_LABEL_SE_MENUOPTIONHIDDEN);
			}
			if (pTutorial->GetRotationY() == 90) {
				if (pStart->GetRotationY() < 90)
					pStart->RotateAroundY(-fAcceleration);
				if (pStart->GetRotationY() > 90) {
					pStart->SetRotationY(90);
					fAcceleration = 0;
					PlaySoundGame(SOUND_LABEL_SE_MENUOPTIONHIDDEN);
				}
			}
			if (pStart->GetRotationY() == 90)
			{
				if (pHard->GetRotationY() < 90)
					pHard->RotateAroundY(-fAcceleration);
				if (pHard->GetRotationY() > 90) {
					pHard->SetRotationY(90);
					fAcceleration = 0;
					PlaySoundGame(SOUND_LABEL_SE_MENUOPTIONHIDDEN);
				}
			}
			if (pHard->GetRotationY() == 90)
			{
				if (pRanking->GetRotationY() < 90)
					pRanking->RotateAroundY(-fAcceleration);
				if (pRanking->GetRotationY() > 90) {
					pRanking->SetRotationY(90);
					fAcceleration = 0;
					PlaySoundGame(SOUND_LABEL_SE_MENUOPTIONHIDDEN);
				}
			}
			return SCENE_MENU;
		}
		fAcceleration = 0;
		return nNextScene;
	}
	if (GetInput(INPUT_TRIGGER_RIGHT)) {
		PlaySoundGame(SOUND_LABEL_SE_MENUMOVE);
		g_nSelectMenu = (MENU)((g_nSelectMenu + NUM_PAUSE_MENU - 1) % NUM_PAUSE_MENU);
	}
	else if (GetInput(INPUT_TRIGGER_LEFT)) {
		PlaySoundGame(SOUND_LABEL_SE_MENUMOVE);
		g_nSelectMenu = (MENU)((g_nSelectMenu + 1) % NUM_PAUSE_MENU);
	}

	pStart->Update();
	pRanking->Update();
	pHard->Update();
	pTutorial->Update();

	
	if (pStart->GetRotationY() != 0 || pRanking->GetRotationY() != 0 || pHard->GetRotationY() != 0 || pTutorial->GetRotationY() != 0)
	{
		fAcceleration += 1;

		if (pTutorial->GetRotationY() > 0)
			pTutorial->RotateAroundY(-fAcceleration);
		if (pTutorial->GetRotationY() < 0) {
			pTutorial->SetRotationY(0);
			fAcceleration = 0;
			PlaySoundGame(SOUND_LABEL_SE_MENUOPTIONHIDDEN);
		}

		if (pTutorial->GetRotationY() == 0)
		{
			if (pStart->GetRotationY() > 0)
				pStart->RotateAroundY(-fAcceleration);
			if (pStart->GetRotationY() < 0) {
				pStart->SetRotationY(0);
				fAcceleration = 0;
				PlaySoundGame(SOUND_LABEL_SE_MENUOPTIONHIDDEN);
			}
		}
		if (pStart->GetRotationY() == 0)
		{
			if (pHard->GetRotationY() > 0)
				pHard->RotateAroundY(-fAcceleration);
			if (pHard->GetRotationY() < 0) {
				pHard->SetRotationY(0);
				fAcceleration = 0;
				PlaySoundGame(SOUND_LABEL_SE_MENUOPTIONHIDDEN);
			}
		}
		if (pHard->GetRotationY() == 0)
		{
			if (pRanking->GetRotationY() > 0)
				pRanking->RotateAroundY(-fAcceleration);
			if (pRanking->GetRotationY() < 0) {
				pRanking->SetRotationY(0);
				fAcceleration = 0;
				PlaySoundGame(SOUND_LABEL_SE_MENUOPTIONHIDDEN);
			}
		}
		return SCENE_MENU;
	}
	fAcceleration = 0;
	switch (g_nSelectMenu) {
	case TUTORIAL:
		pStart->SetAsSelectedOption(false);
		pRanking->SetAsSelectedOption(false);
		pHard->SetAsSelectedOption(false);
		pTutorial->SetAsSelectedOption(true);
		if (GetInput(INPUT_JUMP)) {
			StopSound();
			PlaySoundGame(SOUND_LABEL_SE_GAMESTART2);
			nNextScene = SCENE_TUTORIAL_GAME;
			bOptionSelected = true;
		}
		break;
	case START:
		pStart->SetAsSelectedOption(true);
		pRanking->SetAsSelectedOption(false);
		pHard->SetAsSelectedOption(false);
		pTutorial->SetAsSelectedOption(false);
		if (GetInput(INPUT_JUMP)) {
			StopSound();
			PlaySoundGame(SOUND_LABEL_SE_GAMESTART2);
			nNextScene = SCENE_GAME;
			bOptionSelected = true;
		}
		break;
	case RANKING:
		pStart->SetAsSelectedOption(false);
		pRanking->SetAsSelectedOption(true);
		pHard->SetAsSelectedOption(false);
		pTutorial->SetAsSelectedOption(false);
		if (GetInput(INPUT_JUMP)) {
			StopSound();
			PlaySoundGame(SOUND_LABEL_SE_RANKING_START);
			nNextScene = SCENE_RANKING;
			bOptionSelected = true;
		}
		break;
	case ALT:
		pStart->SetAsSelectedOption(false);
		pRanking->SetAsSelectedOption(false);
		pHard->SetAsSelectedOption(true);
		pTutorial->SetAsSelectedOption(false);
		if (GetInput(INPUT_JUMP)) {
			StopSound();
			PlaySoundGame(SOUND_LABEL_SE_GAMESTART3);
			nNextScene = SCENE_HELL_GAME;
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
	pHard->Draw();
	pTutorial->Draw();
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
