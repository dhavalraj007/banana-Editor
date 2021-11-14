#include"banana/core/window.h"
#include"banana/engine.h"
#include"banana/log.h"
#include"banana/app.h"
#include"banana/graphics/framebuffer.h"
#include"banana/graphics/vertex.h"
#include"banana/graphics/shader.h"
#include"banana/input/mouse.h"
#include"banana/input/keyboard.h"
#include"sdl2/SDL.h"
#include"glad/glad.h"
#include"external/glm/gtc/matrix_transform.hpp"
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
		aspectRatio = 16.0f / 9.0f;
		clearColor = glm::vec4( static_cast<float>(0x64) / static_cast<float>(0xFF),
								static_cast<float>(0x95) / static_cast<float>(0xFF),
								static_cast<float>(0xED) / static_cast<float>(0xFF),1);
	}

	Window::Window()
		:m_Window(nullptr)
		,m_ShouldRendertoScreen(true)
		{}
	Window::~Window()
	{
		if (m_Window)
		{
			shutdown();
		}
	}

	


	bool Window::create(const WindowProperties& props)
	{
		m_WindowProps = props;
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
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

		SDL_SetWindowMinimumSize(m_Window, props.wMin, props.hMin);
		
		m_GLContext = SDL_GL_CreateContext(m_Window);

		if (m_GLContext == nullptr)
		{
			BANANA_ERROR("ERROR creating OpenGL context: {}", SDL_GetError());
			return false;
		}

		gladLoadGLLoader(SDL_GL_GetProcAddress);

		m_Framebuffer = std::make_shared<graphics::Framebuffer>(props.w, props.h);
		m_Framebuffer->SetClearColor(props.clearColor);
		m_ImguiWindow.create(props.imguiProps);

		InitializeScreenRender();
		handleResize(props.w, props.h);
		return true;
	}

	void Window::shutdown()
	{
		m_ScreenVa.reset();
		m_ScreenShader.reset();
		SDL_DestroyWindow(m_Window);
		SDL_GL_DeleteContext(m_GLContext);
		m_Window = nullptr;
	}

	glm::ivec2 Window::getSize()
	{
		int w, h;
		SDL_GetWindowSize(m_Window, &w, &h);
		return glm::ivec2(w, h);
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
			case SDL_WINDOWEVENT:
				if (e.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					handleResize(e.window.data1,e.window.data2);
				}
				break;
			default:
				break;
			}
			m_ImguiWindow.handleSDLEvent(e);
		}
		//input update
		if (!m_ImguiWindow.wantCaptureMouse())
		{
			input::Mouse::update();
		}
		if (!m_ImguiWindow.wantCaptureKeyboard())
		{
			input::Keyboard::update();
		}
	}

	void Window::beginRender()
	{
		auto& rm = Engine::Instance().getRenderManager();
		rm.clear();
		rm.submit(BANANA_SUBMIT_RC(PushFramebuffer,m_Framebuffer));
	}

	void Window::endRender()
	{
		auto& rm = Engine::Instance().getRenderManager();
		rm.submit(BANANA_SUBMIT_RC(PopFramebuffer));
		rm.flush();

		if (m_ShouldRendertoScreen)
		{
			RenderToScreen();
		}

		m_ImguiWindow.beginRender();

		Engine::Instance().getApp().imguiRender();

		m_ImguiWindow.endRender();
		SDL_GL_SwapWindow(m_Window);
	}
	
	void Window::InitializeScreenRender()
	{
		m_ScreenVa = std::make_shared<graphics::VertexArray>();
		{
			BANANA_CRETE_VERTEX_BUFFER(vb, short);
			vb->pushVertex({  1,  1,   1, 1});
			vb->pushVertex({  1, -1,   1, 0});
			vb->pushVertex({ -1, -1,   0, 0});
			vb->pushVertex({ -1,  1,   0, 1});
			vb->setLayout({ 2,2 });
			vb->upload(false);
			m_ScreenVa->pushBuffer(std::move(vb));
		}
		m_ScreenVa->setElements({ 0, 1, 3, 3, 1, 2 });
		m_ScreenVa->upload();
	
		const char* vertexShader = R"(
	            #version 410 core
	            layout (location = 0) in vec2 position;
	            layout (location = 1) in vec2 texcoords;
	            out vec2 uvs;
	
	            uniform mat4 model = mat4(1.0);
	            void main()
	            {
	                uvs = texcoords;
	                gl_Position = model * vec4(position,0.0,1.0);
	            }
	              )";
	
		const char* fragShader = R"(
	            #version 410 core
	            out vec4 outColor;
	            in vec2 uvs;
	
	            uniform sampler2D tex;
	            void main()
	            {
	                outColor = texture(tex,uvs);
	            }
	              )";
	
		m_ScreenShader = std::make_shared<graphics::Shader>(vertexShader, fragShader);
	}
	void Window::RenderToScreen()
	{
		//black bars
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		BANANA_ASSERT(m_ScreenVa->isValid(), "Invalid VertexArray. - did you called upload onm vertexAray?");
		m_ScreenVa->bind();
		glDisable(GL_DEPTH_TEST);
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, m_Framebuffer->getTextureId());
		m_ScreenShader->bind();
		glm::vec2 scale = m_FramebufferSize / (glm::vec2)getSize();
		glm::mat4 model(1.f);
		model = glm::scale(model, { scale.x,scale.y,1.f });
		m_ScreenShader->setUniformMat4("model", model);
		
		glDrawElements(GL_TRIANGLES, m_ScreenVa->getElementCount(), GL_UNSIGNED_INT, 0);

		m_ScreenShader->unbind();
		glBindTexture(GL_TEXTURE_2D, 0);
		m_ScreenVa->unbind();
	}

	

	void Window::handleResize(int width, int height)
	{
		m_FramebufferSize = getSizeInAspectRatio(width,height,m_WindowProps.aspectRatio);
	}

	glm::ivec2 Window::getSizeInAspectRatio(int width, int height, float aspectRatio)
	{
		if (aspectRatio < 0) aspectRatio = m_WindowProps.aspectRatio;
		int FramebufferWidth = (int)(height * aspectRatio);
		int FramebufferHeight = (int)(width * (1.f / aspectRatio));

		if (height >= FramebufferHeight)
		{
			FramebufferWidth = width;
		}
		else
		{
			FramebufferHeight = height;
		}

		return { FramebufferWidth, FramebufferHeight };
	}
}