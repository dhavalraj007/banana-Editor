#include"banana/log.h"
#include"glad/glad.h"

namespace banana::graphics
{
	void checkGLError();
}

#ifndef BANANA_CONFIG_RELEASE
#define BANANA_CHECK_GL_ERROR banana::graphics::checkGLError();
#else
#define BANANA_CHECK_GL_ERROR (void)0;
#endif