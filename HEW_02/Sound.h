//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "DXWindow3D.h"

#include <xaudio2.h>
// Windows7の場合は上記を削除して以下に置き換え.
//#include <C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\xaudio2.h>

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM_TITLE = 0,		// BGM0
	SOUND_LABEL_BGM_MENU,		// BGM0
	SOUND_LABEL_SE_SHINE,
	SOUND_LABEL_SE_BIGSLASH,
	SOUND_LABEL_SE_SWING,
	SOUND_LABEL_SE_GAMESTART,
	SOUND_LABEL_SE_GAMESTART2,
	SOUND_LABEL_SE_GAMESTART3,
	SOUND_LABEL_SE_RANKING_START,
	SOUND_LABEL_SE_MENUOPTIONHIDDEN,
	SOUND_LABEL_SE_MENUOPTIONSHOWN,
	SOUND_LABEL_SE_MENUMOVE,
	SOUND_LABEL_SE_PAUSE,
	SOUND_LABEL_SE_KO,
	SOUND_LABEL_SE_DISSAPPEARED,
	SOUND_LABEL_SE_SWING2,
	SOUND_LABEL_SE_SWING3,
	SOUND_LABEL_SE_NINJA_REGULARSLASH,
	SOUND_LABEL_SE_BIGSLASH2,
	SOUND_LABEL_SE_BIGSLASH3,
	SOUND_LABEL_SE_ENEMYATTACK1,
	SOUND_LABEL_SE_BLOCKGEISHA,
	SOUND_LABEL_SE_GEISHADODGED,
	SOUND_LABEL_MAX,				// 登録数の自動設定
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void	UninitSound(void);
HRESULT PlaySoundGame(SOUND_LABEL label);
void	StopSound(SOUND_LABEL label);
void	StopSound(void);
void	SetVolume(float fVol);

#endif
