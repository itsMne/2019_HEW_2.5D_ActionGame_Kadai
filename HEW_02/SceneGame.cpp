#include "SceneGame.h"
#include "Wall3D.h"
#include "C_Item.h"
#include "C_Ui.h"
SceneGame* CurrentGame = nullptr;

C_Ui* HelloHp00 = nullptr;
C_Ui* HelloHp01 = nullptr;
C_Ui* HelloMp = nullptr;
C_Item* HelloItem = nullptr;
SceneGame::SceneGame(): SceneBase()
{
	Init();
	CurrentGame = this;
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
	pPlayer = new Player3D();
	Fields = new Go_List();
	Walls = new Go_List();
	SkySphere = new Sphere3D("data/texture/Skybox.tga");
	
	Walls->Load("Walls_Level", GO_WALL);
	//Walls->AddWall({ 105,55,0 }, { 2,80,1 });
	//Walls->AddWall({ 40,325,0 }, { 2,50,1 });

	
	SceneCamera->Init();
	SceneLight->Init();
	Fields->Load("Fields_Level", GO_FLOOR);
	//Fields->AddField({ 0,0,0 }, { 300,3, 300 }, "data/texture/field000.jpg");
	//Fields->AddField({ 80,500,0 }, { 100,3, 100 }, "data/texture/field000.jpg");

	InitDebugProc();
	SceneCamera->SetFocalPoint(pPlayer);

	// Hp
	HelloHp00 = new C_Ui("data/texture/HP000.png", UI_HP00);
	HelloHp01 = new C_Ui("data/texture/HP001.png", UI_HP01);
	// Mp
	HelloMp = new C_Ui("data/texture/MP000.png", UI_MP);
	HelloItem = new C_Item(TYPE_ODEN);
	HelloItem->SetPosition({ 10,0,0 });


}

void SceneGame::Uninit()
{
	// フィールド終了処理
	SAFE_DELETE(Fields);
	SAFE_DELETE(pPlayer);
	SAFE_DELETE(SkySphere);
	// モデル表示終了処理
	//HelloModel->UninitModel();

	// 光源終了処理
	SceneLight->End();

	// カメラ終了処理
	if (SceneCamera)
		SceneCamera->End();

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
	
	HelloItem->Update();

	// Hp更新
	HelloHp00->Update();
	HelloHp01->Update();

	// Mp更新
	HelloMp->Update();
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
	HelloItem->Draw();
	pPlayer->Draw();
	SetCullMode(CULLMODE_CCW);
	


	SkySphere->Draw();
	// フィールド描画
	Walls->Draw();
	Fields->Draw();
	
	

	// 背面カリング (通常は表面のみ描画)
	pDeviceContext->RSSetState(pMainWindow->GetRasterizerState(2));
	// Zバッファ無効
	SetZBuffer(false);
	// Hp描画
	HelloHp00->Draw();
	HelloHp01->Draw();

	// Mp描画
	HelloMp->Draw();
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

SceneGame * GetCurrentGame()
{
	return CurrentGame;
}
