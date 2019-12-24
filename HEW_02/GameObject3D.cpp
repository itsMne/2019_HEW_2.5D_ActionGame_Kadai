#include "GameObject3D.h"
#include "Field3D.h"
#include "Wall3D.h"
#include "Spike3D.h"
#include "C_Item.h"
#include "Player3D.h"
#include "InputManager.h"
#include "Goal3D.h"
#include "SceneGame.h"
#include "Mirror3D.h"
#include "Enemy3D.h"
#include "String.h"

GameObject3D::GameObject3D()
{
	pModel = nullptr;
	Init();
}


GameObject3D::~GameObject3D()
{
	Uninit();
}

void GameObject3D::Init()
{
	Rotation = { 0,0,0 };
	Position = { 0,0,0 };
	Scale = { 1,1,1 };
	hitbox = { 0,0,0,0,0,0 };
	pModel = nullptr;
	bMoveable = false;
	bGoToStartPos = false;
	bUnlit = false;
	nDelayFramesBetweenStops = 0;
	nPauseFrames = 0;
#if SHOW_HITBOX
	pVisualHitbox = new Cube3D("data/texture/hbox.tga");
	pVisualHitbox->SetPosition({ GetHitBox().x, GetHitBox().y, GetHitBox().z });
	pVisualHitbox->SetScale({ GetHitBox().SizeX, GetHitBox().SizeY, GetHitBox().SizeZ });
#endif
}

void GameObject3D::Update()
{
	if (pModel)
		pModel->UpdateModel();
	if (bMoveable)
	{
		AutomaticMovementControl();
	}
#if SHOW_HITBOX
	pVisualHitbox->SetPosition({ GetHitBox().x, GetHitBox().y, GetHitBox().z });
	pVisualHitbox->SetScale({ GetHitBox().SizeX, GetHitBox().SizeY, GetHitBox().SizeZ });
#endif

#if DEBUG_MODE
	if (nType == GO_DEBUG_AIM)
		return;
	Player3D* pPlayer = GetMainPlayer();
	SceneGame* pS_Game = GetCurrentGame();
	if (!pPlayer)
		return;
	if (!pPlayer->IsDebugAimOn())
		return;
	if (!pS_Game)
		return;
	DebugAim* pDA = pPlayer->GetDebugAim();
	Go_List* List;
	if (GetInput(INPUT_DEBUGAIM_DELETE) && pDA->GetCurrentType() == DA_DEBUGAIM) {
		if (IsInCollision3D(pDA->GetHitBox(), GetHitBox()))
		{
			switch (nType)
			{
			case GO_FLOOR:
				List = pS_Game->GetFields();
				List->DeleteObject(this);
				return;
			case GO_WALL:
				List = pS_Game->GetWalls();
				List->DeleteObject(this);
				return;
			case GO_ITEM:
				List = pS_Game->GetItems();
				List->DeleteObject(this);
				return;
			case GO_SPIKE:
				List = pS_Game->GetSpikes();
				List->DeleteObject(this);
				return;
			case GO_GOAL:
				List = pS_Game->GetGoals();
				List->DeleteObject(this);
				break;
			case GO_ENEMY:
				List = pS_Game->GetEnemies();
				List->DeleteObject(this);
				break;
			default:
				break;
			}
		}
	}
#endif
}

