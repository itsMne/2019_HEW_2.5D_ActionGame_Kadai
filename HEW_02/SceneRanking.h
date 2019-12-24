#pragma once
#include "SceneBase.h"
#include "C_Ui.h"
class SceneRanking :
	public SceneBase
{
private:
	C_Ui* Ranking;
public:
	SceneRanking();
	~SceneRanking();

	void Init();
	void Uninit();
	int Update();
	void Draw();
};

