#include "GameObject3D.h"
#include "Field3D.h"
#include "Wall3D.h"
#include "Spike3D.h"
#include "C_Item.h"
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
#if SHOW_HITBOX
	pVisualHitbox->SetPosition({ GetHitBox().x, GetHitBox().y, GetHitBox().z });
	pVisualHitbox->SetScale({ GetHitBox().SizeX, GetHitBox().SizeY, GetHitBox().SizeZ });
#endif
}

void GameObject3D::Draw()
{
	bool bPreviousLight = GetMainLight()->IsLightEnabled();
#if SHOW_HITBOX
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
		HeadNode->next = nullptr;
		nObjectCount++;
		return HeadNode->Object;
	}
}

GameObject3D * Go_List::AddWall(XMFLOAT3 newPosition, XMFLOAT3 newScale)
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
		HeadNode->next = nullptr;
		nObjectCount++;
		return HeadNode->Object;
	}

}

GameObject3D * Go_List::AddItem(XMFLOAT3 newPosition, int nType)
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
		nObjectCount++;
		return pWorkList->Object;
	}
	else {
		HeadNode = new go_node();
		HeadNode->Object = new C_Item(nType);
		C_Item* thisItem = (C_Item*)(HeadNode->Object);
		thisItem->SetPosition(newPosition);
		HeadNode->next = nullptr;
		nObjectCount++;
		return HeadNode->Object;
	}

}

GameObject3D * Go_List::AddSpike(XMFLOAT3 newPosition, int SpikesX, int SpikesY, bool binvisible)
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
		HeadNode->next = nullptr;
		nObjectCount++;
		return HeadNode->Object;
	}

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
	ItemContainer* item_container = new ItemContainer();
	SpikesContainer* spike_container = new SpikesContainer();
	if (nType== GO_FLOOR || nType == GO_WALL) {
		while ((fread(go_container, sizeof(GameObjectContainer), 1, pFile)))
		{
			switch (nType)
			{
			case GO_FLOOR:
				AddField(go_container->Pos, go_container->Scale, go_container->texpath);
				break;
			case GO_WALL:
				AddWall(go_container->Pos, go_container->Scale);
				break;
			default:
				break;
			}

		}
	}
	else if (nType == GO_ITEM) {
		while ((fread(item_container, sizeof(ItemContainer), 1, pFile)))
			AddItem(item_container->Pos, item_container->nItemType);
	}
	else if (nType == GO_SPIKE)
	{
		while ((fread(spike_container, sizeof(SpikesContainer), 1, pFile)))
			AddSpike(spike_container->Pos, spike_container->SpikesX, spike_container->SpikesY, spike_container->Invisible);
	}
	delete(go_container);
	fclose(pFile);
}