void GameObject3D::AutomaticMovementControl()
{
	if (--nPauseFrames > 0)
		return;
	if (--nDelayFramesBetweenStops > 0)
		return;
	nPauseFrames = 0;
	XMFLOAT2 Destination = {0,0};
	float fSpeed = x3MoveStartPos.z;
	//printf("%f\n", Position.z);
	float fDelay = x3MoveEndPos.z;

	if (bGoToStartPos)
	{
		Destination.x = x3MoveStartPos.x;
		Destination.y = x3MoveStartPos.y;
	}
	else {
		Destination.x = x3MoveEndPos.x;
		Destination.y = x3MoveEndPos.y;
	}

	bool bIsPlayerFloorOrCrawling = false;
	Player3D* pPlayer = GetMainPlayer();
	if (pPlayer) {
		if (pPlayer->GetFloor() == this)
			bIsPlayerFloorOrCrawling = true;
		if(pPlayer->GetWallCrawling()==this)
			bIsPlayerFloorOrCrawling = true;
	}

	if (Position.x < Destination.x)
	{
		Position.x += fSpeed;
		if (bIsPlayerFloorOrCrawling)
			pPlayer->TranslateX(fSpeed);
		if (Position.x > Destination.x)
			Position.x = Destination.x;
	}
	else if (Position.x > Destination.x){
		Position.x -= fSpeed;
		if (bIsPlayerFloorOrCrawling)
			pPlayer->TranslateX(-fSpeed);
		if (Position.x < Destination.x)
			Position.x = Destination.x;
	}

	if (Position.y < Destination.y)
	{
		Position.y += fSpeed;
		if (bIsPlayerFloorOrCrawling)
			pPlayer->TranslateY(fSpeed);
		if (Position.y > Destination.y)
			Position.y = Destination.y;
	}
	else if (Position.y > Destination.y) {
		Position.y -= fSpeed;
		if (bIsPlayerFloorOrCrawling)
			pPlayer->TranslateY(-fSpeed);
		if (Position.y < Destination.y)
			Position.y = Destination.y;
	}
	if (Position.y == Destination.y && Position.x == Destination.x)
	{
		if (bGoToStartPos)
			bGoToStartPos = false;
		else
			bGoToStartPos = true;
		nDelayFramesBetweenStops = (int)x3MoveEndPos.z*60;
	}

}

void GameObject3D::Draw()
{
	Light3D* pLight = GetMainLight();
	bool bPreviousLight;
#if SHOW_HITBOX
	if (!pLight)
		return;
	bPreviousLight = GetMainLight()->IsLightEnabled();
	GetMainLight()->SetLightEnable(false);
	SetCullMode(CULLMODE_NONE);
	pVisualHitbox->Draw();
	SetCullMode(CULLMODE_CCW);
	GetMainLight()->SetLightEnable(bPreviousLight);
#endif
	if (pModel) {
		if (!pLight)
			return;
		bPreviousLight = GetMainLight()->IsLightEnabled();
		if (bUnlit) {
			GetMainLight()->SetLightEnable(false);
			pModel->SetLight(GetMainLight());
		}
		if (pModel)
			pModel->DrawModel();
		if (bUnlit)
			GetMainLight()->SetLightEnable(bPreviousLight);
	}

}

