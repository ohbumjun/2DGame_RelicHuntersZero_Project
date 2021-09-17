
#include "Player.h"
#include "../Scene/Scene.h"
#include "../Input.h"
#include "../GameManager.h"
#include "../Collision/ColliderBox.h"
#include "../Collision/ColliderSphere.h"
#include "../UI/UICharacterStateHUD.h"
#include "../UI/ProgressBar.h"
#include "../UI/UIText.h"
#include "EffectHit.h"
#include "../Scene/Scene.h"
#include"../Scene/SceneResource.h"

CPlayer::CPlayer() : m_Skill1Enable(false),
					 m_Skill1Time(0.f)
{
}

CPlayer::CPlayer(const CPlayer &obj) : CCharacter(obj)
{
	m_Skill1Time = obj.m_Skill1Time;
	m_Skill1Enable = false;
}

CPlayer::~CPlayer()
{
}

void CPlayer::Start()
{
	CCharacter::Start();

	CInput::GetInst()->SetCallback<CPlayer>("Fire", KeyState_Down,
		this, &CPlayer::BulletFire);

	CInput::GetInst()->SetCallback<CPlayer>("Pause", KeyState_Down,
		this, &CPlayer::Pause);
	CInput::GetInst()->SetCallback<CPlayer>("Resume", KeyState_Down,
		this, &CPlayer::Resume);

	CInput::GetInst()->SetCallback<CPlayer>("Skill1", KeyState_Down,
		this, &CPlayer::Skill1);

	// Move
	CInput::GetInst()->SetCallback<CPlayer>("MoveUp", KeyState_Push,
											this, &CPlayer::MoveUp);

	CInput::GetInst()->SetCallback<CPlayer>("MoveDown", KeyState_Push,
											this, &CPlayer::MoveDown);

	CInput::GetInst()->SetCallback<CPlayer>("MoveLeft", KeyState_Push,
											this, &CPlayer::MoveLeft);

	CInput::GetInst()->SetCallback<CPlayer>("MoveRight", KeyState_Push,
											this, &CPlayer::MoveRight);

	// Run
	CInput::GetInst()->SetCallback<CPlayer>("RunUp", KeyState_Push,
											this, &CPlayer::RunUp);

	CInput::GetInst()->SetCallback<CPlayer>("RunDown", KeyState_Push,
											this, &CPlayer::RunDown);

	CInput::GetInst()->SetCallback<CPlayer>("RunLeft", KeyState_Push,
											this, &CPlayer::RunLeft);

	CInput::GetInst()->SetCallback<CPlayer>("RunRight", KeyState_Push,
											this, &CPlayer::RunRight);

	// Dash
	CInput::GetInst()->SetCallback<CPlayer>("Dash", KeyState_Down,
		this, &CPlayer::Dash);
}

bool CPlayer::Init()
{
	if (!CCharacter::Init())
		return false;

	SetPivot(0.5f, 1.f);

	//SetTexture("Teemo", TEXT("teemo.bmp"));
	CreateAnimation();
	AddAnimation("LucidNunNaRightIdle");
	AddAnimation("LucidNunNaRightWalk", true, 0.6f);
	AddAnimation("LucidNunNaRightAttack", false, 0.5f);
	AddAnimation("LucidNunNaRightSkill1", false, 0.5f);
	AddAnimation("LucidNunNaRightRun", true, 0.6f);

	AddAnimation("LucidNunNaLeftIdle");
	AddAnimation("LucidNunNaLeftWalk", true, 0.6f);
	AddAnimation("LucidNunNaLeftRun", true, 0.6f);

	AddAnimationNotify<CPlayer>("LucidNunNaRightAttack", 2, this, &CPlayer::Fire);
	SetAnimationEndNotify<CPlayer>("LucidNunNaRightAttack", this, &CPlayer::AttackEnd);

	AddAnimationNotify<CPlayer>("LucidNunNaRightSkill1", 2, this, &CPlayer::Skill1Enable);
	SetAnimationEndNotify<CPlayer>("LucidNunNaRightSkill1", this, &CPlayer::Skill1End);

	/*CColliderBox* Body = AddCollider<CColliderBox>("Body");
	Body->SetExtent(80.f, 75.f);
	Body->SetOffset(0.f, -37.5f);*/

	CColliderSphere *Head = AddCollider<CColliderSphere>("Head");
	//Head->SetExtent(40.f, 30.f);
	Head->SetRadius(20.f);
	Head->SetOffset(0.f, -60.f);
	Head->SetCollisionProfile("Player");

	CColliderBox *Body = AddCollider<CColliderBox>("Body");
	Body->SetExtent(80.f, 45.f);
	Body->SetOffset(0.f, -22.5f);
	Body->SetCollisionProfile("Player");

	// HPBar
	m_HPBarWidget = CreateWidgetComponent("HPBarWidget");
	CProgressBar *HPBar = m_HPBarWidget->CreateWidget<CProgressBar>("HPBar");
	HPBar->SetTexture("WorldHPBar", TEXT("CharacterHPBar.bmp"));
	m_HPBarWidget->SetPos(-25.f, -95.f);

	// MPBar
	m_MPBarWidget = CreateWidgetComponent("MPBarWidget");
	CProgressBar* MPBar = m_MPBarWidget->CreateWidget<CProgressBar>("MPBar");
	MPBar->SetTexture("WorldMPBar", TEXT("CharacterMPBar.bmp"));
	m_MPBarWidget->SetPos(-25.f, -85.f);

	CWidgetComponent *NameWidget = CreateWidgetComponent("NameWidget");

	CUIText *NameText = NameWidget->CreateWidget<CUIText>("NameText");

	NameText->SetText(TEXT("Lucid"));
	NameText->SetTextColor(255, 0, 0);

	NameWidget->SetPos(-25.f, -115.f);

	m_CharacterInfo.MP = 5;
	m_CharacterInfo.MPMax = 5;

	m_CharacterInfo.HP = 1000;
	m_CharacterInfo.HPMax = 1000;

	return true;
}

