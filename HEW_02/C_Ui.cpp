#include "C_Ui.h"
#include "Player3D.h"
#include "SceneGame.h"
#include "RankManager.h"
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


#define SCORE_POS_X			(FRAME_POS_X-SCORE_SIZE_X*(SCORE_WIDTH/2))+13	// 文字の表示位置
#define SCORE_POS_Y			(FRAME_POS_Y+SCORE_SIZE_Y/2-8)+15	// 文字の表示位置

#define MAIN_POS_DOOR_RIGHT 40
#define MAIN_POS_DOOR_LEFT -20

#define OPTION_SIZE_X 743/4
#define OPTION_SIZE_Y 1433/4
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
	fScaleOffset = 0;
	nFrameUse = 0;
	fOffsetY = 5;
	vScorePos = XMFLOAT2(SCORE_POS_X, SCORE_POS_Y);
	bSelected = false;
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
		vScorePos = { -30,-220 - 45 };
		break;
	case UI_TITLE:
		SetPolygonSize(SCREEN_WIDTH, SCREEN_HEIGHT);
		SetPolygonPos(0, 0);
		break;
	case UI_MENU:
		SetPolygonSize(SCREEN_WIDTH, SCREEN_HEIGHT);
		SetPolygonPos(0, 0);
		break;
	case UI_MENUFRAME:
		SetPolygonSize(980, 620);
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
		SetPolygonFrameSize(1.0f / 4.0f, 1.0f / 8.0f);
		SetPolygonPos(0, 0);
		break;
	case UI_ZOOM_ATTACK:
		pZoomEffect = this;
		SetPolygonSize(1280, 1280);
		SetPolygonPos(0, 0);
		break;
	case UI_DOOR_RIGHT:
		SetPolygonSize(2048/1.65f, 1447/1.65f);
		SetPolygonPos(840, 25);
		break;
	case UI_DOOR_LEFT:
		SetPolygonSize(2048 / 1.65f, 1447 / 1.65f);
		SetPolygonPos(-820, 25);
		break;
	case UI_PAUSE_BG:
		SetPolygonSize(SCREEN_WIDTH, SCREEN_HEIGHT);
		SetPolygonPos(0, 0);
		SetPolygonAlpha(0.5f);
		break;
	case UI_MENU_OPTION:
		SetPolygonSize(OPTION_SIZE_X, OPTION_SIZE_Y);
		SetPolygonPos(0, 0);
		break;
	case UI_HIT_EFFECT:
		SetPolygonSize(1, 1);
		SetPolygonPos(0, 0);
		break;
	case UI_SAKURALEAF:
		SetPolygonSize(315/3, 315/3);
		SetPolygonAlpha(0);
		vSpeed.x = (float)(rand() % 5 + 2);
		vSpeed.y = (float)(rand() % 5 + 2);
		uv = { (float)(rand()%7),0 };
		SetPolygonFrameSize(1.0f / 7.0f, 1.0f / 1.0f);
		SetPolygonPos((float)(rand() %(SCREEN_WIDTH)- (SCREEN_WIDTH / 2)), (float)(rand() % (SCREEN_HEIGHT / 2) - (SCREEN_HEIGHT / 2)));
		break;
	case UI_RANK_VISUAL:
		SetPolygonSize(368/2, 315/2);
		SetPolygonAlpha(1);
		uv = { 0,0 };
		SetPolygonFrameSize(1.0f / 3.0f, 1.0f / 1.0f);
		SetPolygonPos(500,230);
		break;
	case UI_OWARI:
		SetPolygonSize(0, 0);
		uv = { 0,0 };
		SetPolygonFrameSize(1.0f / 3.0f, 1.0f / 10.0f);
		SetPolygonPos(0, 0);
		break;
	}
	fAcceleration = 0;
	hpDamageCooloff = 0;
	nCounterOwariOnScreen = 0;
	bDoorInPos = false;
	bOwariAnimationisOver = false;
	bDoorOpen = true;
	bIsInUse = false;
}


