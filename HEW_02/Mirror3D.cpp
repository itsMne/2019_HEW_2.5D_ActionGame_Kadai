//*****************************************************************************
// Mirror3D.cpp
// ���i�e���|�[�g�j�̊Ǘ�
//*****************************************************************************
#include "Mirror3D.h"
#include "Player3D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_MIRROR "data/model/Mirror.fbx"

//*****************************************************************************
// �R���X�g���N�^�֐�
//*****************************************************************************
Mirror3D::Mirror3D(): GameObject3D()
{
	Init({0,0,0});
}

Mirror3D::Mirror3D(XMFLOAT3 Destination)
{
	Init(Destination);
}

Mirror3D::~Mirror3D()
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
void Mirror3D::Init(XMFLOAT3 Destination)
{
	x3DestinationPos = Destination;
	InitModel(MODEL_MIRROR);
	pModel->SetScale({ 1, 1, 1 });
	hitbox = { 0,10,0,7,7,5 };
	Position.x += 20;
	nType = GO_MIRROR;
}

//*****************************************************************************
//Update�֐�
//�ύX�֐�
//�����Fvoid
//�߁Fvoid
//*****************************************************************************
void Mirror3D::Update()
{
	GameObject3D::Update();
	Player3D* pPlayer = GetMainPlayer();
#if USE_IN_RENDERZONE
	if (!(GetMainCamera()->IsOnRenderZone(GetHitBox())))
		return;
#endif
	if (!pPlayer)
		return;
	if (IsInCollision3D(pPlayer->GetHitBox(HB_BODY), GetHitBox()) && !(pPlayer->IsDebugAimOn())) {
		PLAYER_ATTACK_MOVE* pAttack = pPlayer->GetPlayerAttack();
		if (pAttack)
		{
			if (pAttack->Animation == SAMURAI_STINGER)
				pPlayer->CancelAttack();
		}
		pPlayer->SetPlayerTeleporting(x3DestinationPos);
	}
}

//*****************************************************************************
//Draw�֐�
//�����_�����O�֐�
//�����Fvoid
//�߁Fvoid
//*****************************************************************************
void Mirror3D::Draw()
{
	Player3D* pPlayer = GetMainPlayer();
#if USE_IN_RENDERZONE
	if (!(GetMainCamera()->IsOnRenderZone(GetHitBox())))
		return;
#endif
	GameObject3D::Draw();
}

//*****************************************************************************
//Uninit�֐�
//�I���֐�
//�����Fvoid
//�߁Fvoid
//*****************************************************************************
void Mirror3D::Uninit()
{
	//�Ȃ�
}

//*****************************************************************************
//SetDestination�֐�
//�v���C���[�Ɠ����蔻�肵����ŁA�v���C���[�̋��_��ݒ肷��
//�����FXMFLOAT3
//�߁Fvoid
//*****************************************************************************
void Mirror3D::SetDestination(XMFLOAT3 newDest)
{
	x3DestinationPos = newDest;
}

//*****************************************************************************
//GetDestination�֐�
//�v���C���[�Ɠ����蔻�肵����ŁA�v���C���[�̋��_��߂�
//�����Fvoid
//�߁FXMFLOAT3
//*****************************************************************************
XMFLOAT3 Mirror3D::GetDestination()
{
	return x3DestinationPos;
}
