#pragma once
#include"banana/core/window.h"

namespace banana
{
	class App
	{
	public:
		App() {}
		~App() {}

		virtual core::WindowProperties GetWindowProperties() { return core::WindowProperties(); }
		virtual void initialize() {}
		virtual void shutdown() {}
		virtual void update() {}
		virtual void render() {}
		virtual void imguiRender() {}

	private:
	};
}