//*****************************************************************************
//SceneMenu.h
//*****************************************************************************
#ifndef SCENEMENU_H
#define SCENEMENU_H
#include "SceneBase.h"
#include "C_Ui.h"

//*****************************************************************************
//�G�i��
//*****************************************************************************
enum MENU {
	TUTORIAL=0,
	START,			// �X�^�[�g
	ALT,			
	RANKING,			// �����L���O
	MENU_MAX
};

//*****************************************************************************
//�N���X
//*****************************************************************************
class SceneMenu :
	public SceneBase
{
private:
	C_Ui* pStart;
	C_Ui* pRanking;
	C_Ui* pHard;
	C_Ui* pTutorial;
	bool bOptionSelected;
	MENU g_nSelectMenu;
	int nNextScene;
public:
	SceneMenu();
	~SceneMenu();
	void Init();
	void Uninit();
	int Update();
	void Draw();

	MENU GetPauseMenu();
	void ResetPauseMenu();
};

//*****************************************************************************
//�v���g�^�C�v�錾
//*****************************************************************************
SceneMenu* GetMenu();

#endif