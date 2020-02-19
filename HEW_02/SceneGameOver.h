//*****************************************************************************
//SceneGameOver.h
//*****************************************************************************
#ifndef SCENEGAMEOVER_H
#define SCENEGAMEOVER_H
#include "SceneBase.h"
#include "C_Ui.h"

//*****************************************************************************
//ƒNƒ‰ƒX
//*****************************************************************************
class SceneGameOver :
	public SceneBase
{
private:
	C_Ui* GameOver;
	C_Ui* Score;
public:
	SceneGameOver();
	~SceneGameOver();
	void Init();
	void Uninit();
	int Update();
	void Draw();
};

#endif