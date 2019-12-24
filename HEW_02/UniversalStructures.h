#pragma once
#include <windows.h>
#include <tchar.h>
#include <mmsystem.h>
#include <d3d11.h>
#include <DirectXMath.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define HITBOX_Z_POS -10
#define GRAVITY_FORCE 0.4f
#define SHOW_HITBOX	false
#define USE_CONSOLE true
#define DEBUG_MODE true
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) {if(x){(x)->Release();x=nullptr;}}
#endif
#ifndef SAFE_DELETE
#define SAFE_DELETE(x) {if(x){delete(x);x=nullptr;}}
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) {if(x){delete[](x);x=nullptr;}}
#endif

#define SCREEN_WIDTH	(1280)				// �E�C���h�E�̕�
#define SCREEN_HEIGHT	(720)				// �E�C���h�E�̍���
#define SCREEN_CENTER_X	(SCREEN_WIDTH/2)	// �E�C���h�E�̒��S�w���W
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT/2)	// �E�C���h�E�̒��S�x���W
#define NUM_VERTEX 4

#define CULLMODE_NONE	0					// �J�����O���Ȃ�
#define CULLMODE_CW		1					// �O�ʃJ�����O
#define CULLMODE_CCW	2					// �w�ʃJ�����O
using namespace DirectX;
//*****************************************************************************
// �\���̒�`
//*****************************************************************************
// ���_�t�H�[�}�b�g( ���_���W[2D] / ���ˌ� / �e�N�X�`�����W )
typedef struct {
	XMFLOAT3 vtx;		// ���_���W
	XMFLOAT4 diffuse;	// �g�U���ˌ�
	XMFLOAT2 tex;		// �e�N�X�`�����W
} VERTEX_2D;

// ���_�t�H�[�}�b�g( ���_���W[3D] / �@���x�N�g�� / ���ˌ� / �e�N�X�`�����W )
typedef struct {
	XMFLOAT3 vtx;		// ���_���W
	XMFLOAT3 nor;		// �@���x�N�g��
	XMFLOAT4 diffuse;	// �g�U���ˌ�
	XMFLOAT2 tex;		// �e�N�X�`�����W
} VERTEX_3D;

typedef struct 
{
	float x, y;//���_
	float w;//���� 
	float h;//����
}Hitbox2D;
enum GENERAL_DIRECTION
{
	LEFT_DIR=-1,
	RIGHT_DIR=1
};
typedef struct Hitbox3D
{
	float x = 0;
	float y = 0;
	float z = 0;
	float SizeX = 0, SizeY = 0, SizeZ = 0;
};
typedef struct UV
{
	float U, V;
};

bool IsInCollision3D(Hitbox3D a, Hitbox3D b);
bool IsInCollision2D(Hitbox2D hbA, Hitbox2D hbB);
bool IsVectorZero(XMFLOAT3 vec);
bool CompVector(XMFLOAT3 a, XMFLOAT3 b);
