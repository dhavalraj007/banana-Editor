#include"banana/core/imguiwindow.h"
#include"banana/engine.h"
#include"SDL2/SDL.h"
#include"external/imgui/imgui.h"
#include"external/imgui/imgui_impl_sdl.h"
#include"external/imgui/imgui_impl_opengl3.h"

namespace banana::core
{
	void ImguiWindow::create(const ImguiWindowProperties& props)
	{
		IMGUI_CHECKVERSION();

		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigWindowsMoveFromTitleBarOnly = props.moveFromTitleBarOnly;
		if (props.isDockingEnable)
		{
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		}
		if (props.isViewPortEnabled)
		{
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		}

		auto& window = Engine::Instance().getWindow();
		ImGui_ImplSDL2_InitForOpenGL(window.getSDLWindow(), window.getGLContext());
		ImGui_ImplOpenGL3_Init("#version 140");
	}

	void ImguiWindow::shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}

	void ImguiWindow::handleSDLEvent(SDL_Event& e)
	{
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	bool ImguiWindow::wantCaptureMouse()
	{
		return ImGui::GetIO().WantCaptureMouse;
	}

	bool ImguiWindow::wantCaptureKeyboard()
	{
		return ImGui::GetIO().WantCaptureKeyboard;
	}

	void ImguiWindow::beginRender()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(Engine::Instance().getWindow().getSDLWindow());
		ImGui::NewFrame();

	}

	void ImguiWindow::endRender()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			auto& window = Engine::Instance().getWindow();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			SDL_GL_MakeCurrent(window.getSDLWindow(), window.getGLContext());
		}
	}
}