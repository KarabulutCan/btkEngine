#pragma once
#include "../Core/PhysicsSystem.h"
#include "../Core/RenderQueue.h"
#include <SDL2/SDL.h>

class Paddle {
public:
    void Init(PhysicsSystem& phys, float px, float py);
    void Update(const Uint8* keys, float dt, SDL_Scancode keyUp, SDL_Scancode keyDown, float speed,float minY, float maxY);
    void Draw(RenderQueue& rq, SDL_Texture* tex, int z);
private:
    b2Body* m_Body = nullptr;
    SDL_Rect m_Dst{ 0, 0, 0, 0 };
};