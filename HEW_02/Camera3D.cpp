//*****************************************************************************
// Camera3D.cpp
// ビルボードのオブジェクトの管理
//*****************************************************************************
#include "Camera3D.h"	
#include "input.h"
#include "debugproc.h"
#include "GameObject3D.h"
#include <stdio.h>

//*****************************************************************************
// マクロ定義
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
// グローバル変数
//*****************************************************************************
Camera3D* MainCamera = nullptr;

//*****************************************************************************
// コンストラクタ関数
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
//Init関数
//初期化関数
//引数：void
//戻：void
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
//Update関数
//変更関数
//引数：void
//戻：void
//*****************************************************************************
void Camera3D::Update()
{
	GameObject3D* FocusPoint = nullptr;
	FocusPoint = (GameObject3D*)FocalPoint;
	if (FocusPoint) {
		hbRenderZone = { vEye.x, vEye.y, FocusPoint->GetPosition().z, 
			hbRenderZone.SizeX,hbRenderZone.SizeY, hbRenderZone.SizeZ };
		vEye = XMFLOAT3(0, 0, -100);//視点
		vLookAt = XMFLOAT3(0, 0, 0);//注視点
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
		//視点
		XMStoreFloat3(&g_posCameraP, XMVector3TransformCoord(XMLoadFloat3(&vEye), identity));

		//注視点
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
//Uninit関数
//終了関数
//引数：void
//戻：void
//*****************************************************************************
void Camera3D::Uninit()
{
	//なし
}

//*****************************************************************************
//SetCamera関数
//現在のフレームにカメラの拠点と回転を設定する
//引数：void
//戻：void
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
//GetCameraPos関数
//カメラの拠点を戻す
//引数：void
//戻：XMFLOAT3&
//*****************************************************************************
XMFLOAT3& Camera3D::GetCameraPos()
{
	return g_posCameraP;
}

//*****************************************************************************
//GetCameraAngle関数
//カメラの回転を戻す
//引数：void
//戻：XMFLOAT3&
//*****************************************************************************
XMFLOAT3& Camera3D::GetCameraAngle(void)
{
	return g_rotCamera;
}

//*****************************************************************************
//GetViewMatrix関数
//カメラのビューマトリックスを戻す
//引数：void
//戻：XMFLOAT4X4&
//*****************************************************************************
XMFLOAT4X4 & Camera3D::GetViewMatrix()
{
	return g_mtxView;
}

//*****************************************************************************
//GetProjMatrix関数
//カメラのプロジェクションマトリックスを戻す
//引数：void
//戻：XMFLOAT4X4&
//*****************************************************************************
XMFLOAT4X4 & Camera3D::GetProjMatrix()
{
	return g_mtxProjection;
}

//*****************************************************************************
//GetForwardCameraVector関数
//カメラのフォーワードベクトルを戻す
//引数：void
//戻：XMFLOAT3
//*****************************************************************************
XMFLOAT3 Camera3D::GetForwardCameraVector()
{
	float pitch = g_rotCamera.y;
	float yaw = g_rotCamera.y;
	return { cosf(pitch)*cosf(yaw),cosf(pitch)*sinf(yaw),sinf(pitch) };
}

//*****************************************************************************
//SetFocalPoint関数
//カメラの焦点を設定する
//引数：void*
//戻：void
//*****************************************************************************
void Camera3D::SetFocalPoint(void * newFocalPoint)
{
	FocalPoint = newFocalPoint;
}

//*****************************************************************************
//ZoomOutZ関数
//カメラのズームアウト（Z座標）を設定する
//引数：float
//戻：void
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
//ZoomInZ関数
//カメラのズームアイン（Z座標）を設定する
//引数：float
//戻：void
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
//SetZoomZ関数
//カメラのズーム（Z座標）を設定する
//引数：float
//戻：void
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
//ResetZoom関数
//カメラのズーム（Z座標）をリセットする
//引数：void
//戻：void
//*****************************************************************************
void Camera3D::ResetZoom()
{
	Offset = OFFSET_VALUE;
}

//*****************************************************************************
//SetRenderZone関数
//レンダリング場所を設定する
//引数：Hitbox3D
//戻：void
//*****************************************************************************
void Camera3D::SetRenderZone(Hitbox3D hb)
{
	hbRenderZone = hb;
}

//*****************************************************************************
//DrawRenderZone関数
//レンダリング場所を表示する
//引数：void
//戻：void
//*****************************************************************************
void Camera3D::DrawRenderZone()
{
	if(pRenderZone)
		pRenderZone->Draw();
}

//*****************************************************************************
//IsOnRenderZone関数
//レンダリングにあることを確認する
//引数：Hitbox3D
//戻：void
//*****************************************************************************
bool Camera3D::IsOnRenderZone(Hitbox3D hb)
{
	return IsInCollision3D(hbRenderZone, hb);
}

//*****************************************************************************
//ShakeCamera関数
//カメラに揺らせる
//引数：XMFLOAT3, int, int
//戻：void
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
//CancelShake関数
//カメラに揺ることをキャンセルする
//引数：void
//戻：void
//*****************************************************************************
void Camera3D::CancelShake()
{
	nFrameShaking = 0;
}

//*****************************************************************************
//GetCurrentZoom関数
//カメラのズームを戻す
//引数：float
//戻：void
//*****************************************************************************
float Camera3D::GetCurrentZoom()
{
	return Offset.z;
}

//*****************************************************************************
//GetRenderZone関数
//レンダリング場所を戻す
//引数：void
//戻：Hitbox3D
//*****************************************************************************
Hitbox3D Camera3D::GetRenderZone()
{
	return hbRenderZone;
}

//*****************************************************************************
//GetFocalPoint関数
//焦点のアドレスを戻す
//引数：void
//戻：void*
//*****************************************************************************
void * Camera3D::GetFocalPoint()
{
	return FocalPoint;
}

//*****************************************************************************
//GetMainCamera関数
//メインカメラを戻す
//引数：void
//戻：Camera3D*
//*****************************************************************************
Camera3D * GetMainCamera()
{
	return MainCamera;
}
