#include"banana/input/mouse.h"
#include"banana/log.h"
#include"SDL2/SDL_Mouse.h"
#include<algorithm>

namespace banana::input
{
	int Mouse::x = 0, Mouse::xLast = 0;
	int Mouse::y = 0, Mouse::yLast = 0;

	std::array<bool, Mouse::buttonCount> Mouse::buttons;
	std::array<bool, Mouse::buttonCount> Mouse::buttonsLast;
	
	void Mouse::initialize()
	{
		std::fill(Mouse::buttons.begin(), Mouse::buttons.end(), false);
		std::fill(Mouse::buttonsLast.begin(), Mouse::buttonsLast.end(), false);
	}

	void Mouse::update()
	{
		xLast = x;
		yLast = y;

		Uint32 state = SDL_GetMouseState(&x, &y);

		for (int i = 0; i < buttonCount; i++)
		{
			buttonsLast[i] = buttons[i];
			buttons[i] = state & SDL_BUTTON(i+1);
		}
		
	}

	bool Mouse::button(int button)
	{
		BANANA_ASSERT(button >= BANANA_INPUT_MOUSE_FIRST && button <= BANANA_INPUT_MOUSE_LAST, "Invalid Mouse Button!");
		if (button >= BANANA_INPUT_MOUSE_FIRST && button <= BANANA_INPUT_MOUSE_LAST)
		{
			return buttons[button - 1];
		}
		return false;
	}
	bool Mouse::buttonDown(int button)
	{
		BANANA_ASSERT(button >= BANANA_INPUT_MOUSE_FIRST && button <= BANANA_INPUT_MOUSE_LAST, "Invalid Mouse Button!");
		if (button >= BANANA_INPUT_MOUSE_FIRST && button <= BANANA_INPUT_MOUSE_LAST)
		{
			return buttons[button - 1] && !buttonsLast[button - 1];
		}
		return false;
	}
	bool Mouse::buttonUp(int button)
	{
		BANANA_ASSERT(button >= BANANA_INPUT_MOUSE_FIRST && button <= BANANA_INPUT_MOUSE_LAST, "Invalid Mouse Button!");
		if (button >= BANANA_INPUT_MOUSE_FIRST && button <= BANANA_INPUT_MOUSE_LAST)
		{
			return !buttons[button - 1] && buttonsLast[button - 1];
		}
		return false;
	}
}