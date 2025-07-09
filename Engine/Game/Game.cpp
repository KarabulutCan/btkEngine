#include "Game.h"
#include <iostream>
#include <SDL2/SDL.h>
#include "Ball.h"

constexpr int TARGET_FPS = 60;
constexpr int FRAME_DELAY = 1000 / TARGET_FPS;

using namespace std;
Game::Game() {}
Game::~Game() { Shutdown(); }

bool Game::Init(const string& title, int widht, int height) {

	//Physics System
	m_Physics.Init(PPM, 0.f, 0.f);


	m_WindowWidth = widht;
	m_WindowHeight = height;

	//Start-Baslangic
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		cerr << "SDL_Init Failed" << SDL_GetError() << endl;
		return false;
	}

	m_Window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_WindowWidth, m_WindowHeight, SDL_WINDOW_SHOWN);

	// Window-Pencere
	if (!m_Window) {
		cerr << "Window creation failed: " << SDL_GetError() << endl;
		return false;
	}

	//Renderer
	m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!m_Renderer) {
		cerr << "Renderer creation failed: " << SDL_GetError() << endl;
		return false;
	}
	//Textures
	m_Assets.Init(m_Renderer);
	//Fonts
	m_Fonts.Init(m_Renderer);
	//Paddles
	SDL_Texture* paddleTex = m_Assets.GetTexture("paddle", "Assets/Textures/paddle.png");
	m_PaddleL.Init(m_Physics, 32, m_WindowHeight / 2.f);
	m_PaddleR.Init(m_Physics, m_WindowWidth - 32, m_WindowHeight / 2.f);
	//Ball
	m_Ball.Init(m_Physics, m_WindowWidth / 2.f, m_WindowHeight / 2.f, 8.f);
	m_Ball.Launch(-300.f, -150.f);
	//Walls
	m_TopWall = m_Physics.CreateBox(m_WindowWidth / 2.f, 5.f, m_WindowWidth, 10.f, false);
	m_BottomWall = m_Physics.CreateBox(m_WindowWidth / 2.f, m_WindowHeight - 5.f, m_WindowWidth, 10.f, false);

	//Sounds
	m_Sounds.Init();
	m_Sounds.GetSound("background", "Assets/Sounds/pixel.mp3");
	m_Sounds.Play("background");

	m_ScoreTexL = nullptr;
	m_ScoreTexR = nullptr;
	m_ScoreLeft = 0;
	m_ScoreRight = 0;

	m_isRunning = true;
	return true;
}

void Game::Run() {
	Uint64 now = SDL_GetPerformanceCounter();
	Uint64 freq = SDL_GetPerformanceFrequency();
	Uint32 fpsTimer = SDL_GetTicks();
	int frameCnt = 0;

	while (m_isRunning) {

		Uint64 newNow = SDL_GetPerformanceCounter();
		float dt = static_cast <float> (newNow - now) / freq;
		now = newNow;
		frameCnt++;

		Uint32 frameStart = SDL_GetTicks();

		ProcessInput();
		Update(dt); 
		Render();

		//60 FPS'e kadar beklet
		Uint32 elapsed = SDL_GetTicks() - frameStart;
		if (elapsed < FRAME_DELAY)
			SDL_Delay(FRAME_DELAY - elapsed);

	}
}

void Game::ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
	
		switch (event.type) {
		case SDL_QUIT:
			cout << "[EVENT] Quit event" << endl;
			m_isRunning = false;
			break;

		case SDL_KEYDOWN:
			cout << "[EVENT] Key pressed: " << SDL_GetKeyName(event.key.keysym.sym) << endl;
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				m_isRunning = false; 
			}
			break;

		case SDL_KEYUP:
			cout << "[EVENT] Key realesed: " << SDL_GetKeyName(event.key.keysym.sym) << endl;
			break;

		case SDL_MOUSEMOTION:
			cout << "[EVENT] Mouse moved: " << event.motion.x << ", " << event.motion.y << endl;
			break;
		case SDL_MOUSEBUTTONDOWN:
			cout << "[EVENT] Mouse button pressed: " << (int) event.button.button << "X=" << event.button.x<< "Y="<< event.button.y<< endl;
			break;
		default:
			break;
		}

		const Uint8* keys = SDL_GetKeyboardState(nullptr);

	}
}

