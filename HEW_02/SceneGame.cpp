#include "SceneGame.h"
#include "Enemy3D.h"

SceneGame* CurrentGame = nullptr;
Enemy3D* HelloEnemy = nullptr;
SceneGame::SceneGame(): SceneBase()
{
	Init();
	CurrentGame = this;
	nScore = 0;
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
	nSceneType = SCENE_GAME;
	MainWindow = GetMainWindow();
	if (MainWindow)
		MainWindow->SetWindowColor(231.0f / 255.0f, 182.0f / 255.0f, 128.0f / 255.0f);
	
	SceneCamera = new Camera3D(true);
	SceneLight = new Light3D();
	//HelloEnemy = new Enemy3D(TYPE_ONI);
	pPlayer = new Player3D();
	Fields = new Go_List();
	Walls = new Go_List();
	Items = new Go_List();
	Spikes = new Go_List();
	SkySphere = new Sphere3D("data/texture/Skybox.tga");
	

	SceneCamera->Init();
	SceneLight->Init();
	Walls->Load("Walls_Level", GO_WALL);
	Fields->Load("Fields_Level", GO_FLOOR);
	Items->Load("Items_Level", GO_ITEM);
	//Items->AddItem({ 10,0,0 }, TYPE_ODEN);
	//Fields->AddField({ 0,0,0 }, { 300,3, 300 }, "data/texture/field000.jpg");
	//Fields->AddField({ 80,500,0 }, { 100,3, 100 }, "data/texture/field000.jpg");
	//Walls->AddWall({ 105,55,0 }, { 2,80,1 });
	//Walls->AddWall({ 40,325,0 }, { 2,50,1 });
	

	InitDebugProc();
	SceneCamera->SetFocalPoint(pPlayer);
	Spikes->Load("Spikes_Level", GO_SPIKE);
	//Spikes->AddSpike({ 10,0,0 }, 1, 1, false);
	//HelloSpike = new Spike3D();

	// Hp
	pHP_UI_BACK = new C_Ui("data/texture/HP000.png", UI_HP00);
	pHP_UI_FRONT = new C_Ui("data/texture/HP001.png", UI_HP01);
	// Mp
	pMP_UI = new C_Ui("data/texture/MP000.png", UI_MP);
	
	pSCORE_UI = new C_Ui("data/texture/number.png", UI_NUMBER);
	// Score
	pSCORE_FRAME_UI = new C_Ui("data/texture/frame_score.png", UI_SCORE);

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
	// デバッグ文字列表示更新
	UpdateDebugProc();

	// デバッグ文字列設定
	StartDebugProc();
	PrintDebugProc("FPS:%d\n\n", GetMainWindowFPS());

	// 光源更新
	SceneLight->Update();

	// カメラ更新
	SceneCamera->Update();

	// モデル更新
	pPlayer->Update();

	// フィールド更新
	Fields->Update();

	SkySphere->Update();

	Walls->Update();
	
	Items->Update();

	Spikes->Update();
	// Hp更新
	pHP_UI_BACK->Update();
	pHP_UI_FRONT->Update();

	// Mp更新
	pMP_UI->Update();

	// Score更新
	pSCORE_FRAME_UI->Update();

	if(HelloEnemy)
		HelloEnemy->Update();

	// Number更新
	pSCORE_UI->Update();
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

	if (HelloEnemy)
		HelloEnemy->Draw();
	SetCullMode(CULLMODE_CCW);
	


	SkySphere->Draw();
	// フィールド描画
	Walls->Draw();
	Fields->Draw();
	
	Spikes->Draw();

	// 背面カリング (通常は表面のみ描画)
	
	// Zバッファ無効
	SetZBuffer(false);
	// Hp描画
	pHP_UI_BACK->Draw();
	pHP_UI_FRONT->Draw();

	// Mp描画
	pMP_UI->Draw();

	// Number描画
	pSCORE_UI->Draw();

	// Score描画
	pSCORE_FRAME_UI->Draw();
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

int SceneGame::GetScore()
{
	return nScore;
}

SceneGame * GetCurrentGame()
{
	return CurrentGame;
}
