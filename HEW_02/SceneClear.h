//*****************************************************************************
//SceneClear.h
//*****************************************************************************
#ifndef SCENECLEAR_H
#define SCENECLEAR_H
#include "SceneBase.h"
#include "C_Ui.h"

//*****************************************************************************
//ƒNƒ‰ƒX
//*****************************************************************************
class SceneClear :
	public SceneBase
{
private:
	C_Ui* Clear;
	C_Ui* Score;
public:
	SceneClear();
	~SceneClear();

	void Init();
	void Uninit();
	int Update();
	void Draw();
};

#endif