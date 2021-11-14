#pragma once
#include"imguiwindow.h"
#include<string>
#include<memory>
#include"external/glm/glm.hpp"

struct SDL_Window;
using SDL_GLContext = void*;
namespace banana::graphics {
	class Framebuffer;
	class VertexArray;
	class Shader;
}
namespace banana::core
{
	struct WindowProperties
	{
		std::string title;
		int x, y, w, h;
		int wMin, hMin;
		float aspectRatio;
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

		inline void setShouldRenderToScreen(bool shouldRender) { m_ShouldRendertoScreen = shouldRender; }

		inline SDL_Window* getSDLWindow() { return m_Window; }
		inline SDL_GLContext getGLContext() { return m_GLContext; }
		inline graphics::Framebuffer* getFramebuffer() { return m_Framebuffer.get(); }
		inline ImguiWindow& getImguiWindow() { return m_ImguiWindow; }
		void pumpEvents();
		
		void beginRender();
		void endRender();

		glm::ivec2 getSizeInAspectRatio(int width, int height, float aspectRatio = -1.f);
	private:
		void InitializeScreenRender();
		void RenderToScreen();
		void handleResize(int width, int height);
	private:
		WindowProperties m_WindowProps;;
		SDL_Window* m_Window;
		SDL_GLContext m_GLContext;
		ImguiWindow m_ImguiWindow;
		std::shared_ptr<graphics::Framebuffer> m_Framebuffer;
		
		//screen render
		bool m_ShouldRendertoScreen;
		glm::vec2 m_FramebufferSize;
		std::shared_ptr<graphics::VertexArray> m_ScreenVa;
		std::shared_ptr<graphics::Shader> m_ScreenShader;
	};
}