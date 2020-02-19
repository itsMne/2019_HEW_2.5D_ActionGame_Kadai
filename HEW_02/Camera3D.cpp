//*****************************************************************************
// Camera3D.cpp
// �r���{�[�h�̃I�u�W�F�N�g�̊Ǘ�
//*****************************************************************************
#include "Camera3D.h"	
#include "input.h"
#include "debugproc.h"
#include "GameObject3D.h"
#include <stdio.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define POS_CAMERA_P_X		(0.0f)
#define POS_CAMERA_P_Y		(100.0f)
#define POS_CAMERA_P_Z		(-200.0f)

#define POS_CAMERA_R_X		(0.0f)
#define POS_CAMERA_R_Y		(0.0f)
#define POS_CAMERA_R_Z		(0.0f)

#define VIEW_ANGLE			(XMConvertToRadians(45.0f))
#define VIEW_ASPECT			((float)SCREEN_WIDTH/(float)SCREEN_HEIGHT)
#define VIEW_NEAR			(10.0f)
#define VIEW_FAR			(1000.0f)
#define VALUE_MOVE_CAMERA	(2.0f)	
#define VALUE_ROTATE_CAMERA	(XM_PI*0.01f)	
#define OFFSET_VALUE	{0,20,-50}
#define SHOW_RENDER_ZONE false

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
Camera3D* MainCamera = nullptr;

//*****************************************************************************
// �R���X�g���N�^�֐�
//*****************************************************************************
Camera3D::Camera3D(): FocalPoint(nullptr)
{
	MainCamera = this;
}

Camera3D::Camera3D(bool bisMainCamera) : FocalPoint(nullptr)
{
	if (bisMainCamera)
		MainCamera = this;
}


Camera3D::~Camera3D()
{
	Uninit();
}

//*****************************************************************************
//Init�֐�
//�������֐�
//�����Fvoid
//�߁Fvoid
//*****************************************************************************
HRESULT Camera3D::Init()
{
	g_posCameraP = XMFLOAT3(POS_CAMERA_P_X, POS_CAMERA_P_Y, POS_CAMERA_P_Z);
	g_posCameraR = XMFLOAT3(POS_CAMERA_R_X, POS_CAMERA_R_Y, POS_CAMERA_R_Z);
	g_vecCameraU = XMFLOAT3(0, 1, 0);
	g_rotCamera = XMFLOAT3(0, 0, 0);
	nFrameShaking = 0;
	nFramesPerShakeCounter = 0;
	nFramesPerShake = 0;
	x3Shake= XMFLOAT3(0, 0, 0);
	float fVecX, fVecZ;
	fVecX = g_posCameraP.x - g_posCameraR.x;
	fVecZ = g_posCameraP.z - g_posCameraR.z;
	g_fLengthInterval = sqrtf(fVecX * fVecX + fVecZ * fVecZ);
	Offset = OFFSET_VALUE;
	pRenderZone = nullptr;
	hbRenderZone = { 0,0,0, 200,200,120 };
#if SHOW_RENDER_ZONE
	pRenderZone = new Cube3D();
	pRenderZone->Init("data/texture/hbox.tga");
	
#endif
	return S_OK;
}

//*****************************************************************************
//Update�֐�
//�ύX�֐�
//�����Fvoid
//�߁Fvoid
//*****************************************************************************
void Camera3D::Update()
{
	GameObject3D* FocusPoint = nullptr;
	FocusPoint = (GameObject3D*)FocalPoint;
	if (FocusPoint) {
		hbRenderZone = { vEye.x, vEye.y, FocusPoint->GetPosition().z, 
			hbRenderZone.SizeX,hbRenderZone.SizeY, hbRenderZone.SizeZ };
		vEye = XMFLOAT3(0, 0, -100);//���_
		vLookAt = XMFLOAT3(0, 0, 0);//�����_
		XMFLOAT3 Shake = XMFLOAT3(0, 0, 0);
		if (++nFramesPerShakeCounter > nFramesPerShake)
		{
			Shake = x3Shake;
			nFramesPerShakeCounter = 0;
		}
		vEye.x=vLookAt.x = FocusPoint->GetPosition().x+ Offset.x+Shake.x;
		vEye.y=vLookAt.y = FocusPoint->GetPosition().y+ Offset.y+Shake.y;
		vEye.z+=Offset.z+ x3Shake.z;
		x3Shake.x*=-1;
		x3Shake.y*=-1;
		x3Shake.z*=-1;
		if (--nFrameShaking <= 0)
		{
			nFrameShaking = 0;
			x3Shake = { 0,0,0 };
		}
		XMMATRIX mtxWorld = XMLoadFloat4x4(FocusPoint->GetModelWorld());
		XMMATRIX identity = XMMatrixIdentity();
		//���_
		XMStoreFloat3(&g_posCameraP, XMVector3TransformCoord(XMLoadFloat3(&vEye), identity));

		//�����_
		XMStoreFloat3(&g_posCameraR, XMVector3TransformCoord(XMLoadFloat3(&vLookAt), identity));

		g_rotCamera = FocusPoint->GetRotation();
	}

	SetCamera();
	if (pRenderZone) {
		pRenderZone->SetPosition({ hbRenderZone.x, hbRenderZone.y, hbRenderZone.z });
		pRenderZone->SetScale({ hbRenderZone.SizeX , hbRenderZone.SizeY, hbRenderZone.SizeZ });
		pRenderZone->Update();
	}
}

