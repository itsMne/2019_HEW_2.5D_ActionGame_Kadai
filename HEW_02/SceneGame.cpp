#include "SceneGame.h"
#include "Enemy3D.h"
#include "Mirror3D.h"
#include "Camera3D.h"
#include "InputManager.h"
#include "RankManager.h"
#define RENDER_BOX_CAMERA { 0,0,0, 200,200,120 };

enum PAUSE_OPTION
{
	PAUSE_CONTINUE=0,
	PAUSE_GIVEUP,
	PAUSE_MAX
};



SceneGame*		CurrentGame = nullptr;
static int		nScore;
C_Ui* pVisualRank;

SceneGame::SceneGame(int type): SceneBase()
{
	for (int i = 0; i < MAX_HIT_EFFECT; i++, HitEffect_UI[i] = nullptr);
	//
	
	nScore = 0;
	bGoalReached = false;
	nSceneGameType = type;
	CurrentGame = nullptr;
	Init();
}


SceneGame::~SceneGame()
{
	Uninit();
	CurrentGame = nullptr;
}

void SceneGame::Init()
{
	if (CurrentGame)
		return;
	CurrentGame = this;
	RankManager::Init();
	g_pDevice = GetDevice();
	nScore = 0;
	fZoomAcc = 0;
	fZoomPause = 0;
	nFramesForZoomPausing = 0;
	fZoomSpeed = 0;
	nTimeSeconds = 0;
	nFrameCounter = 0;
	nPauseFrames = 0;
	
	MainWindow = GetMainWindow();
	bPauseZooming = false;
	bCancelZoom = false;
	bZoomBack = false;
	if (MainWindow)
		MainWindow->SetWindowColor(231.0f / 255.0f, 182.0f / 255.0f, 128.0f / 255.0f);
	SceneCamera = new Camera3D(true);
	SceneLight = new Light3D();
	pPlayer = new Player3D();
	Fields = new Go_List();
	Walls = new Go_List();
	Items = new Go_List();
	Spikes = new Go_List();
	Goals = new Go_List();
	Mirrors = new Go_List();
	Enemies = new Go_List();
	SkySphere = new Sphere3D("data/texture/haikei.jpg");
	SceneCamera->Init();
	SceneLight->Init();



	InitDebugProc();
	SceneCamera->SetFocalPoint(pPlayer);
	// Hp
	pHP_UI_BACK = new C_Ui("data/texture/HP000.png", UI_HP00);
	pHP_UI_FRONT = new C_Ui("data/texture/HP001.png", UI_HP01);
	// Mp
	pMP_UI = new C_Ui("data/texture/MP000.png", UI_MP);
	
	pScore_UI = new C_Ui("data/texture/jNum.tga", UI_NUMBER);
	// Score
	pScore_Frame_UI = new C_Ui("data/texture/frame_score.png", UI_SCORE);

	for (int i = 0; i < MAX_SAKURA_LEAVES; i++)
		pSakuraleaf[i] = new C_Ui("data/texture/LeafTexture.tga", UI_SAKURALEAF);

	//レベルエディター
	pLevel_Editor_UI = new C_Ui("data/texture/UI_LEINSTRUCTIONS.tga", UI_LEVEL_EDITOR);
	pLevel_Editor_MOVEMODE_UI = new C_Ui("data/texture/UI_MOVEABLEMODE.tga", UI_LEVEL_EDITOR_MOVEABLEMODE);
	pLevel_Editor_STATICMODE_UI = new C_Ui("data/texture/UI_STATICMODE.tga", UI_LEVEL_EDITOR_STATICMODE);
	pSpeed_MoveObject_UI = new C_Ui("data/texture/UI_LEVEL_EDITOR_NUM.tga", UI_LEVEL_EDITOR_OBJSPEED);
	pDelay_MoveObject_UI = new C_Ui("data/texture/UI_LEVEL_EDITOR_NUM.tga", UI_LEVEL_EDITOR_DELAY);
	pZoomAttack_UI = new C_Ui("data/texture/ZoomEffect.tga", UI_ZOOM_ATTACK);

	pBG_UI = new C_Ui("data/texture/PauseMenu/PauseMenuBackground.png", UI_PAUSE_BG);
	pContinue_UI = new C_Ui("data/texture/PauseMenu/Continue.png", UI_MENU_OPTION);
	pContinue_UI->SetPolygonPos(250, 0);
	pGiveUp_UI = new C_Ui("data/texture/PauseMenu/GiveUp.png", UI_MENU_OPTION);
	pGiveUp_UI->SetPolygonPos(-250, 0);
	pGiveUp_UI->SetRotationY(90);
	pContinue_UI->SetRotationY(90);
	fPauseOptionsAcceleration = 0;
	fCurrentInGameZoom = SceneCamera->GetCurrentZoom();
	bGameIsPaused = false;
	nCurrentPauseSelection = PAUSE_CONTINUE;
	for (int i = 0; i < MAX_HIT_EFFECT; i++)
		HitEffect_UI[i] = new C_Ui("data/texture/AuraEffect.tga", UI_HIT_EFFECT);
	pVisualRank = new C_Ui("data/texture/rankL.png", UI_RANK_VISUAL);

	switch (nSceneGameType)
	{
	case SCENE_GAMEHELL:
		printf("\nHELL\n");
		Walls->Load("HELL_Walls_Level", GO_WALL);
		Fields->Load("HELL_Fields_Level", GO_FLOOR);
		Items->Load("HELL_Items_Level", GO_ITEM);
		Goals->Load("HELL_Goals_Level", GO_GOAL);
		Spikes->Load("HELL_Spikes_Level", GO_SPIKE);
		Mirrors->Load("HELL_Mirrors_Level", GO_MIRROR);
		Enemies->Load("HELL_Enemies_Level", GO_ENEMY);
		nSceneType = SCENE_HELL_GAME;
		return;
	case SCENE_GAMENORMAL:
		printf("\nNORMAL\n");
		Walls->Load("Walls_Level", GO_WALL);
		Fields->Load("Fields_Level", GO_FLOOR);
		Items->Load("Items_Level", GO_ITEM);
		Goals->Load("Goals_Level", GO_GOAL);
		Spikes->Load("Spikes_Level", GO_SPIKE);
		Mirrors->Load("Mirrors_Level", GO_MIRROR);
		Enemies->Load("Enemies_Level", GO_ENEMY);
		nSceneType = SCENE_GAME;
		return;
	}
}

