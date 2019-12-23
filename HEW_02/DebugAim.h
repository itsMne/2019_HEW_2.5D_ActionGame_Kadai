#pragma once
#include "GameObject3D.h"
#include "Field3D.h"
#include "Wall3D.h"
#include "C_Item.h"
#include "Goal3D.h"
#include "Spike3D.h"
#include "Mirror3D.h"

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
	Mirror3D* pDA_Mirror;
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
};

