#pragma once
#include "GameObject3D.h"
class Spike3D :
	public GameObject3D
{
private:
	Model3D* pSpikeModel;
	int SpikesOnX;
	int SpikesOnY;

public:
	Spike3D();
	~Spike3D();
	void Init();
	void Update();
	void Draw();
	void Uninit();
	void RaiseSpikesX(int rise);
	void RaiseSpikesY(int rise);
};

