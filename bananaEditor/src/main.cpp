#include <iostream>
#include <vector>
#include <string>
#include "banana/engine.h"
#include"banana/app.h"
#include"banana/main.h"
#include"banana/log.h"

#include"banana/core/assetlibrary.h"

#include"banana/graphics/vertex.h"
#include"banana/graphics/shader.h"
#include"banana/graphics/texture.h"
#include"banana/graphics/framebuffer.h"
#include"banana/graphics/camera.h"

#include"banana/input/mouse.h"
#include"banana/input/keyboard.h"
#include"external/imgui/imgui.h"

#include"external/glm/glm.hpp"
#include"external/glm/gtc/matrix_transform.hpp"
#include"external/glm/gtc/type_ptr.hpp"
#include"external/glm/gtx/string_cast.hpp"
#define GLM_ENABLE_EXPERIMENTAL

using namespace banana;

class Editor :public banana::App
{
public:
	bool m_ImguiEnabled = true;
	float xKeyOffset = 0.f;
	float yKeyOffset = 0.f;
	float keySpeed = 0.001f;
	glm::vec2 m_RectPos, m_RectSize;
	glm::vec3 m_CamPos;
	float m_CamRot;
	float m_CamSpeed;
	
	std::shared_ptr<graphics::Shader> m_shader;
	std::shared_ptr<graphics::Texture> m_tex;
	std::shared_ptr<graphics::VertexArray> m_Va;
	std::shared_ptr<graphics::Camera2D> m_Camera2D;

	core::AssetLibrary<graphics::VertexArray> m_VaLibrary;
	core::AssetLibrary<graphics::Texture> m_texLibrary;
	core::AssetLibrary<graphics::Shader> m_shaderLibrary;

	core::WindowProperties getWindowProperties()
	{
		core::WindowProperties props;
		props.title = "BANANA Editor";
		props.w = 854;
		props.h = 480;
		props.imguiProps.isDockingEnable = true;

		return props;
	}

	void initialize() override
	{
		initializeLibrary();
		auto& window = Engine::Instance().getWindow();
		window.setShouldRenderToScreen(false);

		m_Camera2D = std::make_shared<graphics::Camera2D>();
		m_Camera2D->setHeight(2.f);
		m_CamPos = { 0.f,0.f, 1.f };
		m_Camera2D->setView(m_CamPos, m_CamRot);
		m_CamSpeed = 1.0f;

		m_RectPos = glm::vec2(0.f);
		m_RectSize = glm::vec2(1.f);

		m_Va = m_VaLibrary.get("rect");
		m_shader = m_shaderLibrary.get("rect");
		m_tex = m_texLibrary.get("bananaLogo");
		m_tex->setName("tex");
	}

	void shutdown() override
	{
	}

	void update(float deltaTime)   override
	{
		if (input::Keyboard::keyDown(BANANA_INPUT_KEY_GRAVE))
		{
			m_ImguiEnabled = !m_ImguiEnabled;
			auto& window = Engine::Instance().getWindow();
			window.setShouldRenderToScreen(!m_ImguiEnabled);
		}
		if (input::Keyboard::key(BANANA_INPUT_KEY_RIGHT))
		{
			m_CamPos.x += m_CamSpeed*deltaTime;
		}
		if (input::Keyboard::key(BANANA_INPUT_KEY_LEFT))
		{
			m_CamPos.x -= m_CamSpeed*deltaTime;
		}
		m_Camera2D->setView(m_CamPos, m_CamRot);

		glm::mat4 model = glm::mat4(1.f);
		model = glm::scale(model, { m_RectSize.x,m_RectSize.y,0.f });
		model = glm::translate(model, { m_RectPos.x,m_RectPos.y,0.f });
		m_shader->setUniformMat4("model", model);
	}

