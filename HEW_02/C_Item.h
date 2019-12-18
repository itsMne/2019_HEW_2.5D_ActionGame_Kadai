#pragma once
#include "GameObject3D.h"
#include "Billboard2D.h"
class C_Item :
	public GameObject3D
{
private:
	Billboard2D* bbItem;
	bool bUse;
public:
	C_Item();
	~C_Item();
	void Init();
	void Update();
	void Draw();
	void Uninit();
};

