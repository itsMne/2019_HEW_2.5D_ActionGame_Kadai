//*****************************************************************************
// SceneGameOver.cpp
// ゲームオーバーのシーン
//*****************************************************************************
#include "SceneGameOver.h"
#include "Sound.h"
#include "InputManager.h"

//*****************************************************************************
// コンストラクタ関数
//*****************************************************************************
SceneGameOver::SceneGameOver()
{
	GameOver = new C_Ui("data/texture/1574411424324.png", UI_GAMEOVER);
	Score = new C_Ui("data/texture/number.png", UI_RESULT_SCORE);
	PlaySoundGame(SOUND_LABEL_BGM_GAMEOVER);
}


SceneGameOver::~SceneGameOver()
{
	Uninit();
}

//*****************************************************************************
//Init関数
//初期化関数
//引数：void
//戻：void
//*****************************************************************************
void SceneGameOver::Init()
{
	//なし
}

//*****************************************************************************
//Uninit関数
//終了関数
//引数：void
//戻：void
//*****************************************************************************
void SceneGameOver::Uninit()
{
	SAFE_DELETE(GameOver);
	SAFE_DELETE(Score);
	StopSound();
}

//*****************************************************************************
//Update関数
//変更関数
//引数：void
//戻：void
//*****************************************************************************
int SceneGameOver::Update()
{
	GameOver->Update();

	if (GetInput(INPUT_JUMP)) {
		return SCENE_TITLE;
	}

	return SCENE_GAMEOVER;
}

//*****************************************************************************
//Draw関数
//レンダリング関数
//引数：void
//戻：void
//*****************************************************************************
void SceneGameOver::Draw()
{
	SetZBuffer(false);
	GameOver->Draw();

	Score->Draw();
}
