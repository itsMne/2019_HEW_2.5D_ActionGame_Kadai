#include "C_Ui.h"
#include "Player3D.h"
#include "SceneGame.h"
#include "input.h"

// マクロ定義
#define HP_SIZE_X 300
#define HP_SIZE_Y 100
#define HP_POS_X (-SCREEN_WIDTH / 2 + 150)
#define HP_POS_Y (SCREEN_HEIGHT / 2 - 50)

#define MP_SIZE_X 300
#define MP_SIZE_Y 100
#define MP_POS_X (-SCREEN_WIDTH / 2 + 150)
#define MP_POS_Y (SCREEN_HEIGHT / 2 - 150)

#define NUMBER_FRAME_X		10	// 横フレーム数
#define NUMBER_FRAME_Y		1	// 縦フレーム数

#define FRAME_SIZE_X		340							// 枠のサイズ
#define FRAME_SIZE_Y		80							// 枠のサイズ
#define FRAME_POS_X			(SCREEN_WIDTH/2-FRAME_SIZE_X/2)	// 枠の表示位置
#define FRAME_POS_Y			(SCREEN_HEIGHT/2-FRAME_SIZE_Y/2)	// 枠の表示位置

#define SCORE_WIDTH			8							// 表示桁数

#define SCORE_SIZE_X		35							// 文字のサイズ
#define SCORE_SIZE_Y		36							// 文字のサイズ
#define SCORE_POS_X			(FRAME_POS_X-SCORE_SIZE_X*(SCORE_WIDTH/2))+13	// 文字の表示位置
#define SCORE_POS_Y			(FRAME_POS_Y+SCORE_SIZE_Y/2-8)+15	// 文字の表示位置

C_Ui* pZoomEffect = nullptr;

C_Ui::C_Ui()
{
}

C_Ui::C_Ui(const char *Path, int Type) :Polygon2D(Path)
{
	nType = Type;
	pPlayer = NULL;
	nHP = nMaxHP = 0;
	nMP = nMaxMP = 0;
	nScore = 0;
	nFramesToUseZoom = 0;
	vScorePos = XMFLOAT2(SCORE_POS_X, SCORE_POS_Y);
	switch (Type)
	{
	case UI_HP00:
		SetPolygonSize(HP_SIZE_X, HP_SIZE_Y);
		SetPolygonPos(HP_POS_X, HP_POS_Y);
		break;
	case UI_HP01:
		SetPolygonSize(HP_SIZE_X, HP_SIZE_Y);
		SetPolygonPos(HP_POS_X, HP_POS_Y);
		break;
	case UI_MP:
		SetPolygonSize(MP_SIZE_X, MP_SIZE_Y);
		SetPolygonPos(MP_POS_X, MP_POS_Y);
		break;
	case UI_NUMBER:
		break;
	case UI_SCORE:
		SetPolygonSize(FRAME_SIZE_X, FRAME_SIZE_Y);
		SetPolygonPos(FRAME_POS_X, FRAME_POS_Y);
		break;
	case UI_LEVEL_EDITOR:
		SetPolygonSize(SCREEN_WIDTH, SCREEN_HEIGHT);
		SetPolygonPos(0, 0);
		break;
	case UI_LEVEL_EDITOR_MOVEABLEMODE:
		SetPolygonSize(390, 124);
		SetPolygonPos(0, -SCREEN_HEIGHT / 2 + 80);
		break;
	case UI_LEVEL_EDITOR_STATICMODE:
		SetPolygonSize(289, 34);
		SetPolygonPos(0, -SCREEN_HEIGHT / 2 + 34);
		break;
	case UI_LEVEL_EDITOR_OBJSPEED:
		vScorePos = { -30,-222 };
		break;	
	case UI_LEVEL_EDITOR_DELAY:
		vScorePos = { -30,-220-45 };
		break;
	case UI_TITLE:
		SetPolygonSize(SCREEN_WIDTH, SCREEN_HEIGHT);
		SetPolygonPos(0, 0);
		break;
	case UI_GAMEOVER:
		SetPolygonSize(SCREEN_WIDTH, SCREEN_HEIGHT);
		SetPolygonPos(0, 0);
		break;
	case UI_CLEAR:
		SetPolygonSize(SCREEN_WIDTH, SCREEN_HEIGHT);
		SetPolygonPos(0, 0);
		break;
	case UI_RANKING:
		SetPolygonSize(SCREEN_WIDTH, SCREEN_HEIGHT);
		SetPolygonPos(0, 0);
		break;
	case UI_SLASH_EFFECT:
		SetPolygonSize(SCREEN_WIDTH, SCREEN_HEIGHT);
		uv = { 0,0 };
		SetPolygonFrameSize(1.0f/4.0f, 1.0f/8.0f);
		SetPolygonPos(0, 0);
		break;
	case UI_ZOOM_ATTACK:
		pZoomEffect = this;
		SetPolygonSize(1280, 1280);
		SetPolygonPos(0, 0);
		break;
	}

}


