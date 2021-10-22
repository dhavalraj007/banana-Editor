#pragma once

typedef union SDL_Event SDL_Event;
namespace banana::core
{
	struct ImguiWindowProperties
	{
		bool moveFromTitleBarOnly = true;
		bool isDockingEnable = false;
		bool isViewPortEnabled = false;
	};
	class ImguiWindow
	{
	public:
		ImguiWindow() {}
		~ImguiWindow() {}

		void create(const ImguiWindowProperties& props);
		void shutdown();

		void handleSDLEvent(SDL_Event& e);

		bool wantCaptureMouse();
		bool wantCaptureKeyboard();

		void beginRender();
		void endRender();
	};
}