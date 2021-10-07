#pragma once
#include "GameObject.h"
class CPotion :
    public CGameObject
{
public:
    CPotion();
    CPotion(const CPotion& Potion);
    virtual ~CPotion();
protected :
    EPotion_Type m_PotionType;
    float m_PotionAmount;
protected :
	float m_PotionPauseTime;
	bool m_PotionPause;
public :
	EPotion_Type GetPotionType() { return m_PotionType; }
	float GetPotionAmount() { return m_PotionAmount; }
public :
	void SetPotionType(EPotion_Type PotionType)
	{
		m_PotionType = PotionType;
	}
public :
	void AddPotionToPlayer();
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CPotion* Clone();

};