void Game::Update(float dt) {

	m_Physics.Step(dt);
	const Uint8* keys = SDL_GetKeyboardState(nullptr);
	m_PaddleL.Update(keys, dt, SDL_SCANCODE_W, SDL_SCANCODE_S, 800.f,0.f,static_cast<float>(m_WindowHeight));
	m_PaddleR.Update(keys, dt, SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, 800.f, 0.f, static_cast<float>(m_WindowHeight));


	m_Ball.Update();

	b2Vec2 pos = m_Ball.Body()->GetPosition();
	if (pos.x * PPM < 0) {
		m_ScoreRight++;
		m_Ball.Reset(m_WindowWidth / 2.f, m_WindowHeight / 2.f);
		m_Ball.Launch(300.f, -150.f);
	}
	else if (pos.x * PPM > m_WindowWidth) {
		m_ScoreLeft++;
		m_Ball.Reset(m_WindowWidth / 2.f, m_WindowHeight / 2.f);
		m_Ball.Launch(-300.f, 150.f);
	}

	//update score textures
	SDL_Color col{ 255,255,255,255 };
	m_ScoreTexL = m_Fonts.GetTextTexture(to_string(m_ScoreLeft), "Assets/Fonts/roboto.ttf", 24, col);
	m_ScoreTexR = m_Fonts.GetTextTexture(to_string(m_ScoreRight), "Assets/Fonts/roboto.ttf", 24, col);

	
}

void Game::Render() {

	SDL_SetRenderDrawColor(m_Renderer, 125, 30, 200, 255); //Arkaplan 
	SDL_RenderClear(m_Renderer); //Arkaplanı temizle
	m_RQ.Clear();

	SDL_Texture* paddleTex = m_Assets.GetTexture("paddle");
	m_PaddleL.Draw(m_RQ, paddleTex, 10);
	m_PaddleR.Draw(m_RQ, paddleTex, 10);
	SDL_Texture* brickTex = m_Assets.GetTexture("brick", "Assets/Textures/brick.png");
	m_Ball.Draw(m_RQ, brickTex, 5);

	if (m_ScoreTexL && m_ScoreTexR) {
		int w1, h1, w2, h2;
		SDL_QueryTexture(m_ScoreTexL, nullptr, nullptr, &w1, &h1);
		SDL_QueryTexture(m_ScoreTexR, nullptr, nullptr, &w2, &h2);
		SDL_Rect dl{ m_WindowWidth / 4 - w1 / 2, 20, w1, h1 };
		SDL_Rect dr{ 3 * m_WindowWidth / 4 - w2 / 2, 20, w2, h2 };
		m_RQ.Add({ m_ScoreTexL, dl, SDL_Rect{0,0,0,0}, 1 });
		m_RQ.Add({ m_ScoreTexR, dr, SDL_Rect{0,0,0,0}, 1 });
	}


	m_RQ.Flush(m_Renderer);
	SDL_RenderPresent(m_Renderer); //Render et - //Double-buffering
}

void Game::Shutdown() {

	//assets
	m_Assets.Shutdown();
	m_Fonts.Shutdown();

	if (m_Renderer) { SDL_DestroyRenderer(m_Renderer); 
	
	m_Renderer = nullptr;//Renderer yoksa yok et
	} 

	if (m_Window) {
		SDL_DestroyWindow(m_Window);
		m_Window = nullptr; //Window yoksa yok et
	};

	m_Physics.Shutdown();

	SDL_Quit();



}