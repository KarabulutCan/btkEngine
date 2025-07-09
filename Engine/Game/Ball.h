#pragma once
#include "../Core/PhysicsSystem.h"
#include "../Core/RenderQueue.h"
#include <SDL2/SDL.h>

class Ball {
public:
    void Init(PhysicsSystem& phys, float px, float py, float radius);
    void Reset(float px, float py);
    void Launch(float velX, float velY);
    void Update();
    void Draw(RenderQueue& rq, SDL_Texture* tex, int z);
    b2Body* Body() const { return m_Body; }
    SDL_Rect GetRect() const { return m_Dst; }
private:
    b2Body* m_Body = nullptr;
    SDL_Rect m_Dst{ 0,0,0,0 };
};