
#include "MainScene.h"
#include "../Object/Player.h"
#include "../Object/Bullet.h"
#include "../Object/Monster.h"
#include "SceneResource.h"
#include "../Object/EffectHit.h"
#include "Camera.h"
#include "../UI/UIMain.h"
#include "../UI/UICharacterStateHUD.h"

CMainScene::CMainScene()
{
}

CMainScene::~CMainScene()
{
}

bool CMainScene::Init()
{
	LoadSound();

	LoadAnimationSequence();

	GetCamera()->SetWorldResolution(10000.f, 10000.f);

	CEffectHit *EffectPrototype = CreatePrototype<CEffectHit>("HitEffect");

	CBullet *PlayerBullet = CreatePrototype<CBullet>("PlayerBullet");

	CCollider *Collider = PlayerBullet->FindCollider("Body");

	if (Collider)
		Collider->SetCollisionProfile("PlayerAttack");

	CBullet *MonsterBullet = CreatePrototype<CBullet>("MonsterBullet");

	Collider = MonsterBullet->FindCollider("Body");

	if (Collider)
		Collider->SetCollisionProfile("MonsterAttack");

	CPlayer *Player = CreateObject<CPlayer>("Player");

	SetPlayer(Player);

	GetCamera()->SetTarget(Player);
	GetCamera()->SetTargetPivot(0.5f, 0.5f);

	CMonster *Monster = CreateObject<CMonster>("Monster",Vector2(1000.f, 100.f));

	/*CUIWindow* TestWindow = CreateUIWindow<CUIWindow>("TestWindow");

	CUIImage* Image = TestWindow->CreateWidget<CUIImage>("TestImage");

	Image->SetTexture("Test", TEXT("teemo.bmp"));
	Image->SetPos(100.f, 100.f);
	Image->SetZOrder(1);

	CUIImage* Image1 = TestWindow->CreateWidget<CUIImage>("TestImage1");

	Image1->SetTexture("Test1", TEXT("Start.bmp"));
	Image1->SetPos(150.f, 100.f);*/

	CUIMain *MainWindow = CreateUIWindow<CUIMain>("MainWindow");
	CUICharacterStateHUD *StateWindow = CreateUIWindow<CUICharacterStateHUD>("CharacterStateHUD");

	return true;
}