C_Ui::~C_Ui()
{
	Polygon2D::~Polygon2D();
	Uninit();
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
	if (!pCurrentGame && 
		nType != UI_SLASH_EFFECT && nType != UI_DOOR_LEFT
			&& nType != UI_DOOR_RIGHT && nType != UI_MENU_OPTION)
		return;
	if (pCurrentGame)
		nScore = pCurrentGame->GetScore();
	switch (nType)
	{
	case UI_HP00:
		pPlayer = GetMainPlayer();
		if (!pPlayer)
			return;
		nHP = pPlayer->GetPlayerHp();
		nMaxHP = pPlayer->GetPlayerMaxHp();
		if (++hpDamageCooloff > 4) {
			fOffsetY *= -1;
			hpDamageCooloff = 0;
		}
		if (nHP > 0)
			SetPolygonColor(1, (nHP / (float)nMaxHP), (nHP / (float)nMaxHP));
		else
			SetPolygonColor(0.25f, (nHP / (float)nMaxHP), (nHP / (float)nMaxHP));

		if (pPlayer->GetDamage() > 0)
			SetPolygonPos(HP_POS_X , HP_POS_Y + fOffsetY);
		else
			SetPolygonPos(HP_POS_X, HP_POS_Y);
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
		if (++hpDamageCooloff > 4) {
			fOffsetY *= -1;
			hpDamageCooloff = 0;
		}
		SetPolygonSize((float)HP_SIZE_X * (nHP / (float)nMaxHP), HP_SIZE_Y);
		if(pPlayer->GetDamage()>0)
			SetPolygonPos(HP_POS_X - (nMaxHP - nHP), HP_POS_Y+fOffsetY);
		else
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
			SetPolygonColor(1, (nMP / (float)nMaxMP), (nMP / (float)nMaxMP));
		else
			SetPolygonColor(0.25f, 0.25f, 0.25f);
		if (pPlayer->IsStaminaCooldownOn())
			SetPolygonColor(1, 0, 0);
		break;
	case UI_NUMBER:
		break;
	case UI_SLASH_EFFECT:
		if (uv.U == 3 && uv.V == 7)
			break;
		if (++nFrameCounter >= 15)
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
		if (++nFrameCounter >= 1) {
			nFrameCounter = 0;
			g_rotPolygon.z += 20;
			if (g_rotPolygon.z == 360)
				g_rotPolygon.z = 0;
		}
		break;
	case UI_DOOR_LEFT:
		if ((bDoorOpen && Position.x == -820) ||
			(!bDoorOpen && Position.x == MAIN_POS_DOOR_LEFT)) {
			bDoorInPos = true;
			fAcceleration = 0;
		}
		else
			bDoorInPos = false;
		if (!bDoorOpen)
		{

			fAcceleration += 2;
			if (Position.x < MAIN_POS_DOOR_LEFT)
			{
				Position.x += fAcceleration;
				if (Position.x > MAIN_POS_DOOR_LEFT)
					Position.x = MAIN_POS_DOOR_LEFT;
			}
		}
		else {
			fAcceleration += 2;
			if (Position.x > -820)
			{
				Position.x -= fAcceleration;
				if (Position.x < -820)
					Position.x = -820;
			}
		}

		break;
	case UI_DOOR_RIGHT:
		if ((bDoorOpen && Position.x == 840) ||
			(!bDoorOpen && Position.x == MAIN_POS_DOOR_RIGHT))
		{
			fAcceleration = 0;
			bDoorInPos = true;
		}
		else
			bDoorInPos = false;

		if (!bDoorOpen)
		{

			fAcceleration += 2;
			if (Position.x > MAIN_POS_DOOR_RIGHT)
			{
				Position.x -= fAcceleration;
				if (Position.x < MAIN_POS_DOOR_RIGHT)
					Position.x = MAIN_POS_DOOR_RIGHT;
			}
		}
		else {
			fAcceleration += 2;
			if (Position.x < 840)
			{
				Position.x += fAcceleration;
				if (Position.x > 840)
					Position.x = 840;
			}
		}
		break;
	case UI_PAUSE_BG:
		break;
	case UI_MENU_OPTION:
		if (bScalingUP)
		{
			fScaleOffset+=1.0f;
			if (fScaleOffset >= 20)
				bScalingUP = false;
		}
		else 
		{
			fScaleOffset-=1.0f;
			if (fScaleOffset <= -20)
				bScalingUP = true;
		}
		if(bSelected)
			SetPolygonSize(OPTION_SIZE_X+ fScaleOffset, OPTION_SIZE_Y+ fScaleOffset);
		else
			SetPolygonSize(OPTION_SIZE_X, OPTION_SIZE_Y);
		break;
	case UI_HIT_EFFECT:
		if (!bIsInUse)
			break;
		fAcceleration += 10;
		SetPolygonSize(Scale.x+ fAcceleration, Scale.y+ fAcceleration);
		if (Scale.x > 1830)
			bIsInUse = false;
		break;
	case UI_SAKURALEAF:
		pPlayer = GetMainPlayer();
		if (nFrameUse > 0)
		{
			if (GetAlpha() <= 1)
				SetPolygonAlpha(Color.w + 0.05f);
			if (GetAlpha() > 1)
				SetPolygonAlpha(1);
			if (pPlayer) {
				if (GetAlpha() == 1 && pPlayer->GetState() != PLAYER_GEISHA_DODGE)
					nFrameUse--;
			}
		}
		else {
			if (GetAlpha() >= 0)
				SetPolygonAlpha(Color.w - 0.05f);
			if (GetAlpha() < 0)
				SetPolygonAlpha(0);
		}
		Position.x-=vSpeed.x;
		Position.y-=vSpeed.y;
		if (Position.x < -SCREEN_WIDTH / 2 - (Scale.x / 2)
			|| Position.y < -SCREEN_HEIGHT / 2 - (Scale.y / 2)) {
			SetPolygonPos((float)(rand() % (SCREEN_WIDTH) - (SCREEN_WIDTH / 2)), (SCREEN_HEIGHT / 2));
			vSpeed.x = (float)(rand() % 3 + 1);
			vSpeed.y = (float)(rand() % 3 + 1);
		}
		break;
	case UI_RANK_VISUAL:
		
		break;
	case UI_OWARI:
		if (!bIsInUse)
			return;
		if (!CompVector(Scale, { 333, 720, 1 }))
		{
			Scale.z = 1;
			fAcceleration += 2;
			Scale.x += fAcceleration;
			Scale.y += fAcceleration;
			if (Scale.x > 333)
				Scale.x = 333;
			if (Scale.y > 720)
				Scale.y = 720;
			return;
		}
		
		if (++nCounterOwariOnScreen > 120) {
			if (++nFrameCounter >= 2)
			{
				nFrameCounter = 0;
				uv.U++;
				if (uv.U == 3) {
					uv.V++;
					uv.U = 0;
					if (uv.V == 10) {
						uv.V = 9;
						uv.U = 2;
						bOwariAnimationisOver = true;
					}
				}
			}
		}
		break;
	}

}

