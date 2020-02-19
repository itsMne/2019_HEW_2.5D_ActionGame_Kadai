//*****************************************************************************
//SceneManager.h
//*****************************************************************************
#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include "main.h"

//*****************************************************************************
//プロトタイプ宣言
//*****************************************************************************
HRESULT InitScene();
void UpdateScene();
void SwitchScene(int SceneResult);
void DrawScene();
void EndScene();

#endif