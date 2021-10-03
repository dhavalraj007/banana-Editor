#include"core/window.h"
#include"engine.h"
#include"log.h"
#include"sdl2/SDL.h"
#include"glad/glad.h"
#include<iostream>

namespace banana::core
{
	//public
	Window::Window():m_Window(nullptr){}
	Window::~Window()
	{
		if (m_Window)
		{
			shutdown();
		}
	}

	bool Window::create()
	{
		m_Window = SDL_CreateWindow("Banana Yumm", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
		if (!m_Window)
		{
			BANANA_ERROR("ERROR Creating Window: {}",SDL_GetError());
			return false;
		}
		return true;
	}

	void Window::shutdown()
	{
		SDL_DestroyWindow(m_Window);
		m_Window = nullptr;
	}

	void Window::pumpEvents()
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				Engine::Instance().quit();
				break;
			default:
				break;
			}
		}
	}

}