void CPlayer::Update(float DeltaTime)
{
	CCharacter::Update(DeltaTime);

	if (GetAsyncKeyState(VK_F1) & 0x8000)
		SetAttackSpeed(0.5f);

	if (m_Skill1Enable)
	{
		m_Skill1Time += DeltaTime;

		if (m_Skill1Time >= 3.f)
		{
			m_Skill1Enable = false;
			m_Skill1Time = 0.f;

			SetTimeScale(1.f);
			CGameManager::GetInst()->SetTimeScale(1.f);

			/*auto	iter = m_Skill1BulletList.begin();
			auto	iterEnd = m_Skill1BulletList.end();

			for (; iter != iterEnd; ++iter)
			{
				(*iter)->SetTimeScale(1.f);
			}
			m_Skill1BulletList.clear();
			*/
		}
	}

	if (!m_RunEnable && !m_DashEnable)
	{
		if (m_CharacterInfo.MP <= m_CharacterInfo.MPMax)
			m_CharacterInfo.MP += DeltaTime;
	}
	// Run
	if (m_RunEnable)
	{
		if (m_CharacterInfo.MP >= 0)
			m_CharacterInfo.MP -= DeltaTime;
		if (m_CharacterInfo.MP <= 0)
		{
			RunEnd();
		}
	}
	// Dash
	if (m_DashEnable)
	{
		if (m_DashTime >= 0)
			m_DashTime -= DeltaTime;
		if (m_DashTime <= 0)
		{
			DashEnd();
		}
	}
	
	// MPBar
	CUICharacterStateHUD* State = m_Scene->FindUIWindow<CUICharacterStateHUD>("CharacterStateHUD");
	if (State)
		State->SetMPPercent(m_CharacterInfo.MP / (float)m_CharacterInfo.MPMax);
	CProgressBar* MPBar = (CProgressBar*)m_MPBarWidget->GetWidget();
	MPBar->SetPercent(m_CharacterInfo.MP / (float)m_CharacterInfo.MPMax);

	if (CheckCurrentAnimation("LucidNunNaRightAttack"))
		SetOffset(0.f, 20.f);

	else
		SetOffset(0.f, 0.f);
}

void CPlayer::PostUpdate(float DeltaTime)
{
	CCharacter::PostUpdate(DeltaTime);

	if (CheckCurrentAnimation("LucidNunNaRightWalk") &&
		m_Velocity.Length() == 0.f)
	{
		ChangeAnimation("LucidNunNaRightIdle");
	}
	if (CheckCurrentAnimation("LucidNunNaLeftWalk") &&
		m_Velocity.Length() == 0.f)
	{
		ChangeAnimation("LucidNunNaLeftIdle");
	}

	if (CheckCurrentAnimation("LucidNunNaRightRun") &&
		m_Velocity.Length() == 0.f)
	{
		RunEnd();
		ChangeAnimation("LucidNunNaRightIdle");
	}
	if (CheckCurrentAnimation("LucidNunNaLeftRun") &&
		m_Velocity.Length() == 0.f)
	{
		RunEnd();
		ChangeAnimation("LucidNunNaLeftIdle");
	}
}

void CPlayer::Collision(float DeltaTime)
{
	CCharacter::Collision(DeltaTime);
}

void CPlayer::Render(HDC hDC)
{
	CCharacter::Render(hDC);
}

CPlayer *CPlayer::Clone()
{
	return new CPlayer(*this);
}

float CPlayer::SetDamage(float Damage)
{
	Damage = CCharacter::SetDamage(Damage);

	CUICharacterStateHUD *State = m_Scene->FindUIWindow<CUICharacterStateHUD>("CharacterStateHUD");

	if (State)
		State->SetHPPercent(m_CharacterInfo.HP / (float)m_CharacterInfo.HPMax);

	CProgressBar *HPBar = (CProgressBar *)m_HPBarWidget->GetWidget();

	HPBar->SetPercent(m_CharacterInfo.HP / (float)m_CharacterInfo.HPMax);

	return Damage;
}

void CPlayer::MoveUp(float DeltaTime)
{
	RunEnd();
	//m_Pos.y -= 200.f * DeltaTime;
	Move(Vector2(0.f, -1.f));
	ChangeAnimation("LucidNunNaRightWalk");
}

