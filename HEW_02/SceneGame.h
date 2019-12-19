#pragma once
#include "SceneBase.h"
#include "Sphere3D.h"
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
	Sphere3D* SkySphere;
	DXWindow3D* MainWindow;
public:
	SceneGame();
	~SceneGame();
	void Init();
	void Uninit();
	int Update();
	void Draw();
	Go_List* GetFields();
	Go_List* GetWalls();
};
SceneGame* GetCurrentGame();