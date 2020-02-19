//*****************************************************************************
// SceneRanking.cpp
// ランクのシーン
//*****************************************************************************
#include "SceneRanking.h"
#include "InputManager.h"
#include "Sound.h"
#include "SceneGame.h"

//*****************************************************************************
// コンストラクタ関数
//*****************************************************************************
SceneRanking::SceneRanking()
{
	Ranking = new C_Ui("data/texture/ranking.jpg", UI_RANKING);
	Score = new C_Ui("data/texture/number.png", UI_RANKING_SCORE);
	Top = new C_Ui("data/texture/number.png", UI_RANKING_TOP);
	for (int i = 0; i < 3; i++)
	{
		top[i] = 0;
	}
	LoadFile();
	int score = GetScore();
	
	if (score > top[0])
	{
		top[2] = top[1];
		top[1] = top[0];
		top[0] = score;
	}
	else if (score > top[1])
	{
		top[2] = top[1];
		top[1] = score;
	}
	else if (score > top[2])
	{
		top[2] = score;
	}
	SaveRank();
	PlaySoundGame(SOUND_LABEL_BGM_RANKING);
}

SceneRanking::~SceneRanking()
{
	Uninit();
}

//*****************************************************************************
//Init関数
//初期化関数
//引数：void
//戻：void
//*****************************************************************************
void SceneRanking::Init()
{
	//なし
}

//*****************************************************************************
//Uninit関数
//終了関数
//引数：void
//戻：void
//*****************************************************************************
void SceneRanking::Uninit()
{
	SAFE_DELETE(Ranking);
	SAFE_DELETE(Score);
	SAFE_DELETE(Top);
	StopSound();
}

//*****************************************************************************
//Update関数
//変更関数
//引数：void
//戻：void
//*****************************************************************************
int SceneRanking::Update()
{
	Ranking->Update();

	if (GetInput(INPUT_JUMP)) {
		return SCENE_TITLE;
	}

	return SCENE_RANKING;
}

//*****************************************************************************
//Draw関数
//レンダリング関数
//引数：void
//戻：void
//*****************************************************************************
void SceneRanking::Draw()
{
	SetZBuffer(false);
	Ranking->Draw();

	Score->Draw();
	for (int i = 0; i < 3; i++)
	{
		Top->SetRankTop({ -SCORE_SIZE_X * 8, (float)(275 - (i*155))}, top[i]);
		Top->Draw();
	}
	
	
}

//*****************************************************************************
//SaveRank関数
//保存の関数
//引数：void
//戻：void
//*****************************************************************************
void SceneRanking::SaveRank()
{
	FILE *pFile;
	char szFinalfilename[256] = "data/levels/";
	strcat(szFinalfilename, "TopRank");
	strcat(szFinalfilename, ".bin");

	pFile = fopen(szFinalfilename, "wb");

	if (pFile)
		fwrite(top, sizeof(top[3]), 1, pFile);

	printf("SAVED OK: %s\n", szFinalfilename);
	fclose(pFile);
}

//*****************************************************************************
//LoadFile関数
//ロードの関数
//引数：void
//戻：void
//*****************************************************************************
void SceneRanking::LoadFile()
{
	FILE *pFile;
	char szFinalfilename[256] = "data/levels/";
	strcat(szFinalfilename, "TopRank");
	strcat(szFinalfilename, ".bin");
	if ((pFile = fopen(szFinalfilename, "rb")) == NULL)
	{
		printf_s("%sのファイルはない…\n", "TopRank");
		return;
	}
	while ((fread(top, sizeof(top[3]), 1, pFile)));

	fclose(pFile);
}