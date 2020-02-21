//*****************************************************************************
// Model3D.cpp
// 3D���f���̊Ǘ�
//*****************************************************************************
#include "Model3D.h"
#include "main.h"
#include "FbxModel.h"
#include "input.h"
#include "Camera3D.h"
#include "Light3D.h"
#include "GameObject3D.h"
#include "UniversalStructures.h"
#include "debugproc.h"
#include "SceneGame.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_LOOPS	9000	

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
char ModelPaths[MAX_PRELOADED_MODELS][256] =
{
	"data/model/RedOni.fbx"	  ,
	"data/model/GreenOni.fbx" ,
	"data/model/YellowOni.fbx",
	"data/model/BlueOni.fbx"  ,
	"data/model/Warrior.fbx"  ,
	"data/model/Cube.fbx",
	"data/model/Spike.fbx",
	"data/model/SamuraiHew.fbx",
	"data/model/NinjaHew.fbx",
	"data/model/GeishaHew.fbx",
	"data/model/BossOni.fbx",

};
CFbxModel* Models[MAX_PRELOADED_MODELS] = {nullptr};

//*****************************************************************************
// �R���X�g���N�^�֐�
//*****************************************************************************
Model3D::Model3D()
{
	nFramCount = 0;
	nCountLoop = 0;
	AnimationFrameSlowness = 0;
	Parent = nullptr;
	fAnimSpeed = 2;
	bCanLoop = true;
	AnimationFrame = false;
	g_pModel = nullptr;
	bPauseAnimation = false;
}


Model3D::~Model3D()
{
	UninitModel();
}

//*****************************************************************************
//InitModel�֐�
//�������֐�
//�����Fconst char*, void*
//�߁Fvoid
//*****************************************************************************
HRESULT Model3D::InitModel(const char* ModelPath, void* pParent)
{
	HRESULT hr = S_OK;
	nCurrentAnimation = 0;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	pMainCamera = GetMainCamera();
	Parent = pParent;
	// �ʒu�A�����̏����ݒ�
	Position = XMFLOAT3(0.0f, 10.0f, 0.0f);
	Rotation = XMFLOAT3(0.0f, 90.0f, 0.0f);
	Scale = XMFLOAT3(0.1, 0.1, 0.1);
	// FBX�t�@�C���̓ǂݍ���
	g_pModel = new CFbxModel();
	Light3D* pLight = GetMainLight();
	SwitchAnimation(0);
	//fFrame = nCurrentInitialFrame = g_pModel->GetInitialAnimFrame();
	//nCurrentMaxFrame = g_pModel->GetMaxAnimFrame();
	if (!pMainCamera)
	{
		printf("���C���J����������܂���\n");
		return S_OK;
	}
	hr = g_pModel->Init(pDevice, pDeviceContext, ModelPath);
	if (SUCCEEDED(hr)) {
		g_pModel->SetCamera(pMainCamera->GetCameraPos());
		SetLight(pLight);
	}
	bPreLoadedModel = false;
	bPauseAnimation = false;
	return hr;
}

//*****************************************************************************
//InitModel�֐�
//�������֐�
//�����Fint, void*
//�߁Fvoid
//*****************************************************************************
HRESULT Model3D::InitModel(int ModelPath, void * pParent)
{
	if (ModelPath > MAX_PRELOADED_MODELS || ModelPath < 0)
		return S_OK;
	HRESULT hr = S_OK;
	nCurrentAnimation = 0;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	pMainCamera = GetMainCamera();
	if (!pMainCamera)
	{
		printf("���C���J����������܂���\n");
		return S_OK;
	}
	Parent = pParent;
	// �ʒu�A�����̏����ݒ�
	Position = XMFLOAT3(0.0f, 10.0f, 0.0f);
	Rotation = XMFLOAT3(0.0f, 90.0f, 0.0f);
	Scale = XMFLOAT3(0.1, 0.1, 0.1);
	// FBX�t�@�C���̓ǂݍ���
	if (!Models[ModelPath]) {
		Models[ModelPath] = new CFbxModel();
		Light3D* pLight = GetMainLight();
		SwitchAnimation(0);
		Models[ModelPath]->SetCamera(pMainCamera->GetCameraPos());
		Models[ModelPath]->Init(pDevice, pDeviceContext, ModelPaths[ModelPath]);
		g_pModel = Models[ModelPath];
		SetLight(pLight);
	}
	else {
		g_pModel = Models[ModelPath];
	}
	bPreLoadedModel = true;
	return S_OK;
}

