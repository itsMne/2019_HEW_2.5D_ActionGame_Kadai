#pragma once

#include "DXWindow3D.h"
#include "UniversalStructures.h"
#include "Cube3D.h"
class Camera3D
{
private:
	 XMFLOAT3			g_posCameraP;
	 XMFLOAT3			g_posCameraR;
	 XMFLOAT3			g_vecCameraU;
	 XMFLOAT3			g_rotCamera;
	 float				g_fLengthInterval;
	 XMFLOAT4X4			g_mtxProjection;
	 XMFLOAT4X4			g_mtxView;
	 void*				FocalPoint;
	 XMFLOAT3 vEye;
	 XMFLOAT3 Offset;
	 XMFLOAT3 vLookAt;
	 float fAcceleration;
	 Hitbox3D hbRenderZone;
	 Cube3D* pRenderZone;
	 XMFLOAT3 x3Shake;
	 int nFrameShaking;
	 int nFramesPerShake;
	 int nFramesPerShakeCounter;
public:
	Camera3D();
	Camera3D(bool bisMainCamera);
	~Camera3D();
	HRESULT Init();
	void Update();
	void Uninit();
	void SetCamera();
	XMFLOAT3& GetCameraPos();
	XMFLOAT3& GetCameraAngle();
	XMFLOAT4X4& GetViewMatrix();
	XMFLOAT4X4& GetProjMatrix();
	XMFLOAT3 GetForwardCameraVector();
	void SetFocalPoint(void* newFocalPoint);
	void ZoomOutZ(float redZ);
	void ZoomInZ(float incZ);
	void SetZoomZ(float incZ);
	void ResetZoom();
	void SetRenderZone(Hitbox3D hb);
	void DrawRenderZone();
	bool IsOnRenderZone(Hitbox3D hb);
	void ShakeCamera(XMFLOAT3 ShakeForce, int Frames, int FramesPerShake);
	float GetCurrentZoom();
	Hitbox3D GetRenderZone();
};

Camera3D* GetMainCamera();
