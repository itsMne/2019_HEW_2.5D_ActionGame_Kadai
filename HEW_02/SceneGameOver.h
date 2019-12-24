#pragma once
#include "SceneBase.h"
#include "C_Ui.h"
class SceneGameOver :
	public SceneBase
{
private:
	C_Ui* GameOver;
public:
	SceneGameOver();
	~SceneGameOver();

	void Init();
	void Uninit();
	int Update();
	void Draw();
};

