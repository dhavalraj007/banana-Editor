#pragma once

namespace banana::managers
{
	class LogManager
	{
	public:
		LogManager() = default;
		~LogManager() = default;

		void initialize();
		void shutdown();
	};
}