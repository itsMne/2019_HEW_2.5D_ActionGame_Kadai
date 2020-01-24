#pragma once
#include "GameObject3D.h"
#include "Field3D.h"
#include "Wall3D.h"
#include "C_Item.h"
#include "Goal3D.h"
#include "Spike3D.h"
#include "Mirror3D.h"
#include "EventBox3D.h"
#include "BgObject.h"
#include "Enemy3D.h"

enum DEBUG_AIM_OBJECTYPES
{
	DA_DEBUGAIM = 0,
	DA_FIELD,
	DA_WALL,
	DA_SPIKE,
	DA_ITEM_SUSHI,
	DA_ITEM_UDON,
	DA_ITEM_ODEN,
	DA_ITEM_DANGO,
	DA_ITEM_TAI,
	DA_GOAL,
	DA_MIRROR,
	DA_ENEMY_ONI,
	DA_ENEMY_ONIB,
	DA_ENEMY_ONIC,
	DA_ENEMY_ONID,
	DA_ENEMY_WARRIOR,
	DA_ENEMY_ONIBOSS,
	DA_EVENT,
	DA_BGOBJ_HIGANBANA,
	DA_BGOBJ_HIGANBANAS,
	DA_BGOBJ_HICHIFUKUZIN,
	DA_BGOBJ_BONSAI,
	DA_BGOBJ_TREE00,
	DA_BGOBJ_TREE01,
	DA_BGOBJ_TREE02,
	DA_BGOBJ_TREE03,
	DA_BGOBJ_SASA,
	DA_BGOBJ_HANABI,
	DA_BGOBJ_BUMBUKUTYAGAMA,
	DA_BGOBJ_KOMA,
	DA_BGOBJ_UTIMIZU,
	DA_BGOBJ_FURIN,
	DA_MAX
};

class DebugAim :
	public GameObject3D
{
private: 
	int nObjectType;
	Field3D* pDA_Field;
	Wall3D* pDA_Wall;
	C_Item* pDA_Item;
	Spike3D* pDA_Spike;
	Goal3D* pDA_Goal;
	Enemy3D* pDA_Enemy;
	Mirror3D* pDA_Mirror;
	EventBox3D* pDA_EventBox;
	BgObject* pDA_BgObject;
	void* pCurrentGame;
	bool bStaticObject;
	bool bSetStart;
	bool bSetEnd;
	bool bTeleportSet;
	bool bTeleportDestinationSet;
	XMFLOAT3 x3Start;
	XMFLOAT3 x3End;
	float fSpeedMoveable;
	float fDelayBetweenStops;
	Model3D* ModelPosStart;
	Model3D* ModelPosEnd;
	Model3D* ModelPosTeleport;
	XMFLOAT3 TeleportDestination;
	XMFLOAT3 TeleportPosition;
	int EventNum;
public:
	DebugAim();
	~DebugAim();
	void Init();
	void Update();
	void SaveAllControl();
	void MoveableObjectPositionControl();
	void SwitchObjectTypeControl();
	void ScaleControl(float fSpeed);
	void Draw();
	void Uninit();
	int GetCurrentType();
	bool IsStaticMode();
	int GetMoveSpeedInt();
	int GetDelayObj();
	int GetNumEvent();
	bool IsEventSet();
};

