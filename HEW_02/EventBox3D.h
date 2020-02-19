//*****************************************************************************
// EventBox3D.h
//*****************************************************************************
#pragma once

#include "GameObject3D.h"

//*****************************************************************************
// エナム
//*****************************************************************************
enum eEVENT
{
	MOVE_TUTORIAL=0,
	JUMP_TUTORIAL,
	MOVE2_TUTORIAL,
	GEISHA_TUTORIAL,
	ATTACK_TUTORIAL,
	SAMURAI_TUTORIAL,
	GEISHA2_TUTORIAL,
	MAX_EVENTS
};

//*****************************************************************************
// クラス
//*****************************************************************************
class EventBox3D :
	public GameObject3D
{
private:
	int nEvent;
public:
	EventBox3D(int);
	~EventBox3D();
	void Init();
	void Update();
	void Draw();
	void Uninit();
	int GetEvenType();
	void SetScaleWithHitbox(XMFLOAT3 newScale);
};