//*****************************************************************************
//Uninit�֐�
//�I���֐�
//�����Fvoid
//�߁Fvoid
//*****************************************************************************
void Camera3D::Uninit()
{
	//�Ȃ�
}

//*****************************************************************************
//SetCamera�֐�
//���݂̃t���[���ɃJ�����̋��_�Ɖ�]��ݒ肷��
//�����Fvoid
//�߁Fvoid
//*****************************************************************************
void Camera3D::SetCamera()
{
	XMStoreFloat4x4(&g_mtxView, XMMatrixLookAtLH(
		XMVectorSet(g_posCameraP.x, g_posCameraP.y, g_posCameraP.z, 1),
		XMVectorSet(g_posCameraR.x, g_posCameraR.y, g_posCameraR.z, 1),
		XMVectorSet(g_vecCameraU.x, g_vecCameraU.y, g_vecCameraU.z, 0)
	)
	);
	XMStoreFloat4x4(&g_mtxProjection, XMMatrixPerspectiveFovLH(VIEW_ANGLE, VIEW_ASPECT, VIEW_NEAR, VIEW_FAR));
}

//*****************************************************************************
//GetCameraPos�֐�
//�J�����̋��_��߂�
//�����Fvoid
//�߁FXMFLOAT3&
//*****************************************************************************
XMFLOAT3& Camera3D::GetCameraPos()
{
	return g_posCameraP;
}

//*****************************************************************************
//GetCameraAngle�֐�
//�J�����̉�]��߂�
//�����Fvoid
//�߁FXMFLOAT3&
//*****************************************************************************
XMFLOAT3& Camera3D::GetCameraAngle(void)
{
	return g_rotCamera;
}

//*****************************************************************************
//GetViewMatrix�֐�
//�J�����̃r���[�}�g���b�N�X��߂�
//�����Fvoid
//�߁FXMFLOAT4X4&
//*****************************************************************************
XMFLOAT4X4 & Camera3D::GetViewMatrix()
{
	return g_mtxView;
}

//*****************************************************************************
//GetProjMatrix�֐�
//�J�����̃v���W�F�N�V�����}�g���b�N�X��߂�
//�����Fvoid
//�߁FXMFLOAT4X4&
//*****************************************************************************
XMFLOAT4X4 & Camera3D::GetProjMatrix()
{
	return g_mtxProjection;
}

//*****************************************************************************
//GetForwardCameraVector�֐�
//�J�����̃t�H�[���[�h�x�N�g����߂�
//�����Fvoid
//�߁FXMFLOAT3
//*****************************************************************************
XMFLOAT3 Camera3D::GetForwardCameraVector()
{
	float pitch = g_rotCamera.y;
	float yaw = g_rotCamera.y;
	return { cosf(pitch)*cosf(yaw),cosf(pitch)*sinf(yaw),sinf(pitch) };
}

//*****************************************************************************
//SetFocalPoint�֐�
//�J�����̏œ_��ݒ肷��
//�����Fvoid*
//�߁Fvoid
//*****************************************************************************
void Camera3D::SetFocalPoint(void * newFocalPoint)
{
	FocalPoint = newFocalPoint;
}

