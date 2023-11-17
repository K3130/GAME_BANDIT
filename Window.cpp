#include "Window.h"

Window::Window(const std::string& aTitle, int w, int h)
{
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	m_window = SDL_CreateWindow(aTitle.c_str(), SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
	m_font = TTF_OpenFont("arial.ttf", 16);
}

Window::~Window()
{
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	TTF_CloseFont(m_font);
	IMG_Quit();
	SDL_Quit();
}

bool Window::loadResources()
{
	// «агрузка фонового изображени€
	m_backgroundSurface = IMG_Load("res/back.png");
	if (!m_backgroundSurface)
		return false;
	m_backgroundTexture = SDL_CreateTextureFromSurface(m_renderer, m_backgroundSurface);
	if(!m_backgroundTexture)
		return false;

	// «агрузка текстур дл€ барабанов
	std::vector<std::vector<std::string>> texturePaths = {
		{ "res/01spin.png", "res/11spin.png", "res/21spin.png" },
		{ "res/02spin.png", "res/12spin.png", "res/22spin.png" },
		{ "res/03spin.png", "res/13spin.png", "res/23spin.png" },
		{ "res/04spin.png", "res/14spin.png", "res/24spin.png" },
		{ "res/05spin.png", "res/15spin.png", "res/25spin.png" } };

	int totalDrumsWidth = NUM_DRUMS * DRUM_WIDTH;
	int totalSpacingWidth = (NUM_DRUMS - 1) * 20;
	int totalWidth = totalDrumsWidth + totalSpacingWidth;
	int startX = (SDL_GetWindowSurface(m_window)->w - totalWidth) / 2;
	int startY = (SDL_GetWindowSurface(m_window)->h - (DRUM_HEIGHT * 3)) / 2;


	for (int r = 0; r < 3; r++) // ÷икл дл€ формировани€ трех р€дов барабанов
	{
		for (int i = 0; i < NUM_DRUMS; i++) // ÷икл дл€ формировани€ барабанов в р€ду
		{
			Drum drum;
			drum.x = startX + i * (DRUM_WIDTH + 20);

			drum.y = startY + r * DRUM_HEIGHT;

			drum.m_currentTexture = 0;
			drum.m_rotation = 0;
			drum.m_speed = m_drumSpeeds[i] * 10;

			for (int j = 0; j < NUM_TEXTURES; j++)
			{
				SDL_Texture* tmp = IMG_LoadTexture(m_renderer, texturePaths[i][j].c_str());
				if (!tmp)
					return false;
				drum.m_textures.push_back(tmp);
			}

			m_drums.push_back(drum);
		}
	}

	return true;
}

void Window::runEventLoop()
{
	bool isStopping1 = false;
	bool isStopping2 = false;
	bool isStopping3 = false;
	bool isStopping4 = false;
	bool isStopping5 = false;

	SDL_Event event;
	bool running = true;
	bool rotating = false;
	bool isFinishRotating = true;
	Uint32 startTime = 0;
	int mouseX, mouseY;

	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = false;
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				SDL_GetMouseState(&mouseX, &mouseY);
				if (mouseX >= m_buttonRect.x && mouseX <= m_buttonRect.x + m_buttonRect.w &&
					mouseY >= m_buttonRect.y && mouseY <= m_buttonRect.y + m_buttonRect.h) {
					
					// ≈сли все барабаны сто€т на месте, запускаем
					if (isFinishRotating)
					{
						rotating = true;
						isFinishRotating = false;
						startTime = SDL_GetTicks();
					}
										
				}
			}
		}

		if (rotating)
		{
			Uint32 currentTime = SDL_GetTicks();
			Uint32 elapsedTime = currentTime - startTime;

			// ”становка времени вращени€ барабанов
			if (elapsedTime >= 5000)
			{
				rotating = false;
				isFinishRotating = true;
				printPosition();
			}
			else
			{

				handleButton();
								
				// ќстановка барабанов по очереди в 1 секунду
				if (!isStopping1 && elapsedTime >= 1000)
				{
					m_drums[0].m_speed = 0;
					m_drums[5].m_speed = 0;
					m_drums[10].m_speed = 0;
					isStopping1 = true;
				}
				//-------------------------------------------------------
				if (!isStopping2 && elapsedTime >= 2000)
				{
					m_drums[1].m_speed = 0;
					m_drums[6].m_speed = 0;
					m_drums[11].m_speed = 0;
					isStopping2 = true;
				}
				//-------------------------------------------------------
				if (!isStopping3 && elapsedTime >= 3000)
				{
					m_drums[2].m_speed = 0;
					m_drums[7].m_speed = 0;
					m_drums[12].m_speed = 0;
					isStopping3 = true;
				}
				//-------------------------------------------------------
				if (!isStopping4 && elapsedTime >= 4000)
				{
					m_drums[3].m_speed = 0;
					m_drums[8].m_speed = 0;
					m_drums[13].m_speed = 0;
					isStopping4 = true;
				}
				//-------------------------------------------------------
				if (!isStopping5 && elapsedTime >= 5000)
				{
					m_drums[4].m_speed = 0;
					m_drums[9].m_speed = 0;
					m_drums[14].m_speed = 0;
					isStopping5 = true;
				}
				
			}
		}
		//¬озобновление работы барабанов
		if (!rotating)
		{
			m_drums[0].m_speed = m_drumSpeeds[0] * 10;
			m_drums[5].m_speed = m_drumSpeeds[0] * 10;
			m_drums[10].m_speed = m_drumSpeeds[0] * 10;
			isStopping1 = false;

		    m_drums[1].m_speed = m_drumSpeeds[1] * 10;
			m_drums[6].m_speed = m_drumSpeeds[1] * 10;
			m_drums[11].m_speed = m_drumSpeeds[1] * 10;
			isStopping2 = false;

			m_drums[2].m_speed = m_drumSpeeds[2] * 10;
			m_drums[7].m_speed = m_drumSpeeds[2] * 10;
			m_drums[12].m_speed = m_drumSpeeds[2] * 10;
			isStopping3 = false;

			m_drums[3].m_speed = m_drumSpeeds[3] * 10;
			m_drums[8].m_speed = m_drumSpeeds[3] * 10;
			m_drums[13].m_speed = m_drumSpeeds[3] * 10;
			isStopping4 = false;

			m_drums[4].m_speed = m_drumSpeeds[4] * 10;
			m_drums[9].m_speed = m_drumSpeeds[4] * 10;
			m_drums[14].m_speed = m_drumSpeeds[4] * 10;
			isStopping5 = false;
		}

		// ќчистка рендера
		SDL_RenderClear(m_renderer);

		//----------
		drawBackground();
		drawFPS();
		drawButton(255, m_g, 180);
		drawDrum();
		//----------

		// ќбновление рендера
		SDL_RenderPresent(m_renderer);
	}
}

