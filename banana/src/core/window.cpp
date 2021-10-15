#include"banana/core/window.h"
#include"banana/engine.h"
#include"banana/log.h"
#include"banana/input/mouse.h"
#include"banana/input/keyboard.h"
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
		m_Window = SDL_CreateWindow("Banana Yumm", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
		if (!m_Window)
		{
			BANANA_ERROR("ERROR Creating Window: {}",SDL_GetError());
			return false;
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		SDL_SetWindowMinimumSize(m_Window, 200, 200);
		
		m_GLContext = SDL_GL_CreateContext(m_Window);

		if (m_GLContext == nullptr)
		{
			BANANA_ERROR("ERROR creating OpenGL context: {}", SDL_GetError());
			return false;
		}

		gladLoadGLLoader(SDL_GL_GetProcAddress);

		return true;
	}

	void Window::shutdown()
	{
		SDL_DestroyWindow(m_Window);
		SDL_GL_DeleteContext(m_GLContext);
		m_Window = nullptr;
	}

	void Window::getSize(int& w, int& h)
	{
		SDL_GetWindowSize(m_Window, &w, &h);
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
		//input update
		input::Mouse::update();
		input::Keyboard::update();
		
	}

	void Window::beginRender()
	{
		Engine::Instance().getRenderManager().clear();
	}

	void Window::endRender()
	{
		SDL_GL_SwapWindow(m_Window);
	}
}