void C_Ui::Draw()
{
	int nCurrentRank = 1;
	if (nType == UI_ZOOM_ATTACK && nFramesToUseZoom <= 0)
		return;
	GetDeviceContext()->RSSetState(GetMainWindow()->GetRasterizerState(2));
	switch (nType)
	{
	case UI_HP01:
		SetPolygonFrameSize((Scale.x / HP_SIZE_X), Scale.y / HP_SIZE_Y);
		SetPolygonUV(1, 1);
		Polygon2D::DrawPolygon(GetDeviceContext());
		break;
	case UI_MP:
		SetPolygonFrameSize((Scale.x / HP_SIZE_X), Scale.y / HP_SIZE_Y);
		SetPolygonUV(1, 1);
		Polygon2D::DrawPolygon(GetDeviceContext());
		break;
	case UI_NUMBER:
		Draw(&vScorePos, (unsigned)nScore, SCORE_WIDTH,
			SCORE_SIZE_X, SCORE_SIZE_Y);
		break;
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
		if (!(pPlayer->GetDebugAim()->IsStaticMode()))
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
	case UI_MENU:
		Polygon2D::DrawPolygon(GetDeviceContext());
		break;
	case UI_MENUFRAME:
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
	case UI_HIT_EFFECT:
		if (!bIsInUse)
			break;
		Polygon2D::DrawPolygon(GetDeviceContext());
		break;
	case UI_SAKURALEAF:
		if (GetAlpha() == 0)
			return;
		SetPolygonUV(uv.U / 7.0f, 1.0f);
		Polygon2D::DrawPolygon(GetDeviceContext());
		break;
	case UI_RANK_VISUAL:
		nCurrentRank = GetRank();
		if (nCurrentRank <= 1)
			return;
		SetPolygonUV((float)(nCurrentRank-2) / 3.0f, 1.0f);
		Polygon2D::DrawPolygon(GetDeviceContext());
		break;
	case UI_RESULT_SCORE:
		vScorePos = XMFLOAT2(-SCORE_SIZE_X*11, -SCORE_SIZE_Y);
		Draw(&vScorePos, (unsigned)GetScore(), SCORE_WIDTH,
			SCORE_SIZE_X*2.5f, SCORE_SIZE_Y*3);
		break;
	case UI_RANKING_SCORE:
			vScorePos = XMFLOAT2(-SCORE_SIZE_X * 8, -195);
			Draw(&vScorePos, (unsigned)GetScore(), SCORE_WIDTH,
				SCORE_SIZE_X*2.3f, SCORE_SIZE_Y *2.8);
			break;	
	case UI_RANKING_TOP:
			//vScorePos = XMFLOAT2(-SCORE_SIZE_X * 8, -195);
			Draw(&vScorePos, nScore, SCORE_WIDTH,
				SCORE_SIZE_X*2.3f, SCORE_SIZE_Y *2.8);
			break;
	case UI_OWARI:
		if (!bIsInUse)
			return;
		SetPolygonUV(uv.U / 3.0f, uv.V / 10.0f);
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

bool C_Ui::IsDoorInPosition()
{
	return bDoorInPos;
}

void C_Ui::SetDoorOpen(bool bset)
{
	bDoorOpen = bset;
	fAcceleration = 0;
}

void C_Ui::SetAsSelectedOption(bool sel)
{
	bSelected = sel;
}

void C_Ui::SetHitEffectUse()
{
	bIsInUse = true;
	fAcceleration = 0;
	SetPolygonSize(0, 0);
}

bool C_Ui::GetUse()
{
	return bIsInUse;
}

void C_Ui::SetUse(bool use)
{
	bIsInUse = use;
}

void C_Ui::SetFrameUse(int frames)
{
	nFrameUse = frames;
}

void C_Ui::SetRankTop(XMFLOAT2 Pos, int Scorenum)
{
	vScorePos = Pos;
	nScore = Scorenum;
}

bool C_Ui::IsOwariMessageDone()
{
	return bOwariAnimationisOver;
}


void SetFramesForZoomUse(int frames)
{
	if (!pZoomEffect)
		return;
	pZoomEffect->SetFramesForZoomUse(frames);
}
