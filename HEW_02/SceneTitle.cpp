#include "SceneTitle.h"
#include "InputManager.h"



SceneTitle::SceneTitle()
{
	MainWindow = GetMainWindow();
	if (MainWindow)
		MainWindow->SetWindowColor(0, 0, 0);
	Title = new C_Ui("data/texture/haikei.png", UI_TITLE);
	SlashEffect = new C_Ui("data/texture/Slash.tga", UI_SLASH_EFFECT);
	Effect1 = new C_Ui("data/texture/haikei0.png", UI_TITLE);
	Effect2 = new C_Ui("data/texture/haikei1.png", UI_TITLE);
	Effect1->SetPolygonAlpha(0.5f);
	Effect2->SetPolygonAlpha(0.5f);
	alpha1 = 1; 
	alpha2 = -1;
}


SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
}

void SceneTitle::Uninit()
{
}

int SceneTitle::Update()
{
	SlashEffect->Update();
	Effect1->SetPolygonAlpha(Effect1->GetAlpha() - 0.005f*alpha1);
	Effect2->SetPolygonAlpha(Effect2->GetAlpha() - 0.005f*alpha2);

	if (Effect1->GetAlpha() <= 0 || Effect1->GetAlpha() >= 1) {
		alpha1 *= -1;
		if (Effect1->GetAlpha() < 0)
			Effect1->SetPolygonAlpha(0);
		if (Effect1->GetAlpha() > 1)
			Effect1->SetPolygonAlpha(1);
	}
	if (Effect2->GetAlpha() <= 0 || Effect2->GetAlpha() >= 1) {
		alpha2 *= -1;
		if (Effect2->GetAlpha() < 0)
			Effect2->SetPolygonAlpha(0);
		if (Effect2->GetAlpha() > 1)
			Effect2->SetPolygonAlpha(1);
	}

	if (GetInput(INPUT_JUMP) && SlashEffect->GetUV().V >= 7) {
		return SCENE_GAME;
	}
	return SCENE_TITLE;
}

void SceneTitle::Draw()
{
	SetZBuffer(false);
	if (SlashEffect->GetUV().V >= 5) {
		Title->Draw();
		Effect1->Draw();
		Effect2->Draw();
	}
	SlashEffect->Draw();
}

