#include "BossGun.h"

CBossGun::CBossGun()
{
    m_FireTimeMax = 3.f;
}

CBossGun::CBossGun(const CBossGun& obj) : CGun(obj)
{
}

CBossGun::~CBossGun()
{
}

void CBossGun::Start()
{
}

bool CBossGun::Init()
{
    return false;
}

void CBossGun::Update(float DeltaTime)
{
}

void CBossGun::PostUpdate(float DeltaTime)
{
}

void CBossGun::Collision(float DeltaTime)
{
}

void CBossGun::Render(HDC hDC)
{
}

CBossGun* CBossGun::Clone()
{
    return nullptr;
}
