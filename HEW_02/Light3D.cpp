//*****************************************************************************
// Light3D.cpp
// 光の管理
//*****************************************************************************
#include "Light3D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define LIGHT0_DIFFUSE XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
#define LIGHT0_AMBIENT XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
#define LIGHT0_SPECULAR XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
#define LIGHT0_DIR XMFLOAT3(-1.0f, -1.0f, 1.0f)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
Light3D* MainLight = nullptr;


Light3D::Light3D()
{
	MainLight = this;
}


Light3D::~Light3D()
{
	Uninit();
}

//*****************************************************************************
//Init関数
//初期化関数
//引数：void
//戻：void
//*****************************************************************************
HRESULT Light3D::Init(void)
{
	XMFLOAT3 vecDir;
	//XMStoreFloat3(&g_UnivLight.m_direction, XMVector3Normalize(XMLoadFloat3(&LIGHT0_DIR)));
	//g_UnivLight.m_direction = LIGHT0_DIR;
	g_UnivLight.m_diffuse = LIGHT0_DIFFUSE;
	g_UnivLight.m_ambient = LIGHT0_AMBIENT;
	//g_UnivLight.m_specular = LIGHT0_SPECULAR;
	vecDir = XMFLOAT3(0.0f, -1.0f, 1.0f);
	XMStoreFloat3(&g_UnivLight.m_direction, XMVector3Normalize(XMLoadFloat3(&vecDir)));
	bLightEnabled = true;
	return S_OK;
}

//*****************************************************************************
//Uninit関数
//終了関数
//引数：void
//戻：void
//*****************************************************************************
void Light3D::Uninit(void)
{
	//なし
}

//*****************************************************************************
//Update関数
//変更関数
//引数：void
//戻：void
//*****************************************************************************
void Light3D::Update(void)
{
}

//*****************************************************************************
//Update関数
//変更関数
//引数：void
//戻：void
//*****************************************************************************
CFbxLight& Light3D::GetLight(void)
{
	return g_UnivLight;
}

//*****************************************************************************
//SetLightEnable関数
//光を有効する
//引数：bool
//戻：void
//*****************************************************************************
void Light3D::SetLightEnable(bool bEnable)
{
	if (bEnable) {
		XMFLOAT3 vecDir = LIGHT0_DIR;
		XMStoreFloat3(&g_UnivLight.m_direction, XMVector3Normalize(XMLoadFloat3(&vecDir)));
	}
	else {
		g_UnivLight.m_direction = XMFLOAT3(0, 0, 0);
	}
	bLightEnabled = bEnable;
}

//*****************************************************************************
//IsLightEnabled関数
//ライトが有効を確認する
//引数：void
//戻：bool
//*****************************************************************************
bool Light3D::IsLightEnabled()
{
	return bLightEnabled;
}

//*****************************************************************************
//GetMainLight関数
//ライトのアドレスを戻す
//引数：void
//戻：Light3D*
//*****************************************************************************
Light3D * GetMainLight()
{
	return MainLight;
}
