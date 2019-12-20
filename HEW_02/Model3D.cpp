#include "Model3D.h"
#include "main.h"
#include "FbxModel.h"
#include "input.h"
#include "Camera3D.h"
#include "Light3D.h"
#include "GameObject3D.h"
#include "UniversalStructures.h"
#include "debugproc.h"

#define MAX_LOOPS	9000

Model3D::Model3D()
{
	nFramCount = 0;
	nCountLoop = 0;
	AnimationFrameSlowness = 0;
	Parent = nullptr;
	fAnimSpeed = 2;
	bCanLoop = true;
	AnimationFrame = false;
}


Model3D::~Model3D()
{
	UninitModel();
}

//=============================================================================
// ����������
//=============================================================================
HRESULT Model3D::InitModel(const char* ModelPath, void* pParent)
{
	HRESULT hr = S_OK;
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
	fFrame = g_pModel->GetInitialAnimFrame();
	if (!pMainCamera)
	{
		printf("���C���J����������܂���\n");
		return S_OK;
	}
	hr = g_pModel->Init(pDevice, pDeviceContext, ModelPath);
	if (SUCCEEDED(hr)) {
		g_pModel->SetCamera(pMainCamera->GetCameraPos());
		if (pLight)
			g_pModel->SetLight(pLight->GetLight());
	}
	return hr;
}


//=============================================================================
// �I������
//=============================================================================
void Model3D::UninitModel(void)
{
	// FBX�t�@�C���̉��
	SAFE_DELETE(g_pModel);
}

//=============================================================================
// �X�V����
//=============================================================================
void Model3D::UpdateModel(void)
{
	//AnimationFrame = true;
}

void Model3D::SwitchAnimation(int nNewAnimNum)
{
	if (g_pModel->GetCurrentAnimation() == nNewAnimNum)
		return;
	g_pModel->SetAnimStack(nNewAnimNum);
	fFrame = g_pModel->GetInitialAnimFrame();
	nFramCount = nCountLoop = 0;
}

//=============================================================================
// �`�揈��
//=============================================================================
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

	AnimationFrame = true;
	if (AnimationFrame) {
		AnimationControl();
		AnimationFrame = false;
	}
	SetZWrite(true);
	g_pModel->Render(g_mtxWorld, pMainCamera->GetViewMatrix(), pMainCamera->GetProjMatrix(), eOpacityOnly);
	SetZWrite(false);
	g_pModel->Render(g_mtxWorld, pMainCamera->GetViewMatrix(), pMainCamera->GetProjMatrix(), eTransparentOnly);
}

void Model3D::AnimationControl()
{
	if (++nFramCount >= AnimationFrameSlowness) {
		nFramCount = 0;
		fFrame += fAnimSpeed;
		if (++fFrame >= g_pModel->GetMaxAnimFrame()) {
			if (bCanLoop) {
				if (++nCountLoop > MAX_LOOPS)
					nCountLoop = MAX_LOOPS;
				fFrame = g_pModel->GetInitialAnimFrame();
			}
			else {
				fFrame--;
			}
		}
			g_pModel->SetAnimFrame((int)fFrame);
	}
}

XMFLOAT3 Model3D::GetPosition()
{
	return Position;
}

XMFLOAT3 Model3D::GetScale()
{
	return Scale;
}

XMFLOAT4X4 * Model3D::GetModelWorld()
{
	return &g_mtxWorld;
}

XMFLOAT3 Model3D::GetRotation()
{
	return Rotation;
}

void Model3D::SetRotationY(float y)
{
	Rotation.y = y;
}

int Model3D::GetMaxNumberofAnimations()
{
	if (g_pModel)
		return g_pModel->GetMaxNumberOfAnimations();
	return 0;
}

void Model3D::SwitchAnimationSlowness(int nFrameSpeed)
{
	AnimationFrameSlowness = nFrameSpeed;
}

void Model3D::SwitchAnimationSpeed(float nFrameSpeed)
{
	fAnimSpeed = nFrameSpeed;
}

void Model3D::SetScale(float newScale)
{
	Scale = { newScale ,newScale ,newScale };
}

void Model3D::SetScale(XMFLOAT3 newScale)
{
	Scale = newScale;
}

void Model3D::ReduceScaleOnX(float red)
{
	Scale.x -= red;
}

void Model3D::ReduceScaleOnY(float red)
{
	Scale.y -= red;
}

void Model3D::ReduceScaleOnZ(float red)
{
	Scale.z -= red;
}

void Model3D::SetPosition(XMFLOAT3 newPos)
{
	Position = newPos;
}

void Model3D::SetPositionX(float newPos)
{
	Position.x = newPos;
}

void Model3D::SetPositionY(float newPos)
{
	Position.y = newPos;
}

void Model3D::SetPositionZ(float newPos)
{
	Position.z = newPos;
}

void Model3D::SetRotation(XMFLOAT3 rot)
{
	Rotation = rot;
}

void Model3D::RotateAroundY(float y)
{
	Rotation.y += y;
	/*if (Rotation.y < -XM_PI) {
		Rotation.y += XM_2PI;
	}*/
}

int Model3D::GetEndFrameOfCurrentAnimation()
{
	if (g_pModel)
		return g_pModel->GetMaxAnimFrame();
	return 0;
}

int Model3D::GetCurrentFrame()
{
	if (g_pModel)
		return fFrame;
	return 0;
}

int Model3D::GetLoops()
{
	return nCountLoop;
}

void Model3D::SetCanLoop(bool loop)
{
	bCanLoop = loop;
}