C_Ui::~C_Ui()
{
}

void C_Ui::Init()
{
}

void C_Ui::Uninit()
{
	if (nType == UI_ZOOM_ATTACK)
		pZoomEffect = nullptr;
}

void C_Ui::Update()
{
	//if (GetKeyPress(VK_Z))
	SceneGame* pCurrentGame = GetCurrentGame();
	if (!pCurrentGame && nType!= UI_SLASH_EFFECT)
		return;
	if(pCurrentGame)
		nScore = pCurrentGame->GetScore();

	switch (nType)
	{
	case UI_HP00:
		pPlayer = GetMainPlayer();
		if (!pPlayer)
			return;
		nHP = pPlayer->GetPlayerHp();
		nMaxHP = pPlayer->GetPlayerMaxHp();
		if(nHP>0)
			SetPolygonColor(1, (nHP / (float)nMaxHP), (nHP / (float)nMaxHP));
		else
			SetPolygonColor(0.25f, (nHP / (float)nMaxHP), (nHP / (float)nMaxHP));
		break;
	case UI_HP01:
		pPlayer = GetMainPlayer();
		if (!pPlayer)
			return;
		nHP = pPlayer->GetPlayerHp();
		nMaxHP = pPlayer->GetPlayerMaxHp();
		//printf("hp: %d, max: %d\n", nHP, nMaxHP);
		if (nMaxHP == 0)
		{
			break;
		}
		SetPolygonSize((float)HP_SIZE_X * (nHP / (float)nMaxHP), HP_SIZE_Y);
		SetPolygonPos(HP_POS_X - (nMaxHP - nHP), HP_POS_Y);
		break;
	case UI_MP:
		pPlayer = GetMainPlayer();
		nMP = pPlayer->GetPlayerMp();
		nMaxMP = pPlayer->GetPlayerMaxMp();
		if (nMaxMP == 0)
		{
			break;
		}
		SetPolygonSize((260.0f * (nMP / (float)nMaxMP)) + 40, 100);
		SetPolygonPos(MP_POS_X - ((nMaxMP - nMP)*4.25f), MP_POS_Y);
		nHP = pPlayer->GetPlayerHp();
		if (nHP > 0)
			SetPolygonColor(1, (nMP/ (float)nMaxMP), (nMP / (float)nMaxMP));
		else
			SetPolygonColor(0.25f, 0.25f, 0.25f);
		if(pPlayer->IsStaminaCooldownOn())
			SetPolygonColor(1, 0, 0);
		break;
	case UI_NUMBER:
		break;
	case UI_SLASH_EFFECT:
		if (uv.U == 3 && uv.V == 7)
			break;
		if (++nFrameCounter >= 60)
		{
			uv.U++;
			if (uv.U == 4) {
				uv.V++;
				uv.U = 0;
			}
		}
		break;
	case UI_ZOOM_ATTACK:
		if (--nFramesToUseZoom <= 0)
			nFramesToUseZoom = 0;
		SetPolygonAlpha(0.6f);
		if (++nFrameCounter >= 8) {
			nFrameCounter = 0;
			g_rotPolygon.z += 90;
			if (g_rotPolygon.z == 360)
				g_rotPolygon.z = 0;
		}
		break;
	}


}

