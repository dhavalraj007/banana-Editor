#pragma once
#include<array>

namespace banana::input
{
	class Mouse
	{
	public:
		static void initialize();
		static void update();

		static bool button(int button);
		static bool buttonDown(int button);
		static bool buttonUp(int button);

		inline static int X() { return x; }
		inline static int Y() { return y; }
		inline static int DX() { return x - xLast; }
		inline static int DY() { return y - yLast; }
		
		constexpr static const int buttonCount = 5;	// SDL supports 5 mouse buttons
	private:


		static int x, xLast;
		static int y, yLast;
		
		static std::array<bool, buttonCount> buttons;
		static std::array<bool, buttonCount> buttonsLast;

	};
}

enum
{
	BANANA_INPUT_MOUSE_FIRST = 1,
	BANANA_INPUT_MOUSE_LEFT = BANANA_INPUT_MOUSE_FIRST,
	BANANA_INPUT_MOUSE_MIDDLE = 2,
	BANANA_INPUT_MOUSE_RIGHT = 3,
	BANANA_INPUT_MOUSE_X1 = 4,
	BANANA_INPUT_MOUSE_X2 = 5,
	BANANA_INPUT_MOUSE_LAST = 5,
};