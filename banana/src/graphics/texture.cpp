#include "banana/graphics/texture.h"
#include "banana/graphics/helper.h"
#define STB_IMAGE_IMPLEMENTATION
#include"external/stb/stb_image.h"
#include"glad/glad.h"

banana::graphics::Texture::Texture(const std::string& name, const std::string& path, int texUnit)
	:m_Name(name)
	,m_Path(path)
	,m_Id(0)
	,m_Width(0)
	,m_Height(0)
	,m_NumOfChannels(0)
	,m_Pixels(nullptr)
	,m_Filter(TextureFiltering::Linear)
	,m_TexUnit(texUnit)
{
	BANANA_ASSERT(m_TexUnit > 0, "Texture Unit 0 is not available for User acess. - please use texture units >= 1");
	int width, height, numOfChannels;
	stbi_set_flip_vertically_on_load(1);
	m_Pixels = stbi_load(path.c_str(), &width, &height, &numOfChannels, 0);

	if (m_Pixels)
	{
		m_Width = width;
		m_Height = height;
		m_NumOfChannels = numOfChannels;
	}
	else
	{
		BANANA_ERROR("Error loading texture from path :{}", path);
	}
	LoadTexture();
}

banana::graphics::Texture::~Texture()
{
	stbi_image_free(m_Pixels);
	m_Pixels = nullptr;
}

void banana::graphics::Texture::setFilter(TextureFiltering filter)
{
	glActiveTexture(GL_TEXTURE0 + m_TexUnit);
	glBindTexture(GL_TEXTURE_2D, m_Id); BANANA_CHECK_GL_ERROR;
	switch (filter)
	{
	case TextureFiltering::Nearest:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); BANANA_CHECK_GL_ERROR
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); BANANA_CHECK_GL_ERROR
			break;
	case TextureFiltering::Linear:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); BANANA_CHECK_GL_ERROR
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); BANANA_CHECK_GL_ERROR
			break;
	default:
		break;
	}
	glBindTexture(GL_TEXTURE_2D, 0); BANANA_CHECK_GL_ERROR;
}

void banana::graphics::Texture::bind()
{
	glActiveTexture(GL_TEXTURE0 + m_TexUnit);
	glBindTexture(GL_TEXTURE_2D, m_Id); BANANA_CHECK_GL_ERROR;
}

void banana::graphics::Texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void banana::graphics::Texture::LoadTexture()
{
	glGenTextures(1, &m_Id); BANANA_CHECK_GL_ERROR;
	glActiveTexture(GL_TEXTURE0 + m_TexUnit);
	glBindTexture(GL_TEXTURE_2D, m_Id); BANANA_CHECK_GL_ERROR;

	GLenum channelFormat = 0;
	if (m_NumOfChannels == 3)
	{
		channelFormat = GL_RGB;
	}
	else if (m_NumOfChannels == 4)
	{
		channelFormat = GL_RGBA;
	}
	if (m_Pixels && channelFormat == 0)
	{
		BANANA_ERROR("Error not supported texture channel :{} ", m_Path);
	}

	if (m_Pixels && channelFormat != 0)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, channelFormat, m_Width, m_Height, 0, channelFormat, GL_UNSIGNED_BYTE, m_Pixels); BANANA_CHECK_GL_ERROR;
		setFilter(m_Filter);
		BANANA_TRACE("Loaded {}-Channel Texture :{}", m_NumOfChannels, m_Path);
	}
	else
	{
		float pixels[] = {
			1.0f,0.f,1.0f,		1.0f,1.f,1.0f,		1.0f,0.f,1.0f,		1.0f,1.f,1.0f,
			1.0f,1.0f,1.0f,		1.0f,0.f,1.0f,		1.0f,1.0f,1.0f,		1.0f,0.f,1.0f,
			1.0f,0.f,1.0f,		1.0f,1.f,1.0f,		1.0f,0.f,1.0f,		1.0f,1.f,1.0f,
			1.0f,1.0f,1.0f,		1.0f,0.f,1.0f,		1.0f,1.0f,1.0f,		1.0f,0.f,1.0f,
		};
		m_Width = 4;
		m_Height = 4;
		m_NumOfChannels = 3;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_FLOAT, pixels); BANANA_CHECK_GL_ERROR;
		setFilter(TextureFiltering::Nearest);
		BANANA_WARN("Unable to Load Texture : {} - loading checkerboard pattern", m_Path);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}
