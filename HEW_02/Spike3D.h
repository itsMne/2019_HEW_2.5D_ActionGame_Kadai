//*****************************************************************************
//Spike3D.h
//*****************************************************************************
#ifndef SPIKE3D_H
#define SPIKE3D_H
#include "GameObject3D.h"

//*****************************************************************************
//ƒNƒ‰ƒX
//*****************************************************************************
class Spike3D :
	public GameObject3D
{
private:
	Model3D* pSpikeModel;
	int nSpikesOnX;
	int nSpikesOnY;
	bool bInvisible;
public:
	Spike3D();
	~Spike3D();
	void Init();
	void Update();
	void Draw();
	void Uninit();
	void RaiseSpikesX(int rise);
	void RaiseSpikesY(int rise);
	void SetSpikesNum(int x, int y);
	void SetInvisibility(bool invisible);
	XMFLOAT2 GetSpikesNum();
};

#endif