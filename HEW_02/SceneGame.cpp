#include "SceneGame.h"
#include "Wall3D.h"
#include "C_Item.h"
#include "C_Ui.h"

Wall3D* HelloWallA = nullptr;
Wall3D* HelloWallB = nullptr;
C_Ui* HelloHp00 = nullptr;
C_Ui* HelloHp01 = nullptr;
C_Ui* HelloMp = nullptr;
Field3D* HelloField2 = nullptr;
C_Item* HelloItem = nullptr;
SceneGame::SceneGame(): SceneBase()
{
	Init();
}


SceneGame::~SceneGame()
{

}

void SceneGame::Init()
{
	nScore = 0;
	nSceneType = SCENE_GAME;
	MainWindow = GetMainWindow();
	if (MainWindow)
		MainWindow->SetWindowColor(231.0f / 255.0f, 182.0f / 255.0f, 128.0f / 255.0f);
	SceneCamera = new Camera3D(true);
	SceneLight = new Light3D();
	HelloPlayer = new Player3D();
	HelloField = new Field3D();
	HelloField2 = new Field3D();
	SkySphere = new Sphere3D("data/texture/Skybox.tga");
	HelloWallA = new Wall3D();
	HelloWallA->SetPosition({ 105,55,0 });
	HelloWallA->SetScale({ 2,80,1 });

	HelloWallB = new Wall3D();
	HelloWallB->SetPosition({ 40,325,0 });
	HelloWallB->SetScale({ 2,50,1 });

	g_pDevice = GetDevice();
	SceneCamera->Init();
	SceneLight->Init();
	HelloField->Init("data/texture/field000.jpg");
	HelloField->SetScaleWithHitbox(300);
	
	HelloField2->Init("data/texture/field000.jpg");
	HelloField2->SetScaleWithHitbox(100);
	HelloField2->SetPosition({ 80,500,0 });
	InitDebugProc();
	SceneCamera->SetFocalPoint(HelloPlayer);

	// Hp
	HelloHp00 = new C_Ui("data/texture/HP000.png", UI_HP00);
	HelloHp01 = new C_Ui("data/texture/HP001.png", UI_HP01);
	// Mp
	HelloMp = new C_Ui("data/texture/MP000.png", UI_MP);
	HelloItem = new C_Item();
	HelloItem->SetPosition({ 10,0,0 });
}

void SceneGame::Uninit()
{
	// フィールド終了処理
	HelloField->UninitField();
	SAFE_DELETE(HelloPlayer);
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
	HelloPlayer->Update();

	// フィールド更新
	HelloField->UpdateField();
	HelloField2->UpdateField();

	SkySphere->Update();

	HelloWallA->Update();
	HelloWallB->Update();
	
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
	HelloPlayer->Draw();
	SetCullMode(CULLMODE_CCW);
	

	// 背面カリング (通常は表面のみ描画)
	//pDeviceContext->RSSetState(pMainWindow->GetRasterizerState(2));

	SkySphere->Draw();
	// フィールド描画
	HelloWallA->Draw();
	HelloWallB->Draw();
	HelloField->DrawField();
	HelloField2->DrawField();
	
	
	
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
