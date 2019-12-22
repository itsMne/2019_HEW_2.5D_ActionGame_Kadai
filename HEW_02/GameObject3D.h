#pragma once
#include "Model3D.h"
#include "Cube3D.h"
enum goType
{
	GO_PLAYER = 0,
	GO_WALL,
	GO_ITEM,
	GO_SPIKE,
	GO_FLOOR,
	GO_GOAL,
	GO_DEBUG_AIM,
	MAX_TYPE
};
class GameObject3D
{
private:
	//自動で動いているオブジェクトなら
	bool bMoveable;
	bool bGoToStartPos;
	XMFLOAT3 x3MoveStartPos;
	XMFLOAT3 x3MoveEndPos;
protected:
	Model3D* pModel;//モデル
	XMFLOAT3 Rotation;//回転
	XMFLOAT3 Position;//拠点
	XMFLOAT3 Scale;//大きさ
	XMFLOAT4X4 WorldMatrix;
	int nType;//タイプ
	Hitbox3D hitbox;
	Cube3D* pVisualHitbox;
	bool bUnlit;
public:
	GameObject3D();
	~GameObject3D();
	virtual void Init();
	virtual void Update();
	void AutomaticMovementControl();
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
	void SetHitbox(Hitbox3D hb);
	int GetType();
	void SetMovement(XMFLOAT3 Start, XMFLOAT3 End);
	//自動で動いているオブジェクトなら
	bool IsMoveableObject();
	XMFLOAT3 GetMoveStartPosition();
	XMFLOAT3 GetMoveEndPosition();
};

//*****************************************************************************
// クラス-リスト
//*****************************************************************************
typedef struct go_node {//リストの管理の為に、この構造体が要る
	GameObject3D* Object;
	go_node *next;
};
typedef struct GameObjectContainer {
	XMFLOAT3 Pos;
	XMFLOAT3 Scale;
	char texpath[256];
	bool bMoveable = false;
	XMFLOAT3 MoveStartPos;
	XMFLOAT3 MoveEndPos;
};
typedef struct MiscContainer {
	XMFLOAT3 Pos;
	XMFLOAT3 Scale;
	bool bMoveable = false;
	XMFLOAT3 MoveStartPos;
	XMFLOAT3 MoveEndPos;
	int nType;
};
typedef struct ItemContainer {
	XMFLOAT3 Pos;
	int nItemType;
	bool bMoveable = false;
	XMFLOAT3 MoveStartPos;
	XMFLOAT3 MoveEndPos;
};
typedef struct SpikesContainer {
	XMFLOAT3 Pos;
	int SpikesX;
	int SpikesY;
	bool Invisible = false;
	bool bMoveable = false;
	XMFLOAT3 MoveStartPos;
	XMFLOAT3 MoveEndPos;
};

class Go_List
{
private:
	int nObjectCount;
public:
	go_node * HeadNode;
	Go_List();
	~Go_List();
	int GetNumberOfObjects();
	GameObject3D* AddField(XMFLOAT3 newPosition, XMFLOAT3 newScale, const char* TexturePath);
	GameObject3D* AddField(XMFLOAT3 newPosition, XMFLOAT3 newScale, const char* TexturePath, bool Moveable, XMFLOAT3 Start, XMFLOAT3 End);
	GameObject3D* AddWall(XMFLOAT3 newPosition, XMFLOAT3 newScale);
	GameObject3D* AddWall(XMFLOAT3 newPosition, XMFLOAT3 newScale, bool Moveable, XMFLOAT3 Start, XMFLOAT3 End);
	GameObject3D* AddItem(XMFLOAT3 newPosition, int nType);
	GameObject3D* AddItem(XMFLOAT3 newPosition, int nType, bool Moveable, XMFLOAT3 Start, XMFLOAT3 End);
	GameObject3D* AddSpike(XMFLOAT3 newPosition, int SpikesX, int SpikesY, bool binvisible);
	GameObject3D* AddSpike(XMFLOAT3 newPosition, int SpikesX, int SpikesY, bool binvisible, bool Moveable, XMFLOAT3 Start, XMFLOAT3 End);
	GameObject3D* AddMisc(XMFLOAT3 newPosition, int nType);
	GameObject3D* AddMisc(XMFLOAT3 newPosition, int nType, bool Moveable, XMFLOAT3 Start, XMFLOAT3 End);

	void DeleteLastPosObject();
	void DeleteObject(GameObject3D*);
	void Update();
	void Draw();
	void End();
	void SaveFields(const char* szFilename);
	void SaveWalls(const char* szFilename);
	void SaveItems(const char* szFilename);
	void SaveSpikes(const char* szFilename);
	void SaveMisc(const char* szFilename);
	//void SaveItem(const char* szFilename);
	void Load(const char* szFilename, int nType);
};
