#include "DebugAim.h"
#include "Player3D.h"
#include "InputManager.h"
#define DEBUG_AIM_SPEED 3

enum DEBUG_AIM_OBJECTYPES
{
	DA_DEBUGAIM=0,
	DA_FIELD,
	DA_MAX
};

DebugAim::DebugAim(): GameObject3D()
{
	Init();
}


DebugAim::~DebugAim()
{
	Uninit();
}

void DebugAim::Init()
{
	nType = GO_DEBUG_AIM;
	pDA_Field = nullptr;
	nObjectType = DA_DEBUGAIM;
	InitModel("data/model/DebugAim.fbx");
}

void DebugAim::Update()
{
	GameObject3D::Update();
	int fSpeed = DEBUG_AIM_SPEED;
	if (GetInput(INPUT_DEBUGAIM_SPEEDDOWN))
		fSpeed *= 0.5f;
	else if (GetInput(INPUT_DEBUGAIM_SPEEDUP))
		fSpeed *= 2;
	if (GetInput(INPUT_UP))
		Position.y+= fSpeed;
	if (GetInput(INPUT_DOWN))
		Position.y-= fSpeed;
	if (GetInput(INPUT_RIGHT))
		Position.x+= fSpeed;
	if (GetInput(INPUT_LEFT))
		Position.x-= fSpeed;
	
	if (GetInput(INPUT_SWITCH_DEBUG_TYPE_UP))
	{
		nObjectType++;
		if (nObjectType >= DA_MAX)
			nObjectType = 0;
		SwitchObjectTypeControl();
	}
	if (GetInput(INPUT_SWITCH_DEBUG_TYPE_DOWN))
	{
		nObjectType--;
		if (nObjectType < 0)
			nObjectType = DA_MAX-1;
		SwitchObjectTypeControl();
	}
	Player3D* pPlayer = nullptr;
	switch (nObjectType)
	{
	case DA_DEBUGAIM:
		if (GetInput(INPUT_DEBUGAIM_ACCEPT))
		{
			pPlayer = GetMainPlayer();
			if (!pPlayer)
				break;
			pPlayer->SetPosition(Position);
			printf("‹’“_F(%f, %f, %f);\n", Position.x, Position.y, Position.z);
		}
		break;
	case DA_FIELD:
		if (!pDA_Field) {
			pDA_Field = new Field3D();
			pDA_Field->Init("data/texture/field000.jpg");
			Scale = { 50,50,50 };
		}
		else {
			pDA_Field->SetPosition({ Position.x,Position.y+12,Position.z });
			pDA_Field->SetScaleWithHitbox(Scale);

			ScaleControl(fSpeed);
		}
	default:
		break;
	}
}

void DebugAim::SwitchObjectTypeControl()
{
	if (pDA_Field)
		SAFE_DELETE(pDA_Field);
	Scale = { 1,1,1 };
}

void DebugAim::ScaleControl(int fSpeed)
{
	if (GetInput(INPUT_SCALE_UP_Z))
		Scale.z += fSpeed;
	if (GetInput(INPUT_SCALE_DOWN_Z))
		Scale.z -= fSpeed;
	if (GetInput(INPUT_SCALE_UP_X))
		Scale.x += fSpeed;
	if (GetInput(INPUT_SCALE_DOWN_X))
		Scale.x -= fSpeed;
	if (Scale.x < 0)
		Scale.x = 0;
	if (Scale.y < 0)
		Scale.y = 0;
	if (Scale.z < 0)
		Scale.z = 0;
}

void DebugAim::Draw()
{
	
	switch (nObjectType)
	{
	case DA_DEBUGAIM:
		GameObject3D::Draw();
		break;
	case DA_FIELD:
		SetCullMode(CULLMODE_NONE);
		if (pDA_Field) {
			pDA_Field->DrawField();
		}
		SetCullMode(CULLMODE_CCW);
	default:
		break;
	}
}

void DebugAim::Uninit()
{
	GameObject3D::Uninit();
}
