 #pragma once
#include "SceneBase.h"
#include "Sphere3D.h"
#include "C_Ui.h"
#define MAX_HIT_EFFECT 10
class SceneGame :
	public SceneBase
{
private:
	int				nScore;
	ID3D11Device*	g_pDevice;
	Camera3D*		SceneCamera;
	Light3D*		SceneLight;
	Player3D*		pPlayer;
	Go_List*		Fields;
	Go_List*		Walls;
	Go_List*		Items;
	Go_List*		Spikes;
	Go_List*		Goals;
	Go_List*		Mirrors;
	Go_List*		Enemies;
	Sphere3D*		SkySphere;
	DXWindow3D*		MainWindow;
	
	//UIÇÃïœêî
	C_Ui*			pHP_UI_BACK;
	C_Ui*			pHP_UI_FRONT;
	C_Ui*			pMP_UI;
	C_Ui*			pScore_UI;
	C_Ui*			pScore_Frame_UI;
	C_Ui*			pLevel_Editor_UI;
	C_Ui*			pLevel_Editor_MOVEMODE_UI;
	C_Ui*			pLevel_Editor_STATICMODE_UI;
	C_Ui*			pSpeed_MoveObject_UI;
	C_Ui*			pDelay_MoveObject_UI;
	C_Ui*			pZoomAttack_UI;
	//É|Å[ÉXÇÃUI
	C_Ui*			pBG_UI;
	C_Ui*			pContinue_UI;
	C_Ui*			pGiveUp_UI;
	C_Ui*			HitEffect_UI[MAX_HIT_EFFECT];
	float			fPauseOptionsAcceleration;
	int				nCurrentPauseSelection;
	bool			bGameIsPaused;
	float fZoomPause;
	float fZoomSpeed;
	int nFramesForZoomPausing;
	float fCurrentInGameZoom;
	Hitbox3D hbPreviousRenderingBox;
	bool bGoalReached;
	bool bPauseZooming;
	float fZoomAcc;
	int nTimeSeconds;
	int nFrameCounter;
	int nPauseFrames;
	bool bPauseFramesWhenZoom;
	bool bZoomBack;
	bool bCancelZoom;
public:
	
	SceneGame();
	~SceneGame();
	void Init();
	void Uninit();
	int Update();
	void Draw();
	Go_List* GetFields();
	Go_List* GetWalls();
	Go_List* GetItems();
	Go_List* GetSpikes();
	Go_List* GetGoals();
	Go_List* GetMirrors();
	Go_List* GetEnemies();
	int GetScore();
	void RaiseScore(int rais);
	void SetGoalReached();
	Sphere3D* GetSkySphere();
	void ZoomPause(float fDistance, int nFrames, float Speed, bool PauseFramesWhenZoom, bool ZoomBack);
	void CancelZoom();
	void SetPauseFrames(int nPauseF);
	bool IsGamePaused();
	void SetHitEffect();
};
SceneGame* GetCurrentGame();
