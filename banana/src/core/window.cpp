#include"banana/core/window.h"
#include"banana/engine.h"
#include"banana/log.h"
#include"banana/app.h"
#include"banana/input/mouse.h"
#include"banana/input/keyboard.h"
#include"sdl2/SDL.h"
#include"glad/glad.h"

#include<iostream>

namespace banana::core
{
	//public
	WindowProperties::WindowProperties()
	{
		title = "Banana App!";
		x = SDL_WINDOWPOS_CENTERED;
		y = SDL_WINDOWPOS_CENTERED;
		w = 1024;
		h = 576;
		wMin = 320;
		hMin = 180;
		
		flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
		ccR = static_cast<float>(0x64) / static_cast<float>(0xFF);
		ccG = static_cast<float>(0x95) / static_cast<float>(0xFF);
		ccB = static_cast<float>(0xED) / static_cast<float>(0xFF);
	}

	Window::Window():m_Window(nullptr){}
	Window::~Window()
	{
		if (m_Window)
		{
			shutdown();
		}
	}

	


	bool Window::create(const WindowProperties& props)
	{
		m_Window = SDL_CreateWindow(props.title.c_str(), props.x, props.y,props.w, props.h, props.flags);
		if (!m_Window)
		{
			BANANA_ERROR("ERROR Creating Window: {}",SDL_GetError());
			return false;
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		SDL_SetWindowMinimumSize(m_Window, props.wMin, props.hMin);
		
		m_GLContext = SDL_GL_CreateContext(m_Window);

		if (m_GLContext == nullptr)
		{
			BANANA_ERROR("ERROR creating OpenGL context: {}", SDL_GetError());
			return false;
		}

		gladLoadGLLoader(SDL_GL_GetProcAddress);
		Engine::Instance().getRenderManager().setClearColor(props.ccR, props.ccG, props.ccB, 1.f);

		m_ImguiWindow.create(props.imguiProps);

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
		if (!m_ImguiWindow.wantCaptureMouse())
		{
			input::Mouse::update();
		}
		if (m_ImguiWindow.wantCaptureMouse())
		{
			input::Keyboard::update();
		}
		
	}

	void Window::beginRender()
	{
		Engine::Instance().getRenderManager().clear();
	}

	void Window::endRender()
	{

		m_ImguiWindow.beginRender();

		Engine::Instance().getApp().imguiRender();

		m_ImguiWindow.endRender();
		SDL_GL_SwapWindow(m_Window);
	}
}