//*****************************************************************************
//ZoomOutZ�֐�
//�J�����̃Y�[���A�E�g�iZ���W�j��ݒ肷��
//�����Ffloat
//�߁Fvoid
//*****************************************************************************
void Camera3D::ZoomOutZ(float redZ)
{
	Offset.z -= redZ;
	if (Offset.z < -612) {
		Offset.z = -612;
		return;
	}
	if (Offset.z > 63) {
		Offset.z = 63;
		return;
	}
	hbRenderZone.SizeX += redZ*2;
	hbRenderZone.SizeY += redZ*2;
}

//*****************************************************************************
//ZoomInZ�֐�
//�J�����̃Y�[���A�C���iZ���W�j��ݒ肷��
//�����Ffloat
//�߁Fvoid
//*****************************************************************************
void Camera3D::ZoomInZ(float incZ)
{
	Offset.z += incZ;
	if (Offset.z < -612) {
		Offset.z = -612;
		return;
	}
	if (Offset.z > 63) {
		Offset.z = 63;
		return;
	}
	hbRenderZone.SizeX -= incZ * 2;
	hbRenderZone.SizeY -= incZ * 2;
}

//*****************************************************************************
//SetZoomZ�֐�
//�J�����̃Y�[���iZ���W�j��ݒ肷��
//�����Ffloat
//�߁Fvoid
//*****************************************************************************
void Camera3D::SetZoomZ(float incZ)
{
	Offset.z = incZ;
	if (Offset.z < -612) {
		Offset.z = -612;
		return;
	}
	if (Offset.z > 63) {
		Offset.z = 63;
		return;
	}
}

//*****************************************************************************
//ResetZoom�֐�
//�J�����̃Y�[���iZ���W�j�����Z�b�g����
//�����Fvoid
//�߁Fvoid
//*****************************************************************************
void Camera3D::ResetZoom()
{
	Offset = OFFSET_VALUE;
}

//*****************************************************************************
//SetRenderZone�֐�
//�����_�����O�ꏊ��ݒ肷��
//�����FHitbox3D
//�߁Fvoid
//*****************************************************************************
void Camera3D::SetRenderZone(Hitbox3D hb)
{
	hbRenderZone = hb;
}

//*****************************************************************************
//DrawRenderZone�֐�
//�����_�����O�ꏊ��\������
//�����Fvoid
//�߁Fvoid
//*****************************************************************************
void Camera3D::DrawRenderZone()
{
	if(pRenderZone)
		pRenderZone->Draw();
}

//*****************************************************************************
//IsOnRenderZone�֐�
//�����_�����O�ɂ��邱�Ƃ��m�F����
//�����FHitbox3D
//�߁Fvoid
//*****************************************************************************
bool Camera3D::IsOnRenderZone(Hitbox3D hb)
{
	return IsInCollision3D(hbRenderZone, hb);
}

//*****************************************************************************
//ShakeCamera�֐�
//�J�����ɗh�点��
//�����FXMFLOAT3, int, int
//�߁Fvoid
//*****************************************************************************
void Camera3D::ShakeCamera(XMFLOAT3 ShakeForce, int Frames, int FramesPerShake)
{
	if (nFrameShaking > 0)
		return;
	x3Shake = ShakeForce;
	nFrameShaking = Frames;
	nFramesPerShake = FramesPerShake;
}

//*****************************************************************************
//CancelShake�֐�
//�J�����ɗh�邱�Ƃ��L�����Z������
//�����Fvoid
//�߁Fvoid
//*****************************************************************************
void Camera3D::CancelShake()
{
	nFrameShaking = 0;
}

//*****************************************************************************
//GetCurrentZoom�֐�
//�J�����̃Y�[����߂�
//�����Ffloat
//�߁Fvoid
//*****************************************************************************
float Camera3D::GetCurrentZoom()
{
	return Offset.z;
}

//*****************************************************************************
//GetRenderZone�֐�
//�����_�����O�ꏊ��߂�
//�����Fvoid
//�߁FHitbox3D
//*****************************************************************************
Hitbox3D Camera3D::GetRenderZone()
{
	return hbRenderZone;
}

//*****************************************************************************
//GetFocalPoint�֐�
//�œ_�̃A�h���X��߂�
//�����Fvoid
//�߁Fvoid*
//*****************************************************************************
void * Camera3D::GetFocalPoint()
{
	return FocalPoint;
}

//*****************************************************************************
//GetMainCamera�֐�
//���C���J������߂�
//�����Fvoid
//�߁FCamera3D*
//*****************************************************************************
Camera3D * GetMainCamera()
{
	return MainCamera;
}
