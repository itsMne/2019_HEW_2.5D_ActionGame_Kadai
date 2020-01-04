#pragma once
#include "main.h"
#include "UniversalStructures.h"
#include "Cube3D.h"
#include "Light3D.h"
#include "GameObject3D.h"
class Field3D: public GameObject3D
{
private:
	VERTEX_3D					g_vertexWk[NUM_VERTEX];	// 頂点情報格納ワーク
	int nTexture;
	ID3D11Buffer*				g_pConstantBuffer[2];	// 定数バッファ
	ID3D11Buffer*				g_pVertexBuffer;		// 頂点バッファ
	ID3D11SamplerState*			g_pSamplerState;		// テクスチャ サンプラ
	ID3D11VertexShader*			g_pVertexShader;		// 頂点シェーダ
	ID3D11InputLayout*			g_pInputLayout;			// 頂点フォーマット
	ID3D11PixelShader*			g_pPixelShader;			// ピクセルシェーダ

	XMFLOAT4X4					g_mtxWorldField;		// ワールドマトリックス
	XMFLOAT4X4					g_mtxTexture;			// テクスチャマトリックス
	//XMFLOAT3					Position;				// 現在の位置
	//XMFLOAT3					Rotation;				// 現在の向き
	//XMFLOAT3					Scale;				// 現在の向き

	// マテリアル
	XMFLOAT4						g_Ka;		// アンビエント
	XMFLOAT4						g_Kd;		// ディフューズ
	XMFLOAT4						g_Ks;		// スペキュラ
	float						g_fPower;	// スペキュラ強度
	XMFLOAT4						g_Ke;		// エミッシブ
	Light3D*					pSceneLight;
	Cube3D* pVisualHitbox;
	char szTexturePath[256];
public:
	Field3D(const char* TexturePath);
	~Field3D();
	HRESULT Init(const char* TexturePath);
	void Uninit(void);
	void UninitTextures(void);
	void Update(void);
	void Draw(void);
	void SetFieldLight(Light3D* SceneLight);
	HRESULT MakeVertexField(ID3D11Device* pDevice);
	void SetPosition(XMFLOAT3 newPos);
	//Hitbox3D GetHitBox();
	void SetScaleWithHitbox(float newScale);
	void SetScaleWithHitbox(XMFLOAT3 newScale);
	void SetRotation(XMFLOAT3 newRot);
	char* GetTexturePath();
};

