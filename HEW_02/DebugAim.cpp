#include "DebugAim.h"
#include "Player3D.h"
#include "SceneGame.h"
#include "InputManager.h"
#define DEBUG_AIM_SPEED 3



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
	pDA_Spike = nullptr;
	pDA_Goal = nullptr;
	nObjectType = DA_DEBUGAIM;
	InitModel("data/model/DebugAim.fbx");
	hitbox = { 0,10.0f,0,2,2,6 };
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
	if (GetInput(INPUT_SAVE_LEVEL))
	{
		p_sCurrentGame->GetFields()->SaveFields("Fields_Level");
		p_sCurrentGame->GetGoals()->SaveMisc("Goals_Level");
		p_sCurrentGame->GetWalls()->SaveWalls("Walls_Level");
		p_sCurrentGame->GetItems()->SaveItems("Items_Level");
		p_sCurrentGame->GetSpikes()->SaveSpikes("Spikes_Level");
		p_sCurrentGame->GetGoals()->SaveMisc("Goals_Level");
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
			printf("‹’“_F(%f, %f, %f);\n", Position.x, Position.y, Position.z);
		}
		break;
	case DA_FIELD:
		if (!pDA_Field) {
			pDA_Field = new Field3D("data/texture/field000.jpg");
			Scale = { 50,50,50 };
			pDA_Field->SetPosition({ Position.x,Position.y + 12,Position.z });
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
		}
		break;
	case DA_WALL:
		if (!pDA_Wall) {
			pDA_Wall = new Wall3D();
			Scale = { 1,1,1 };
			pDA_Wall->SetPosition({ Position.x,Position.y + 16, Position.z });
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
		}
		break;
	case DA_ITEM_SUSHI: case DA_ITEM_UDON: case DA_ITEM_ODEN: case DA_ITEM_TAI: case DA_ITEM_DANGO:
		nitemType = nObjectType - DA_ITEM_SUSHI;
		if (!pDA_Item) {
			pDA_Item = new C_Item(nitemType);
			Scale = { 1,1,1 };
			pDA_Item->SetPosition(Position);
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
		}
		break;
	case DA_SPIKE:
		if (!pDA_Spike)
		{
			pDA_Spike = new Spike3D();
			Scale = { 1,1,1 };
		}
		else {
			pDA_Spike->Update();
			pDA_Spike->SetPosition(Position);
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
			else {
				fSpeed *= 3.5f;
				if (GetInput(INPUT_SCALE_UP_Z)) {
					pDA_Spike->RaiseSpikesY(1);
					GetMainCamera()->ZoomOutZ(fSpeed);
				}
				if (GetInput(INPUT_SCALE_DOWN_Z)) {
					pDA_Spike->RaiseSpikesY(-1);
					GetMainCamera()->ZoomOutZ(-fSpeed);
				}
				if (GetInput(INPUT_SCALE_UP_X)) {
					pDA_Spike->RaiseSpikesX(1);
					GetMainCamera()->ZoomOutZ(fSpeed);
				}
				if (GetInput(INPUT_SCALE_DOWN_X)) {
					pDA_Spike->RaiseSpikesX(-1);
					GetMainCamera()->ZoomOutZ(-fSpeed);
				}
			}
			if (GetInput(INPUT_DEBUGAIM_ACCEPT))
			{
				p_sCurrentGame->GetSpikes()->AddSpike(Position, ((int)pDA_Spike->GetSpikesNum().x), ((int)pDA_Spike->GetSpikesNum().y), true);
				printf("Spikes->AddSpike({ %ff ,%ff ,%ff}, %d, %d, true);\n", Position.x, Position.y, Position.z, ((int)pDA_Spike->GetSpikesNum().x), ((int)pDA_Spike->GetSpikesNum().y));
			}
			if (GetInput(INPUT_DEBUGAIM_DELETE))
			{
				p_sCurrentGame->GetSpikes()->DeleteLastPosObject();
			}
		}
		break;
	case DA_GOAL:
		if (!pDA_Goal) {
			pDA_Goal = new Goal3D();
			pDA_Goal->SetPosition(Position);
			Scale = { 1,1,1 };
		}
		else {
			pDA_Goal->Update();
			pDA_Goal->SetPosition(Position);

			if (GetInput(INPUT_DEBUGAIM_ACCEPT))
			{
				p_sCurrentGame->GetGoals()->AddMisc(Position, GO_GOAL);
				printf("Misc->AddGoal({ %ff ,%ff ,%ff}, GO_GOAL);\n", Position.x, Position.y, Position.z);
			}
			if (GetInput(INPUT_DEBUGAIM_DELETE))
			{
				p_sCurrentGame->GetGoals()->DeleteLastPosObject();
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
	SAFE_DELETE(pDA_Spike);

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
	case DA_SPIKE:
		if (pDA_Spike)
			pDA_Spike->Draw();
		break;
	case DA_GOAL:
		if (pDA_Goal)
			pDA_Goal->Draw();
		break;
	default:
		break;
	}
}

void DebugAim::Uninit()
{
	GameObject3D::Uninit();
}

int DebugAim::GetCurrentType()
{
	return nObjectType;
}
