#include "C_Ui.h"
#include "Player3D.h"
#define HP_SIZE_X 300
#define HP_SIZE_Y 100
#define MP_SIZE_X 300


C_Ui::C_Ui()
{
}

C_Ui::C_Ui(const char *Path,int Type):Polygon2D(Path)
{
	nType = Type;
	pPlayer = NULL;
	nHP = nMaxHP = 0;
	nMP = nMaxMP = 0;
	switch (Type)
	{
	case UI_HP00:
		SetPolygonSize(HP_SIZE_X, HP_SIZE_Y);
		SetPolygonPos(-SCREEN_WIDTH / 2 + 150, SCREEN_HEIGHT / 2 - 50);
		break;
	case UI_HP01:
		SetPolygonSize(HP_SIZE_X, HP_SIZE_Y);
		SetPolygonPos(-SCREEN_WIDTH / 2 + 150, SCREEN_HEIGHT / 2 - 50);
		break;
	case UI_MP:
		SetPolygonSize(MP_SIZE_X, 100);
		SetPolygonPos(-SCREEN_WIDTH / 2 + 150, SCREEN_HEIGHT / 2 - 150);
		break;
	case UI_Menu:
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
	
}

void C_Ui::Update()
{

	switch (nType)
	{
	case UI_HP00:
		break;
	case UI_HP01:
		pPlayer = GetMainPlayer();
		nHP = pPlayer->GetPlayerHp();
		nMaxHP = pPlayer->GetPlayerMaxHp();
		//printf("hp: %d, max: %d\n", nHP, nMaxHP);
		if (nMaxHP == 0)
		{
			break;
		}
		SetPolygonSize(300.0f * (nHP / (float)nMaxHP), 100);
		SetPolygonPos(-SCREEN_WIDTH / 2 + 150 -(nMaxHP- nHP), SCREEN_HEIGHT / 2 - 50);
		break;
	case UI_MP:
		pPlayer = GetMainPlayer();
		nMP = pPlayer->GetPlayerMp();
		nMaxMP = pPlayer->GetPlayerMaxMp();
		if (nMaxMP == 0)
		{
			break;
		}
		SetPolygonSize((260.0f * (nMP / (float)nMaxMP))+40, 100);
		SetPolygonPos(-SCREEN_WIDTH / 2 + 150 - (nMaxMP - nMP), SCREEN_HEIGHT / 2 - 150);
		break;
	case UI_Menu:
		break;
	}
	

}

void C_Ui::Draw()
{
	switch (nType)
	{
	case UI_HP01:
		SetPolygonFrameSize((g_sizPolygon.x / HP_SIZE_X), g_sizPolygon.y/HP_SIZE_Y);
		SetPolygonUV(1,1);
		Polygon2D::DrawPolygon(GetDeviceContext());
		break;
	case UI_MP:
		SetPolygonFrameSize((g_sizPolygon.x / HP_SIZE_X), g_sizPolygon.y / HP_SIZE_Y);
		SetPolygonUV(1, 1);
		Polygon2D::DrawPolygon(GetDeviceContext());
		break;
	default:
		Polygon2D::DrawPolygon(GetDeviceContext());
		break;
	}
	
}
