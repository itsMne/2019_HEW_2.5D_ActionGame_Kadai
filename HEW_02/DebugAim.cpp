#include "DebugAim.h"
#include "Player3D.h"
#include "SceneGame.h"
#include "InputManager.h"
#define DEBUG_AIM_SPEED 3

enum DEBUG_AIM_OBJECTYPES
{
	DA_DEBUGAIM=0,
	DA_FIELD,
	DA_WALL,
	DA_ITEM_SUSHI,
	DA_ITEM_UDON,
	DA_ITEM_ODEN,
	DA_ITEM_DANGO,
	DA_ITEM_TAI,
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
	pDA_Wall = nullptr;
	pCurrentGame = nullptr;
	pDA_Item = nullptr;
	nObjectType = DA_DEBUGAIM;
	InitModel("data/model/DebugAim.fbx");
}

void DebugAim::Update()
{
	GameObject3D::Update();
	if (!pCurrentGame)
	{
		pCurrentGame = GetCurrentGame();
		return;
	}
	SceneGame* p_sCurrentGame = (SceneGame*)pCurrentGame;
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
	if (GetInput(INPUT_SHIFT)) {
		if (GetInput(INPUT_SCALE_UP_Z)) {
			GetMainCamera()->ZoomOutZ(fSpeed);
		}
		if (GetInput(INPUT_SCALE_DOWN_Z)) {
			GetMainCamera()->ZoomOutZ(-fSpeed);
		}
		if (GetInput(INPUT_SCALE_UP_X)) {
			GetMainCamera()->ZoomOutZ(fSpeed);
		}
		if (GetInput(INPUT_SCALE_DOWN_X)) {
			GetMainCamera()->ZoomOutZ(-fSpeed);
		}
	}
	Player3D* pPlayer = nullptr;
	int nitemType;
	switch (nObjectType)
	{
	case DA_DEBUGAIM:
		if (GetInput(INPUT_DEBUGAIM_ACCEPT))
		{
			pPlayer = GetMainPlayer();
			if (!pPlayer)
				break;
			pPlayer->SetPosition(Position);
			printf("���_�F(%f, %f, %f);\n", Position.x, Position.y, Position.z);
		}

		break;
	case DA_FIELD:
		if (!pDA_Field) {
			pDA_Field = new Field3D("data/texture/field000.jpg");
			Scale = { 50,50,50 };
		}
		else {
			pDA_Field->SetPosition({ Position.x,Position.y+12,Position.z });
			pDA_Field->SetScaleWithHitbox(Scale);
			ScaleControl(fSpeed);
			if (GetInput(INPUT_DEBUGAIM_ACCEPT))
			{
				p_sCurrentGame->GetFields()->AddField({ Position.x,Position.y + 12,Position.z }, { Scale.x,3,Scale.z }, "data/texture/field000.jpg");
				printf("Fields->AddField({ %ff ,%ff ,%ff}, { %ff, %ff ,%ff }, \"%s\");\n", Position.x, Position.y + 12, Position.z, Scale.x, 3, Scale.z, "data/texture/field000.jpg");
			}
			if (GetInput(INPUT_DEBUGAIM_DELETE))
			{
				p_sCurrentGame->GetFields()->DeleteLastPosObject();
			}
			if (GetInput(INPUT_SAVE_LEVEL))
			{
				p_sCurrentGame->GetFields()->SaveFields("Fields_Level");
			}
		}
		break;
	case DA_WALL:
		if (!pDA_Wall) {
			pDA_Wall = new Wall3D();
			Scale = { 1,1,1 };
		}
		else {
			pDA_Wall->SetPosition({ Position.x,Position.y+16, Position.z });
			pDA_Wall->SetScale(Scale);
			ScaleControl(fSpeed*0.25f);
			if (GetInput(INPUT_DEBUGAIM_ACCEPT))
			{
				p_sCurrentGame->GetWalls()->AddWall({ Position.x,Position.y + 16, Position.z }, Scale);
				printf("Walls->AddWall({ %ff ,%ff ,%ff}, { %ff, %ff ,%ff });\n", Position.x, Position.y+16, Position.z, Scale.x, Scale.y, Scale.z);
			}
			if (GetInput(INPUT_DEBUGAIM_DELETE))
			{
				p_sCurrentGame->GetWalls()->DeleteLastPosObject();
			}
			if (GetInput(INPUT_SAVE_LEVEL))
			{
				p_sCurrentGame->GetWalls()->SaveWalls("Walls_Level");
			}
		}
		break;
	case DA_ITEM_SUSHI: case DA_ITEM_UDON: case DA_ITEM_ODEN: case DA_ITEM_TAI: case DA_ITEM_DANGO:
		nitemType = nObjectType - DA_ITEM_SUSHI;
		if (!pDA_Item) {
			pDA_Item = new C_Item(nitemType);
			Scale = { 1,1,1 };
		}
		else {
			pDA_Item->SetPosition(Position);
			pDA_Item->Update();
			pDA_Item->SetUse(true);

			if (GetInput(INPUT_DEBUGAIM_ACCEPT))
			{
				p_sCurrentGame->GetItems()->AddItem(Position, nitemType);
				printf("Items->AddItem({ %ff ,%ff ,%ff}, %d);\n", Position.x, Position.y, Position.z, nitemType);
			}
			if (GetInput(INPUT_DEBUGAIM_DELETE))
			{
				p_sCurrentGame->GetItems()->DeleteLastPosObject();
			}
			if (GetInput(INPUT_SAVE_LEVEL))
			{
				p_sCurrentGame->GetItems()->SaveItems("Items_Level");
			}
		}
		break;
	default:
		break;
	}
}

