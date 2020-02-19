//*****************************************************************************
// SceneManager.cpp
// �Q�[���̏󋵂��Ǘ�����
//*****************************************************************************
#include "SceneManager.h"
#include "SceneTitle.h"
#include "SceneMenu.h"
#include "SceneGame.h"
#include "SceneGameOver.h"
#include "SceneClear.h"
#include "SceneRanking.h"
#include "C_Ui.h"

//*****************************************************************************
// �G�i���ϐ�
//*****************************************************************************
enum DOORSENUM
{
	RIGHTDOOR=0,
	LEFTDOOR,
	MAX_DOORS
};

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
SceneBase* Scene = nullptr;
int nSceneType = SCENE_TITLE;
C_Ui* Doors[MAX_DOORS] = { nullptr };
bool bDoorTransition = false;
int nNextScene = 0;
int nWaitFrames = 0;

//*****************************************************************************
//InitScenes�֐�
//�������֐�
//�����Fvoid
//�߁Fvoid
//*****************************************************************************
HRESULT InitScene()
{
	if (Scene)
		return S_OK;
	switch (nSceneType)
	{
	case SCENE_TITLE:
		if (Doors[LEFTDOOR])
			Doors[LEFTDOOR]->SetDoorOpen(true);
		if (Doors[RIGHTDOOR])
			Doors[RIGHTDOOR]->SetDoorOpen(true);
		Scene = new SceneTitle();
		break;
	case SCENE_MENU:
		if (Doors[LEFTDOOR])
			Doors[LEFTDOOR]->SetDoorOpen(false);
		if (Doors[RIGHTDOOR])
			Doors[RIGHTDOOR]->SetDoorOpen(false);
		Scene = new SceneMenu();
		break;
	case SCENE_GAME:
		if (Doors[LEFTDOOR])
			Doors[LEFTDOOR]->SetDoorOpen(true);
		if (Doors[RIGHTDOOR])
			Doors[RIGHTDOOR]->SetDoorOpen(true);
		Scene = new SceneGame(SCENE_GAMENORMAL);
		break;
	case SCENE_GAMEOVER:
		if (Doors[LEFTDOOR])
			Doors[LEFTDOOR]->SetDoorOpen(true);
		if (Doors[RIGHTDOOR])
			Doors[RIGHTDOOR]->SetDoorOpen(true);
		Scene = new SceneGameOver();
		break;
	case SCENE_CLEAR:
		if (Doors[LEFTDOOR])
			Doors[LEFTDOOR]->SetDoorOpen(true);
		if (Doors[RIGHTDOOR])
			Doors[RIGHTDOOR]->SetDoorOpen(true);
		Scene = new SceneClear();
		break;
	case SCENE_RANKING:
		if (Doors[LEFTDOOR])
			Doors[LEFTDOOR]->SetDoorOpen(true);
		if (Doors[RIGHTDOOR])
			Doors[RIGHTDOOR]->SetDoorOpen(true);
		Scene = new SceneRanking();
		break;
	case SCENE_HELL_GAME:
		if (Doors[LEFTDOOR])
			Doors[LEFTDOOR]->SetDoorOpen(true);
		if (Doors[RIGHTDOOR])
			Doors[RIGHTDOOR]->SetDoorOpen(true);
		Scene = new SceneGame(SCENE_GAMEHELL);
		break;
	case SCENE_TUTORIAL_GAME:
		if (Doors[LEFTDOOR])
			Doors[LEFTDOOR]->SetDoorOpen(true);
		if (Doors[RIGHTDOOR])
			Doors[RIGHTDOOR]->SetDoorOpen(true);
		Scene = new SceneGame(SCENE_TUTORIAL_GAME);
		break;
	default:
		break;
	}
	if (!Doors[LEFTDOOR])
		Doors[LEFTDOOR] = new C_Ui("data/texture/LeftDoor.tga", UI_DOOR_LEFT);
	if (!Doors[RIGHTDOOR])
		Doors[RIGHTDOOR] = new C_Ui("data/texture/RightDoor.tga",UI_DOOR_RIGHT);

	return S_OK;
}

//*****************************************************************************
//UpdateScenes�֐�
//�ύX�֐�
//�����Fvoid
//�߁Fvoid
//*****************************************************************************
void UpdateScene()
{
	if (!Scene)
		return;
	if (--nWaitFrames > 0)
		return;
	if (Doors[LEFTDOOR])
		Doors[LEFTDOOR]->Update();
	if (Doors[RIGHTDOOR])
		Doors[RIGHTDOOR]->Update();
	if (bDoorTransition)
	{
		if (Doors[LEFTDOOR] && Doors[RIGHTDOOR]) {
			if (Doors[LEFTDOOR]->IsDoorInPosition()
				&& Doors[RIGHTDOOR]->IsDoorInPosition())
			{
				SAFE_DELETE(Scene);
				nSceneType = nNextScene;
				InitScene();
				bDoorTransition = false;
				nWaitFrames = 60;
				//Scene->Update();
			}
		}
		return;
	}

	nWaitFrames = 0;
	int SceneResult = Scene->Update();
	if (SceneResult != nSceneType)
		SwitchScene(SceneResult);


}

//*****************************************************************************
//SwitchScene�֐�
//�V�����V�[���̏�Ԃ�ݒ肷��
//�����Fint
//�߁Fvoid
//*****************************************************************************
void SwitchScene(int SceneResult)
{
	nNextScene = SceneResult;
	bDoorTransition = true;
	if (Doors[LEFTDOOR])
		Doors[LEFTDOOR]->SetDoorOpen(false);
	if (Doors[RIGHTDOOR])
		Doors[RIGHTDOOR]->SetDoorOpen(false);
}

//*****************************************************************************
//DrawScenes�֐�
//�����_�����O�֐�
//�����Fvoid
//�߁Fvoid
//*****************************************************************************
void DrawScene()
{
	if (Scene && nSceneType != SCENE_MENU)
		Scene->Draw();
	if (Doors[LEFTDOOR])
		Doors[LEFTDOOR]->Draw();
	if (Doors[RIGHTDOOR])
		Doors[RIGHTDOOR]->Draw();
	if (Scene && nSceneType == SCENE_MENU)
		Scene->Draw();
}

//*****************************************************************************
//EndScenes�֐�
//�I���֐�
//�����Fvoid
//�߁Fvoid
//*****************************************************************************
void EndScene()
{
	SAFE_DELETE(Scene);
}
