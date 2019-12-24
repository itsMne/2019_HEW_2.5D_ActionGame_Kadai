#pragma once
#include "SceneBase.h"
#include "C_Ui.h"
class SceneTitle :
	public SceneBase
{
private:
	C_Ui* Title;
public:
	SceneTitle();
	~SceneTitle();
	void Init();
	void Uninit();
	int Update();
	void Draw();
};



