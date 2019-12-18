#pragma once
#include "Model3D.h"
#include "Cube3D.h"
enum goType
{
	GO_PLAYER = 0,
	GO_WALL,
	GO_ITEM,
	GO_FLOOR,
	GO_DEBUG_AIM,
	MAX_TYPE
};
class GameObject3D
{
protected:
	Model3D* pModel;//���f��
	XMFLOAT3 Rotation;//��]
	XMFLOAT3 Position;//���_
	XMFLOAT3 Scale;//�傫��
	XMFLOAT4X4 WorldMatrix;
	int nType;//�^�C�v
	Hitbox3D hitbox;
	Cube3D* pVisualHitbox;
	bool bUnlit;
public:
	GameObject3D();
	~GameObject3D();
	virtual void Init();
	virtual void Update();
	virtual void Draw();
	virtual void Uninit();
	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();
	XMFLOAT3 GetScale();
	XMFLOAT4X4* GetModelWorld();

	void SetPosition(XMFLOAT3);
	void SetRotation(XMFLOAT3);
	void SetScale(XMFLOAT3);
	void InitModel(const char* szPath);
	Hitbox3D GetHitBox();
};

