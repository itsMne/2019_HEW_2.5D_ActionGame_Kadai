#ifndef	MODEL3D
#define MODEL3D
#include "DXWindow3D.h"
#include "Camera3D.h"
#include "FbxModel.h"
#include "Light3D.h"
//#include "Camera3D.h"

enum MyEnum
{
	ENEMY_ONI_MODEL_RED,
	ENEMY_ONI_MODEL_GREEN,
	ENEMY_ONI_MODEL_YELLOW,
	ENEMY_ONI_MODEL_BLUE,
	ENEMY_WARRIOR,
	WALL_MODEL,
	SPIKE_MODEL,
	SAMURAI_MODEL_PATH,
	NINJA_MODEL_PATH,
	GEISHA_MODEL_PATH,
	ENEMY_BOSS_MODEL_PATH,
	MAX_PRELOADED_MODELS,
};

class Model3D
{
private:
	CFbxModel*		g_pModel;		// FBXモデルへのポインタ
	XMFLOAT3		Position;		// 現在の位置
	XMFLOAT3		Rotation;		// 現在の向き
	XMFLOAT3		Scale;		// 現在の向き
	XMFLOAT4X4	g_mtxWorld;		// ワールドマトリックス
	bool bIsMoving = false;
	Camera3D* pMainCamera = nullptr;
	float fFrame;
	int nFramCount;
	float fAnimSpeed;
	int AnimationFrameSlowness;
	void* Parent;
	int nCountLoop;
	bool bCanLoop;
	bool AnimationFrame;
	int nCurrentAnimation;
	int nCurrentMaxFrame;
	int nCurrentInitialFrame;
	bool bPreLoadedModel;
public:
	Model3D();
	~Model3D();
	HRESULT InitModel(const char*ModelPath, void* Parent);
	HRESULT InitModel(int ModelPath, void* Parent);
	void SetLight(Light3D * pLight);
	void UninitModel(void);
	void UpdateModel(void);
	void SwitchAnimation(int nNewAnimNum);
	void DrawModel(void);
	void AnimationControl();
	XMFLOAT3 GetPosition();
	XMFLOAT3 GetScale();
	XMFLOAT4X4 *GetModelWorld();
	XMFLOAT3 GetRotation();
	void SetRotationY(float y);
	int GetMaxNumberofAnimations();
	void SwitchAnimationSlowness(int nFrameSpeed);
	void SwitchAnimationSpeed(float nFrameSpeed);
	void SetScale(float newScale);
	void SetScale(XMFLOAT3 newScale);
	void ReduceScaleOnX(float red);
	void ReduceScaleOnY(float red);
	void ReduceScaleOnZ(float red);
	void SetPosition(XMFLOAT3 newPos);
	void SetPositionX(float newPos);
	void SetPositionY(float newPos);
	void SetPositionZ(float newPos);
	void SetRotation(XMFLOAT3 rot);
	void RotateAroundY(float);
	int GetAnimation();
	int GetEndFrameOfCurrentAnimation();
	int GetCurrentFrame();
	int GetLoops();
	void SetCanLoop(bool);
	void SetFrame(int frame);
};
#endif
