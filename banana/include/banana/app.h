#pragma once
#include"banana/core/window.h"

namespace banana
{
	class App
	{
	public:
		App() {}
		~App() {}

		virtual core::WindowProperties getWindowProperties() { return core::WindowProperties(); }
		virtual void initialize() {}
		virtual void shutdown() {}
		virtual void update(float deltatime) {}
		virtual void render() {}
		virtual void imguiRender() {}

	private:
	};
}