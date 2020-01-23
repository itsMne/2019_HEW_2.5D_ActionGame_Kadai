#include "DebugAim.h"
#include "Player3D.h"
#include "SceneGame.h"
#include "InputManager.h"
#define DEBUG_AIM_SPEED 3
#define MODEL_PATH_X "data/model/X1.fbx"
#define MODEL_PATH_X2 "data/model/X2.fbx"


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
	pDA_Mirror = nullptr;
	pDA_Enemy = nullptr;
	pDA_EventBox = nullptr;
	nObjectType = DA_DEBUGAIM;
	InitModel("data/model/DebugAim.fbx");
	hitbox = { 0,10.0f,0,2,2,9 };
	bStaticObject = true;
	x3Start = {0,0,0};
	x3End = {0,0,0};
	TeleportDestination = {0,0,0};
	TeleportPosition = {0,0,0};
	fSpeedMoveable = 1;
	fDelayBetweenStops = 0;
	bSetStart =false;
	bSetEnd = false;
	bTeleportSet = false;
	bTeleportDestinationSet = false;
	ModelPosStart = new Model3D();
	ModelPosStart->InitModel(MODEL_PATH_X, nullptr);
	ModelPosEnd = new Model3D();
	ModelPosEnd->InitModel(MODEL_PATH_X, nullptr);
	ModelPosTeleport = new Model3D();
	ModelPosTeleport->InitModel(MODEL_PATH_X2, nullptr);
	EventNum = 0;
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
	if (GetInput(INPUT_SWITCH_STATIC_OBJECT))
	{
		if (bStaticObject)
			bStaticObject = false;
		else {
			bSetStart = false;
			bSetEnd = false;
			bStaticObject = true;
		}
	}
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

	if (!bStaticObject) {
		if (GetInput(INPUT_MOVEABLE_OBJECT_SPEED_UP))
		{
			fSpeedMoveable += 0.5f;
		}
		if (GetInput(INPUT_MOVEABLE_OBJECT_SPEED_DOWN))
		{
			fSpeedMoveable -= 0.5f;
			if (fSpeedMoveable < 0.5f)
				fSpeedMoveable = 0.5f;
		}

		if (GetInput(INPUT_MOVEABLE_OBJECT_DELAY_UP))
		{
			fDelayBetweenStops += 1;
		}
		if (GetInput(INPUT_MOVEABLE_OBJECT_DELAY_DOWN))
		{
			fDelayBetweenStops -= 1;
			if (fDelayBetweenStops < 0)
				fDelayBetweenStops = 0;
		}
	}
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
	SaveAllControl();
	MoveableObjectPositionControl();
	x3Start.z = fSpeedMoveable;
	x3End.z = fDelayBetweenStops;
	Player3D* pPlayer = nullptr;
	int nitemType;
	if (GetInput(INPUT_MOVEABLE_OBJECT_DELAY_UP))
		EventNum++;
	if (GetInput(INPUT_MOVEABLE_OBJECT_DELAY_DOWN))
		EventNum--;
	if (EventNum < 0)
		EventNum = 0;
	if (EventNum >= MAX_EVENTS)
		EventNum = MAX_EVENTS - 1;
	
	switch (nObjectType)
	{
	case DA_DEBUGAIM:
		if (GetInput(INPUT_DEBUGAIM_ACCEPT))
		{
			pPlayer = GetMainPlayer();
			if (!pPlayer)
				break;
			pPlayer->SetPosition(Position);
			printf("拠点：(%f, %f, %f);\n", Position.x, Position.y, Position.z);
		}
		bSetStart = false;
		bSetEnd = false;
		break;
	case DA_FIELD:
		if (!pDA_Field) {
			pDA_Field = new Field3D("data/texture/field000.jpg");
			pDA_Field->SetIgnoreRenderingZone(true);
			Scale = { 50,50,50 };
			pDA_Field->SetPosition({ Position.x,Position.y + 12,Position.z });
		}
		else {
			pDA_Field->SetPosition({ Position.x,Position.y+12,Position.z });
			pDA_Field->SetScaleWithHitbox(Scale);
			ScaleControl(fSpeed);
			if (GetInput(INPUT_DEBUGAIM_ACCEPT))
			{
				if (bStaticObject) {
					p_sCurrentGame->GetFields()->AddField({ Position.x,Position.y + 12,Position.z }, { Scale.x,3,Scale.z }, "data/texture/field000.jpg");
					printf("Fields->AddField({ %ff ,%ff ,%ff}, { %ff, %ff ,%ff }, \"%s\");\n", Position.x, Position.y + 12, Position.z, Scale.x, 3, Scale.z, "data/texture/field000.jpg");
				}
				else {
					if (bSetStart && bSetEnd) {
						bSetStart = false;
						bSetEnd = false;
						printf("動けるオブジェクト置いた\n");
						p_sCurrentGame->GetFields()->AddField({ Position.x,Position.y + 12,Position.z }, { Scale.x,3,Scale.z }, "data/texture/field000.jpg", true, x3Start, x3End);
					}
				}
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
			pDA_Wall->SetIgnoreRenderingZone(true);
			Scale = { 1,1,1 };
			pDA_Wall->SetPosition({ Position.x,Position.y + 16, Position.z });
		}
		else {
			pDA_Wall->SetPosition({ Position.x,Position.y+16, Position.z });
			pDA_Wall->SetScale(Scale);
			ScaleControl(fSpeed*0.25f);
			if (GetInput(INPUT_DEBUGAIM_ACCEPT))
			{
				if (bStaticObject) {
					p_sCurrentGame->GetWalls()->AddWall({ Position.x,Position.y + 16, Position.z }, Scale);
					printf("Walls->AddWall({ %ff ,%ff ,%ff}, { %ff, %ff ,%ff });\n", Position.x, Position.y + 16, Position.z, Scale.x, Scale.y, Scale.z);
				}
				else {
					if (bSetStart && bSetEnd) {
						bSetStart = false;
						bSetEnd = false;
						printf("動けるオブジェクト置いた\n");
						p_sCurrentGame->GetWalls()->AddWall({ Position.x,Position.y + 16, Position.z }, Scale, true, x3Start, x3End);
					}
				}
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
			pDA_Item->SetIgnoreRenderingZone(true);
			Scale = { 1,1,1 };
			pDA_Item->SetPosition(Position);
		}
		else {
			pDA_Item->SetPosition(Position);
			pDA_Item->Update();
			pDA_Item->SetUse(true);

			if (GetInput(INPUT_DEBUGAIM_ACCEPT))
			{
				if (bStaticObject) {
					p_sCurrentGame->GetItems()->AddItem(Position, nitemType);
					printf("Items->AddItem({ %ff ,%ff ,%ff}, %d);\n", Position.x, Position.y, Position.z, nitemType);
				}
				else {
					if (bSetStart && bSetEnd) {
						bSetStart = false;
						bSetEnd = false;
						printf("動けるオブジェクト置いた\n");
						p_sCurrentGame->GetItems()->AddItem(Position, nitemType, true, x3Start, x3End);
					}
				}
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
			pDA_Spike->SetIgnoreRenderingZone(true);
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
				if (bStaticObject) {
					p_sCurrentGame->GetSpikes()->AddSpike(Position, ((int)pDA_Spike->GetSpikesNum().x), ((int)pDA_Spike->GetSpikesNum().y), true);
					printf("Spikes->AddSpike({ %ff ,%ff ,%ff}, %d, %d, true);\n", Position.x, Position.y, Position.z, ((int)pDA_Spike->GetSpikesNum().x), ((int)pDA_Spike->GetSpikesNum().y));
				}
				else {
					if (bSetStart && bSetEnd) {
						bSetStart = false;
						bSetEnd = false;
						printf("動けるオブジェクト置いた\n");
						p_sCurrentGame->GetSpikes()->AddSpike(Position, ((int)pDA_Spike->GetSpikesNum().x), ((int)pDA_Spike->GetSpikesNum().y), true, true, x3Start, x3End);
					}
				}
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
			pDA_Goal->SetIgnoreRenderingZone(true);
			pDA_Goal->SetPosition(Position);
			Scale = { 1,1,1 };
		}
		else {
			pDA_Goal->Update();
			pDA_Goal->SetPosition(Position);
			if (GetInput(INPUT_DEBUGAIM_ACCEPT))
			{
				if (bStaticObject) {
					p_sCurrentGame->GetGoals()->AddMisc(Position, GO_GOAL);
					printf("Misc->AddGoal({ %ff ,%ff ,%ff}, GO_GOAL);\n", Position.x, Position.y, Position.z);
				}
				else {
					if (bSetStart && bSetEnd) {
						bSetStart = false;
						bSetEnd = false;
						printf("動けるオブジェクト置いた\n");
						p_sCurrentGame->GetGoals()->AddMisc(Position, GO_GOAL, true, x3Start, x3End);
					}
				}
			}
			if (GetInput(INPUT_DEBUGAIM_DELETE))
			{
				p_sCurrentGame->GetGoals()->DeleteLastPosObject();
			}
		}
		break;
	case DA_MIRROR:
		if (!pDA_Mirror)
		{
			pDA_Mirror = new Mirror3D();
			pDA_Mirror->SetIgnoreRenderingZone(true);
			pDA_Mirror->SetPosition(Position);
			Scale = { 1,1,1 };
		}
		else {
			if(!bTeleportSet)
				pDA_Mirror->SetPosition(Position);
			else {
				pDA_Mirror->SetPosition(TeleportPosition);
				ModelPosTeleport->SetPosition({ Position.x,Position.y + 12,Position.z });
			}
			if (GetInput(INPUT_DEBUGAIM_ACCEPT))
			{
				if (bStaticObject || (!bStaticObject && bSetStart && bSetEnd)) {
					if (!bTeleportSet)
					{
						TeleportPosition = Position;
						bTeleportSet = true;
						return;
					}
					if (bTeleportSet)
					{
						TeleportDestination = { Position.x,Position.y + 12,Position.z };
						bTeleportDestinationSet = true;
					}
				}
				if (bTeleportSet && bTeleportDestinationSet)
				{
					if (bStaticObject)
					{
						p_sCurrentGame->GetMirrors()->AddMirror(TeleportPosition, TeleportDestination);
					}
					else {
						bSetStart = false;
						bSetEnd = false;
						printf("動けるオブジェクト置いた\n");
						p_sCurrentGame->GetMirrors()->AddMirror(TeleportPosition, TeleportDestination, true, x3Start, x3End);
					}
					bTeleportDestinationSet = bTeleportSet = false;
				}
			}
			if (GetInput(INPUT_DEBUGAIM_DELETE))
			{
				p_sCurrentGame->GetMirrors()->DeleteLastPosObject();
			}
		}
		break;
	case DA_ENEMY_ONI: case DA_ENEMY_ONIB: case DA_ENEMY_ONIC: case DA_ENEMY_ONID: case DA_ENEMY_WARRIOR: case DA_ENEMY_ONIBOSS: 
		if (!pDA_Enemy) {
			pDA_Enemy = new Enemy3D(nObjectType - DA_ENEMY_ONI);
			pDA_Enemy->SetIgnoreRenderingZone(true);
			Scale = { 1,1,1 };
			pDA_Enemy->SetPosition({ Position.x,Position.y,Position.z });
		}
		else {
			pDA_Enemy->SetPosition({ Position.x,Position.y,Position.z });
			if (GetInput(INPUT_DEBUGAIM_ACCEPT))
			{
				if (bStaticObject) {
					p_sCurrentGame->GetEnemies()->AddEnemy({ Position.x,Position.y,Position.z }, nObjectType - DA_ENEMY_ONI);
				}
				else {
					if (bSetStart && bSetEnd) {
						bSetStart = false;
						bSetEnd = false;
						printf("動けるオブジェクト置いた\n");
						p_sCurrentGame->GetEnemies()->AddEnemy({ Position.x,Position.y,Position.z }, nObjectType - DA_ENEMY_ONI, true, x3Start, x3End);
					}
				}
			}
			if (GetInput(INPUT_DEBUGAIM_DELETE))
			{
				p_sCurrentGame->GetEnemies()->DeleteLastPosObject();
			}
		}
		break;
	case DA_EVENT:
		if (!pDA_EventBox) {
			EventNum = 0;
			pDA_EventBox = new EventBox3D(EventNum);
			pDA_EventBox->SetIgnoreRenderingZone(true);
			Scale = { 10,10,10 };
			pDA_EventBox->SetPosition({ Position.x,Position.y + 12,Position.z });
			pDA_EventBox->SetHitbox({ 0,0,0, Scale.x,Scale.y,Scale.z });
		}
		else {
			pDA_EventBox->SetPosition({ Position.x,Position.y + 12,Position.z });
			pDA_EventBox->SetScaleWithHitbox(Scale);
			pDA_EventBox->Update();
			ScaleControl(fSpeed);
			if (GetInput(INPUT_DEBUGAIM_ACCEPT))
			{
				Hitbox3D pHB = pDA_EventBox->GetHitBox();
				pHB.x = 0;
				pHB.y = 0;
				pHB.z = 0;
				p_sCurrentGame->GetEvents()->AddEvent({ Position.x,Position.y + 12,Position.z }, pHB, EventNum);
				printf("EVENT:%d\n", pDA_EventBox->GetEvenType());
			}
			if (GetInput(INPUT_DEBUGAIM_DELETE))
			{
				p_sCurrentGame->GetEvents()->DeleteLastPosObject();
			}
		}

		break;
	default:
		break;
	}
}

void DebugAim::SaveAllControl()
{
	if (GetInput(INPUT_SAVE_LEVEL))
	{
		SceneGame* p_sCurrentGame = (SceneGame*)pCurrentGame;
		int nSceneType = p_sCurrentGame->GetSceneType();

		switch (nSceneType)
		{
		case SCENE_HELL_GAME:
			p_sCurrentGame->GetFields()->SaveFields("HELL/Fields_Level");
			p_sCurrentGame->GetGoals()->SaveMisc("HELL/Goals_Level");
			p_sCurrentGame->GetWalls()->SaveWalls("HELL/Walls_Level");
			p_sCurrentGame->GetItems()->SaveItems("HELL/Items_Level");
			p_sCurrentGame->GetSpikes()->SaveSpikes("HELL/Spikes_Level");
			p_sCurrentGame->GetGoals()->SaveMisc("HELL/Goals_Level");
			p_sCurrentGame->GetMirrors()->SaveMirrors("HELL/Mirrors_Level");
			p_sCurrentGame->GetEvents()->SaveEvents("TUTORIAL/Events_Level");
			return;
		case SCENE_GAME:
			p_sCurrentGame->GetFields()->SaveFields("HEWLEVEL/Fields_Level");
			p_sCurrentGame->GetGoals()->SaveMisc("HEWLEVEL/Goals_Level");
			p_sCurrentGame->GetWalls()->SaveWalls("HEWLEVEL/Walls_Level");
			p_sCurrentGame->GetItems()->SaveItems("HEWLEVEL/Items_Level");
			p_sCurrentGame->GetSpikes()->SaveSpikes("HEWLEVEL/Spikes_Level");
			p_sCurrentGame->GetGoals()->SaveMisc("HEWLEVEL/Goals_Level");
			p_sCurrentGame->GetMirrors()->SaveMirrors("HEWLEVEL/Mirrors_Level");
			p_sCurrentGame->GetEnemies()->SaveEnemies("HEWLEVEL/Enemies_Level");
			p_sCurrentGame->GetEvents()->SaveEvents("TUTORIAL/Events_Level");
			return;
		case SCENE_TUTORIAL_GAME:
			p_sCurrentGame->GetFields()->SaveFields("TUTORIAL/Fields_Level");
			p_sCurrentGame->GetGoals()->SaveMisc("TUTORIAL/Goals_Level");
			p_sCurrentGame->GetWalls()->SaveWalls("TUTORIAL/Walls_Level");
			p_sCurrentGame->GetItems()->SaveItems("TUTORIAL/Items_Level");
			p_sCurrentGame->GetSpikes()->SaveSpikes("TUTORIAL/Spikes_Level");
			p_sCurrentGame->GetGoals()->SaveMisc("TUTORIAL/Goals_Level");
			p_sCurrentGame->GetMirrors()->SaveMirrors("TUTORIAL/Mirrors_Level");
			p_sCurrentGame->GetEnemies()->SaveEnemies("TUTORIAL/Enemies_Level");
			p_sCurrentGame->GetEvents()->SaveEvents("TUTORIAL/Events_Level");

			return;
		default:
			break;
		}

	}
}

void DebugAim::MoveableObjectPositionControl()
{
	if (!bStaticObject && nObjectType != DA_DEBUGAIM) {
		if (ModelPosEnd) {
			ModelPosEnd->UpdateModel();
			if (!bSetEnd) {
				ModelPosEnd->SetPosition({ Position.x,Position.y + 12,Position.z });
				if (bSetStart)
				{
					if (GetInput(INPUT_DEBUGAIM_ACCEPT)) {
						bSetEnd = true;
						x3End = { Position.x, Position.y + 12, fDelayBetweenStops };
					}
				}
			}
		}
		if (ModelPosStart) {
			ModelPosStart->UpdateModel();
			if (!bSetStart) {
				ModelPosStart->SetPosition({ Position.x,Position.y + 12,Position.z });
				if (GetInput(INPUT_DEBUGAIM_ACCEPT)) {
					bSetStart = true;
					x3Start = { Position.x, Position.y + 12, fSpeedMoveable };
				}
			}
		}
	}
}

void DebugAim::SwitchObjectTypeControl()
{
	SAFE_DELETE(pDA_Field);
	SAFE_DELETE(pDA_Wall);
	SAFE_DELETE(pDA_Item);
	SAFE_DELETE(pDA_Spike);
	SAFE_DELETE(pDA_Goal);
	SAFE_DELETE(pDA_Mirror);
	SAFE_DELETE(pDA_Enemy);
	SAFE_DELETE(pDA_EventBox);
	Scale = { 1,1,1 };
}

void DebugAim::ScaleControl(float fSpeed)
{
	switch (nObjectType)
	{
	case DA_FIELD: case DA_EVENT:
		if (!(GetInput(INPUT_SHIFT))) {
			if (GetInput(INPUT_SCALE_UP_Z)) {
				if(nObjectType == DA_FIELD)
					Scale.z += fSpeed;
				else
					Scale.y += fSpeed;
			}
			if (GetInput(INPUT_SCALE_DOWN_Z)) {
				if (nObjectType == DA_FIELD)
					Scale.z -= fSpeed;
				else
					Scale.y -= fSpeed;
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
			if (!GetInput(INPUT_SPACE)) {
				if (GetInput(INPUT_SCALE_UP_Z)) {
					Scale.y += fSpeed;
					GetMainCamera()->ZoomOutZ(fSpeed * 12);
				}
				if (GetInput(INPUT_SCALE_DOWN_Z)) {
					Scale.y -= fSpeed;
					GetMainCamera()->ZoomOutZ(-fSpeed * 12);
				}
			}
			else
			{
				if (GetInput(INPUT_SCALE_UP_Z)) {
					Scale.z += fSpeed;
					GetMainCamera()->ZoomOutZ(fSpeed * 12);
				}
				if (GetInput(INPUT_SCALE_DOWN_Z)) {
					Scale.z -= fSpeed;
					GetMainCamera()->ZoomOutZ(-fSpeed * 12);
				}
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
	case DA_MIRROR:
		if (pDA_Mirror)
			pDA_Mirror->Draw();
		break;
	case DA_ENEMY_ONI: case DA_ENEMY_ONIB: case DA_ENEMY_ONIC: case DA_ENEMY_ONID: case DA_ENEMY_WARRIOR: case DA_ENEMY_ONIBOSS:
		if (pDA_Enemy)
			pDA_Enemy->Draw();
		break;
	case DA_EVENT:
		if (pDA_EventBox)
			pDA_EventBox->Draw();
		break;
	default:
		break;
	}
	if (!bStaticObject && nObjectType!=DA_DEBUGAIM) {
		if (ModelPosStart)
			ModelPosStart->DrawModel();
		if (ModelPosEnd)
			ModelPosEnd->DrawModel();
	}
	if (bTeleportSet && ModelPosTeleport) 
		ModelPosTeleport->DrawModel();
}

void DebugAim::Uninit()
{
	GameObject3D::Uninit();
}

int DebugAim::GetCurrentType()
{
	return nObjectType;
}

bool DebugAim::IsStaticMode()
{
	return bStaticObject;
}

int DebugAim::GetMoveSpeedInt()
{
	return (int)(fSpeedMoveable*10);
}

int DebugAim::GetDelayObj()
{
	return (int)fDelayBetweenStops;
}
int DebugAim::GetNumEvent()
{
	return EventNum;
}

bool DebugAim::IsEventSet()
{
	if (pDA_EventBox)
		return true;
	return false;
}