void Model3D::SetLight(Light3D * pLight)
{
	if(g_pModel)
		g_pModel->SetLight(pLight->GetLight());
}

//*****************************************************************************
//UninitModel�֐�
//�I���֐�
//�����Fvoid
//�߁Fvoid
//*****************************************************************************
void Model3D::UninitModel(void)
{
	// FBX�t�@�C���̉��
	if(!bPreLoadedModel)
		SAFE_DELETE(g_pModel);
}

//*****************************************************************************
//UpdateModel�֐�
//�ύX�֐�
//�����Fvoid
//�߁Fvoid
//*****************************************************************************
void Model3D::UpdateModel(void)
{
	//�Ȃ�
}

//*****************************************************************************
//SwitchAnimation�֐�
//�A�j���[�V������ς���
//�����Fint
//�߁Fvoid
//*****************************************************************************
void Model3D::SwitchAnimation(int nNewAnimNum)
{
	if (!g_pModel)
		return;
	g_pModel->SetAnimStack(nCurrentAnimation);
	if (nCurrentAnimation == nNewAnimNum)
		return;
	nCurrentAnimation = nNewAnimNum;
	g_pModel->SetAnimStack(nNewAnimNum);
	fFrame = nCurrentInitialFrame = g_pModel->GetInitialAnimFrame();
	nCurrentMaxFrame = g_pModel->GetMaxAnimFrame();
	nFramCount = nCountLoop = 0;
}

//*****************************************************************************
//DrawModel�֐�
//�����_�����O�֐�
//�����Fvoid
//�߁Fvoid
//*****************************************************************************
void Model3D::DrawModel(void)
{
	if (!pMainCamera)
	{
		printf("���C���J����������܂���\n");
		return;
	}

	GameObject3D* goParent = (GameObject3D*)Parent;//�e�̃|�C���^�[���g��
	XMFLOAT3 ParentPos = {0,0,0};
	XMFLOAT3 ParentRot = {0,0,0};
	XMFLOAT3 ParentScale = {1,1,1};//�e�̋��_�Ɛe�̑傫��
	if (goParent) {
		ParentPos = goParent->GetPosition();
		ParentScale = goParent->GetScale();
		ParentRot = goParent->GetRotation();
	}
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	XMMATRIX mtxWorld, mtxRot, mtxTranslate, mtxScale;

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();
	XMStoreFloat4x4(&this->g_mtxWorld, mtxWorld);
	//�T�C�Y
	mtxScale = XMMatrixScaling(Scale.x * ParentScale.x, Scale.y * ParentScale.y, Scale.z * ParentScale.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScale);

	// ��]�𔽉f
	mtxRot = XMMatrixRotationRollPitchYaw(Rotation.x + ParentRot.x, Rotation.y + ParentRot.y, Rotation.z + ParentRot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);



	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(Position.x + ParentPos.x, Position.y + ParentPos.y, Position.z + ParentPos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);



	// ���[���h�}�g���b�N�X�̐ݒ�
	XMStoreFloat4x4(&g_mtxWorld, mtxWorld);

	SwitchAnimation(nCurrentAnimation);
	AnimationControl();
	SetZWrite(true);
	g_pModel->Render(g_mtxWorld, pMainCamera->GetViewMatrix(), pMainCamera->GetProjMatrix(), eOpacityOnly);
	SetZWrite(false);
	g_pModel->Render(g_mtxWorld, pMainCamera->GetViewMatrix(), pMainCamera->GetProjMatrix(), eTransparentOnly);
}

