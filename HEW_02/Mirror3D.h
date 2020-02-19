//*****************************************************************************
//Mirror3D.h
//*****************************************************************************
#ifndef MIRROR3D
#define MIRROR3D
#include "GameObject3D.h"

//*****************************************************************************
//ƒNƒ‰ƒX
//*****************************************************************************
class Mirror3D :
	public GameObject3D
{
private:
	XMFLOAT3 x3DestinationPos;
public:
	Mirror3D();
	Mirror3D(XMFLOAT3 Destination);
	~Mirror3D();
	void Init(XMFLOAT3 Destination);
	void Update();
	void Draw();
	void Uninit();
	void SetDestination(XMFLOAT3 newDest);
	XMFLOAT3 GetDestination();
};
#endif