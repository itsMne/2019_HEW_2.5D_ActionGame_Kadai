//*****************************************************************************
// Model3D.cpp
// 3Dモデルの管理
//*****************************************************************************
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

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_LOOPS	9000	

//*****************************************************************************
// グローバル変数
//*****************************************************************************
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
	"data/model/BossOni.fbx",

};
CFbxModel* Models[MAX_PRELOADED_MODELS] = {nullptr};

//*****************************************************************************
// コンストラクタ関数
//*****************************************************************************
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
	bPauseAnimation = false;
}


Model3D::~Model3D()
{
	UninitModel();
}

//*****************************************************************************
//InitModel関数
//初期化関数
//引数：const char*, void*
//戻：void
//*****************************************************************************
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
	bPauseAnimation = false;
	return hr;
}

//*****************************************************************************
//InitModel関数
//初期化関数
//引数：int, void*
//戻：void
//*****************************************************************************
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

//*****************************************************************************
//UninitModel関数
//終了関数
//引数：void
//戻：void
//*****************************************************************************
void Model3D::UninitModel(void)
{
	// FBXファイルの解放
	if(!bPreLoadedModel)
		SAFE_DELETE(g_pModel);
}

//*****************************************************************************
//UpdateModel関数
//変更関数
//引数：void
//戻：void
//*****************************************************************************
void Model3D::UpdateModel(void)
{
	//なし
}

//*****************************************************************************
//SwitchAnimation関数
//アニメーションを変える
//引数：int
//戻：void
//*****************************************************************************
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

//*****************************************************************************
//DrawModel関数
//レンダリング関数
//引数：void
//戻：void
//*****************************************************************************
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

