#pragma once
#include "main.h"
#include "FbxModel.h"
class Light3D
{
private:
	CFbxLight g_UnivLight;
	bool bLightEnabled;
public:
	Light3D();
	~Light3D();
	HRESULT Init(void);
	void End(void);
	void Update(void);
	CFbxLight& GetLight(void);
	void SetLightEnable(bool bEnable);
	bool IsLightEnabled();
};

Light3D* GetMainLight();