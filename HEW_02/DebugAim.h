#pragma once
#include "GameObject3D.h"
#include "Field3D.h"
class DebugAim :
	public GameObject3D
{
private: 
	int nObjectType;
	Field3D* pDA_Field;
public:
	DebugAim();
	~DebugAim();
	void Init();
	void Update();
	void SwitchObjectTypeControl();
	void ScaleControl(int fSpeed);
	void Draw();
	void Uninit();
};

