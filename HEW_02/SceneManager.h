//*****************************************************************************
//SceneManager.h
//*****************************************************************************
#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include "main.h"

//*****************************************************************************
//�v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitScene();
void UpdateScene();
void SwitchScene(int SceneResult);
void DrawScene();
void EndScene();

#endif