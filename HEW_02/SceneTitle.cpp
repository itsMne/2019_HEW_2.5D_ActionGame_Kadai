//*****************************************************************************
// SceneTitle.cpp
// タイトルのシーン
//*****************************************************************************
#include "SceneTitle.h"
#include "Sound.h"
#include "InputManager.h"

//*****************************************************************************
// コンストラクタ関数
//*****************************************************************************
SceneTitle::SceneTitle()
{
	MainWindow = GetMainWindow();
	if (MainWindow)
		MainWindow->SetWindowColor(0, 0, 0);
	Title = new C_Ui("data/texture/Title/haikei.png", UI_TITLE);
	SlashEffect = new C_Ui("data/texture/Slash.tga", UI_SLASH_EFFECT);
	Effect1 = new C_Ui("data/texture/Title/haikei0.png", UI_TITLE);
	Effect2 = new C_Ui("data/texture/Title/haikei1.png", UI_TITLE);
	for (int i = 0; i < 4; i++)
	{
		switch (i)
		{
		case 0:
			Kanji[i] = new C_Ui("data/texture/Title/TE_1.tga", MAXUITYPE);
			Kanji[i]->SetPolygonPos(80, 150);
			break;
		case 1:
			Kanji[i] = new C_Ui("data/texture/Title/TE_2.tga", MAXUITYPE);
			Kanji[i]->SetPolygonPos(80, 0);
			break;
		case 2:
			Kanji[i] = new C_Ui("data/texture/Title/TE_3.tga", MAXUITYPE);
			Kanji[i]->SetPolygonPos(-80, 150);
			break;
		case 3:
			Kanji[i] = new C_Ui("data/texture/Title/TE_4.tga", MAXUITYPE);
			Kanji[i]->SetPolygonPos(-80, 0);
			break;
		default:
			break;
		}
		Kanji[i]->SetRotationY(90);
		Kanji[i]->SetPolygonSize(164, 167);
	}
	Effect1->SetPolygonAlpha(0.5f);
	Effect2->SetPolygonAlpha(0.5f);
	alpha1 = 1; 
	nRotatingKanji = 0;
	fAccelerationRot = 0;
	alpha2 = -1;
	for (int i = 0; i < MAX_SAKURA; i++)
	{
		sakura[i] = new C_Ui("data/texture/sakura.tga", UI_SAKURA); //桜 particle　で検索 +桜の花びら LeafTexture.tga
	}
}

SceneTitle::~SceneTitle()
{
	Uninit();
}

//*****************************************************************************
//Init関数
//初期化関数
//引数：void
//戻：void
//*****************************************************************************
void SceneTitle::Init()
{
	//なし
}

//*****************************************************************************
//Uninit関数
//終了関数
//引数：void
//戻：void
//*****************************************************************************
void SceneTitle::Uninit()
{
	SAFE_DELETE(Title);
	SAFE_DELETE(SlashEffect);
	SAFE_DELETE(Effect1);
	SAFE_DELETE(Effect2);
	for (int i = 0; i < 4; i++)
		SAFE_DELETE(Kanji[i]);
	for (int i = 0; i < MAX_SAKURA; i++)
		SAFE_DELETE(sakura[i]);
}

//*****************************************************************************
//Update関数
//変更関数
//引数：void
//戻：void
//*****************************************************************************
int SceneTitle::Update()
{
	if (nRotatingKanji < 4) {
		fAccelerationRot += 1.0f;
		Kanji[nRotatingKanji]->RotateAroundY(-fAccelerationRot);
		if (Kanji[nRotatingKanji]->GetRotationY() <= 0)
		{
			Kanji[nRotatingKanji]->SetRotationY(0);
			nRotatingKanji++;
			fAccelerationRot = 0;
			PlaySoundGame(SOUND_LABEL_SE_SHINE);
		}
		if(nRotatingKanji==2)
			PlaySoundGame(SOUND_LABEL_BGM_TITLE);
		if (nRotatingKanji < 4)
			return SCENE_TITLE;
	}
	
	SlashEffect->Update();
	Effect1->SetPolygonAlpha(Effect1->GetAlpha() - 0.005f*alpha1);
	Effect2->SetPolygonAlpha(Effect2->GetAlpha() - 0.005f*alpha2);

	if (Effect1->GetAlpha() <= 0 || Effect1->GetAlpha() >= 1) {
		alpha1 *= -1;
		if (Effect1->GetAlpha() < 0)
			Effect1->SetPolygonAlpha(0);
		if (Effect1->GetAlpha() > 1)
			Effect1->SetPolygonAlpha(1);
	}
	if (Effect2->GetAlpha() <= 0 || Effect2->GetAlpha() >= 1) {
		alpha2 *= -1;
		if (Effect2->GetAlpha() < 0)
			Effect2->SetPolygonAlpha(0);
		if (Effect2->GetAlpha() > 1)
			Effect2->SetPolygonAlpha(1);
	}
	if (SlashEffect->GetUV().V == 1 && SlashEffect->GetUV().U == 0)
		PlaySoundGame(SOUND_LABEL_SE_BIGSLASH);
	if (GetInput(INPUT_JUMP) && SlashEffect->GetUV().V >= 7) {
		StopSound();
		PlaySoundGame(SOUND_LABEL_SE_GAMESTART);
		return SCENE_MENU;
	}
	for (int i = 0; i < MAX_SAKURA; i++)
	{
		sakura[i]->Update();
	}
	return SCENE_TITLE;
}

//*****************************************************************************
//Draw関数
//レンダリング関数
//引数：void
//戻：void
//*****************************************************************************
void SceneTitle::Draw()
{
	SetZBuffer(false);
	for (int i = 0; i < 4; i++)
		Kanji[i]->Draw();
	if (SlashEffect->GetUV().V >= 5) {
		Title->Draw();
		Effect1->Draw();
		Effect2->Draw();
		for (int i = 0; i < MAX_SAKURA; i++)
		{
			sakura[i]->Draw();
		}
	}
	SlashEffect->Draw();
}