void GameObject3D::Uninit()
{
	SAFE_DELETE(pModel);
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

void GameObject3D::SetHitbox(Hitbox3D hb)
{
	hitbox = hb;
}

int GameObject3D::GetType()
{
	return nType;
}

void GameObject3D::SetMovement(XMFLOAT3 Start, XMFLOAT3 End)
{
	x3MoveStartPos = Start;
	x3MoveEndPos = End;
	bGoToStartPos = true;
	bMoveable = true;
}

bool GameObject3D::IsMoveableObject()
{
	return bMoveable;
}

XMFLOAT3 GameObject3D::GetMoveStartPosition()
{
	return x3MoveStartPos;
}

XMFLOAT3 GameObject3D::GetMoveEndPosition()
{
	return x3MoveEndPos;
}

void GameObject3D::PauseObject(int pauseFrames)
{
	nPauseFrames = pauseFrames;
}

Go_List::Go_List()
{
	HeadNode = nullptr;
	nObjectCount = 0;
}

Go_List::~Go_List()
{
	End();
}

int Go_List::GetNumberOfObjects()
{
	return nObjectCount;
}

GameObject3D * Go_List::AddField(XMFLOAT3 newPosition, XMFLOAT3 newScale, const char * TexturePath)
{
	return AddField(newPosition, newScale, TexturePath, false, { 0,0,0 }, { 0,0,0 });
}

GameObject3D * Go_List::AddField(XMFLOAT3 newPosition, XMFLOAT3 newScale, const char * TexturePath, bool Moveable, XMFLOAT3 Start, XMFLOAT3 End)
{
	go_node* pPositionList = HeadNode;
	if (HeadNode != nullptr) {
		while (pPositionList->next != nullptr) {
			pPositionList = pPositionList->next;
		}
		go_node* pWorkList = new go_node();
		pWorkList->Object = new Field3D(TexturePath);
		Field3D* ThisField = (Field3D*)(pWorkList->Object);
		ThisField->SetScaleWithHitbox(newScale);
		ThisField->SetPosition(newPosition);
		if (Moveable)
			ThisField->SetMovement(Start, End);
		pWorkList->next = nullptr;
		pPositionList->next = pWorkList;
		nObjectCount++;
		return pWorkList->Object;
	}
	else {
		HeadNode = new go_node();
		HeadNode->Object = new Field3D(TexturePath);
		Field3D* ThisField = (Field3D*)(HeadNode->Object);
		ThisField->SetScaleWithHitbox(newScale);
		ThisField->SetPosition(newPosition);
		if (Moveable)
			ThisField->SetMovement(Start, End);
		HeadNode->next = nullptr;
		nObjectCount++;
		return HeadNode->Object;
	}
}

GameObject3D * Go_List::AddWall(XMFLOAT3 newPosition, XMFLOAT3 newScale)
{
	return AddWall(newPosition, newScale, false, { 0,0,0 }, { 0,0,0 });
}

GameObject3D * Go_List::AddWall(XMFLOAT3 newPosition, XMFLOAT3 newScale, bool Moveable, XMFLOAT3 Start, XMFLOAT3 End)
{
	go_node* pPositionList = HeadNode;
	if (HeadNode != nullptr) {
		while (pPositionList->next != nullptr) {
			pPositionList = pPositionList->next;
		}
		go_node* pWorkList = new go_node();
		pWorkList->Object = new Wall3D();
		Wall3D* ThisWall = (Wall3D*)(pWorkList->Object);
		ThisWall->SetScale(newScale);
		ThisWall->SetPosition(newPosition);
		if (Moveable)
			ThisWall->SetMovement(Start, End);
		pWorkList->next = nullptr;
		pPositionList->next = pWorkList;
		nObjectCount++;
		return pWorkList->Object;
	}
	else {
		HeadNode = new go_node();
		HeadNode->Object = new Wall3D();
		Wall3D* ThisWall = (Wall3D*)(HeadNode->Object);
		ThisWall->SetScale(newScale);
		ThisWall->SetPosition(newPosition);
		if (Moveable)
			ThisWall->SetMovement(Start, End);
		HeadNode->next = nullptr;
		nObjectCount++;
		return HeadNode->Object;
	}
}

GameObject3D * Go_List::AddItem(XMFLOAT3 newPosition, int nType)
{
	return AddItem(newPosition, nType, false, { 0,0,0 }, { 0,0,0 });
}

GameObject3D * Go_List::AddItem(XMFLOAT3 newPosition, int nType, bool Moveable, XMFLOAT3 Start, XMFLOAT3 End)
{
	go_node* pPositionList = HeadNode;
	if (HeadNode != nullptr) {
		while (pPositionList->next != nullptr) {
			pPositionList = pPositionList->next;
		}
		go_node* pWorkList = new go_node();
		pWorkList->Object = new C_Item(nType);
		C_Item* thisItem = (C_Item*)(pWorkList->Object);
		thisItem->SetPosition(newPosition);
		pWorkList->next = nullptr;
		pPositionList->next = pWorkList;
		if (Moveable)
			thisItem->SetMovement(Start, End);
		nObjectCount++;
		return pWorkList->Object;
	}
	else {
		HeadNode = new go_node();
		HeadNode->Object = new C_Item(nType);
		C_Item* thisItem = (C_Item*)(HeadNode->Object);
		thisItem->SetPosition(newPosition);
		if (Moveable)
			thisItem->SetMovement(Start, End);
		HeadNode->next = nullptr;
		nObjectCount++;
		return HeadNode->Object;
	}
}

GameObject3D * Go_List::AddSpike(XMFLOAT3 newPosition, int SpikesX, int SpikesY, bool binvisible)
{
	
	return AddSpike(newPosition, SpikesX, SpikesY, binvisible, false, { 0,0,0 }, { 0,0,0 });
}

GameObject3D * Go_List::AddSpike(XMFLOAT3 newPosition, int SpikesX, int SpikesY, bool binvisible, bool Moveable, XMFLOAT3 Start, XMFLOAT3 End)
{
	go_node* pPositionList = HeadNode;
	if (HeadNode != nullptr) {
		while (pPositionList->next != nullptr) {
			pPositionList = pPositionList->next;
		}
		go_node* pWorkList = new go_node();
		pWorkList->Object = new Spike3D();
		Spike3D* thisSpike = (Spike3D*)(pWorkList->Object);
		thisSpike->SetPosition(newPosition);
		thisSpike->SetSpikesNum(SpikesX, SpikesY);
		if (Moveable)
			thisSpike->SetMovement(Start, End);
		pWorkList->next = nullptr;
		pPositionList->next = pWorkList;
		nObjectCount++;
		return pWorkList->Object;
	}
	else {
		HeadNode = new go_node();
		HeadNode->Object = new Spike3D();
		Spike3D* thisSpike = (Spike3D*)(HeadNode->Object);
		thisSpike->SetPosition(newPosition);
		thisSpike->SetSpikesNum(SpikesX, SpikesY);
		thisSpike->SetInvisibility(binvisible);
		if (Moveable)
			thisSpike->SetMovement(Start, End);
		HeadNode->next = nullptr;
		nObjectCount++;
		return HeadNode->Object;
	}
}

GameObject3D * Go_List::AddMisc(XMFLOAT3 newPosition, int nType)
{
	return AddMisc(newPosition, nType, false, {0,0,0}, {0,0,0});
}

GameObject3D * Go_List::AddMisc(XMFLOAT3 newPosition, int nType, bool Moveable, XMFLOAT3 Start, XMFLOAT3 End)
{
	go_node* pPositionList = HeadNode;
	if (HeadNode != nullptr) {
		while (pPositionList->next != nullptr) {
			pPositionList = pPositionList->next;
		}
		go_node* pWorkList = new go_node();
		switch (nType)
		{
		case GO_GOAL:
			pWorkList->Object = new Goal3D();
			break;
		}
		GameObject3D* thisObj = pWorkList->Object;
		thisObj->SetPosition(newPosition);
		if (Moveable)
			thisObj->SetMovement(Start, End);
		pWorkList->next = nullptr;
		pPositionList->next = pWorkList;
		nObjectCount++;
		return pWorkList->Object;
	}
	else {
		HeadNode = new go_node();
		switch (nType)
		{
		case GO_GOAL:
			HeadNode->Object = new Goal3D();
			break;
		}
		GameObject3D* thisObj = HeadNode->Object;
		thisObj->SetPosition(newPosition);
		if (Moveable)
			thisObj->SetMovement(Start, End);
		HeadNode->next = nullptr;
		nObjectCount++;
		return HeadNode->Object;
	}
}

GameObject3D * Go_List::AddMirror(XMFLOAT3 newPosition, XMFLOAT3 Destination)
{
	return AddMirror(newPosition, Destination, false, { 0,0,0 }, { 0,0,0 });
}

GameObject3D * Go_List::AddMirror(XMFLOAT3 newPosition, XMFLOAT3 Destination, bool Moveable, XMFLOAT3 Start, XMFLOAT3 End)
{
	go_node* pPositionList = HeadNode;
	if (HeadNode != nullptr) {
		while (pPositionList->next != nullptr) {
			pPositionList = pPositionList->next;
		}
		go_node* pWorkList = new go_node();
		pWorkList->Object = new Mirror3D();
		Mirror3D* thisMirror = (Mirror3D*)(pWorkList->Object);
		thisMirror->SetPosition(newPosition);
		thisMirror->SetDestination(Destination);
		if (Moveable) {
			thisMirror->SetMovement(Start, End);
		}
		pWorkList->next = nullptr;
		pPositionList->next = pWorkList;
		nObjectCount++;
		return pWorkList->Object;
	}
	else {
		HeadNode = new go_node();
		HeadNode->Object = new Mirror3D();
		Mirror3D* thisMirror = (Mirror3D*)(HeadNode->Object);
		thisMirror->SetPosition(newPosition);
		thisMirror->SetDestination(Destination);
		if (Moveable)
			thisMirror->SetMovement(Start, End);
		HeadNode->next = nullptr;
		nObjectCount++;
		return HeadNode->Object;
	}
}

GameObject3D * Go_List::AddEnemy(XMFLOAT3 newPosition, int EnemyType)
{
	return AddEnemy(newPosition, EnemyType, false, { 0,0,0 }, {0,0,0});
}

GameObject3D * Go_List::AddEnemy(XMFLOAT3 newPosition, int EnemyType, bool Moveable, XMFLOAT3 Start, XMFLOAT3 End)
{
	go_node* pPositionList = HeadNode;
	if (HeadNode != nullptr) {
		while (pPositionList->next != nullptr) {
			pPositionList = pPositionList->next;
		}
		go_node* pWorkList = new go_node();
		pWorkList->Object = new Enemy3D(TYPE_ONI);
		Enemy3D* thisEnemy = (Enemy3D*)(pWorkList->Object);
		thisEnemy->SetPosition(newPosition);
		printf("{%f, %f, %f}\n", newPosition.x, newPosition.y, newPosition.z);
		if (Moveable) {
			thisEnemy->SetMovement(Start, End);
		}
		pWorkList->next = nullptr;
		pPositionList->next = pWorkList;
		nObjectCount++;
		return pWorkList->Object;
	}
	else {
		HeadNode = new go_node();
		HeadNode->Object = new Enemy3D(TYPE_ONI);
		Enemy3D* thisMirror = (Enemy3D*)(HeadNode->Object);
		thisMirror->SetPosition(newPosition);
		printf("{%f, %f, %f}\n", newPosition.x, newPosition.y, newPosition.z);
		if (Moveable)
			thisMirror->SetMovement(Start, End);
		HeadNode->next = nullptr;
		nObjectCount++;
		return HeadNode->Object;
	}
}

GameObject3D * Go_List::CheckCollision(Hitbox3D hb)
{
	if (HeadNode == nullptr)
		return nullptr;
	go_node* pPositionList = HeadNode;
	while (true) {
		if (pPositionList == nullptr)
			break;
		if (pPositionList->Object != nullptr) 
		{
			if (IsInCollision3D(pPositionList->Object->GetHitBox(), hb))
				return pPositionList->Object;
		}
		pPositionList = pPositionList->next;
	}
	return nullptr;
}



void Go_List::DeleteLastPosObject()
{
	if (HeadNode == nullptr)
		return;
	go_node* pPositionList = HeadNode->next;
	go_node* PreviousPos = HeadNode;
	if (pPositionList == nullptr)
	{
		if (PreviousPos->Object) {
			delete(PreviousPos->Object);
			PreviousPos->Object = nullptr;
			delete(PreviousPos);
			HeadNode = PreviousPos = nullptr;
			return;
		}
	}
	while (true) {
		if (pPositionList->next == nullptr)
			break;
		pPositionList = pPositionList->next;
		PreviousPos = PreviousPos->next;
	}
	if (pPositionList->Object) {
		delete(pPositionList->Object);
		pPositionList->Object = nullptr;
		delete(pPositionList);
		pPositionList = nullptr;
		PreviousPos->next = nullptr;
	}
	return;
}

void Go_List::DeleteObject(GameObject3D * pSearch)
{
	if (HeadNode == nullptr)
		return;
	if (HeadNode->Object == pSearch)
	{
		go_node* pPositionList = HeadNode;
		HeadNode = HeadNode->next;
		delete(pPositionList->Object);
		pPositionList->Object = nullptr;
		delete(pPositionList);
		pPositionList = nullptr;
		return;
	}
	if (HeadNode->next->Object == pSearch)
	{
		go_node* pPositionList = HeadNode->next;
		HeadNode->next = HeadNode->next->next;
		delete(pPositionList->Object);
		pPositionList->Object = nullptr;
		delete(pPositionList);
		pPositionList = nullptr;
		return;
	}
	go_node* pPositionList = HeadNode->next;
	go_node* PreviousPos = HeadNode;
	if (pPositionList == nullptr)
		return;
	while (true) {
		if (pPositionList->next == nullptr)
			return;
		if (pPositionList->Object == pSearch)
			break;
		pPositionList = pPositionList->next;
		PreviousPos = PreviousPos->next;
	}
	if (pPositionList->Object) {
		PreviousPos->next = pPositionList->next;
		delete(pPositionList->Object);
		pPositionList->Object = nullptr;
		delete(pPositionList);
		pPositionList = nullptr;
	}
	return;
}

void Go_List::Update()
{
	if (HeadNode == nullptr)
		return;
	go_node* pPositionList = HeadNode;
	while (true) {
		if (pPositionList == nullptr)
			break;
		if (pPositionList->Object != nullptr)
			pPositionList->Object->Update();
		pPositionList = pPositionList->next;
	}
}

void Go_List::Draw()
{
	if (HeadNode == nullptr)
		return;
	go_node* pPositionList = HeadNode;
	while (true) {

		if (pPositionList == nullptr)
			break;

		if (pPositionList->Object != nullptr)
			pPositionList->Object->Draw();
		pPositionList = pPositionList->next;
	}
}

void Go_List::End()
{
	if (HeadNode == nullptr)
		return;
	go_node* pPositionList = HeadNode;
	go_node* pWork = nullptr;
	int Count = 0;
	while (true) {
		if (pPositionList == nullptr)
			break;
		if (pPositionList->Object != nullptr) {
			Count++;
			delete(pPositionList->Object);
			pPositionList->Object = nullptr;
		}
		pWork = pPositionList;
		pPositionList = pPositionList->next;
		if (pWork) {
			delete(pWork);
			pWork = nullptr;
		}
	}
	printf("GameObjects Deleted: %d\n", Count);
	nObjectCount = 0;
	HeadNode = nullptr;
}

void Go_List::SaveFields(const char* szFilename)
{
	FILE *pFile;
	char szFinalfilename[256]= "data/levels/";
	strcat(szFinalfilename, szFilename);
	strcat(szFinalfilename, ".bin");
	if (strcmp(szFilename, "") == 0)
	{
		strcpy(szFinalfilename, "Default.bin");
	}
	pFile = fopen(szFinalfilename, "wb");
	if (HeadNode == nullptr)
		return;
	go_node* pPositionList = HeadNode;
	while (true) {

		if (pPositionList == nullptr)
			break;

		if (pPositionList->Object != nullptr)
		{
			if (pPositionList->Object->GetType() == GO_FLOOR)
			{
				GameObjectContainer field;
				Field3D* thisField = (Field3D*)pPositionList->Object;
				field.Pos = thisField->GetPosition();
				field.Scale = thisField->GetScale();
				field.bMoveable = thisField->IsMoveableObject();
				field.MoveStartPos = thisField->GetMoveStartPosition();
				field.MoveEndPos = thisField->GetMoveEndPosition();
				strcpy(field.texpath, thisField->GetTexturePath());
				fwrite(&field, sizeof(GameObjectContainer), 1, pFile);
			}
		}
		pPositionList = pPositionList->next;
	}
	printf("SAVED OK: %s\n", szFinalfilename);
	fclose(pFile);
}

void Go_List::SaveWalls(const char * szFilename)
{
	FILE *pFile;
	char szFinalfilename[256] = "data/levels/";
	strcat(szFinalfilename, szFilename);
	strcat(szFinalfilename, ".bin");
	if (strcmp(szFilename, "") == 0)
	{
		strcpy(szFinalfilename, "Default.bin");
	}
	pFile = fopen(szFinalfilename, "wb");
	if (HeadNode == nullptr)
		return;
	go_node* pPositionList = HeadNode;
	while (true) {

		if (pPositionList == nullptr)
			break;

		if (pPositionList->Object != nullptr)
		{
			if (pPositionList->Object->GetType() == GO_WALL)
			{
				GameObjectContainer wall;
				Wall3D* thisField = (Wall3D*)pPositionList->Object;
				wall.Pos = thisField->GetPosition();
				wall.Scale = thisField->GetScale();
				wall.bMoveable = thisField->IsMoveableObject();
				wall.MoveStartPos = thisField->GetMoveStartPosition();
				wall.MoveEndPos = thisField->GetMoveEndPosition();
				fwrite(&wall, sizeof(GameObjectContainer), 1, pFile);
			}
		}
		pPositionList = pPositionList->next;
	}
	printf("SAVED OK: %s\n", szFinalfilename);
	fclose(pFile);
}

void Go_List::SaveItems(const char * szFilename)
{
	FILE *pFile;
	char szFinalfilename[256] = "data/levels/";
	strcat(szFinalfilename, szFilename);
	strcat(szFinalfilename, ".bin");
	if (strcmp(szFilename, "") == 0)
	{
		strcpy(szFinalfilename, "Default.bin");
	}
	pFile = fopen(szFinalfilename, "wb");
	if (HeadNode == nullptr)
		return;
	go_node* pPositionList = HeadNode;
	while (true) {

		if (pPositionList == nullptr)
			break;

		if (pPositionList->Object != nullptr)
		{
			if (pPositionList->Object->GetType() == GO_ITEM)
			{
				ItemContainer Item;
				C_Item* thisItem = (C_Item*)pPositionList->Object;
				Item.Pos = thisItem->GetPosition();
				Item.bMoveable = thisItem->IsMoveableObject();
				Item.MoveStartPos = thisItem->GetMoveStartPosition();
				Item.MoveEndPos = thisItem->GetMoveEndPosition();
				Item.nItemType = thisItem->GetItemType();
				fwrite(&Item, sizeof(ItemContainer), 1, pFile);
			}
		}
		pPositionList = pPositionList->next;
	}
	printf("SAVED OK: %s\n", szFinalfilename);
	fclose(pFile);
}

void Go_List::SaveSpikes(const char * szFilename)
{
	FILE *pFile;
	char szFinalfilename[256] = "data/levels/";
	strcat(szFinalfilename, szFilename);
	strcat(szFinalfilename, ".bin");
	if (strcmp(szFilename, "") == 0)
	{
		strcpy(szFinalfilename, "Default.bin");
	}
	pFile = fopen(szFinalfilename, "wb");
	if (HeadNode == nullptr)
		return;
	go_node* pPositionList = HeadNode;
	while (true) {

		if (pPositionList == nullptr)
			break;
		if (pPositionList->Object != nullptr)
		{
			if (pPositionList->Object->GetType() == GO_SPIKE)
			{
				
				SpikesContainer Spike;
				Spike3D* thisSpike = (Spike3D*)pPositionList->Object;
				Spike.Pos = thisSpike->GetPosition();
				Spike.bMoveable = thisSpike->IsMoveableObject();
				Spike.MoveStartPos = thisSpike->GetMoveStartPosition();
				Spike.MoveEndPos = thisSpike->GetMoveEndPosition();
				Spike.SpikesX = (int)(thisSpike->GetSpikesNum().x);
				Spike.SpikesY = (int)(thisSpike->GetSpikesNum().y);
				fwrite(&Spike, sizeof(SpikesContainer), 1, pFile);
			}
		}
		pPositionList = pPositionList->next;
	}
	printf("SAVED OK: %s\n", szFinalfilename);
	fclose(pFile);
}

void Go_List::SaveMisc(const char * szFilename)
{
	FILE *pFile;
	char szFinalfilename[256] = "data/levels/";
	strcat(szFinalfilename, szFilename);
	strcat(szFinalfilename, ".bin");
	if (strcmp(szFilename, "") == 0)
	{
		strcpy(szFinalfilename, "Default.bin");
	}
	pFile = fopen(szFinalfilename, "wb");
	if (HeadNode == nullptr)
		return;
	go_node* pPositionList = HeadNode;
	while (true) {

		if (pPositionList == nullptr)
			break;
		if (pPositionList->Object != nullptr)
		{
			MiscContainer gameObject;
			GameObject3D* thisObject = (GameObject3D*)pPositionList->Object;
			gameObject.Pos = thisObject->GetPosition();
			gameObject.bMoveable = thisObject->IsMoveableObject();
			gameObject.MoveStartPos = thisObject->GetMoveStartPosition();
			gameObject.MoveEndPos = thisObject->GetMoveEndPosition();
			gameObject.nType = thisObject->GetType();
			fwrite(&gameObject, sizeof(MiscContainer), 1, pFile);
		}
		pPositionList = pPositionList->next;
	}
	printf("SAVED OK: %s\n", szFinalfilename);
	fclose(pFile);
}

void Go_List::SaveMirrors(const char * szFilename)
{
	FILE *pFile;
	char szFinalfilename[256] = "data/levels/";
	strcat(szFinalfilename, szFilename);
	strcat(szFinalfilename, ".bin");
	if (strcmp(szFilename, "") == 0)
	{
		strcpy(szFinalfilename, "Default.bin");
	}
	pFile = fopen(szFinalfilename, "wb");
	if (HeadNode == nullptr)
		return;
	go_node* pPositionList = HeadNode;
	while (true) {

		if (pPositionList == nullptr)
			break;
		if (pPositionList->Object != nullptr)
		{
			if (pPositionList->Object->GetType() == GO_MIRROR)
			{
				MirrorContainer thisMirror;
				Mirror3D* thisObject = (Mirror3D*)pPositionList->Object;
				thisMirror.Pos = thisObject->GetPosition();
				thisMirror.bMoveable = thisObject->IsMoveableObject();
				thisMirror.MoveStartPos = thisObject->GetMoveStartPosition();
				thisMirror.MoveEndPos = thisObject->GetMoveEndPosition();
				thisMirror.Destination = thisObject->GetDestination();
				fwrite(&thisMirror, sizeof(MirrorContainer), 1, pFile);
			}
		}
		pPositionList = pPositionList->next;
	}
	printf("SAVED OK: %s\n", szFinalfilename);
	fclose(pFile);
}

void Go_List::SaveEnemies(const char * szFilename)
{
	FILE *pFile;
	char szFinalfilename[256] = "data/levels/";
	strcat(szFinalfilename, szFilename);
	strcat(szFinalfilename, ".bin");
	if (strcmp(szFilename, "") == 0)
	{
		strcpy(szFinalfilename, "Default.bin");
	}
	pFile = fopen(szFinalfilename, "wb");
	if (HeadNode == nullptr)
		return;
	go_node* pPositionList = HeadNode;
	while (true) {

		if (pPositionList == nullptr)
			break;
		if (pPositionList->Object != nullptr)
		{
			if (pPositionList->Object->GetType() == GO_ENEMY)
			{
				EnemyContainer thisMirror;
				Enemy3D* thisObject = (Enemy3D*)pPositionList->Object;
				thisMirror.Pos = thisObject->GetPosition();
				thisMirror.bMoveable = thisObject->IsMoveableObject();
				thisMirror.MoveStartPos = thisObject->GetMoveStartPosition();
				thisMirror.MoveEndPos = thisObject->GetMoveEndPosition();
				thisMirror.EnemyType = thisObject->GetEnemyType();
				fwrite(&thisMirror, sizeof(EnemyContainer), 1, pFile);
			}
		}
		pPositionList = pPositionList->next;
	}
	printf("SAVED OK: %s\n", szFinalfilename);
	fclose(pFile);
}

void Go_List::Load(const char * szFilename, int nType)
{
	FILE *pFile;
	char szFinalfilename[256]= "data/levels/";
	strcat(szFinalfilename, szFilename);
	strcat(szFinalfilename, ".bin");
	if ((pFile = fopen(szFinalfilename, "rb")) == NULL)
	{
		printf_s("%sのファイルはない…\n", szFinalfilename);
		return;
	}
	GameObjectContainer* go_container = new GameObjectContainer();
	MiscContainer* misc_container = new MiscContainer();
	ItemContainer* item_container = new ItemContainer();
	SpikesContainer* spike_container = new SpikesContainer();
	MirrorContainer* mirror_container = new MirrorContainer();
	EnemyContainer* enemy_container = new EnemyContainer();
	switch (nType)
	{
	case GO_FLOOR: 
		while ((fread(go_container, sizeof(GameObjectContainer), 1, pFile))) 
			AddField(go_container->Pos, go_container->Scale, go_container->texpath, go_container->bMoveable, go_container->MoveStartPos, go_container->MoveEndPos);
		break;
	case GO_WALL:
		while ((fread(go_container, sizeof(GameObjectContainer), 1, pFile)))
			AddWall(go_container->Pos, go_container->Scale, go_container->bMoveable, go_container->MoveStartPos, go_container->MoveEndPos);
		break;
	case GO_ITEM:
		while ((fread(item_container, sizeof(ItemContainer), 1, pFile)))
			AddItem(item_container->Pos, item_container->nItemType, item_container->bMoveable, item_container->MoveStartPos, item_container->MoveEndPos);
		break;
	case GO_SPIKE:
		while ((fread(spike_container, sizeof(SpikesContainer), 1, pFile)))
			AddSpike(spike_container->Pos, spike_container->SpikesX, spike_container->SpikesY, spike_container->Invisible, spike_container->bMoveable, spike_container->MoveStartPos, spike_container->MoveEndPos);
		break;
	case GO_GOAL:
		while ((fread(misc_container, sizeof(MiscContainer), 1, pFile)))
			AddMisc(misc_container->Pos, GO_GOAL, misc_container->bMoveable, misc_container->MoveStartPos, misc_container->MoveEndPos);
	case GO_MIRROR:
		while ((fread(mirror_container, sizeof(MirrorContainer), 1, pFile)))
			AddMirror(mirror_container->Pos, mirror_container->Destination, mirror_container->bMoveable, mirror_container->MoveStartPos, mirror_container->MoveEndPos);
		break;
	case GO_ENEMY:
		while ((fread(enemy_container, sizeof(EnemyContainer), 1, pFile)))
			AddEnemy(enemy_container->Pos, enemy_container->EnemyType, enemy_container->bMoveable, enemy_container->MoveStartPos, enemy_container->MoveEndPos);
		break;
		break;
	default:
		break;
	}
	delete(go_container);
	fclose(pFile);
}
