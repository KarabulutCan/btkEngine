#pragma once

#include <Box2D/Box2D.h>
constexpr float PPM = 100.f;
class PhysicsSystem {
public:
	bool Init(float ppm, float gravityX = 0.f, float gravityY = 0.f);
	void Shutdown();
	void Step(float dt);

	b2Body* CreateBox(float px, float py, float w, float h, bool dynamic, float density = 1.f, float restitution = 0.5f);
	b2Body* CreateCircle(float px, float py, float radius, bool dynamic, float density = 1.f, float restitution = 0.5f);

private:
	float m_PPM = 100.f;
	b2World* m_World = nullptr;
};
