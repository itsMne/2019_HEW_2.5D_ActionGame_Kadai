#pragma once
#include "DXWindow3D.h"
#include "debugproc.h"
#include "Camera3D.h"
#include "Light3D.h"
#include "Field3D.h"
#include "Player3D.h"
#include "Billboard2D.h"
#include "UniversalStructures.h"
#include "Polygon2D.h"
enum SceneType
{
	SCENE_TITLE=1,
	SCENE_MENU,
	SCENE_GAME,
	SCENE_GAMEOVER,
	SCENE_RANKING,
	SCENE_CLEAR,
	SCENE_HELL_GAME,
	SCENE_TUTORIAL_GAME,
	SCENE_MAX
};
class SceneBase
{
protected:
	int nSceneType;
public:
	SceneBase();
	virtual ~SceneBase();
	virtual void Init()=0;
	virtual void Draw()=0;
	virtual int Update()=0;
	virtual void Uninit()=0;
};