void SceneGame::Uninit()
{
	SAFE_DELETE(SceneCamera);
	SAFE_DELETE(SceneLight);
	SAFE_DELETE(pPlayer);
	SAFE_DELETE(Fields);
	SAFE_DELETE(Walls);
	SAFE_DELETE(Items);
	SAFE_DELETE(Spikes);
	SAFE_DELETE(Goals);
	SAFE_DELETE(Mirrors);
	SAFE_DELETE(Enemies);
	SAFE_DELETE(SkySphere);
	SAFE_DELETE(pHP_UI_BACK);
	SAFE_DELETE(pHP_UI_FRONT);
	SAFE_DELETE(pMP_UI);
	SAFE_DELETE(pScore_UI);
	SAFE_DELETE(pScore_Frame_UI);
	SAFE_DELETE(pLevel_Editor_UI);
	SAFE_DELETE(pLevel_Editor_MOVEMODE_UI);
	SAFE_DELETE(pLevel_Editor_STATICMODE_UI);
	SAFE_DELETE(pSpeed_MoveObject_UI);
	SAFE_DELETE(pDelay_MoveObject_UI);
	SAFE_DELETE(pZoomAttack_UI);


	// デバッグ文字列表示終了処理
	//UninitDebugProc();
}

int SceneGame::Update()
{
	if (pPlayer->PlayerGameOver())
	{
		printf("\nGAME OVER\n");
		return SCENE_GAMEOVER;//ゲームオーバーなら
	}
	if (bGoalReached)
	{
		printf("\nGOAL\n");
		return SCENE_CLEAR;//後で次のシーンで変更する
	}
	if (bGameIsPaused)
	{
		pContinue_UI->Update();
		pGiveUp_UI->Update();
		fPauseOptionsAcceleration += 1;
		if (pContinue_UI->GetRotationY() > 0)
			pContinue_UI->RotateAroundY(-fPauseOptionsAcceleration);
		if (pContinue_UI->GetRotationY() < 0)
			pContinue_UI->SetRotationY(0);
		if (pContinue_UI->GetRotationY() == 0) {
			if (pGiveUp_UI->GetRotationY() > 0)
				pGiveUp_UI->RotateAroundY(-fPauseOptionsAcceleration);
			if (pGiveUp_UI->GetRotationY() < 0)
				pGiveUp_UI->SetRotationY(0);
		}
		if (GetInput(INPUT_TRIGGER_LEFT))
			nCurrentPauseSelection++;

		if (GetInput(INPUT_TRIGGER_RIGHT))
			nCurrentPauseSelection--;
		if (nCurrentPauseSelection == PAUSE_MAX)
			nCurrentPauseSelection = 0;
		if (nCurrentPauseSelection < 0)
			nCurrentPauseSelection = PAUSE_MAX - 1;
		if (GetInput(INPUT_PAUSE) && pContinue_UI->GetRotationY() == 0 && pGiveUp_UI->GetRotationY() == 0)
			bGameIsPaused = false;

		switch (nCurrentPauseSelection)
		{
		case PAUSE_CONTINUE:
			pContinue_UI->SetAsSelectedOption(true);
			pGiveUp_UI->SetAsSelectedOption(false);
			if (GetInput(INPUT_JUMP))
				bGameIsPaused = false;
			break;
		case PAUSE_GIVEUP:
			pContinue_UI->SetAsSelectedOption(false);
			pGiveUp_UI->SetAsSelectedOption(true);
			if (GetInput(INPUT_JUMP))
				return SCENE_TITLE;
			break;
		default:
			break;
		}

		return nSceneType;
	}
	for (int i = 0; i < MAX_HIT_EFFECT; i++)
		HitEffect_UI[i]->Update();
	fPauseOptionsAcceleration = 0;
	pGiveUp_UI->SetRotationY(90);
	pContinue_UI->SetRotationY(90);

	//UIに関する
	pScore_UI->Update();// Number更新
	pLevel_Editor_UI->Update();//レベルエディター更新
	pLevel_Editor_MOVEMODE_UI->Update();
	pLevel_Editor_STATICMODE_UI->Update();
	pSpeed_MoveObject_UI->Update();
	pDelay_MoveObject_UI->Update();
	pZoomAttack_UI->Update();
	pHP_UI_BACK->Update();// Hp更新
	pHP_UI_FRONT->Update();
	pMP_UI->Update();// Mp更新
	pScore_Frame_UI->Update();// Score更新
	pBG_UI->Update();


	//背景にかんする
	SkySphere->Update();//スカイスフィア
	SceneCamera->Update();//カメラ

	//フレームとズームに関してエフェクト

	
	if (++nFrameCounter == 60) {//タイムのカウンター
		nTimeSeconds++;
		nFrameCounter = 0;
	}
	if (bPauseZooming)
	{
		if (nFramesForZoomPausing <= 0)
		{	
			if (--nPauseFrames > 0)
				return nSceneType;
			if (bZoomBack && !bCancelZoom)
			{
				fZoomAcc += fZoomSpeed;
				if (fCurrentInGameZoom > SceneCamera->GetCurrentZoom())
				{
					SceneCamera->ZoomInZ(fZoomAcc);
					if (fCurrentInGameZoom < SceneCamera->GetCurrentZoom())
						SceneCamera->SetZoomZ(fCurrentInGameZoom);
				}
				if (fCurrentInGameZoom < SceneCamera->GetCurrentZoom())
				{
					SceneCamera->ZoomOutZ(fZoomAcc);
					if (fCurrentInGameZoom > SceneCamera->GetCurrentZoom())
						SceneCamera->SetZoomZ(fCurrentInGameZoom);
				}
				if (fCurrentInGameZoom != SceneCamera->GetCurrentZoom())
					return nSceneType;
			}
			
			bPauseZooming = false;
			bZoomBack = false;
			SceneCamera->SetZoomZ(fCurrentInGameZoom);
			Hitbox3D render = RENDER_BOX_CAMERA;
			SceneCamera->SetRenderZone(render);
			return nSceneType;
		}
		fZoomAcc += fZoomSpeed;
		if (fZoomPause > SceneCamera->GetCurrentZoom())
		{
			SceneCamera->ZoomInZ(fZoomAcc);
			if (fZoomPause < SceneCamera->GetCurrentZoom())
				SceneCamera->SetZoomZ(fZoomPause);
		}
		if (fZoomPause < SceneCamera->GetCurrentZoom())
		{
			SceneCamera->ZoomOutZ(fZoomAcc);
			if (fZoomPause > SceneCamera->GetCurrentZoom())
				SceneCamera->SetZoomZ(fZoomPause);
		}
		if (fZoomPause == SceneCamera->GetCurrentZoom()) {
			nFramesForZoomPausing--;
			fZoomAcc = 0;
		}
		
		if(bPauseFramesWhenZoom)
			return nSceneType;
	}
	if (GetInput(INPUT_PAUSE))
		bGameIsPaused = true;
	if (--nPauseFrames > 0)
		return nSceneType;
	nPauseFrames = 0;
	UpdateDebugProc();// デバッグ文字列表示更新
	StartDebugProc();// デバッグ文字列設定
	PrintDebugProc("FPS:%d\n\n", GetMainWindowFPS());// デバッグ文字列設定
	SceneLight->Update();// 光源更新

	// ゲームのオブジェクト
	pPlayer->Update();//プレイヤー
	Fields->Update();//床
	Walls->Update();//壁
	Items->Update();//アイテム
	Spikes->Update();//スパイク
	Mirrors->Update();//鏡（テレポート）
	Enemies->Update();//敵
	Goals->Update();//ゴール

	//ランクマネージャ
	RankManager::Update();

	//桜の葉
	for (int i = 0; i < MAX_SAKURA_LEAVES; i++)
		pSakuraleaf[i]->Update();
	return nSceneType;
}