void C_Ui::Draw()
{
	if (nType == UI_ZOOM_ATTACK && nFramesToUseZoom <= 0)
		return;
	GetDeviceContext()->RSSetState(GetMainWindow()->GetRasterizerState(2));
	switch (nType)
	{
	case UI_HP01:
		SetPolygonFrameSize((g_sizPolygon.x / HP_SIZE_X), g_sizPolygon.y / HP_SIZE_Y);
		SetPolygonUV(1, 1);
		Polygon2D::DrawPolygon(GetDeviceContext());
		break;
	case UI_MP:
		SetPolygonFrameSize((g_sizPolygon.x / HP_SIZE_X), g_sizPolygon.y / HP_SIZE_Y);
		SetPolygonUV(1, 1);
		Polygon2D::DrawPolygon(GetDeviceContext());
		break;
	case UI_NUMBER:
		Draw(&vScorePos, (unsigned)nScore, SCORE_WIDTH,
			SCORE_SIZE_X, SCORE_SIZE_Y);
	case UI_LEVEL_EDITOR:
		pPlayer = GetMainPlayer();
		if (!pPlayer)
			return;
		if (!(pPlayer->IsDebugAimOn()))
			return;
		DrawPolygon(GetDeviceContext());
		break;
	case UI_LEVEL_EDITOR_MOVEABLEMODE:
		pPlayer = GetMainPlayer();
		if (!pPlayer)
			return;
		if (!(pPlayer->IsDebugAimOn()))
			return;
		if(!(pPlayer->GetDebugAim()->IsStaticMode()))
			DrawPolygon(GetDeviceContext());
		break;
	case UI_LEVEL_EDITOR_STATICMODE:
		pPlayer = GetMainPlayer();
		if (!pPlayer)
			return;
		if (!(pPlayer->IsDebugAimOn()))
			return;
		if (pPlayer->GetDebugAim()->IsStaticMode())
			DrawPolygon(GetDeviceContext());
		break;
	case UI_LEVEL_EDITOR_OBJSPEED:
		pPlayer = GetMainPlayer();
		if (!pPlayer)
			return;
		if (!(pPlayer->IsDebugAimOn()))
			return;
		if (pPlayer->GetDebugAim()->IsStaticMode())
			return;
		Draw(&vScorePos, pPlayer->GetDebugAim()->GetMoveSpeedInt(), 5,
			25, 25);
		break;	
	case UI_LEVEL_EDITOR_DELAY:
		pPlayer = GetMainPlayer();
		if (!pPlayer)
			return;
		if (!(pPlayer->IsDebugAimOn()))
			return;
		if (pPlayer->GetDebugAim()->IsStaticMode())
			return;
		Draw(&vScorePos, pPlayer->GetDebugAim()->GetDelayObj(), 5,
			25, 25);
		break;
	case UI_TITLE:
		Polygon2D::DrawPolygon(GetDeviceContext());
		break;
	case UI_GAMEOVER:
		Polygon2D::DrawPolygon(GetDeviceContext());
		break;
	case UI_CLEAR:
		Polygon2D::DrawPolygon(GetDeviceContext());
		break;
	case UI_RANKING:
		Polygon2D::DrawPolygon(GetDeviceContext());
		break;
	case UI_SLASH_EFFECT:
		SetPolygonUV(uv.U / 4.0f, uv.V / 8.0f);
		Polygon2D::DrawPolygon(GetDeviceContext());
		break;
	default:
		Polygon2D::DrawPolygon(GetDeviceContext());
		break;
	}

}

void C_Ui::Draw(XMFLOAT2* pPos, unsigned uNumber, int nWidth,
	float fSizeX, float fSizeY)
{

	SetPolygonSize(fSizeX, fSizeY);
	SetPolygonFrameSize(1.0f / NUMBER_FRAME_X, 1.0f / NUMBER_FRAME_Y);
	float fX = pPos->x + (nWidth - 0.5f) * fSizeX;
	float fY = pPos->y - fSizeY * 0.5f;
	for (int i = 0; i < nWidth; ++i) {
		SetPolygonPos(fX, fY);
		unsigned n = uNumber % 10;
		uNumber /= 10;
		int u = n % NUMBER_FRAME_X;
		int v = n / NUMBER_FRAME_X;
		SetPolygonUV(u / (float)NUMBER_FRAME_X, v / (float)NUMBER_FRAME_Y);
		fX -= fSizeX;
		Polygon2D::DrawPolygon(GetDeviceContext());
	}

}

void C_Ui::SetFramesForZoomUse(int frames)
{
	nFramesToUseZoom = frames;
}

void SetFramesForZoomUse(int frames)
{
	if (!pZoomEffect)
		return;
	pZoomEffect->SetFramesForZoomUse(frames);
}
