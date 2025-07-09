#include "Ball.h"

void Ball::Init(PhysicsSystem& phys, float px, float py, float radius) {
    m_Body = phys.CreateCircle(px, py, radius, true, 1.f, 1.f);
    if (m_Body) {
        m_Body->SetBullet(true);
        m_Body->SetFixedRotation(true);
    }
    m_Dst.w = m_Dst.h = static_cast<int>(radius * 2.f);
    Reset(px, py);
}

void Ball::Reset(float px, float py) {
    if (m_Body)
        m_Body->SetTransform({ px / PPM, py / PPM }, 0);
    if (m_Body)
        m_Body->SetLinearVelocity({ 0,0 });
}

void Ball::Launch(float velX, float velY) {
    if (m_Body)
        m_Body->SetLinearVelocity({ velX / PPM, velY / PPM });
}

void Ball::Update() {
    if (!m_Body) return;
    b2Vec2 pos = m_Body->GetPosition();
    m_Dst.x = static_cast<int>(pos.x * PPM - m_Dst.w * 0.5f);
    m_Dst.y = static_cast<int>(pos.y * PPM - m_Dst.h * 0.5f);
}

void Ball::Draw(RenderQueue& rq, SDL_Texture* tex, int z) {
    rq.Add({ tex, m_Dst, SDL_Rect{0,0,0,0}, z });
}