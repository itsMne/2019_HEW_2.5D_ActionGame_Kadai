#include "EventBox3D.h"
#include "Player3D.h"
#include "SceneGame.h"
#include "C_Ui.h"
#define VIEW_EVENT_BOX


EventBox3D::EventBox3D(int Event): GameObject3D()
{
	nType = GO_EVENT;
	nEvent = Event;
	Init();
}


EventBox3D::~EventBox3D()
{
	SAFE_DELETE(pVisualHitbox);
}

void EventBox3D::Init()
{
	hitbox = { 0,0,0,10,10,10 };

	if(!pVisualHitbox)
		pVisualHitbox = new Cube3D("data/texture/hbox.tga");
	pVisualHitbox->SetPosition({ GetHitBox().x, GetHitBox().y, GetHitBox().z });
	pVisualHitbox->SetScale({ GetHitBox().SizeX, GetHitBox().SizeY, GetHitBox().SizeZ });
}

void EventBox3D::Update()
{
	pVisualHitbox->SetPosition({ GetHitBox().x, GetHitBox().y, GetHitBox().z });
	pVisualHitbox->SetScale({ GetHitBox().SizeX, GetHitBox().SizeY, GetHitBox().SizeZ });
	Player3D* pPlayer = GetMainPlayer();
	SceneGame* pGame = GetCurrentGame();
	if (!pPlayer)
		return;
	if (pPlayer->GetState() == PLAYER_TELEPORTING)
		return;
	if (IsInCollision3D(pPlayer->GetHitBox(HB_BODY), GetHitBox()) || IsInCollision3D(pPlayer->GetHitBox(HB_FEET), GetHitBox()))
	{
		switch (nEvent)
		{
		case MOVE_TUTORIAL:
			pGame->SwitchTutorialMessage(TUTORIAL_MESSAGE_MOVE);
			break;
		case JUMP_TUTORIAL:
			pGame->SwitchTutorialMessage(TUTORIAL_MESSAGE_JUMP);
			break;
		case MOVE2_TUTORIAL:
			pGame->SwitchTutorialMessage(TUTORIAL_MESSAGE_MOVEUP);
			break;
		case GEISHA_TUTORIAL:
			if(pPlayer->GetCurrentTransformation()==MODEL_GEISHA)
				pGame->SwitchTutorialMessage(TUTORIAL_MESSAGE_GEISHA_A);
			else
				pGame->SwitchTutorialMessage(TUTORIAL_MESSAGE_GEISHA_B);
			break;
		case ATTACK_TUTORIAL:
			pGame->SwitchTutorialMessage(TUTORIAL_MESSAGE_ATTACK);
			break;
		case SAMURAI_TUTORIAL:
			if (pPlayer->GetCurrentTransformation() != MODEL_SAMURAI)
				pGame->SwitchTutorialMessage(TUTORIAL_MESSAGE_SAMURAI_A);
			else
				pGame->SwitchTutorialMessage(TUTORIAL_MESSAGE_SAMURAI_B);
			break;
		case GEISHA2_TUTORIAL:
			if (pPlayer->GetCurrentTransformation() == MODEL_GEISHA)
				pGame->SwitchTutorialMessage(TUTORIAL_MESSAGE_GEISHA_C);
			else
				pGame->SwitchTutorialMessage(TUTORIAL_MESSAGE_GEISHA_B);
			break;
		default:
			printf("EVENT ON\n");
			printf("Position: (%f, %f, %f)\n", Position.x, Position.y, Position.z);
			break;
		}
		return;
	}
}

void EventBox3D::Draw()
{
	Player3D* pPlayer = GetMainPlayer();
	if (!pPlayer)
		return;
	if (!pPlayer->IsDebugAimOn())
		return;
	pVisualHitbox->Draw();
}

void EventBox3D::Uninit()
{
}

int EventBox3D::GetEvenType()
{
	return nEvent;
}

void EventBox3D::SetScaleWithHitbox(XMFLOAT3 newScale)
{
	Scale = newScale;
	hitbox.SizeX = newScale.x;
	hitbox.SizeY = newScale.y;
	hitbox.SizeZ = newScale.z;
}