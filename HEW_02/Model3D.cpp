#include "Model3D.h"
#include "main.h"
#include "FbxModel.h"
#include "input.h"
#include "Camera3D.h"
#include "Light3D.h"
#include "GameObject3D.h"
#include "UniversalStructures.h"
#include "debugproc.h"
#include "SceneGame.h"

#define MAX_LOOPS	9000	
char ModelPaths[MAX_PRELOADED_MODELS][256] =
{
	"data/model/RedOni.fbx"	  ,
	"data/model/GreenOni.fbx" ,
	"data/model/YellowOni.fbx",
	"data/model/BlueOni.fbx"  ,
	"data/model/Warrior.fbx"  ,
	"data/model/Cube.fbx",
	"data/model/Spike.fbx",
	"data/model/SamuraiHew.fbx",
	"data/model/NinjaHew.fbx",
	"data/model/GeishaHew.fbx",

};

CFbxModel* Models[MAX_PRELOADED_MODELS] = {nullptr};
Model3D::Model3D()
{
	nFramCount = 0;
	nCountLoop = 0;
	AnimationFrameSlowness = 0;
	Parent = nullptr;
	fAnimSpeed = 2;
	bCanLoop = true;
	AnimationFrame = false;
	g_pModel = nullptr;
}


Model3D::~Model3D()
{
	UninitModel();
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Model3D::InitModel(const char* ModelPath, void* pParent)
{
	HRESULT hr = S_OK;
	nCurrentAnimation = 0;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	pMainCamera = GetMainCamera();
	Parent = pParent;
	// 位置、向きの初期設定
	Position = XMFLOAT3(0.0f, 10.0f, 0.0f);
	Rotation = XMFLOAT3(0.0f, 90.0f, 0.0f);
	Scale = XMFLOAT3(0.1, 0.1, 0.1);
	// FBXファイルの読み込み
	g_pModel = new CFbxModel();
	Light3D* pLight = GetMainLight();
	SwitchAnimation(0);
	//fFrame = nCurrentInitialFrame = g_pModel->GetInitialAnimFrame();
	//nCurrentMaxFrame = g_pModel->GetMaxAnimFrame();
	if (!pMainCamera)
	{
		printf("メインカメラがありません\n");
		return S_OK;
	}
	hr = g_pModel->Init(pDevice, pDeviceContext, ModelPath);
	if (SUCCEEDED(hr)) {
		g_pModel->SetCamera(pMainCamera->GetCameraPos());
		SetLight(pLight);
	}
	bPreLoadedModel = false;
	return hr;
}

HRESULT Model3D::InitModel(int ModelPath, void * pParent)
{
	if (ModelPath > MAX_PRELOADED_MODELS || ModelPath < 0)
		return S_OK;
	HRESULT hr = S_OK;
	nCurrentAnimation = 0;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	pMainCamera = GetMainCamera();
	if (!pMainCamera)
	{
		printf("メインカメラがありません\n");
		return S_OK;
	}
	Parent = pParent;
	// 位置、向きの初期設定
	Position = XMFLOAT3(0.0f, 10.0f, 0.0f);
	Rotation = XMFLOAT3(0.0f, 90.0f, 0.0f);
	Scale = XMFLOAT3(0.1, 0.1, 0.1);
	// FBXファイルの読み込み
	if (!Models[ModelPath]) {
		Models[ModelPath] = new CFbxModel();
		Light3D* pLight = GetMainLight();
		SwitchAnimation(0);
		Models[ModelPath]->SetCamera(pMainCamera->GetCameraPos());
		Models[ModelPath]->Init(pDevice, pDeviceContext, ModelPaths[ModelPath]);
		g_pModel = Models[ModelPath];
		SetLight(pLight);
	}
	else {
		g_pModel = Models[ModelPath];
	}
	bPreLoadedModel = true;
	return S_OK;
}

void Model3D::SetLight(Light3D * pLight)
{
	if(g_pModel)
		g_pModel->SetLight(pLight->GetLight());
}


//=============================================================================
// 終了処理
//=============================================================================
void Model3D::UninitModel(void)
{
	// FBXファイルの解放
	if(!bPreLoadedModel)
		SAFE_DELETE(g_pModel);
}

//=============================================================================
// 更新処理
//=============================================================================
void Model3D::UpdateModel(void)
{
	//AnimationFrame = true;
}

void Model3D::SwitchAnimation(int nNewAnimNum)
{
	if (!g_pModel)
		return;
	g_pModel->SetAnimStack(nCurrentAnimation);
	if (nCurrentAnimation == nNewAnimNum)
		return;
	nCurrentAnimation = nNewAnimNum;
	g_pModel->SetAnimStack(nNewAnimNum);
	fFrame = nCurrentInitialFrame = g_pModel->GetInitialAnimFrame();
	nCurrentMaxFrame = g_pModel->GetMaxAnimFrame();
	nFramCount = nCountLoop = 0;
}

//=============================================================================
// 描画処理
//=============================================================================
void Model3D::DrawModel(void)
{
	if (!pMainCamera)
	{
		printf("メインカメラがありません\n");
		return;
	}

	GameObject3D* goParent = (GameObject3D*)Parent;//親のポインターを使う
	XMFLOAT3 ParentPos = {0,0,0};
	XMFLOAT3 ParentRot = {0,0,0};
	XMFLOAT3 ParentScale = {1,1,1};//親の拠点と親の大きさ
	if (goParent) {
		ParentPos = goParent->GetPosition();
		ParentScale = goParent->GetScale();
		ParentRot = goParent->GetRotation();
	}
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
	XMMATRIX mtxWorld, mtxRot, mtxTranslate, mtxScale;

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();
	XMStoreFloat4x4(&this->g_mtxWorld, mtxWorld);
	//サイズ
	mtxScale = XMMatrixScaling(Scale.x * ParentScale.x, Scale.y * ParentScale.y, Scale.z * ParentScale.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScale);

	// 回転を反映
	mtxRot = XMMatrixRotationRollPitchYaw(Rotation.x + ParentRot.x, Rotation.y + ParentRot.y, Rotation.z + ParentRot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);



	// 移動を反映
	mtxTranslate = XMMatrixTranslation(Position.x + ParentPos.x, Position.y + ParentPos.y, Position.z + ParentPos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);



	// ワールドマトリックスの設定
	XMStoreFloat4x4(&g_mtxWorld, mtxWorld);

	SwitchAnimation(nCurrentAnimation);
	AnimationControl();
	SetZWrite(true);
	g_pModel->Render(g_mtxWorld, pMainCamera->GetViewMatrix(), pMainCamera->GetProjMatrix(), eOpacityOnly);
	SetZWrite(false);
	g_pModel->Render(g_mtxWorld, pMainCamera->GetViewMatrix(), pMainCamera->GetProjMatrix(), eTransparentOnly);
}

void Model3D::AnimationControl()
{
	SceneGame* pGame = GetCurrentGame();
	if (pGame->IsGamePaused()) {
		g_pModel->SetAnimFrame((int)fFrame);
		return;
	}
	if (++nFramCount >= AnimationFrameSlowness) {
		nFramCount = 0;
		//printf("%f\n",fAnimSpeed);
		fFrame += fAnimSpeed;
		if (fFrame >= nCurrentMaxFrame) {
			if (bCanLoop) {
				if (++nCountLoop > MAX_LOOPS)
					nCountLoop = MAX_LOOPS;
				fFrame = nCurrentInitialFrame;
			}
			else {
				fFrame--;
			}
		}
			g_pModel->SetAnimFrame((int)fFrame);
	}
}

XMFLOAT3 Model3D::GetPosition()
{
	return Position;
}

XMFLOAT3 Model3D::GetScale()
{
	return Scale;
}

XMFLOAT4X4 * Model3D::GetModelWorld()
{
	return &g_mtxWorld;
}

XMFLOAT3 Model3D::GetRotation()
{
	return Rotation;
}

void Model3D::SetRotationY(float y)
{
	Rotation.y = y;
}

int Model3D::GetMaxNumberofAnimations()
{
	if (g_pModel)
		return g_pModel->GetMaxNumberOfAnimations();
	return 0;
}

void Model3D::SwitchAnimationSlowness(int nFrameSpeed)
{
	AnimationFrameSlowness = nFrameSpeed;
}

void Model3D::SwitchAnimationSpeed(float fFrameSpeed)
{
	fAnimSpeed = fFrameSpeed;
}

void Model3D::SetScale(float newScale)
{
	Scale = { newScale ,newScale ,newScale };
}

void Model3D::SetScale(XMFLOAT3 newScale)
{
	Scale = newScale;
}

void Model3D::ReduceScaleOnX(float red)
{
	Scale.x -= red;
}

void Model3D::ReduceScaleOnY(float red)
{
	Scale.y -= red;
}

void Model3D::ReduceScaleOnZ(float red)
{
	Scale.z -= red;
}

void Model3D::SetPosition(XMFLOAT3 newPos)
{
	Position = newPos;
}

void Model3D::SetPositionX(float newPos)
{
	Position.x = newPos;
}

void Model3D::SetPositionY(float newPos)
{
	Position.y = newPos;
}

void Model3D::SetPositionZ(float newPos)
{
	Position.z = newPos;
}

void Model3D::SetRotation(XMFLOAT3 rot)
{
	Rotation = rot;
}

void Model3D::RotateAroundY(float y)
{
	Rotation.y += y;
	/*if (Rotation.y < -XM_PI) {
		Rotation.y += XM_2PI;
	}*/
}

int Model3D::GetAnimation()
{
	return nCurrentAnimation;
}

int Model3D::GetEndFrameOfCurrentAnimation()
{
	if (g_pModel)
		return nCurrentMaxFrame;
	return 0;
}

int Model3D::GetCurrentFrame()
{
	if (g_pModel)
		return (int)fFrame;
	return 0;
}

int Model3D::GetLoops()
{
	return nCountLoop;
}

void Model3D::SetCanLoop(bool loop)
{
	bCanLoop = loop;
}

void Model3D::SetFrame(int frame)
{
	fFrame = frame;
}
