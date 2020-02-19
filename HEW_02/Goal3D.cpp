//*****************************************************************************
// Goal3D.cpp
// �S�[���̊Ǘ�
//*****************************************************************************
#include "Goal3D.h"
#include "Player3D.h"
#include "Sound.h"
#include "SceneGame.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define GOAL_MODEL_PATH "data/model/Goal.fbx"

//*****************************************************************************
// �R���X�g���N�^�֐�
//*****************************************************************************
Goal3D::Goal3D(): GameObject3D()
{
	Init();
}


Goal3D::~Goal3D()
{
	GameObject3D::~GameObject3D();
	Uninit();
}

//*****************************************************************************
//Init�֐�
//�������֐�
//�����Fvoid
//�߁Fvoid
//*****************************************************************************
void Goal3D::Init()
{
	InitModel(GOAL_MODEL_PATH);
	pModel->SetScale(0.025f);
	pModel->SetRotationY(180);
	hitbox = {0,25,0,3,12,3};
	nType = GO_GOAL;
}

//*****************************************************************************
//Update�֐�
//�ύX�֐�
//�����Fvoid
//�߁Fvoid
//*****************************************************************************
void Goal3D::Update()
{
	GameObject3D::Update();
	Player3D* pPlayer = GetMainPlayer();
#if USE_IN_RENDERZONE
	if (pPlayer) {
		if (!(GetMainCamera()->IsOnRenderZone(GetHitBox())) && pPlayer->GetState()!=PLAYER_OVER)
			return;
		if (pPlayer->IsDebugAimOn())
			return;
	}
#endif
	if (!pPlayer)
		return;
	if (IsInCollision3D(pPlayer->GetHitBox(HB_BODY), GetHitBox()))
	{
		if (pPlayer->GetState() != PLAYER_OVER) {
			StopSound();
			PlaySoundGame(SOUND_LABEL_SE_END_REACHED);
		}
		pPlayer->SetPlayerState(PLAYER_OVER);
		
	}
}

//*****************************************************************************
//Draw�֐�
//�����_�����O�֐�
//�����Fvoid
//�߁Fvoid
//*****************************************************************************
void Goal3D::Draw()
{
	Player3D* pPlayer = GetMainPlayer();
#if USE_IN_RENDERZONE
	if (pPlayer) {
		if (!(GetMainCamera()->IsOnRenderZone(GetHitBox())))
			return;
	}
#endif
	GameObject3D::Draw();
}

//*****************************************************************************
//Uninit�֐�
//�I���֐�
//�����Fvoid
//�߁Fvoid
//*****************************************************************************
void Goal3D::Uninit()
{
	//�Ȃ�
}
