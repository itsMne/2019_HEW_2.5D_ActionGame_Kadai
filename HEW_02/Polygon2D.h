//*****************************************************************************
//Polygon2D.h
//*****************************************************************************
#ifndef	 POLYGON_H
#define  POLYGON_H
#include "main.h"
#include "UniversalStructures.h"

//*****************************************************************************
// �N���X
//*****************************************************************************
class Polygon2D
{
private:
	bool bPolygonInited;
protected:
	ID3D11ShaderResourceView*	g_pTexture;				// �e�N�X�`���ւ̃|�C���^

	VERTEX_2D					g_vertexWk[NUM_VERTEX];	// ���_���i�[���[�N

	XMFLOAT3						Position;			// �|���S���̈ړ���
	XMFLOAT3						g_rotPolygon;			// �|���S���̉�]��
	XMFLOAT3						Scale;			// �|���S���̃T�C�Y
	XMFLOAT4						Color;			// �|���S���̒��_�J���[
	bool							g_bInvalidate;			// ���_�f�[�^�X�V�t���O

	XMFLOAT2						g_posTexFrame;			// UV���W
	XMFLOAT2						g_sizTexFrame;			// �e�N�X�`�����o�T�C�Y

	ID3D11Buffer*				g_pConstantBuffer;		// �萔�o�b�t�@
	ID3D11Buffer*				g_pVertexBuffer;		// ���_�o�b�t�@
	ID3D11SamplerState*			g_pSamplerState;		// �e�N�X�`�� �T���v��
	ID3D11VertexShader*			g_pVertexShader;		// ���_�V�F�[�_
	ID3D11InputLayout*			g_pInputLayout;			// ���_�t�H�[�}�b�g
	ID3D11PixelShader*			g_pPixelShader;			// �s�N�Z���V�F�[�_

	XMFLOAT4X4					g_mProj;				// �ˉe�ϊ��s��
	XMFLOAT4X4					g_mView;				// �r���[�ϊ��s��
	XMFLOAT4X4					g_mWorld;				// ���[���h�ϊ��s��
	XMFLOAT4X4					g_mTex;					// �e�N�X�`���ϊ��s��
	UV uv;
	int nFrameCounter;

public:
	Polygon2D();
	Polygon2D(const char * TexturePath);
	virtual ~Polygon2D();
	HRESULT InitPolygon(ID3D11Device* pDevice);

	void UninitPolygon();
	void UpdatePolygon();
	void DrawPolygon(ID3D11DeviceContext* pDeviceContext);
	void SetPolygonTexture(ID3D11ShaderResourceView* pTexture);
	void SetPolygonPos(float fX, float fY);
	void SetPolygonSize(float fScaleX, float fScaleY);
	void SetPolygonAngle(float fAngle);
	void SetPolygonUV(float fU, float fV);
	void SetPolygonFrameSize(float fWidth, float fHeight);
	void SetPolygonColor(float fRed, float fGreen, float fBlue);
	void SetPolygonAlpha(float fAlpha);
	void ReduceAlpha(float fAlpha);
	void RotateAroundY(float rot);
	void SetRotationY(float rot);
	float GetRotationY();
	float GetAlpha();
	UV GetUV();
	HRESULT MakeVertexPolygon(ID3D11Device* pDevice);
	void SetVertexPolygon(void);
};
#endif