#pragma once

namespace banana
{
	class App
	{
	public:
		App() {}
		~App() {}

		virtual void initialize() {}
		virtual void shutdown() {}
		virtual void update() {}
		virtual void render() {}


	private:
	};
}