void DebugAim::SwitchObjectTypeControl()
{
	SAFE_DELETE(pDA_Field);
	SAFE_DELETE(pDA_Wall);
	SAFE_DELETE(pDA_Item);

	Scale = { 1,1,1 };
}

void DebugAim::ScaleControl(float fSpeed)
{
	switch (nObjectType)
	{
	case DA_FIELD:
		if (!(GetInput(INPUT_SHIFT))) {
			if (GetInput(INPUT_SCALE_UP_Z)) {
				Scale.z += fSpeed;
			}
			if (GetInput(INPUT_SCALE_DOWN_Z)) {
				Scale.z -= fSpeed;
			}
			if (GetInput(INPUT_SCALE_UP_X)) {
				Scale.x += fSpeed;
				GetMainCamera()->ZoomOutZ(fSpeed);
			}
			if (GetInput(INPUT_SCALE_DOWN_X)) {
				Scale.x -= fSpeed;
				GetMainCamera()->ZoomOutZ(-fSpeed);
			}
		}
		break;
	default:
		if (!(GetInput(INPUT_SHIFT))) {
			if (GetInput(INPUT_SCALE_UP_Z)) {
				Scale.y += fSpeed;
				GetMainCamera()->ZoomOutZ(fSpeed * 12);
			}
			if (GetInput(INPUT_SCALE_DOWN_Z)) {
				Scale.y -= fSpeed;
				GetMainCamera()->ZoomOutZ(-fSpeed * 12);
			}
			if (GetInput(INPUT_SCALE_UP_X)) {
				Scale.x += fSpeed;
				GetMainCamera()->ZoomOutZ(fSpeed * 12);
			}
			if (GetInput(INPUT_SCALE_DOWN_X)) {
				Scale.x -= fSpeed;
				GetMainCamera()->ZoomOutZ(-fSpeed * 12);
			}
		}
		break;
	}

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
			pDA_Field->Draw();
		}
		SetCullMode(CULLMODE_CCW);
		break;
	case DA_WALL:
		SetCullMode(CULLMODE_NONE);
		if(pDA_Wall)
			pDA_Wall->Draw();
		SetCullMode(CULLMODE_CCW);
		break;
	case DA_ITEM_SUSHI: case DA_ITEM_UDON:case DA_ITEM_ODEN:case DA_ITEM_TAI:case DA_ITEM_DANGO:
		if (pDA_Item)
			pDA_Item->Draw();
		break;
	default:
		break;
	}
}

void DebugAim::Uninit()
{
	GameObject3D::Uninit();
}
