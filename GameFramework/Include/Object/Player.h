#pragma once
#include "Character.h"
#include "Bullet.h"
class CPlayer : public CCharacter
{
	friend class CScene;

protected:
	CPlayer();
	CPlayer(const CPlayer &obj);
	virtual ~CPlayer();

protected:
	bool m_Skill1Enable;
	float m_Skill1Time;
	std::list<CSharedPtr<CBullet>> m_Skill1BulletList;
	// Widget
	CSharedPtr<CWidgetComponent> m_HPBarWidget;
	CSharedPtr<CWidgetComponent> m_MPBarWidget;
	CSharedPtr<CWidgetComponent> m_NameWidget;

public:
	virtual void SetAttackSpeed(float Speed)
	{
		CCharacter::SetAttackSpeed(Speed);

		SetAnimationPlayScale("LucidNunNaRightAttack", Speed);
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CPlayer *Clone();
	virtual float SetDamage(float Damage);

// Move
private:
	void MoveUp(float DeltaTime);
	void MoveDown(float DeltaTime);
	void MoveLeft(float DeltaTime);
	void MoveRight(float DeltaTime);
	void Move(const Vector2& Dir);
	void Move(const Vector2& Dir, float Speed);
	void ChangeMoveAnimation(); // 오른쪽,왼쪽 only
// Run
private:
	void RunLeft(float DeltaTime);
	void RunRight(float DeltaTime);
	void RunUp(float DeltaTime);
	void RunDown(float DeltaTime);
	bool m_RunEnable;
	void RunStart();
	void RunEnd();
	void ChangeRunAnimation(); // 오른쪽 왼쪽 only
// Dash
private:
	void Dash(float DeltaTime);
	void DashEnd();
	void DashCollide();
	bool m_DashEnable;
	float m_DashTime;
// Other Skills
private:
	void BulletFire(float DeltaTime);
	void Pause(float DeltaTime);
	void Resume(float DeltaTime);
	void Skill1(float DeltaTime);
// Collision
private :
	bool CollisionCheck();
	void CollisionBegin(class CCollider* Src, class CCollider* Dest, float DeltaTime);
	Vector2 GetColliderPos();
// Stun
private :
	virtual void Stun();
	virtual void StunEnd();
// 순간 이동
private :
	bool    m_TeleportEnable;
	float   m_TeleportTime;
	Vector2 m_TeleportPos;
	void Teleport(float DeltaTime);
	void SetTeleportPos(float DeltaTime);

public:
	void AttackEnd();
	void Fire();
	void Skill1End();
	void Skill1Enable();
	virtual void GoOppDirection(Vector2 Dir)
	{
		Vector2 Dist = Vector2(Dir.x * 20, Dir.y * 20);
		m_Pos += Dist;
	};
};
