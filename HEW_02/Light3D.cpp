#include "Light3D.h"

//*****************************************************************************
// É}ÉNÉçíËã`
//*****************************************************************************
#define LIGHT0_DIFFUSE XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
#define LIGHT0_AMBIENT XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
#define LIGHT0_SPECULAR XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
#define LIGHT0_DIR XMFLOAT3(-1.0f, -1.0f, 1.0f)

Light3D* MainLight = nullptr;
Light3D::Light3D()
{
	MainLight = this;
}


Light3D::~Light3D()
{
	Uninit();
}

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

void Light3D::Uninit(void)
{
}

void Light3D::Update(void)
{
}

CFbxLight& Light3D::GetLight(void)
{
	return g_UnivLight;
}

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

bool Light3D::IsLightEnabled()
{
	return bLightEnabled;
}

Light3D * GetMainLight()
{
	return MainLight;
}
