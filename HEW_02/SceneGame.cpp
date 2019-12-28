#include "SceneGame.h"
#include "Enemy3D.h"
#include "Mirror3D.h"
#include "Camera3D.h"
#define RENDER_BOX_CAMERA { 0,0,0, 200,200,120 };
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
	nFramesForZoomPausing = 0;
	fZoomSpeed = 0;
	nTimeSeconds = 0;
	nFrameCounter = 0;
	nSceneType = SCENE_GAME;
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
	pZoomAttack_UI = new C_Ui("data/texture/ZoomEffect.tga", UI_ZOOM_ATTACK);
	fCurrentInGameZoom = SceneCamera->GetCurrentZoom();
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
		return SCENE_GAMEOVER;//�Q�[���I�[�o�[�Ȃ�
	}
	if (bGoalReached)
	{
		return SCENE_CLEAR;//��Ŏ��̃V�[���ŕύX����
	}

	//UI�Ɋւ���
	pScore_UI->Update();// Number�X�V
	pLevel_Editor_UI->Update();//���x���G�f�B�^�[�X�V
	pLevel_Editor_MOVEMODE_UI->Update();
	pLevel_Editor_STATICMODE_UI->Update();
	pSpeed_MoveObject_UI->Update();
	pDelay_MoveObject_UI->Update();
	pZoomAttack_UI->Update();
	pHP_UI_BACK->Update();// Hp�X�V
	pHP_UI_FRONT->Update();
	pMP_UI->Update();// Mp�X�V
	pScore_Frame_UI->Update();// Score�X�V
	
	//�w�i�ɂ��񂷂�
	SkySphere->Update();//�X�J�C�X�t�B�A
	SceneCamera->Update();//�J����

	//�t���[���ƃY�[���Ɋւ��ăG�t�F�N�g

	
	if (++nFrameCounter == 60) {//�^�C���̃J�E���^�[
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
	if (--nPauseFrames > 0)
		return nSceneType;
	nPauseFrames = 0;

	UpdateDebugProc();// �f�o�b�O������\���X�V
	StartDebugProc();// �f�o�b�O������ݒ�
	PrintDebugProc("FPS:%d\n\n", GetMainWindowFPS());// �f�o�b�O������ݒ�
	SceneLight->Update();// �����X�V

	// �Q�[���̃I�u�W�F�N�g
	pPlayer->Update();//�v���C���[
	Fields->Update();//��
	Walls->Update();//��
	Items->Update();//�A�C�e��
	Spikes->Update();//�X�p�C�N
	Mirrors->Update();//���i�e���|�[�g�j
	Enemies->Update();//�G
	Goals->Update();//�S�[��

	return nSceneType;
}

void SceneGame::Draw()
{
	DXWindow3D* pMainWindow = GetMainWindow();
	// Z�o�b�t�@�L��
	SetZBuffer(true);

	// �O�ʃJ�����O (FBX�͕\�������]���邽��)
	ID3D11DeviceContext* pDeviceContext = pMainWindow->GetDeviceContext();
	

	// ���f���`��
	SetCullMode(CULLMODE_NONE);
	Items->Draw();
	pPlayer->Draw();
	pDeviceContext->RSSetState(pMainWindow->GetRasterizerState(1));
	Enemies->Draw();
	SetCullMode(CULLMODE_CCW);
	
	//pDeviceContext->RSSetState(pMainWindow->GetRasterizerState(2));
	
	Goals->Draw();

	SkySphere->Draw();
	// �t�B�[���h�`��
	Walls->Draw();
	Spikes->Draw();
	Fields->Draw();
	
	

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
	pZoomAttack_UI->Draw();
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

SceneGame * GetCurrentGame()
{
	return CurrentGame;
}
