#pragma once
#include "SceneBase.h"
#include "C_Ui.h"
class SceneClear :
	public SceneBase
{
private:
	C_Ui* Clear;
public:
	SceneClear();
	~SceneClear();

	void Init();
	void Uninit();
	int Update();
	void Draw();
};