void SceneGame::Draw()
{
	DXWindow3D* pMainWindow = GetMainWindow();
	// Zバッファ有効
	SetZBuffer(true);

	// 前面カリング (FBXは表裏が反転するため)
	ID3D11DeviceContext* pDeviceContext = pMainWindow->GetDeviceContext();
	

	// モデル描画
	SetCullMode(CULLMODE_NONE);
	Items->Draw();
	pPlayer->Draw();
	pDeviceContext->RSSetState(pMainWindow->GetRasterizerState(1));
	Enemies->Draw();
	SetCullMode(CULLMODE_CCW);
	
	//pDeviceContext->RSSetState(pMainWindow->GetRasterizerState(2));
	
	Goals->Draw();

	SkySphere->Draw();
	// フィールド描画
	Walls->Draw();
	Spikes->Draw();
	Fields->Draw();
	
	

	Mirrors->Draw();
	
	SceneCamera->DrawRenderZone();
	// Zバッファ無効
	SetZBuffer(false);
	// Hp描画
	pHP_UI_BACK->Draw();
	pHP_UI_FRONT->Draw();

	// Mp描画
	pMP_UI->Draw();

	// Number描画
	pScore_UI->Draw();

	 //Score描画
	pScore_Frame_UI->Draw();

	//レベルエディター更新
	pLevel_Editor_UI->Draw();
	pLevel_Editor_MOVEMODE_UI->Draw();
	pLevel_Editor_STATICMODE_UI->Draw();
	pSpeed_MoveObject_UI->Draw();
	pDelay_MoveObject_UI->Draw();
	pZoomAttack_UI->Draw();
	for (int i = 0; i < MAX_SAKURA_LEAVES; i++)
		pSakuraleaf[i]->Draw();
	for (int i = 0; i < MAX_HIT_EFFECT; i++)
		HitEffect_UI[i]->Draw();
	pVisualRank->Draw();
	if (bGameIsPaused)
	{
		pBG_UI->Draw();
		pGiveUp_UI->Draw();
		pContinue_UI->Draw();
	}
	
	// デバッグ文字列表示
	DrawDebugProc();
}

