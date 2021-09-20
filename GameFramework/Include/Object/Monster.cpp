
#include "Monster.h"
#include "Bullet.h"
#include "../Scene/Scene.h"
#include "../Scene/Camera.h"
#include "../Collision/ColliderBox.h"
#include "../UI/ProgressBar.h"
#include "Player.h"

CMonster::CMonster()	:
	m_FireTime(0.f),
	m_FireTimeMax(1.f),
	m_Count(0),
	m_RandomMoveTime(MONSTER_TARGET_POS_LIMIT_TIME)
{
	m_Dir.x = 0.f;
	m_Dir.y = 1.f;
	m_ObjType = EObject_Type::Monster;
	/// m_MoveTargetPos = SetRandomTargetPos();
	// SetRandomTargetDir();
}

CMonster::CMonster(const CMonster& obj) :
	CCharacter(obj)
{
	m_Dir = obj.m_Dir;
	m_FireTime = obj.m_FireTime;
	m_FireTimeMax = obj.m_FireTimeMax;
	m_Count = obj.m_Count;
	m_RandomMoveTime = MONSTER_TARGET_POS_LIMIT_TIME;
}

CMonster::~CMonster()
{
}

void CMonster::Start()
{
	CCharacter::Start();
}

bool CMonster::Init()
{
	if (!CCharacter::Init())
		return false;

	SetPivot(0.5f, 1.f);

	CreateAnimation();
	AddAnimation("LucidNunNaLeftIdle");

	CColliderBox* Body = AddCollider<CColliderBox>("Body");
	Body->SetExtent(82.f, 73.f);
	Body->SetOffset(0.f, -36.5f);
	Body->SetCollisionProfile("Monster");

	// HPBar
	m_HPBarWidget = CreateWidgetComponent("HPBarWidget");
	CProgressBar* HPBar = m_HPBarWidget->CreateWidget<CProgressBar>("HPBar");
	HPBar->SetTexture("WorldHPBar", TEXT("CharacterHPBar.bmp"));
	m_HPBarWidget->SetPos(-25.f, -95.f);

	// MPBar
	m_MPBarWidget = CreateWidgetComponent("MPBarWidget");
	CProgressBar* MPBar = m_MPBarWidget->CreateWidget<CProgressBar>("MPBar");
	MPBar->SetTexture("WorldMPBar", TEXT("CharacterMPBar.bmp"));
	m_MPBarWidget->SetPos(-25.f, -85.f);

	return true;
}

void CMonster::Update(float DeltaTime)
{
	CCharacter::Update(DeltaTime);
	m_Pos += m_Dir * m_MoveSpeed * DeltaTime;

	// World 상 안에서 움직인다
	MoveWithinWorldResolution();

	// Target limit time이 지나면 다시 target pos 쪽으로 dir을 바꾼다
	m_RandomMoveTime -= DeltaTime;
	if (m_RandomMoveTime <= 0.f)
	{
		SetRandomTargetDir();
		m_RandomMoveTime = MONSTER_TARGET_POS_LIMIT_TIME;
	}

	// 그러다가, 범위내에 player가 들어오면, 해당 쪽으로 이동
	if (m_Player)
	{
		Vector2 PlayerPos = m_Player->GetPos();
		float DistToPlayer = (m_Pos - PlayerPos).Length();
		if (DistToPlayer <= m_CharacterInfo.DashDistance)
		{
			float Angle = GetAngle(m_Pos, PlayerPos);
			SetDir(Angle);
			m_RandomMoveTime = MONSTER_TARGET_POS_LIMIT_TIME;
		}
	}

	// 랜덤한 위치로 이동
	// 위아래 이동 
	/*
	* if (m_Pos.y >= 720.f)
	{
		m_Pos.y = 720.f;
		m_Dir.y = -1.f;
	}
	else if (m_Pos.y - m_Size.y <= 0.f)
	{
		m_Pos.y = m_Size.y;
		m_Dir.y = 1.f;
	}
	*/
	

	m_FireTime += DeltaTime;
	if (m_FireTime >= m_FireTimeMax)
	{
		m_FireTime -= m_FireTimeMax;
		++m_Count;
		CSharedPtr<CBullet> Bullet = m_Scene->CreateObject<CBullet>("Bullet",
			"MonsterBullet", Vector2(m_Pos - Vector2(m_Size.x / 2.f + 25.f, m_Size.y / 2.f)),
			Vector2(50.f, 50.f));
		Bullet->SetBulletDamage(m_CharacterInfo.Attack);

		if (m_Count % 3 != 0)
			Bullet->SetDir(-1.f, 0.f);

		else
		{
			//CGameObject* Player = m_Scene->FindObject("Player");
			CGameObject* Player = m_Scene->GetPlayer();

			float	Angle = GetAngle(Bullet->GetPos(), Player->GetPos());
			Bullet->SetDir(Angle);
		}
	}
}

void CMonster::PostUpdate(float DeltaTime)
{
	CCharacter::PostUpdate(DeltaTime);
}

void CMonster::Collision(float DeltaTime)
{
	CCharacter::Collision(DeltaTime);
}

void CMonster::Render(HDC hDC)
{
	CCharacter::Render(hDC);
}

CMonster* CMonster::Clone()
{
	return new CMonster(*this);
}

float CMonster::SetDamage(float Damage)
{
	Damage = CCharacter::SetDamage(Damage);
	
	CProgressBar* HPBar = (CProgressBar*)m_HPBarWidget->GetWidget();
	HPBar->SetPercent(m_CharacterInfo.HP / (float)m_CharacterInfo.HPMax);
	return Damage;
}

void CMonster::CharacterDestroy()
{
}

void CMonster::SetRandomTargetDir()
{
	m_MoveTargetPos = SetRandomTargetPos();
	float	Angle = GetAngle(m_Pos, m_MoveTargetPos);
	SetDir(Angle);
}

Vector2 CMonster::SetRandomTargetPos()
{
	Vector2 WorldResolution = m_Scene->GetCamera()->GetWorldResolution();
	float x = rand() % (int)WorldResolution.x;
	float y = rand() % (int)WorldResolution.y;
	m_MoveTargetPos = Vector2(x, y);
	return m_MoveTargetPos;
}