void CMainScene::LoadAnimationSequence()
{
	GetSceneResource()->CreateAnimationSequence("LucidNunNaRightIdle",
												"LucidNunNaRightIdle", TEXT("Player/Right/astand.bmp"));

	GetSceneResource()->SetTextureColorKey("LucidNunNaRightIdle",
										   255, 0, 255);

	for (int i = 0; i < 6; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("LucidNunNaRightIdle",
												  i * 82.f, 0.f, 82.f, 73.f);
	}

	GetSceneResource()->CreateAnimationSequence("LucidNunNaRightWalk",
												"LucidNunNaRightWalk", TEXT("Player/Right/awalk.bmp"));

	GetSceneResource()->SetTextureColorKey("LucidNunNaRightWalk",
										   255, 0, 255);

	for (int i = 0; i < 4; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("LucidNunNaRightWalk",
												  i * 85.f, 0.f, 85.f, 75.f);
	}

	GetSceneResource()->CreateAnimationSequence("LucidNunNaRightAttack",
												"LucidNunNaRightAttack", TEXT("Player/Right/aswing.bmp"));

	GetSceneResource()->SetTextureColorKey("LucidNunNaRightAttack",
										   255, 0, 255);

	for (int i = 0; i < 3; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("LucidNunNaRightAttack",
												  i * 176.f, 0.f, 176.f, 89.f);
	}

	GetSceneResource()->CreateAnimationSequence("LucidNunNaRightSkill1",
												"LucidNunNaRightSkill1", TEXT("Player/Right/ashoot1.bmp"));

	GetSceneResource()->SetTextureColorKey("LucidNunNaRightSkill1",
										   255, 0, 255);

	for (int i = 0; i < 3; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("LucidNunNaRightSkill1",
												  i * 70.f, 0.f, 70.f, 81.f);
	}

	GetSceneResource()->CreateAnimationSequence("HitRight",
												"HitRight", TEXT("Hit2.bmp"));

	GetSceneResource()->SetTextureColorKey("HitRight",
										   255, 0, 255);

	for (int i = 0; i < 6; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("HitRight",
												  i * 178.f, 0.f, 178.f, 164.f);
	}

	GetSceneResource()->CreateAnimationSequence("Bullet",
												"Bullet", TEXT("Smoke.bmp"));

	GetSceneResource()->SetTextureColorKey("Bullet",
										   255, 0, 255);

	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			GetSceneResource()->AddAnimationFrameData("Bullet",
													  j * 92.f, i * 92.f, 92.f, 92.f);
		}
	}

	GetSceneResource()->CreateAnimationSequence("LucidNunNaLeftIdle",
												"LucidNunNaLeftIdle", TEXT("Player/Left/astand_left.bmp"));

	GetSceneResource()->SetTextureColorKey("LucidNunNaLeftIdle",
										   255, 0, 255);

	for (int i = 0; i < 6; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("LucidNunNaLeftIdle",
												  i * 82.f, 0.f, 82.f, 73.f);
	}

	// Left  Walk
	GetSceneResource()->CreateAnimationSequence("LucidNunNaLeftWalk",
												"LucidNunNaLeftWalk", TEXT("Player/Left/awalk_left.bmp"));

	GetSceneResource()->SetTextureColorKey("LucidNunNaLeftWalk",
										   255, 0, 255);

	for (int i = 0; i < 4; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("LucidNunNaLeftWalk",
												  i * 85.f, 0.f, 85.f, 75.f);
	}

	// Run
	GetSceneResource()->CreateAnimationSequence("LucidNunNaLeftRun",
												"LucidNunNaLeftRun", TEXT("Player/Left/alert_left.bmp"));

	GetSceneResource()->SetTextureColorKey("LucidNunNaLeftRun",
										   255, 0, 255);

	for (int i = 0; i < 3; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("LucidNunNaLeftRun",
												  i * 59.f, 0.f, 59.f, 87.f);
	}

	// right run
	GetSceneResource()->CreateAnimationSequence("LucidNunNaRightRun",
												"LucidNunNaRightRun", TEXT("Player/Right/alert.bmp"));

	GetSceneResource()->SetTextureColorKey("LucidNunNaRightRun",
										   255, 0, 255);

	for (int i = 0; i < 3; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("LucidNunNaRightRun",
												  i * 59.f, 0.f, 59.f, 87.f);
	}

	// Stun
	GetSceneResource()->CreateAnimationSequence("LucidNunNaStun",
		"LucidNunNaStun", TEXT("Player.bmp"));

	GetSceneResource()->SetTextureColorKey("LucidNunNaStun",
		255, 0, 255);
	for (int i = 0; i < 3; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("LucidNunNaRightRun",
			i * 32.f, 0.f, 32.f, 32.f);
	}

}

void CMainScene::LoadSound()
{
	GetSceneResource()->LoadSound("BGM", true, "MainBGM", "MainBgm.mp3");
	GetSceneResource()->SoundPlay("MainBGM");
	GetSceneResource()->SetVolume("MainBGM", 10);

	GetSceneResource()->LoadSound("Effect", false, "Fire", "Fire1.wav");
	GetSceneResource()->SetVolume("Effect", 10);

	GetSceneResource()->LoadSound("Effect", false, "TextSound", "water-step-01.ogg");

	// Player --> run, dash
	GetSceneResource()->LoadSound("Player", false, "Run", "snow-step-1.ogg");
	GetSceneResource()->LoadSound("Player", false, "Dash", "snow-step-2.ogg");
}