//*****************************************************************************
//AnimationControl�֐�
//�A�j���[�V�������Ǘ�����
//�����Fvoid
//�߁Fvoid
//*****************************************************************************
void Model3D::AnimationControl()
{
	SceneGame* pGame = GetCurrentGame();
	if (pGame->IsUsingPauseFrames() || pGame->IsGamePaused() || bPauseAnimation) {
		g_pModel->SetAnimFrame((int)fFrame);
		return;
	}
	if (++nFramCount >= AnimationFrameSlowness) {
		nFramCount = 0;
		//printf("%f\n",fAnimSpeed);
		fFrame += fAnimSpeed;
		if (fFrame >= nCurrentMaxFrame) {
			if (bCanLoop) {
				if (++nCountLoop > MAX_LOOPS)
					nCountLoop = MAX_LOOPS;
				fFrame = nCurrentInitialFrame;
			}
			else {
				fFrame--;
			}
		}
			g_pModel->SetAnimFrame((int)fFrame);
	}
}

//*****************************************************************************
//GetPosition�֐�
//���_��߂�����
//�����Fvoid
//�߁FXMFLOAT3
//*****************************************************************************
XMFLOAT3 Model3D::GetPosition()
{
	return Position;
}

//*****************************************************************************
//GetScale�֐�
//�傫����߂�����
//�����Fvoid
//�߁FXMFLOAT3
//*****************************************************************************
XMFLOAT3 Model3D::GetScale()
{
	return Scale;
}

//*****************************************************************************
//GetModelWorld�֐�
//���[���h�}�g���b�N�X�̃A�h���X��߂�����
//�����Fvoid
//�߁FXMFLOAT4X4*
//*****************************************************************************
XMFLOAT4X4 * Model3D::GetModelWorld()
{
	return &g_mtxWorld;
}

//*****************************************************************************
//GetRotation�֐�
//��]��߂�����
//�����Fvoid
//�߁FXMFLOAT4X4*
//*****************************************************************************
XMFLOAT3 Model3D::GetRotation()
{
	return Rotation;
}

//*****************************************************************************
//SetRotationY�֐�
//��]��ݒ肷��iY���W�j
//�����Ffloat
//�߁Fvoid
//*****************************************************************************
void Model3D::SetRotationY(float y)
{
	Rotation.y = y;
}

//*****************************************************************************
//GetMaxNumberofAnimations�֐�
//�A�j���[�V�����̐���߂�
//�����Fvoid
//�߁Fint
//*****************************************************************************
int Model3D::GetMaxNumberofAnimations()
{
	if (g_pModel)
		return g_pModel->GetMaxNumberOfAnimations();
	return 0;
}

//*****************************************************************************
//SwitchAnimationSlowness�֐�
//�A�j���[�V�����̑�����ݒ肷��
//�����Fint
//�߁Fvoid
//*****************************************************************************
void Model3D::SwitchAnimationSlowness(int nFrameSpeed)
{
	AnimationFrameSlowness = nFrameSpeed;
}

//*****************************************************************************
//SwitchAnimationSpeed�֐�
//�A�j���[�V�����̑�����ݒ肷��
//�����Ffloat
//�߁Fvoid
//*****************************************************************************
void Model3D::SwitchAnimationSpeed(float fFrameSpeed)
{
	fAnimSpeed = fFrameSpeed;
}

//*****************************************************************************
//SetScale�֐�
//�傫����ݒ肷��
//�����Ffloat
//�߁Fvoid
//*****************************************************************************
void Model3D::SetScale(float newScale)
{
	Scale = { newScale ,newScale ,newScale };
}

//*****************************************************************************
//SetScale�֐�
//�傫����ݒ肷��
//�����FXMFLOAT3
//�߁Fvoid
//*****************************************************************************
void Model3D::SetScale(XMFLOAT3 newScale)
{
	Scale = newScale;
}

//*****************************************************************************
//ReduceScaleOnX�֐�
//�傫�������炷�iX���W�j
//�����Ffloat
//�߁Fvoid
//*****************************************************************************
void Model3D::ReduceScaleOnX(float red)
{
	Scale.x -= red;
}

