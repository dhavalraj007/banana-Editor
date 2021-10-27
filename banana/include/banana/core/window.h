#pragma once
#include"imguiwindow.h"
#include<string>
#include<memory>
#include"external/glm/glm.hpp"

struct SDL_Window;
using SDL_GLContext = void*;
namespace banana::graphics {
	class Framebuffer;
}
namespace banana::core
{
	struct WindowProperties
	{
		std::string title;
		int x, y, w, h;
		int wMin, hMin;
		int flags;
		glm::vec4 clearColor;
		ImguiWindowProperties imguiProps;

		WindowProperties();
	};

	class Window
	{
	public:
		Window();
		~Window();

		bool create(const WindowProperties& props);
		void shutdown();
		
		glm::ivec2 getSize();

		inline SDL_Window* getSDLWindow() { return m_Window; }
		inline SDL_GLContext getGLContext() { return m_GLContext; }
		inline graphics::Framebuffer* getFramebuffer() { return m_Framebuffer.get(); }
		inline ImguiWindow& getImguiWindow() { return m_ImguiWindow; }
		void pumpEvents();
		
		void beginRender();
		void endRender();
	private:
		SDL_Window* m_Window;
		SDL_GLContext m_GLContext;
		ImguiWindow m_ImguiWindow;
		std::shared_ptr<graphics::Framebuffer> m_Framebuffer;
	};
}