//*****************************************************************************
//AnimationControl関数
//アニメーションを管理する
//引数：void
//戻：void
//*****************************************************************************
void Model3D::AnimationControl()
{
	SceneGame* pGame = GetCurrentGame();
	if (pGame->IsUsingPauseFrames() || pGame->IsGamePaused() || bPauseAnimation) {
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

//*****************************************************************************
//GetPosition関数
//拠点を戻すする
//引数：void
//戻：XMFLOAT3
//*****************************************************************************
XMFLOAT3 Model3D::GetPosition()
{
	return Position;
}

//*****************************************************************************
//GetScale関数
//大きさを戻すする
//引数：void
//戻：XMFLOAT3
//*****************************************************************************
XMFLOAT3 Model3D::GetScale()
{
	return Scale;
}

//*****************************************************************************
//GetModelWorld関数
//ワールドマトリックスのアドレスを戻すする
//引数：void
//戻：XMFLOAT4X4*
//*****************************************************************************
XMFLOAT4X4 * Model3D::GetModelWorld()
{
	return &g_mtxWorld;
}

//*****************************************************************************
//GetRotation関数
//回転を戻すする
//引数：void
//戻：XMFLOAT4X4*
//*****************************************************************************
XMFLOAT3 Model3D::GetRotation()
{
	return Rotation;
}

//*****************************************************************************
//SetRotationY関数
//回転を設定する（Y座標）
//引数：float
//戻：void
//*****************************************************************************
void Model3D::SetRotationY(float y)
{
	Rotation.y = y;
}

//*****************************************************************************
//GetMaxNumberofAnimations関数
//アニメーションの数を戻す
//引数：void
//戻：int
//*****************************************************************************
int Model3D::GetMaxNumberofAnimations()
{
	if (g_pModel)
		return g_pModel->GetMaxNumberOfAnimations();
	return 0;
}

//*****************************************************************************
//SwitchAnimationSlowness関数
//アニメーションの速さを設定する
//引数：int
//戻：void
//*****************************************************************************
void Model3D::SwitchAnimationSlowness(int nFrameSpeed)
{
	AnimationFrameSlowness = nFrameSpeed;
}

//*****************************************************************************
//SwitchAnimationSpeed関数
//アニメーションの速さを設定する
//引数：float
//戻：void
//*****************************************************************************
void Model3D::SwitchAnimationSpeed(float fFrameSpeed)
{
	fAnimSpeed = fFrameSpeed;
}

//*****************************************************************************
//SetScale関数
//大きさを設定する
//引数：float
//戻：void
//*****************************************************************************
void Model3D::SetScale(float newScale)
{
	Scale = { newScale ,newScale ,newScale };
}

//*****************************************************************************
//SetScale関数
//大きさを設定する
//引数：XMFLOAT3
//戻：void
//*****************************************************************************
void Model3D::SetScale(XMFLOAT3 newScale)
{
	Scale = newScale;
}

//*****************************************************************************
//ReduceScaleOnX関数
//大きさを減らす（X座標）
//引数：float
//戻：void
//*****************************************************************************
void Model3D::ReduceScaleOnX(float red)
{
	Scale.x -= red;
}

//*****************************************************************************
//ReduceScaleOnY関数
//大きさを減らす（Y座標）
//引数：float
//戻：void
//*****************************************************************************
void Model3D::ReduceScaleOnY(float red)
{
	Scale.y -= red;
}

//*****************************************************************************
//ReduceScaleOnZ関数
//大きさを減らす（Z座標）
//引数：float
//戻：void
//*****************************************************************************
void Model3D::ReduceScaleOnZ(float red)
{
	Scale.z -= red;
}

//*****************************************************************************
//SetPosition関数
//拠点を設定する
//引数：XMFLOAT3
//戻：void
//*****************************************************************************
void Model3D::SetPosition(XMFLOAT3 newPos)
{
	Position = newPos;
}

//*****************************************************************************
//SetPositionX関数
//拠点を設定する（X座標）
//引数：float
//戻：void
//*****************************************************************************
void Model3D::SetPositionX(float newPos)
{
	Position.x = newPos;
}

//*****************************************************************************
//SetPositionY関数
//拠点を設定する（Y座標）
//引数：float
//戻：void
//*****************************************************************************
void Model3D::SetPositionY(float newPos)
{
	Position.y = newPos;
}

//*****************************************************************************
//SetPositionZ関数
//拠点を設定する（Z座標）
//引数：float
//戻：void
//*****************************************************************************
void Model3D::SetPositionZ(float newPos)
{
	Position.z = newPos;
}

//*****************************************************************************
//SetRotation関数
//回転を設定する
//引数：XMFLOAT3
//戻：void
//*****************************************************************************
void Model3D::SetRotation(XMFLOAT3 rot)
{
	Rotation = rot;
}

//*****************************************************************************
//RotateAroundY関数
//回転を変える（Y座標）
//引数：float
//戻：void
//*****************************************************************************
void Model3D::RotateAroundY(float y)
{
	Rotation.y += y;
}

//*****************************************************************************
//GetAnimation関数
//現在のアニメーションを戻す
//引数：void
//戻：int
//*****************************************************************************
int Model3D::GetAnimation()
{
	return nCurrentAnimation;
}

//*****************************************************************************
//GetEndFrameOfCurrentAnimation関数
//アニメーションの最後のフレームを戻す
//引数：void
//戻：int
//*****************************************************************************
int Model3D::GetEndFrameOfCurrentAnimation()
{
	if (g_pModel)
		return nCurrentMaxFrame;
	return 0;
}

//*****************************************************************************
//GetCurrentFrame関数
//アニメーションの現在のフレームを戻す
//引数：void
//戻：int
//*****************************************************************************
int Model3D::GetCurrentFrame()
{
	if (g_pModel)
		return (int)fFrame;
	return 0;
}

//*****************************************************************************
//GetLoops関数
//アニメーションのループを戻す
//引数：void
//戻：int
//*****************************************************************************
int Model3D::GetLoops()
{
	return nCountLoop;
}

//*****************************************************************************
//SetCanLoop関数
//アニメーションがループが出来ることを設定する
//引数：bool
//戻：int
//*****************************************************************************
void Model3D::SetCanLoop(bool loop)
{
	bCanLoop = loop;
}

//*****************************************************************************
//SetFrame関数
//アニメーションのフレームを設定する
//引数：int
//戻：void
//*****************************************************************************
void Model3D::SetFrame(int frame)
{
	fFrame = frame;
}

//*****************************************************************************
//PauseModelAnimation関数
//アニメーションをポースする
//引数：bool
//戻：void
//*****************************************************************************
void Model3D::PauseModelAnimation(bool pause)
{
	bPauseAnimation = pause;
}