//*****************************************************************************
//ReduceScaleOnY�֐�
//�傫�������炷�iY���W�j
//�����Ffloat
//�߁Fvoid
//*****************************************************************************
void Model3D::ReduceScaleOnY(float red)
{
	Scale.y -= red;
}

//*****************************************************************************
//ReduceScaleOnZ�֐�
//�傫�������炷�iZ���W�j
//�����Ffloat
//�߁Fvoid
//*****************************************************************************
void Model3D::ReduceScaleOnZ(float red)
{
	Scale.z -= red;
}

//*****************************************************************************
//SetPosition�֐�
//���_��ݒ肷��
//�����FXMFLOAT3
//�߁Fvoid
//*****************************************************************************
void Model3D::SetPosition(XMFLOAT3 newPos)
{
	Position = newPos;
}

//*****************************************************************************
//SetPositionX�֐�
//���_��ݒ肷��iX���W�j
//�����Ffloat
//�߁Fvoid
//*****************************************************************************
void Model3D::SetPositionX(float newPos)
{
	Position.x = newPos;
}

//*****************************************************************************
//SetPositionY�֐�
//���_��ݒ肷��iY���W�j
//�����Ffloat
//�߁Fvoid
//*****************************************************************************
void Model3D::SetPositionY(float newPos)
{
	Position.y = newPos;
}

//*****************************************************************************
//SetPositionZ�֐�
//���_��ݒ肷��iZ���W�j
//�����Ffloat
//�߁Fvoid
//*****************************************************************************
void Model3D::SetPositionZ(float newPos)
{
	Position.z = newPos;
}

//*****************************************************************************
//SetRotation�֐�
//��]��ݒ肷��
//�����FXMFLOAT3
//�߁Fvoid
//*****************************************************************************
void Model3D::SetRotation(XMFLOAT3 rot)
{
	Rotation = rot;
}

//*****************************************************************************
//RotateAroundY�֐�
//��]��ς���iY���W�j
//�����Ffloat
//�߁Fvoid
//*****************************************************************************
void Model3D::RotateAroundY(float y)
{
	Rotation.y += y;
}

//*****************************************************************************
//GetAnimation�֐�
//���݂̃A�j���[�V������߂�
//�����Fvoid
//�߁Fint
//*****************************************************************************
int Model3D::GetAnimation()
{
	return nCurrentAnimation;
}

//*****************************************************************************
//GetEndFrameOfCurrentAnimation�֐�
//�A�j���[�V�����̍Ō�̃t���[����߂�
//�����Fvoid
//�߁Fint
//*****************************************************************************
int Model3D::GetEndFrameOfCurrentAnimation()
{
	if (g_pModel)
		return nCurrentMaxFrame;
	return 0;
}

//*****************************************************************************
//GetCurrentFrame�֐�
//�A�j���[�V�����̌��݂̃t���[����߂�
//�����Fvoid
//�߁Fint
//*****************************************************************************
int Model3D::GetCurrentFrame()
{
	if (g_pModel)
		return (int)fFrame;
	return 0;
}

//*****************************************************************************
//GetLoops�֐�
//�A�j���[�V�����̃��[�v��߂�
//�����Fvoid
//�߁Fint
//*****************************************************************************
int Model3D::GetLoops()
{
	return nCountLoop;
}

//*****************************************************************************
//SetCanLoop�֐�
//�A�j���[�V���������[�v���o���邱�Ƃ�ݒ肷��
//�����Fbool
//�߁Fint
//*****************************************************************************
void Model3D::SetCanLoop(bool loop)
{
	bCanLoop = loop;
}

//*****************************************************************************
//SetFrame�֐�
//�A�j���[�V�����̃t���[����ݒ肷��
//�����Fint
//�߁Fvoid
//*****************************************************************************
void Model3D::SetFrame(int frame)
{
	fFrame = frame;
}

//*****************************************************************************
//PauseModelAnimation�֐�
//�A�j���[�V�������|�[�X����
//�����Fbool
//�߁Fvoid
//*****************************************************************************
void Model3D::PauseModelAnimation(bool pause)
{
	bPauseAnimation = pause;
}
