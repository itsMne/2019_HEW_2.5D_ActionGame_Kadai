#pragma once
#include "SceneBase.h"
#include "Sphere3D.h"
#include "C_Ui.h"
class SceneGame :
	public SceneBase
{
private:
	int nScore;
	ID3D11Device* g_pDevice;
	Camera3D* SceneCamera;
	Light3D* SceneLight;
	Player3D* pPlayer;
	Go_List* Fields;
	Go_List* Walls;
	Go_List* Items;
	Sphere3D* SkySphere;
	DXWindow3D* MainWindow;
	
	//UIÇÃïœêî
	C_Ui* HelloHp00;
	C_Ui* HelloHp01;
	C_Ui* HelloMp;
	//C_Item* HelloItem;
	C_Ui* HelloNumber;
	C_Ui* HelloScore;
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
	int GetScore();
};
SceneGame* GetCurrentGame();
