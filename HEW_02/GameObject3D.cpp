#include "GameObject3D.h"



GameObject3D::GameObject3D()
{
	Init();
}


GameObject3D::~GameObject3D()
{
}

void GameObject3D::Init()
{
	Rotation = { 0,0,0 };
	Position = { 0,0,0 };
	Scale = { 1,1,1 };
	hitbox = { 0,0,0,0,0,0 };
	pModel = nullptr;
#if USE_HITBOX
	pVisualHitbox = new Cube3D("data/texture/hbox.tga");
	pVisualHitbox->SetPosition({ GetHitBox().x, GetHitBox().y, GetHitBox().z });
	pVisualHitbox->SetScale({ GetHitBox().SizeX, GetHitBox().SizeY, GetHitBox().SizeZ });
#endif
}

void GameObject3D::Update()
{
	if (pModel)
		pModel->UpdateModel();
#if USE_HITBOX
	pVisualHitbox->SetPosition({ GetHitBox().x, GetHitBox().y, GetHitBox().z });
	pVisualHitbox->SetScale({ GetHitBox().SizeX, GetHitBox().SizeY, GetHitBox().SizeZ });
#endif
}

void GameObject3D::Draw()
{
	bool bPreviousLight = GetMainLight()->IsLightEnabled();
#if USE_HITBOX
	GetMainLight()->SetLightEnable(false);
	SetCullMode(CULLMODE_NONE);
	pVisualHitbox->Draw();
	SetCullMode(CULLMODE_CCW);
	GetMainLight()->SetLightEnable(bPreviousLight);
#endif
	
	if (bUnlit)
		GetMainLight()->SetLightEnable(false);
	if (pModel)
		pModel->DrawModel();
	if (bUnlit)
		GetMainLight()->SetLightEnable(bPreviousLight);
}

void GameObject3D::Uninit()
{
	if (pModel)
		delete(pModel);
}

XMFLOAT3 GameObject3D::GetPosition()
{
	return Position;
}

XMFLOAT3 GameObject3D::GetRotation()
{
	return Rotation;
}

XMFLOAT3 GameObject3D::GetScale()
{
	return Scale;
}

XMFLOAT4X4 * GameObject3D::GetModelWorld()
{
	XMMATRIX mtxWorld, mtxRot, mtxTranslate, mtxScale;
	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	mtxScale = XMMatrixScaling(Scale.x, Scale.y, Scale.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScale);

	mtxRot = XMMatrixRotationRollPitchYaw(Rotation.x, Rotation.y, Rotation.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	mtxTranslate = XMMatrixTranslation(Position.x, Position.y, Position.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	XMStoreFloat4x4(&WorldMatrix, mtxWorld);
	return &WorldMatrix;
}

void GameObject3D::SetPosition(XMFLOAT3 pos)
{
	Position = pos;
}

void GameObject3D::SetRotation(XMFLOAT3 rot)
{
	Rotation = rot;
}

void GameObject3D::SetScale(XMFLOAT3 scale)
{
	Scale = scale;
}

void GameObject3D::InitModel(const char * szPath)
{
	if (!pModel)
		pModel = new Model3D();
	pModel->InitModel(szPath, this);
}

Hitbox3D GameObject3D::GetHitBox()
{
	return { hitbox.x + Position.x, hitbox.y + Position.y,hitbox.z + Position.z,hitbox.SizeX,hitbox.SizeY,hitbox.SizeZ };
}