void Window::drawBackground()
{
	SDL_RenderCopy(m_renderer, m_backgroundTexture, nullptr, nullptr);
}

void Window::drawFPS()
{
	m_fps.setRect(SDL_GetWindowSurface(m_window)->w - 150, SDL_GetWindowSurface(m_window)->h - 90, 100, 50);
	m_fps.m_frames++;
	Uint32 currentTime = SDL_GetTicks();
	Uint32 elapsedTime = currentTime - m_fps.m_prevTime;
	if (elapsedTime >= 1)
	{
		std::stringstream ss;
		ss << "FPS: " << int(m_fps.m_frames / (elapsedTime / 1000.0f));
		std::string fpsString = ss.str();

		m_fps.m_fpsSurface = TTF_RenderText_Solid(m_font, fpsString.c_str(), { 255, 255, 255 });
		m_fps.m_fpsTexture = SDL_CreateTextureFromSurface(m_renderer, m_fps.m_fpsSurface);
		SDL_FreeSurface(m_fps.m_fpsSurface);
		

		SDL_RenderCopy(m_renderer, m_fps.m_fpsTexture, nullptr, &m_fps.m_fpsRect);
		m_fps.m_prevTime = currentTime;
		m_fps.m_frames = 0;
		SDL_DestroyTexture(m_fps.m_fpsTexture);
	}
}

void Window::drawButton(int r, int g, int b)
{
	if (m_g > 0) {
		m_g -= 1;
	}
	else {
		m_g = 255;
	}
	SDL_Surface* surface = TTF_RenderText_Solid(m_font, "Click Me", { 255, 255, 255 });
	SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
	SDL_FreeSurface(surface);

	int buttonWidth = 100;
	int buttonHeight = 50;
	m_buttonRect.x = SDL_GetWindowSurface(m_window)->w - buttonWidth - 10;
	m_buttonRect.y = 10;
	m_buttonRect.w = buttonWidth;
	m_buttonRect.h = buttonHeight;

	SDL_SetRenderDrawColor(m_renderer, r, g, b, 255);
	SDL_RenderFillRect(m_renderer, &m_buttonRect);

	SDL_RenderCopy(m_renderer, texture, nullptr, &m_buttonRect);
	SDL_DestroyTexture(texture);
}

void Window::handleButton()
{
	updateDrums(m_drums);
}

void Window::drawDrum()
{
	for (size_t i = 0; i < m_drums.size(); i++)
	{
		SDL_Rect drumRect = { m_drums[i].x, m_drums[i].y, DRUM_WIDTH, DRUM_HEIGHT };
		SDL_RenderCopy(m_renderer, m_drums[i].m_textures[m_drums[i].m_currentTexture], nullptr, &drumRect);

		// ѕроверка индекса текущей текстуры
		if (m_drums[i].m_currentTexture == 0)
		{
			// »зменение индекса текущей текстуры дл€ других р€дов и барабанов
			if (i + NUM_DRUMS < m_drums.size())
			{
				m_drums[i + NUM_DRUMS].m_currentTexture = 1;
			}

			if (i + (NUM_DRUMS * 2) < m_drums.size())
			{
				m_drums[i + (NUM_DRUMS * 2)].m_currentTexture = 2;
			}
		}
	}
}

// ¬ывод в терминал текущие индексы значений на барабанах
void Window::printPosition()
{
	for (int r = 0; r < 3; r++) {
		for (int i = 0; i < NUM_DRUMS; i++) {
			std::cout << m_drums[i + r * NUM_DRUMS].m_currentTexture;

			if (i != NUM_DRUMS - 1) {
				std::cout << ", ";
			}
		}

		std::cout << std::endl;
	}
}

// ”величивает угол поворота барабана на его скорость вращени€,
// если угол поворота достиг значени€ 360 градусов,
// устанавливаетс€ следующа€ текстура на барабане, и если это последн€€ текстура, то устанавливаетс€ перва€ текстура. 
void Window::updateDrums(std::vector<Drum>& aDrums)
{
	for (Drum& drum : aDrums)
	{
		drum.m_rotation += drum.m_speed;

		if (drum.m_rotation >= 360)
		{
			drum.m_rotation = 0;

			drum.m_currentTexture++;
			if (drum.m_currentTexture >= NUM_TEXTURES)
			{
				drum.m_currentTexture = 0;
			}
		}
	}
}


