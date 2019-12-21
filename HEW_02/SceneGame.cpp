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
	SAFE_DELETE(Fields);//���I������
	SAFE_DELETE(pPlayer);//�v���C���[�I������
	SAFE_DELETE(SkySphere);//�X�J�C�X�t�B�A�I������
	SAFE_DELETE(SceneLight);// ���I������
	SAFE_DELETE(SceneCamera);// �J�����I������

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

	Spikes->Update();
	// Hp�X�V
	pHP_UI_BACK->Update();
	pHP_UI_FRONT->Update();

	// Mp�X�V
	pMP_UI->Update();

	// Score�X�V
	pSCORE_FRAME_UI->Update();

	if(HelloEnemy)
		HelloEnemy->Update();

	// Number�X�V
	pSCORE_UI->Update();
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

	if (HelloEnemy)
		HelloEnemy->Draw();
	SetCullMode(CULLMODE_CCW);
	


	SkySphere->Draw();
	// �t�B�[���h�`��
	Walls->Draw();
	Fields->Draw();
	
	Spikes->Draw();

	// �w�ʃJ�����O (�ʏ�͕\�ʂ̂ݕ`��)
	
	// Z�o�b�t�@����
	SetZBuffer(false);
	// Hp�`��
	pHP_UI_BACK->Draw();
	pHP_UI_FRONT->Draw();

	// Mp�`��
	pMP_UI->Draw();

	// Number�`��
	pSCORE_UI->Draw();

	// Score�`��
	pSCORE_FRAME_UI->Draw();
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
