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

		bool create();
		void shutdown();
		
		void getSize(int& w, int& h);
		void pumpEvents();
		
		void beginRender();
		void endRender();
	private:
		SDL_Window* m_Window;
		SDL_GLContext m_GLContext;
	};
}