	void render()   override
	{
		auto& rm = Engine::Instance().getRenderManager();
		//rm.setWireframeMode(true);
		rm.submit(BANANA_SUBMIT_RC(PushCamera, m_Camera2D));
		rm.submit(BANANA_SUBMIT_RC(RenderTexturedVertexArray, m_Va, m_tex, m_shader));
		rm.submit(BANANA_SUBMIT_RC(PopCamera));
	}

	void imguiRender()  override
	{
		if (m_ImguiEnabled)
		{
			ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
			if (ImGui::Begin("Controls"))
			{
				if (ImGui::RadioButton("wireframe Mode", false))
				{
					auto& rm = Engine::Instance().getRenderManager();
					rm.setWireframeMode(true);
				}
				if (ImGui::RadioButton("Fill Mode", true))
				{
					auto& rm = Engine::Instance().getRenderManager();
					rm.setWireframeMode(false);
				}
				ImGui::DragFloat2("Rect Pos ", glm::value_ptr(m_RectPos), 0.01f);
				ImGui::DragFloat2("Rect Size", glm::value_ptr(m_RectSize), 0.01f);
				ImGui::Separator();
				float camheight = m_Camera2D->getHeight();
				ImGui::DragFloat("Camera Height", &camheight, 0.01f);
				m_Camera2D->setHeight(camheight);
				glm::vec3 campos = m_CamPos;
				float camrot = m_CamRot;
				ImGui::DragFloat3("Camera Pos", glm::value_ptr(campos), 0.01f);
				ImGui::DragFloat("Camera Rotation", &camrot, .1f);
				if (camrot != m_CamRot || campos != m_CamPos)
				{
					m_CamPos = campos;
					m_CamRot = camrot;
					m_Camera2D->setView(campos, camrot);
				}
			}
			ImGui::End();

			if (ImGui::Begin("Options"))
			{
				if (ImGui::Button("Rect"))
				{
					m_Va = m_VaLibrary.get("rect");
					m_shader = m_shaderLibrary.get("rect");
				}
				ImGui::SameLine();
				if (ImGui::Button("texturedRect"))
				{
					m_Va = m_VaLibrary.get("texturedRect");
					m_shader = m_shaderLibrary.get("texturedRect");
				}
				ImGui::SameLine();
				if (ImGui::Button("banana"))
				{
					m_tex = m_texLibrary.get("bananaLogo");
					m_tex->setName("tex");
				}     
				ImGui::SameLine();
				if (ImGui::Button("pirate"))
				{
					m_tex = m_texLibrary.get("pirate");
					m_tex->setName("tex");
				}
				ImGui::SameLine();
				if (ImGui::Button("container"))
				{
					m_tex = m_texLibrary.get("container");
					m_tex->setName("tex");
				}
			}
			ImGui::End();

			if (ImGui::Begin("Asset Library"))
			{
				ImVec4 datacol(0, 1, 0, 1);
				ImVec4 errorcol(1, 0, 0, 1);
				if (ImGui::TreeNode("TextureLibrary"))
				{
					for (const auto& kv : m_texLibrary.getMap())
					{
						auto texName = kv.first + "##Asset Library.TextureLibrary";
						auto& ptr = kv.second;
						if (ImGui::TreeNode(texName.c_str()))
						{
							graphics::Texture* tex = ptr.get();
							if (tex)
							{
								ImGui::TextColored(datacol, "use Count: "); ImGui::SameLine();
								ImGui::Text("%03d", (int)ptr.use_count());
								ImGui::TextColored(datacol, "Size : "); ImGui::SameLine();
								ImGui::Text("%dx%d", tex->getWidth(), tex->getHeight());
								ImGui::TextColored(datacol, "Channels : "); ImGui::SameLine();
								ImGui::Text("%d", tex->getNumOfChannels());
								ImGui::TextColored(datacol, "Shader Refrence name: "); ImGui::SameLine();
								ImGui::Text("%s", tex->getName().c_str());
								ImGui::TextColored(datacol, "Path : "); ImGui::SameLine();
								ImGui::Text("%s", tex->getPath().c_str());
								ImVec2 size{ 100.f,(float)tex->getHeight() * 100.f / (float)tex->getWidth() };
								ImGui::Image((void*)(intptr_t)tex->getId(), size, { 0,1 }, { 1,0 });
							}
							else
							{
								ImGui::TextColored(errorcol, "Invalid Texture");
							}
							ImGui::TreePop();
						}
					}
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("ShaderLibrary"))
				{
					for (const auto& kv : m_shaderLibrary.getMap())
					{
						auto name = kv.first + "##Asset Library.ShaderLibrary";
						auto& ptr = kv.second;
						if (ImGui::TreeNode(name.c_str()))
						{
							graphics::Shader* shader = ptr.get();
							if (shader)
							{
								ImGui::TextColored(datacol, "use Count: "); ImGui::SameLine();
								ImGui::Text("%03d", (int)ptr.use_count());
								std::string tmp = "VertexSource##Asset Library.ShaderLibrary." + kv.first;
								if (ImGui::TreeNode(tmp.c_str()))
								{
									ImGui::TextWrapped(shader->getVertexShaderSource().c_str());
									ImGui::TreePop();
								}
								tmp = "FragmentSource##Asset Library.ShaderLibrary." + kv.first;
								if (ImGui::TreeNode(tmp.c_str()))
								{
									ImGui::TextWrapped(shader->getFragmentShaderSource().c_str());
									ImGui::TreePop();
								}
							}
							else
							{
								ImGui::TextColored(errorcol, "Invalid Shader");
							}
							ImGui::TreePop();
						}
					}
					ImGui::TreePop();
				}
			}
			ImGui::End();

			if (ImGui::Begin("Game View"))
			{
				if (ImGui::IsWindowHovered())
				{
					ImGui::CaptureMouseFromApp(false);
					ImGui::CaptureKeyboardFromApp(false);
				}
				auto& window = Engine::Instance().getWindow();
				ImVec2 winSize = ImGui::GetWindowSize();
				glm::ivec2 arSize = window.getSizeInAspectRatio((int)winSize.x, (int)winSize.y);
				ImVec2 size{ (float)arSize.x - 15.f,(float)arSize.y - 35.f };
				ImVec2 pos{
					(winSize.x - size.x) * 0.5f,
					((winSize.y - size.y) * 0.5f) + 7.f
				};
				ImGui::SetCursorPos(pos);
				ImVec2 uv0{ 0,1 };
				ImVec2 uv1{ 1,0 };
				ImGui::Image((ImTextureID)(intptr_t)window.getFramebuffer()->getTextureId(), size, uv0, uv1);
			}
			ImGui::End();
		}
	}

