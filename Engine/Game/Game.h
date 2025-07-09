#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../Core/PhysicsSystem.h"
#include "../Core/RenderQueue.h"
#include <string>
#include "../Assets/AssetManager.h"
#include "../Core/Animation.h"
#include "../Assets/FontManager.h"
#include "../Assets/SoundManager.h"
#include "Paddle.h"
#include "Ball.h"

using namespace std;

class Game
{
public:
	Game();
	~Game();
	bool Init(const string& title, int width, int height); //baslangic
	void Run(); //game loop
	void Shutdown(); // bitis

	//funcs.
private:
	void ProcessInput();
	void Update(float deltaTime);
	void Render();

	//vars.
private:
	SDL_Window* m_Window;
	SDL_Renderer* m_Renderer;

	//Physics
	PhysicsSystem m_Physics;

	//vectors
	struct Vec2 { float x{ 0.f }, y{ 0.f }; };

	//Sorting
	RenderQueue m_RQ;
	//Asset
	Paddle m_PaddleL, m_PaddleR;
	Ball m_Ball;
	AssetManager m_Assets;
	//Fonts
	FontManager m_Fonts;

	//Walls
	b2Body* m_TopWall = nullptr;
	b2Body* m_BottomWall = nullptr;

	int m_ScoreLeft = 0;
	int m_ScoreRight = 0;
	SDL_Texture* m_ScoreTexL = nullptr;
	SDL_Texture* m_ScoreTexR = nullptr;


	//Sounds
	SoundManager m_Sounds;

	bool m_isRunning = false;
	int m_WindowWidth = 800;
	int m_WindowHeight = 600;

	//F11 ile tam ekran
	bool m_isFullScreen = false;
};