//*****************************************************************************
//SceneRanking.h
//*****************************************************************************
#ifndef SCENERANKING_H
#define SCENERANKING_H
#include "SceneBase.h"
#include "C_Ui.h"

//*****************************************************************************
//ƒNƒ‰ƒX
//*****************************************************************************
class SceneRanking :
	public SceneBase
{
private:
	C_Ui* Ranking;
	C_Ui* Score;
	C_Ui* Top;
	int top[3];
public:
	SceneRanking();
	void SaveRank();
	void LoadFile();
	~SceneRanking();
	void Init();
	void Uninit();
	int Update();
	void Draw();
};

#endif