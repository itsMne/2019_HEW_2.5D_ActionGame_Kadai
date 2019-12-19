#include "Camera3D.h"	
#include "input.h"
#include "debugproc.h"
#include "GameObject3D.h"
#include <stdio.h>
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
#define OFFSET_VALUE	{0,20,0}
Camera3D* MainCamera = nullptr;

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

HRESULT Camera3D::Init()
{
	g_posCameraP = XMFLOAT3(POS_CAMERA_P_X, POS_CAMERA_P_Y, POS_CAMERA_P_Z);
	g_posCameraR = XMFLOAT3(POS_CAMERA_R_X, POS_CAMERA_R_Y, POS_CAMERA_R_Z);
	g_vecCameraU = XMFLOAT3(0, 1, 0);
	g_rotCamera = XMFLOAT3(0, 0, 0);
	float fVecX, fVecZ;
	fVecX = g_posCameraP.x - g_posCameraR.x;
	fVecZ = g_posCameraP.z - g_posCameraR.z;
	g_fLengthInterval = sqrtf(fVecX * fVecX + fVecZ * fVecZ);
	Offset = OFFSET_VALUE;
	return S_OK;
}

void Camera3D::Update()
{

	GameObject3D* FocusPoint = nullptr;
	FocusPoint = (GameObject3D*)FocalPoint;
	if (FocusPoint) {
		vEye = XMFLOAT3(0, 0, -100);//視点
		vLookAt = XMFLOAT3(0, 0, 0);//注視点
		vEye.x=vLookAt.x = FocusPoint->GetPosition().x+ Offset.x;
		vEye.y=vLookAt.y = FocusPoint->GetPosition().y+ Offset.y;
		vEye.z+=Offset.z;
		XMMATRIX mtxWorld = XMLoadFloat4x4(FocusPoint->GetModelWorld());
		XMMATRIX identity = XMMatrixIdentity();
		//視点
		//XMStoreFloat3(&g_posCameraP, XMVector3TransformCoord(XMLoadFloat3(&vEye), mtxWorld));
		XMStoreFloat3(&g_posCameraP, XMVector3TransformCoord(XMLoadFloat3(&vEye), identity));

		//注視点
		//XMStoreFloat3(&g_posCameraR, XMVector3TransformCoord(XMLoadFloat3(&vLookAt), mtxWorld));
		XMStoreFloat3(&g_posCameraR, XMVector3TransformCoord(XMLoadFloat3(&vLookAt), identity));

		g_rotCamera = FocusPoint->GetRotation();
	}

	SetCamera();

	//PrintDebugProc("[ｶﾒﾗ ｲﾁ:(%f, %f, %f)]\n", g_posCameraP.x, g_posCameraP.y, g_posCameraP.z);
}

void Camera3D::Uninit()
{
}

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

XMFLOAT3& Camera3D::GetCameraPos()
{
	return g_posCameraP;
}

XMFLOAT3& Camera3D::GetCameraAngle(void)
{
	return g_rotCamera;
}

XMFLOAT4X4 & Camera3D::GetViewMatrix()
{
	return g_mtxView;
}

XMFLOAT4X4 & Camera3D::GetProjMatrix()
{
	return g_mtxProjection;
}

XMFLOAT3 Camera3D::GetForwardCameraVector()
{
	float pitch = g_rotCamera.y;
	float yaw = g_rotCamera.y;
	return { cosf(pitch)*cosf(yaw),cosf(pitch)*sinf(yaw),sinf(pitch) };
}

void Camera3D::SetFocalPoint(void * newFocalPoint)
{
	FocalPoint = newFocalPoint;
}

void Camera3D::ZoomOutZ(float redZ)
{
	Offset.z -= redZ;
	if (Offset.z < -612)
		Offset.z = -612;
	if (Offset.z > 63)
		Offset.z = 63;
	printf("%f\n", Offset.z);
}

void Camera3D::ResetZoom()
{
	Offset = OFFSET_VALUE;
}

Camera3D * GetMainCamera()
{
	return MainCamera;
}