	void initializeLibrary()
	{
		// Vb Va
		{
			std::shared_ptr<graphics::VertexArray> va = std::make_shared<graphics::VertexArray>();
			{
				BANANA_CRETE_VERTEX_BUFFER(vb, float);
				vb->pushVertex({ 0.5f,  0.5f, 0.f });
				vb->pushVertex({ 0.5f, -0.5f, 0.f });
				vb->pushVertex({ -0.5f,-0.5f, 0.f });
				vb->pushVertex({ -0.5f, 0.5f, 0.f });
				vb->setLayout({ 3 });
				vb->upload(false);
				va->pushBuffer(std::move(vb));
			}
			va->setElements({ 0, 1, 3, 3, 1, 2 });
			va->upload();
			m_VaLibrary.load("rect", va);
		}
		

		{
			std::shared_ptr<graphics::VertexArray> va = std::make_shared<graphics::VertexArray>();
			{
				BANANA_CRETE_VERTEX_BUFFER(vb, float);
				vb->pushVertex({ 0.5f,  0.5f, 0.f });
				vb->pushVertex({ 0.5f, -0.5f, 0.f });
				vb->pushVertex({ -0.5f,-0.5f, 0.f });
				vb->pushVertex({ -0.5f, 0.5f, 0.f });
				vb->setLayout({ 3 });
				vb->upload(false);
				va->pushBuffer(std::move(vb));
			}
			{
				BANANA_CRETE_VERTEX_BUFFER(vb, short);
				vb->pushVertex({ 1,1 });
				vb->pushVertex({ 1,0 });
				vb->pushVertex({ 0,0 });
				vb->pushVertex({ 0,1 });
				vb->setLayout({ 2 });
				vb->upload(false);
				va->pushBuffer(std::move(vb));
			}
			va->setElements({ 0, 1, 3, 3, 1, 2 });
			va->upload();
			m_VaLibrary.load("texturedRect", va);
		}

		//Shaders
		{
			const char* vertexShader = R"(
                    #version 410 core
                    layout (location = 0) in vec3 position;

                    uniform mat4 proj = mat4(1.0f);
                    uniform mat4 view = mat4(1.0f);
                    uniform mat4 model = mat4(1.0);

                    void main()
                    {
                        gl_Position = proj * view * model * vec4(position,1.0);
                    }
                      )";

			const char* fragShader = R"(
                    #version 410 core
                    out vec4 outColor;
                    uniform vec4 color = vec4(1.0);
                    void main()
                    {
                        outColor = color;
                    }
                      )";