Go_List * SceneGame::GetFields()
{
	return Fields;
}

Go_List * SceneGame::GetWalls()
{
	return Walls;
}

Go_List * SceneGame::GetItems()
{
	return Items;
}

Go_List * SceneGame::GetSpikes()
{
	return Spikes;
}

Go_List * SceneGame::GetGoals()
{
	return Goals;
}

Go_List * SceneGame::GetMirrors()
{
	return Mirrors;
}

Go_List * SceneGame::GetEnemies()
{
	return Enemies;
}

int SceneGame::GetScore()
{
	return nScore;
}

void SceneGame::RaiseScore(int rais)
{
	nScore += rais;
}

void SceneGame::RaiseScoreWithRank(int rais)
{
	nScore += rais*GetRank();
}

void SceneGame::SetGoalReached()
{
	bGoalReached = true;
}

Sphere3D * SceneGame::GetSkySphere()
{
	return SkySphere;
}

void SceneGame::ZoomPause(float fDistance, int nFrames, float Speed, bool PauseFramesWhenZoom, bool ZoomBack)
{
	if (!SceneCamera)
		return;
	if (bPauseZooming || bZoomBack)
	{
		if(nFramesForZoomPausing>0)
			nFramesForZoomPausing = nFrames;
		return;
	}
	fZoomPause = fDistance + SceneCamera->GetCurrentZoom();
	bCancelZoom = false;
	fZoomAcc = 0;
	nFramesForZoomPausing = nFrames;
	fZoomSpeed = Speed;
	fCurrentInGameZoom = SceneCamera->GetCurrentZoom();
	bPauseZooming = true;
	bPauseFramesWhenZoom = PauseFramesWhenZoom;
	bZoomBack = ZoomBack;
	if (fZoomSpeed <= 0)
		nFramesForZoomPausing = 0;
}

