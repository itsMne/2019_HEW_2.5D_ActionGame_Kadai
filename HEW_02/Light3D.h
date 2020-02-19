//*****************************************************************************
//Light3D.h
//*****************************************************************************
#ifndef LIGHT3D_H
#define LIGHT3D_H
#include "main.h"
#include "FbxModel.h"

//*****************************************************************************
//�N���X
//*****************************************************************************
class Light3D
{
private:
	CFbxLight g_UnivLight;
	bool bLightEnabled;
public:
	Light3D();
	~Light3D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	CFbxLight& GetLight(void);
	void SetLightEnable(bool bEnable);
	bool IsLightEnabled();
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
Light3D* GetMainLight();

#endif