void CPlayer::MoveDown(float DeltaTime)
{
	RunEnd();
	Move(Vector2(0.f, 1.f));
	ChangeAnimation("LucidNunNaLeftWalk");
}

void CPlayer::MoveLeft(float DeltaTime)
{
	RunEnd();
	Move(Vector2(-1.f, 0.f));
	ChangeAnimation("LucidNunNaLeftWalk");
}

void CPlayer::MoveRight(float DeltaTime)
{
	RunEnd();
	Move(Vector2(1.f, 0.f));
	ChangeAnimation("LucidNunNaRightWalk");
}

void CPlayer::RunLeft(float DeltaTime)
{
	Move(Vector2(-1.f, 0.f));
	RunStart();
	ChangeAnimation("LucidNunNaLeftRun");
}

void CPlayer::RunRight(float DeltaTime)
{
	Move(Vector2(1.f, 0.f));
	RunStart();
	ChangeAnimation("LucidNunNaRightRun");
}

void CPlayer::RunUp(float DeltaTime)
{
	Move(Vector2(0.f, -1.f));
	RunStart();
	ChangeAnimation("LucidNunNaRightRun");
}

void CPlayer::RunDown(float DeltaTime)
{
	Move(Vector2(0.f, 1.f));
	RunStart();
	ChangeAnimation("LucidNunNaLeftRun");
}

void CPlayer::RunStart()
{
	if (m_CharacterInfo.MP <= 0.2 * m_CharacterInfo.MPMax || m_RunEnable )
		return;
	m_RunEnable = true;
	CEffectHit *Hit = m_Scene->CreateObject<CEffectHit>("HitEffect", "HitEffect",
														m_Pos, Vector2(178.f, 164.f));
	m_Scene->GetSceneResource()->SoundPlay("Run");

	m_MoveSpeed = m_SpeedInfo.Fast;
}

void CPlayer::RunEnd()
{
	if (!m_RunEnable)
		return;
	m_RunEnable = false;
	m_MoveSpeed = m_SpeedInfo.Normal;

	if (CheckCurrentAnimation("LucidNunNaRightRun"))
	{
		ChangeAnimation("LucidNunNaRightWalk");
	}
	if (CheckCurrentAnimation("LucidNunNaLeftRun"))
	{
		ChangeAnimation("LucidNunNaLeftWalk");
	}
}

void CPlayer::Dash(float DelatTime)
{
	if (m_DashEnable || m_CharacterInfo.MP < 0.5 * m_CharacterInfo.MPMax) return;
	m_DashTime = 0.15;
	m_DashEnable = true;
	m_MoveSpeed = m_SpeedInfo.Dash;
	if (m_CharacterInfo.MP >= 0.5 * m_CharacterInfo.MPMax)
		m_CharacterInfo.MP -= 0.5 * m_CharacterInfo.MPMax;
	CEffectHit* Hit = m_Scene->CreateObject<CEffectHit>("HitEffect", "HitEffect",
		m_Pos, Vector2(178.f, 164.f));
	m_Scene->GetSceneResource()->SoundPlay("Dash");

	// 벽에 대시한 경우( 어떤 충돌체와 충돌하던 뒤로 밀려난다 )
	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();
	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->IsCollisionListEmpty())
		{
			DashEnd();
			Vector2 Dist = Vector2(-100 * m_Dir.x, -100 * m_Dir.y);
			m_Pos += Dist;
		}
	}
}

void CPlayer::DashEnd()
{
	if (!m_DashEnable) return;
	m_DashEnable = false;
	m_MoveSpeed = m_SpeedInfo.Normal;
}

void CPlayer::BulletFire(float DeltaTime)
{
	ChangeAnimation("LucidNunNaRightAttack");

	/*if (m_Skill1Enable)
	{
		Bullet->SetTimeScale(0.f);
		m_Skill1BulletList.push_back(Bullet);
	}*/
}

void CPlayer::Pause(float DeltaTime)
{
	CGameManager::GetInst()->SetTimeScale(0.f);
}

void CPlayer::Resume(float DeltaTime)
{
	CGameManager::GetInst()->SetTimeScale(1.f);
}

void CPlayer::Skill1(float DeltaTime)
{
	ChangeAnimation("LucidNunNaRightSkill1");
}

void CPlayer::AttackEnd()
{
	ChangeAnimation("LucidNunNaRightIdle");
}

void CPlayer::Fire()
{
	CSharedPtr<CBullet> Bullet = m_Scene->CreateObject<CBullet>("Bullet",
																"PlayerBullet",
																Vector2(m_Pos + Vector2(75.f, 0.f)),
																Vector2(50.f, 50.f));
}

void CPlayer::Skill1End()
{
	ChangeAnimation("LucidNunNaRightIdle");
}

void CPlayer::Skill1Enable()
{
	CGameManager::GetInst()->SetTimeScale(0.01f);
	SetTimeScale(100.f);
	m_Skill1Enable = true;
	//m_Skill1Time = 0.f;
}
