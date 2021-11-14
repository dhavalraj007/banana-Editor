#include"banana/input/keyboard.h"
#include"banana/log.h"
#include"SDL2/SDL_Keyboard.h"
#include<algorithm>

namespace banana::input
{

	std::array<bool, Keyboard::keyCount> Keyboard::keys;
	std::array<bool, Keyboard::keyCount> Keyboard::keysLast;

	void Keyboard::initialize()
	{
		std::fill(Keyboard::keys.begin(), Keyboard::keys.end(), false);
		std::fill(Keyboard::keysLast.begin(), Keyboard::keysLast.end(), false);
	}

	void Keyboard::update()
	{
		keysLast = keys;
		const Uint8* state = SDL_GetKeyboardState(nullptr);
		for (int i = BANANA_INPUT_KEY_FIRST; i < keyCount; i++)
		{
			keys[i] = state[i];
		}
	}

	bool Keyboard::key(int key)
	{
		BANANA_ASSERT(key >= BANANA_INPUT_KEY_FIRST && key < keyCount, "Invalid Keyboard key!");
		if (key >= BANANA_INPUT_KEY_FIRST && key < keyCount)
		{
			return keys[key];
		}
		return false;
	}
	bool Keyboard::keyDown(int key)
	{
		BANANA_ASSERT(key >= BANANA_INPUT_KEY_FIRST && key < keyCount, "Invalid Keyboard key!");
		if (key >= BANANA_INPUT_KEY_FIRST && key < keyCount)
		{
			return   keys[key] && !keysLast[key];
		}
		return false;
	}
	bool Keyboard::keyUp(int key)
	{
		BANANA_ASSERT(key >= BANANA_INPUT_KEY_FIRST && key < keyCount, "Invalid Keyboard key!");
		if (key >= BANANA_INPUT_KEY_FIRST && key < keyCount)
		{
			return !keys[key] && keysLast[key]; 
		}
		return false;
	}
}