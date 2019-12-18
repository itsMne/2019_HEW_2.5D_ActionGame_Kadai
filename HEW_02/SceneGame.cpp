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
	// �t�B�[���h�I������
	HelloField->UninitField();
	SAFE_DELETE(HelloPlayer);
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
	HelloPlayer->Update();

	// �t�B�[���h�X�V
	HelloField->UpdateField();
	HelloField2->UpdateField();

	SkySphere->Update();

	HelloWallA->Update();
	HelloWallB->Update();
	
	HelloItem->Update();

	// Hp�X�V
	HelloHp00->Update();
	HelloHp01->Update();

	// Mp�X�V
	HelloMp->Update();
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
	HelloItem->Draw();
	HelloPlayer->Draw();
	SetCullMode(CULLMODE_CCW);
	

	// �w�ʃJ�����O (�ʏ�͕\�ʂ̂ݕ`��)
	//pDeviceContext->RSSetState(pMainWindow->GetRasterizerState(2));

	SkySphere->Draw();
	// �t�B�[���h�`��
	HelloWallA->Draw();
	HelloWallB->Draw();
	HelloField->DrawField();
	HelloField2->DrawField();
	
	
	
	// Z�o�b�t�@����
	SetZBuffer(false);
	// Hp�`��
	HelloHp00->Draw();
	HelloHp01->Draw();

	// Mp�`��
	HelloMp->Draw();
	// �f�o�b�O������\��
	DrawDebugProc();
}
