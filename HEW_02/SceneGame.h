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
	Player3D* HelloPlayer;
	Field3D* HelloField;
	Sphere3D* SkySphere;
	DXWindow3D* MainWindow;
public:
	SceneGame();
	~SceneGame();
	void Init();
	void Uninit();
	int Update();
	void Draw();
};

