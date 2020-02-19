//*****************************************************************************
//Light3D.h
//*****************************************************************************
#ifndef LIGHT3D_H
#define LIGHT3D_H
#include "main.h"
#include "FbxModel.h"

//*****************************************************************************
//クラス
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
// プロトタイプ宣言
//*****************************************************************************
Light3D* GetMainLight();

#endif