//*****************************************************************************
//SceneTitle.h
//*****************************************************************************
#ifndef SCENETITLE_H
#define SCENETITLE_H
#include "SceneBase.h"
#include "C_Ui.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define MAX_SAKURA 15

//*****************************************************************************
//クラス
//*****************************************************************************
class SceneTitle :
	public SceneBase
{
private:
	DXWindow3D* MainWindow;
	C_Ui* Title;
	C_Ui* SlashEffect;
	C_Ui* Effect1;
	C_Ui* Effect2;
	C_Ui* Kanji[4];
	C_Ui* sakura[MAX_SAKURA];
	int nRotatingKanji;
	float fAccelerationRot;
	float alpha1, alpha2;
public:
	SceneTitle();
	~SceneTitle();
	void Init();
	void Uninit();
	int Update();
	void Draw();
};

#endif