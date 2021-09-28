#include "MPPotion.h"

CMPPotion::CMPPotion()
{
	m_PotionType = EPotion_Type::MP;
	m_PotionAmount = 1;
}

CMPPotion::~CMPPotion()
{
}

void CMPPotion::Start()
{
	CPotion::Start();
}

bool CMPPotion::Init()
{
	if (!CPotion::Init()) return false;

	return true;
}

void CMPPotion::Update(float DeltaTime)
{
	CPotion::Update(DeltaTime);
}

void CMPPotion::PostUpdate(float DeltaTime)
{
	CPotion::PostUpdate(DeltaTime);
}

void CMPPotion::Collision(float DeltaTime)
{
	CPotion::Collision(DeltaTime);
}

void CMPPotion::Render(HDC hDC)
{
	CPotion::Render(hDC);
}

CMPPotion* CMPPotion::Clone()
{
	return new CMPPotion(*this);
}