			m_shaderLibrary.load("rect", std::make_shared<graphics::Shader>(vertexShader, fragShader));
		}

		{
			const char* vertexShader = R"(
                    #version 410 core
                    layout (location = 0) in vec3 position;
                    layout (location = 1) in vec2 texcoords;
                    out vec2 uvs;

                    uniform mat4 proj = mat4(1.0f);
                    uniform mat4 view = mat4(1.0f);
                    uniform mat4 model = mat4(1.0f);

                    void main()
                    {
                        uvs = texcoords;
                        gl_Position = proj * view * model * vec4(position,1.0);
                    }
                      )";

			const char* fragShader = R"(
                    #version 410 core
                    out vec4 outColor;
                    in vec2 uvs;

                    uniform sampler2D tex;
                    void main()
                    {
                        outColor = texture(tex,uvs);
                    }
                      )";

			m_shaderLibrary.load("texturedRect", std::make_shared<graphics::Shader>(vertexShader, fragShader));
		}

		{
			const char* vertexShader = R"(
                    #version 410 core
                    layout (location = 0) in vec3 position;
                    layout (location = 1) in vec2 texcoords;
                    out vec2 uvs;
					out vec4 pos;
                    uniform mat4 proj = mat4(1.0f);
                    uniform mat4 view = mat4(1.0f);
                    uniform mat4 model = mat4(1.0f);

                    void main()
                    {
                        uvs = texcoords;
						pos = proj * view * model * vec4(position,1.0);
                        gl_Position = pos;
                    }
                      )";

			const char* fragShader = R"(
                    #version 410 core
                    out vec4 outColor;
                    in vec2 uvs;
                    in vec4 pos;

                    uniform sampler2D tex;
                    void main()
                    {
                       
						if(pos.z < -0.7)
                        {
							outColor = vec4(1.0,0.0,0.0,1.0);
						}
						else
						{
							outColor = vec4(0.0,0.0,0.0,1.0);
						}
                    }
                      )";

			m_shaderLibrary.load("debugShader", std::make_shared<graphics::Shader>(vertexShader, fragShader));
		}

		//Textures
		{
			std::shared_ptr<graphics::Texture> BananaTex = std::make_shared<graphics::Texture>("bananaTex", "./res/banana.png", 1);
			m_texLibrary.load("bananaLogo", BananaTex);
		}

		{
			std::shared_ptr<graphics::Texture> pirateTex = std::make_shared<graphics::Texture>("pirateTex", "./res/pirate.png", 2);
			m_texLibrary.load("pirate", pirateTex);
		}
		{
			std::shared_ptr<graphics::Texture> contTex = std::make_shared<graphics::Texture>("contTex", "./res/container.jpg", 3);
			m_texLibrary.load("container", contTex);
		}
	}
};

banana::App* createApp()
{
	return new Editor();
}