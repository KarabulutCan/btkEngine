#include "Paddle.h"

void Paddle::Init(PhysicsSystem& phys, float px, float py) {
	float w = 16.f, h = 96.f; //Paddle Boyutu
	m_Body = phys.CreateBox(px, py, w, h, true, 1.f, 0.2f);

	m_Body->SetType(b2_kinematicBody);

	m_Body->SetFixedRotation(true);
	m_Body->SetBullet(true);

	m_Dst.w = static_cast<int>(w);
	m_Dst.h = static_cast<int> (h);
}

void Paddle::Update(const Uint8* keys, float dt, SDL_Scancode keyUp, SDL_Scancode keyDown, float speed, float minY, float maxY) {
	int dir = 0;
	if (keys[keyUp]) dir = -1;
	if (keys[keyDown]) dir = 1;

	b2Vec2 vel = m_Body->GetLinearVelocity();
	vel.y = dir * (speed / PPM);
	m_Body->SetLinearVelocity(vel);

	b2Vec2 pos = m_Body->GetPosition();
	float halffH = m_Dst.h * 0.5f;
	float minPos = (minY + halffH) / PPM;
	float maxPos = (maxY - halffH) / PPM;

	if (pos.y < minPos) {
		pos.y = minPos;
		m_Body->SetTransform(pos, 0);
		m_Body->SetLinearVelocity({ 0,0 });
	}

	else if (pos.y > maxPos) {
		pos.y = maxPos;
		m_Body->SetTransform(pos, 0);
		m_Body->SetLinearVelocity({ 0,0 });
	}

	m_Dst.x = static_cast<int> (pos.x * PPM - m_Dst.w * 0.5f);
	m_Dst.y = static_cast<int> (pos.y * PPM - m_Dst.h * 0.5f);


}

void Paddle::Draw(RenderQueue& rq, SDL_Texture* tex, int z) {
	rq.Add({ tex, m_Dst, SDL_Rect{0,0,0,0},z });
}