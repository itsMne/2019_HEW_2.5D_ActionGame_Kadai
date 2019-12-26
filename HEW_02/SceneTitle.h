#pragma once
#include "SceneBase.h"
#include "C_Ui.h"
class SceneTitle :
	public SceneBase
{
private:
	DXWindow3D* MainWindow;
	C_Ui* Title;
	C_Ui* SlashEffect;
	C_Ui* Effect1;
	C_Ui* Effect2;
	float alpha1, alpha2;
public:
	SceneTitle();
	~SceneTitle();
	void Init();
	void Uninit();
	int Update();
	void Draw();
};



