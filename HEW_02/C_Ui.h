#pragma once
#include "Polygon2D.h"
#include "Player3D.h"

#define MSTART_SIZE_X	 	 60		// ���j���[�����̃T�C�Y
#define MSTART_SIZE_Y		320		// ���j���[�����̃T�C�Y
#define MSTART_POS_X		160		// ���j���[�����̕\���ʒu
#define MSTART_POS_Y		  0		// ���j���[�����̕\���ʒu

#define MRANKING_SIZE_X		 60		// ���j���[�����̃T�C�Y
#define MRANKING_SIZE_Y		320		// ���j���[�����̃T�C�Y
#define MRANKING_POS_X		  0		// ���j���[�����̕\���ʒu
#define MRANKING_POS_Y		  0		// ���j���[�����̕\���ʒu

#define MEND_SIZE_X			 60		// ���j���[�����̃T�C�Y
#define MEND_SIZE_Y			320		// ���j���[�����̃T�C�Y
#define MEND_POS_X		   -160		// ���j���[�����̕\���ʒu
#define MEND_POS_Y			  0		// ���j���[�����̕\���ʒu

#define SCORE_SIZE_X		35							// �����̃T�C�Y
#define SCORE_SIZE_Y		36							// �����̃T�C�Y
enum UITYPE
{
	UI_HP00 = 0,
	UI_HP01,
	UI_MP,
	UI_NUMBER,
	UI_SCORE,
	UI_LEVEL_EDITOR,
	UI_LEVEL_EDITOR_MOVEABLEMODE,
	UI_LEVEL_EDITOR_STATICMODE,
	UI_LEVEL_EDITOR_OBJSPEED,
	UI_LEVEL_EDITOR_DELAY,
	UI_TITLE,
	UI_MENU,
	UI_MENUFRAME,
	UI_GAMEOVER,
	UI_CLEAR,
	UI_RANKING,
	UI_SLASH_EFFECT,
	UI_ZOOM_ATTACK,
	UI_DOOR_RIGHT,
	UI_DOOR_LEFT,
	UI_PAUSE_BG,
	UI_MENU_OPTION,
	UI_HIT_EFFECT,
	UI_SAKURALEAF,
	UI_RANK_VISUAL,
	UI_RESULT_SCORE,
	UI_RANKING_SCORE,
	UI_RANKING_TOP,
	UI_OWARI,
	MAXUITYPE,
};
class C_Ui :public Polygon2D
{
private:
	// �����o�ϐ�
	int nType;
	Player3D* pPlayer;
	int nHP, nMaxHP;
	int nMP, nMaxMP;
	int nScore;
	XMFLOAT2 vScorePos;
	XMFLOAT2 vSpeed;
	int nFramesToUseZoom;
	//�t�F�[�h�Ɋւ���
	bool bDoorOpen, bDoorInPos;
	float fAcceleration;
	bool bSelected;
	bool bScalingUP;
	float fScaleOffset;
	bool bIsInUse;
	bool bOwariAnimationisOver;
	int nFrameUse;
	int nCounterOwariOnScreen;
	float fOffsetY;
	int hpDamageCooloff;
public:
	C_Ui();
	C_Ui(const char*, int);
	~C_Ui();
	//�����o�֐�
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Draw(XMFLOAT2* pPos, unsigned uNumber, int nWidth,
		float fSizeX, float fSizeY);
	void SetFramesForZoomUse(int frames);
	bool IsDoorInPosition();
	void SetDoorOpen(bool bset);
	void SetAsSelectedOption(bool sel);
	void SetHitEffectUse();
	bool GetUse();
	void SetUse(bool use);
	void SetFrameUse(int frames);
	void SetRankTop(XMFLOAT2 Pos, int Scorenum);
	bool IsOwariMessageDone();
};

void SetFramesForZoomUse(int frames);
