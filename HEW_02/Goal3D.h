#pragma once
#include "GameObject3D.h"
class Goal3D :
	public GameObject3D
{
public:
	Goal3D();
	~Goal3D();
	void Init();
	void Update();
	void Draw();
	void Uninit();
};

