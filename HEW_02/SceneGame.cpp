#include "SceneGame.h"
#include "Enemy3D.h"
#include "Mirror3D.h"
#include "Camera3D.h"

SceneGame* CurrentGame = nullptr;

SceneGame::SceneGame(): SceneBase()
{
	Init();
	CurrentGame = this;
	nScore = 0;
	bGoalReached = false;
}


SceneGame::~SceneGame()
{
	Uninit();
	CurrentGame = nullptr;
}

void SceneGame::Init()
{
	g_pDevice = GetDevice();
	nScore = 0;
	fZoomAcc = 0;
	fZoomPause = 0;
	fFramesForZoomPausing = 0;
	fZoomSpeed = 0;
	fPreviousZoom = 0;
	nTimeSeconds = 0;
	nFrameCounter = 0;
	hbPreviousRenderingBox = {0,0,0,0,0,0};
	nSceneType = SCENE_GAME;
	MainWindow = GetMainWindow();
	bPauseZooming = false;
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

	Walls->Load("Walls_Level", GO_WALL);
	Fields->Load("Fields_Level", GO_FLOOR);
	Items->Load("Items_Level", GO_ITEM);
	Goals->Load("Goals_Level", GO_GOAL);
	Spikes->Load("Spikes_Level", GO_SPIKE);
	Mirrors->Load("Mirrors_Level", GO_MIRROR);
	Enemies->Load("Enemies_Level", GO_ENEMY);
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

	//レベルエディター
	pLevel_Editor_UI = new C_Ui("data/texture/UI_LEINSTRUCTIONS.tga", UI_LEVEL_EDITOR);
	pLevel_Editor_MOVEMODE_UI = new C_Ui("data/texture/UI_MOVEABLEMODE.tga", UI_LEVEL_EDITOR_MOVEABLEMODE);
	pLevel_Editor_STATICMODE_UI = new C_Ui("data/texture/UI_STATICMODE.tga", UI_LEVEL_EDITOR_STATICMODE);
	pSpeed_MoveObject_UI = new C_Ui("data/texture/UI_LEVEL_EDITOR_NUM.tga", UI_LEVEL_EDITOR_OBJSPEED);
	pDelay_MoveObject_UI = new C_Ui("data/texture/UI_LEVEL_EDITOR_NUM.tga", UI_LEVEL_EDITOR_DELAY);

}

void SceneGame::Uninit()
{
	SAFE_DELETE(Fields);//床終了処理
	SAFE_DELETE(pPlayer);//プレイヤー終了処理
	SAFE_DELETE(SkySphere);//スカイスフィア終了処理
	SAFE_DELETE(SceneLight);// 光終了処理
	SAFE_DELETE(SceneCamera);// カメラ終了処理

	// デバッグ文字列表示終了処理
	UninitDebugProc();
}

int SceneGame::Update()
{
	if (pPlayer->PlayerGameOver())
	{
		return SCENE_GAMEOVER;
	}
	if (bGoalReached)
	{
		return SCENE_CLEAR;//後で次のシーンで変更する
	}
	if (++nFrameCounter == 60) {//タイムのカウンター
		nTimeSeconds++;
		nFrameCounter = 0;
	}
	SceneCamera->Update();
	if (bPauseZooming)
	{
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
		if(fZoomPause == SceneCamera->GetCurrentZoom())
			fFramesForZoomPausing--;
		if (fFramesForZoomPausing <= 0)
		{
			bPauseZooming = false;
			SceneCamera->SetZoomZ(fPreviousZoom);
			SceneCamera->SetRenderZone(hbPreviousRenderingBox);
		}
		return nSceneType;
	}
	// デバッグ文字列表示更新
	UpdateDebugProc();

	// デバッグ文字列設定
	StartDebugProc();
	PrintDebugProc("FPS:%d\n\n", GetMainWindowFPS());

	// 光源更新
	SceneLight->Update();

	// カメラ更新
	
	
	// モデル更新
	pPlayer->Update();

	// フィールド更新
	Fields->Update();

	SkySphere->Update();

	Walls->Update();
	
	Items->Update();

	Spikes->Update();

	Mirrors->Update();

	// Hp更新
	pHP_UI_BACK->Update();
	pHP_UI_FRONT->Update();

	// Mp更新
	pMP_UI->Update();

	// Score更新
	pScore_Frame_UI->Update();

	Enemies->Update();
	Goals->Update();
	// Number更新
	pScore_UI->Update();
	//レベルエディター更新
	pLevel_Editor_UI->Update();
	pLevel_Editor_MOVEMODE_UI->Update();
	pLevel_Editor_STATICMODE_UI->Update();
	pSpeed_MoveObject_UI->Update();
	pDelay_MoveObject_UI->Update();
	return nSceneType;
}

void SceneGame::Draw()
{
	DXWindow3D* pMainWindow = GetMainWindow();
	// Zバッファ有効
	SetZBuffer(true);

	// 前面カリング (FBXは表裏が反転するため)
	ID3D11DeviceContext* pDeviceContext = pMainWindow->GetDeviceContext();
	pDeviceContext->RSSetState(pMainWindow->GetRasterizerState(1));

	// モデル描画
	SetCullMode(CULLMODE_NONE);
	Items->Draw();
	pPlayer->Draw();
	SetCullMode(CULLMODE_CCW);
	
	//pDeviceContext->RSSetState(pMainWindow->GetRasterizerState(2));
	Enemies->Draw();
	Goals->Draw();

	SkySphere->Draw();
	// フィールド描画
	Walls->Draw();
	Fields->Draw();
	
	Spikes->Draw();

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

	// Score描画
	pScore_Frame_UI->Draw();
	//レベルエディター更新
	pLevel_Editor_UI->Draw();
	pLevel_Editor_MOVEMODE_UI->Draw();
	pLevel_Editor_STATICMODE_UI->Draw();
	pSpeed_MoveObject_UI->Draw();
	pDelay_MoveObject_UI->Draw();
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

void SceneGame::SetGoalReached()
{
	bGoalReached = true;
}

Sphere3D * SceneGame::GetSkySphere()
{
	return SkySphere;
}

void SceneGame::ZoomPause(float fDistance, int nFrames, float Speed)
{
	if (!SceneCamera)
		return;
	fZoomPause = fDistance + SceneCamera->GetCurrentZoom();
	fZoomAcc = 0;
	fFramesForZoomPausing = nFrames;
	fZoomSpeed = Speed;
	fPreviousZoom = SceneCamera->GetCurrentZoom();
	hbPreviousRenderingBox = SceneCamera->GetRenderZone();
	bPauseZooming = true;
	if (fZoomSpeed <= 0)
		fFramesForZoomPausing = 0;
}

SceneGame * GetCurrentGame()
{
	return CurrentGame;
}
