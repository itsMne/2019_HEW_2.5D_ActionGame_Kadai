#include "SceneManager.h"
#include "SceneGame.h"

SceneBase* Scene = nullptr;
int nSceneType = SCENE_GAME;
HRESULT InitScene()
{
	if (Scene)
		return S_OK;
	switch (nSceneType)
	{

	case SCENE_GAME:
		Scene = new SceneGame();
		break;
	default:
		break;
	}
	
	return S_OK;
}

void UpdateScene()
{
	if (!Scene)
		return;
	int SceneResult = Scene->Update();
	if (SceneResult != nSceneType)
		SwitchScene(SceneResult);
	
}

void SwitchScene(int SceneResult)
{
	SAFE_DELETE(Scene);
	nSceneType = SceneResult;
	InitScene();
}

void DrawScene()
{
	if (Scene)
		Scene->Draw();
}

void EndScene()
{
	SAFE_DELETE(Scene);
}