void SceneGame::CancelZoom()
{
	if (nFramesForZoomPausing <= 0)
		return;
	bPauseZooming = false;
	nFramesForZoomPausing = 0;
	bZoomBack = false;
	SceneCamera->SetZoomZ(fCurrentInGameZoom);
	Hitbox3D render = RENDER_BOX_CAMERA;
	SceneCamera->SetRenderZone(render);
}

void SceneGame::SetPauseFrames(int nPauseF)
{
	nPauseFrames = nPauseF;
}

bool SceneGame::IsGamePaused()
{
	return nPauseFrames>0;
}

void SceneGame::SetHitEffect()
{
	for (int i = 0; i < MAX_HIT_EFFECT; i++)
	{
		printf("eff: %d\n", i);
		if (!HitEffect_UI[i])
			continue;
		
		if (HitEffect_UI[i]->GetUse())
			continue;
		
		HitEffect_UI[i]->SetHitEffectUse();
		return;
	}
}

void SceneGame::SetPetalsFrames(int frames)
{
	for (int i = 0; i < MAX_SAKURA_LEAVES; i++)
	{
		if (pSakuraleaf[i])
			pSakuraleaf[i]->SetFrameUse(frames);
	}
}

SceneGame * GetCurrentGame()
{
	return CurrentGame;
}

int GetScore()
{
	return nScore;
}
