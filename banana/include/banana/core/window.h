#pragma once

struct SDL_Window;
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
	};
}