#pragma once

#include<string>

namespace banana::graphics
{
	enum class TextureFiltering
	{
		Nearest,
		Linear
	};
	class Texture
	{
	public:
		//the SAME name must be refrenced in shader code to be able to use this texture
		Texture(const std::string& name,const std::string& path,int texUnit);
		~Texture();

		inline std::string getName() const { return m_Name; }
		inline uint32_t getId() const { return m_Id; }
		inline uint32_t getWidth() const { return m_Width; }
		inline uint32_t getHeight() const { return m_Height; }
		inline uint32_t getNumOfChannels() const { return m_NumOfChannels; }
		inline TextureFiltering getFilter() const { return m_Filter; }
		inline int getTexUnit() const { return m_TexUnit; }

		void setTexUnit(int texUnit) { m_TexUnit = texUnit; }
		void setFilter(TextureFiltering filter);

		void bind();
		void unbind();
	private:
		void LoadTexture();
	private:
		std::string m_Name;
		std::string m_Path;
		uint32_t m_Id;
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_NumOfChannels;
		unsigned char* m_Pixels;
		TextureFiltering m_Filter;
		int m_TexUnit;
	};
}