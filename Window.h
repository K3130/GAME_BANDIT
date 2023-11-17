#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

// Для барабанов
const int DRUM_WIDTH = 100;
const int DRUM_HEIGHT = 100;
const int NUM_DRUMS = 5;
const int NUM_TEXTURES = 3;
// --

struct FPS
{
	Uint32 m_prevTime = SDL_GetTicks();
	Uint32 m_frames = 0;
	SDL_Rect m_fpsRect;
	SDL_Surface* m_fpsSurface = nullptr;
	SDL_Texture* m_fpsTexture = nullptr;
	void setRect(int x, int y, int w, int h) {
		m_fpsRect.x = x; m_fpsRect.y = y; m_fpsRect.w = w; m_fpsRect.h = h;
	}
};

struct Drum
{
	int x;
	int y;
	int m_currentTexture;
	int m_rotation;
	int m_speed;
	std::vector<SDL_Texture*> m_textures;
};

class Window
{
public:
	Window(const std::string& aTitle, int w, int h);
	~Window();
	bool loadResources();
	void runEventLoop();

private:
	void drawBackground();
	void drawFPS();
	void drawButton(int r, int g, int b);
	void drawDrum();
	void updateDrums(std::vector<Drum>& aDrums);
	void printPosition();
	void handleButton();

private:
	SDL_Window* m_window = nullptr;
	SDL_Renderer* m_renderer = nullptr;
	TTF_Font* m_font = nullptr;
	FPS m_fps;
	std::vector<Drum> m_drums;
	std::vector<int> m_drumSpeeds{5,4,3,2,1};

private:
	// Фон
	SDL_Surface* m_backgroundSurface = nullptr;
	SDL_Texture* m_backgroundTexture = nullptr;
	// Кнопка
	SDL_Rect m_buttonRect = {};
	// Для эфекта мигания кнопки
	int m_g = 105;
};

