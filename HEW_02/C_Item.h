#pragma once
#include "GameObject3D.h"
#include "Billboard2D.h"
enum ItemType
{
	TYPE_SUSHI = 0,
	TYPE_UDON,
	TYPE_ODEN,
	TYPE_DANGO,
	TYPE_TAI,
	MAX_ITEMTYPE
};
class C_Item :
	public GameObject3D
{
private:
	Billboard2D* bbItem;
	bool bUse;
	int nItemType;
public:
	C_Item(int);
	~C_Item();
	void Init();
	void Update();
	void Draw();
	void Uninit();
};

