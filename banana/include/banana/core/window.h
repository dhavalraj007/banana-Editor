#pragma once

struct SDL_Window;
using SDL_GLContext = void*;
namespace banana::core
{
	class Window
	{
	public:
		Window();
		~Window();

		//Calls SDL_CreateWindow
		bool create();
		void shutdown();
		void pumpEvents();

	private:
		SDL_Window* m_Window;
		SDL_GLContext m_GLContext;
	};
}