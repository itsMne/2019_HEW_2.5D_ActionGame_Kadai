#pragma once
#include "GameObject3D.h"
#include "Field3D.h"
#include "Wall3D.h"
#include "C_Item.h"
#include "Spike3D.h"
class DebugAim :
	public GameObject3D
{
private: 
	int nObjectType;
	Field3D* pDA_Field;
	Wall3D* pDA_Wall;
	C_Item* pDA_Item;
	Spike3D* pDA_Spike;
	void* pCurrentGame;
public:
	DebugAim();
	~DebugAim();
	void Init();
	void Update();
	void SwitchObjectTypeControl();
	void ScaleControl(float fSpeed);
	void Draw();
	void Uninit();
};

