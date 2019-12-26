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

	//���x���G�f�B�^�[
	pLevel_Editor_UI = new C_Ui("data/texture/UI_LEINSTRUCTIONS.tga", UI_LEVEL_EDITOR);
	pLevel_Editor_MOVEMODE_UI = new C_Ui("data/texture/UI_MOVEABLEMODE.tga", UI_LEVEL_EDITOR_MOVEABLEMODE);
	pLevel_Editor_STATICMODE_UI = new C_Ui("data/texture/UI_STATICMODE.tga", UI_LEVEL_EDITOR_STATICMODE);
	pSpeed_MoveObject_UI = new C_Ui("data/texture/UI_LEVEL_EDITOR_NUM.tga", UI_LEVEL_EDITOR_OBJSPEED);
	pDelay_MoveObject_UI = new C_Ui("data/texture/UI_LEVEL_EDITOR_NUM.tga", UI_LEVEL_EDITOR_DELAY);

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
	if (pPlayer->PlayerGameOver())
	{
		return SCENE_GAMEOVER;
	}
	if (bGoalReached)
	{
		return SCENE_CLEAR;//��Ŏ��̃V�[���ŕύX����
	}
	if (++nFrameCounter == 60) {//�^�C���̃J�E���^�[
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
	// �f�o�b�O������\���X�V
	UpdateDebugProc();

	// �f�o�b�O������ݒ�
	StartDebugProc();
	PrintDebugProc("FPS:%d\n\n", GetMainWindowFPS());

	// �����X�V
	SceneLight->Update();

	// �J�����X�V
	
	
	// ���f���X�V
	pPlayer->Update();

	// �t�B�[���h�X�V
	Fields->Update();

	SkySphere->Update();

	Walls->Update();
	
	Items->Update();

	Spikes->Update();

	Mirrors->Update();

	// Hp�X�V
	pHP_UI_BACK->Update();
	pHP_UI_FRONT->Update();

	// Mp�X�V
	pMP_UI->Update();

	// Score�X�V
	pScore_Frame_UI->Update();

	Enemies->Update();
	Goals->Update();
	// Number�X�V
	pScore_UI->Update();
	//���x���G�f�B�^�[�X�V
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
	
	//pDeviceContext->RSSetState(pMainWindow->GetRasterizerState(2));
	Enemies->Draw();
	Goals->Draw();

	SkySphere->Draw();
	// �t�B�[���h�`��
	Walls->Draw();
	Fields->Draw();
	
	Spikes->Draw();

	Mirrors->Draw();
	
	SceneCamera->DrawRenderZone();
	// Z�o�b�t�@����
	SetZBuffer(false);
	// Hp�`��
	pHP_UI_BACK->Draw();
	pHP_UI_FRONT->Draw();

	// Mp�`��
	pMP_UI->Draw();

	// Number�`��
	pScore_UI->Draw();

	// Score�`��
	pScore_Frame_UI->Draw();
	//���x���G�f�B�^�[�X�V
	pLevel_Editor_UI->Draw();
	pLevel_Editor_MOVEMODE_UI->Draw();
	pLevel_Editor_STATICMODE_UI->Draw();
	pSpeed_MoveObject_UI->Draw();
	pDelay_MoveObject_UI->Draw();
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
