//*****************************************************************************
//debugproc.h
//*****************************************************************************
#ifndef DEBUGPROC_H
#define DEBUGPROC_H
#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitDebugProc(void);
void UninitDebugProc(void);
void UpdateDebugProc(void);
void DrawDebugProc(void);
void StartDebugProc(bool hiragana = false);
void PrintDebugProc(const char *fmt, ...);
bool SetHiragana(bool hiragana = true);

#endif