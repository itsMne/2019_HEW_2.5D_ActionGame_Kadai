//*****************************************************************************
//C_Item.h
//*****************************************************************************
#ifndef C_ITEM_H
#define C_ITEM_H
#include "GameObject3D.h"
#include "Billboard2D.h"

//*****************************************************************************
//エナム
//*****************************************************************************
enum ItemType
{
	TYPE_SUSHI = 0,
	TYPE_UDON,
	TYPE_ODEN,
	TYPE_DANGO,
	TYPE_TAI,
	MAX_ITEMTYPE
};

//*****************************************************************************
//クラス
//*****************************************************************************
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
	void SetUse(bool);
	int GetItemType();
};

#endif