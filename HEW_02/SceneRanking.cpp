//*****************************************************************************
// SceneRanking.cpp
// �����N�̃V�[��
//*****************************************************************************
#include "SceneRanking.h"
#include "InputManager.h"
#include "Sound.h"
#include "SceneGame.h"

//*****************************************************************************
// �R���X�g���N�^�֐�
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
//Init�֐�
//�������֐�
//�����Fvoid
//�߁Fvoid
//*****************************************************************************
void SceneRanking::Init()
{
	//�Ȃ�
}

//*****************************************************************************
//Uninit�֐�
//�I���֐�
//�����Fvoid
//�߁Fvoid
//*****************************************************************************
void SceneRanking::Uninit()
{
	SAFE_DELETE(Ranking);
	SAFE_DELETE(Score);
	SAFE_DELETE(Top);
	StopSound();
}

//*****************************************************************************
//Update�֐�
//�ύX�֐�
//�����Fvoid
//�߁Fvoid
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
//Draw�֐�
//�����_�����O�֐�
//�����Fvoid
//�߁Fvoid
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
//SaveRank�֐�
//�ۑ��̊֐�
//�����Fvoid
//�߁Fvoid
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
//LoadFile�֐�
//���[�h�̊֐�
//�����Fvoid
//�߁Fvoid
//*****************************************************************************
void SceneRanking::LoadFile()
{
	FILE *pFile;
	char szFinalfilename[256] = "data/levels/";
	strcat(szFinalfilename, "TopRank");
	strcat(szFinalfilename, ".bin");
	if ((pFile = fopen(szFinalfilename, "rb")) == NULL)
	{
		printf_s("%s�̃t�@�C���͂Ȃ��c\n", "TopRank");
		return;
	}
	while ((fread(top, sizeof(top[3]), 1, pFile)));

	fclose(pFile);
}