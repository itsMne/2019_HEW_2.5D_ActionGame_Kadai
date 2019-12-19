#include "SceneGame.h"
#include "Wall3D.h"
#include "C_Item.h"
SceneGame* CurrentGame = nullptr;

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
	pPlayer = new Player3D();
	Fields = new Go_List();
	Walls = new Go_List();
	Items = new Go_List();
	SkySphere = new Sphere3D("data/texture/Skybox.tga");
	
	Walls->Load("Walls_Level", GO_WALL);
	//Walls->AddWall({ 105,55,0 }, { 2,80,1 });
	//Walls->AddWall({ 40,325,0 }, { 2,50,1 });

	
	SceneCamera->Init();
	SceneLight->Init();
	Fields->Load("Fields_Level", GO_FLOOR);
	//Fields->AddField({ 0,0,0 }, { 300,3, 300 }, "data/texture/field000.jpg");
	//Fields->AddField({ 80,500,0 }, { 100,3, 100 }, "data/texture/field000.jpg");
	Items->Load("Items_Level", GO_ITEM);

	InitDebugProc();
	SceneCamera->SetFocalPoint(pPlayer);

	// Hp
	HelloHp00 = new C_Ui("data/texture/HP000.png", UI_HP00);
	HelloHp01 = new C_Ui("data/texture/HP001.png", UI_HP01);
	// Mp
	HelloMp = new C_Ui("data/texture/MP000.png", UI_MP);
	
	//Items->AddItem({ 10,0,0 }, TYPE_ODEN);
	HelloNumber = new C_Ui("data/texture/number.png", UI_NUMBER);
	// Score
	HelloScore = new C_Ui("data/texture/frame_score.png", UI_SCORE);

}

void SceneGame::Uninit()
{
	// �t�B�[���h�I������
	SAFE_DELETE(Fields);
	SAFE_DELETE(pPlayer);
	SAFE_DELETE(SkySphere);
	// ���f���\���I������
	//HelloModel->UninitModel();

	// �����I������
	SceneLight->End();

	// �J�����I������
	if (SceneCamera)
		SceneCamera->End();

	// �f�o�b�O������\���I������
	UninitDebugProc();
}

int SceneGame::Update()
{
	// �f�o�b�O������\���X�V
	UpdateDebugProc();

	// �f�o�b�O������ݒ�
	StartDebugProc();
	PrintDebugProc("FPS:%d\n\n", GetMainWindowFPS());

	// �����X�V
	SceneLight->Update();

	// �J�����X�V
	SceneCamera->Update();

	// ���f���X�V
	pPlayer->Update();

	// �t�B�[���h�X�V
	Fields->Update();

	SkySphere->Update();

	Walls->Update();
	
	Items->Update();

	// Hp�X�V
	HelloHp00->Update();
	HelloHp01->Update();

	// Mp�X�V
	HelloMp->Update();

	// Score�X�V
	HelloScore->Update();

	// Number�X�V
	HelloNumber->Update();
	return nSceneType;
}

void SceneGame::Draw()
{
	DXWindow3D* pMainWindow = GetMainWindow();
	// Z�o�b�t�@�L��
	SetZBuffer(true);

	// �O�ʃJ�����O (FBX�͕\�������]���邽��)
	ID3D11DeviceContext* pDeviceContext = pMainWindow->GetDeviceContext();
	pDeviceContext->RSSetState(pMainWindow->GetRasterizerState(1));

	// ���f���`��
	SetCullMode(CULLMODE_NONE);
	Items->Draw();
	pPlayer->Draw();
	SetCullMode(CULLMODE_CCW);
	


	SkySphere->Draw();
	// �t�B�[���h�`��
	Walls->Draw();
	Fields->Draw();
	
	

	// �w�ʃJ�����O (�ʏ�͕\�ʂ̂ݕ`��)
	pDeviceContext->RSSetState(pMainWindow->GetRasterizerState(2));
	// Z�o�b�t�@����
	SetZBuffer(false);
	// Hp�`��
	HelloHp00->Draw();
	HelloHp01->Draw();

	// Mp�`��
	HelloMp->Draw();

	// Number�`��
	HelloNumber->Draw();

	// Score�`��
	HelloScore->Draw();
	// �f�o�b�O������\��
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

int SceneGame::GetScore()
{
	return nScore;
}

SceneGame * GetCurrentGame()
{
	return CurrentGame;
}
