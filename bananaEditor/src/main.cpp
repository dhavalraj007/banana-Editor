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

    float xKeyOffset = 0.f;
    float yKeyOffset = 0.f;
    float keySpeed = 0.001f;
    glm::vec2 m_RectPos, m_RectSize;

    std::shared_ptr<graphics::Shader> m_shader;
    std::shared_ptr<graphics::Texture> m_tex;
    std::shared_ptr<graphics::VertexArray> m_Va;
    

    core::AssetLibrary<graphics::VertexArray> m_VaLibrary;
    core::AssetLibrary<graphics::Texture> m_texLibrary;
    core::AssetLibrary<graphics::Shader> m_shaderLibrary;

    core::WindowProperties getWindowProperties()
    {
        core::WindowProperties props;
        props.title = "BANANA Editor";
        props.w = 800;
        props.h = 600;
        props.imguiProps.isDockingEnable = true;

        return props;
    }

    void initialize() override
    {
        initializeLibrary();
       
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

    void update()   override
    {
        auto windowSize = Engine::Instance().getWindow().getSize();

        float xNorm = (float)input::Mouse::X() / (float)windowSize.x;
        float yNorm = (float)(windowSize.y - input::Mouse::Y()) / (float)windowSize.y;

        if (input::Keyboard::key(BANANA_INPUT_KEY_LEFT)) { xKeyOffset -= keySpeed;}
        if (input::Keyboard::key(BANANA_INPUT_KEY_RIGHT)) { xKeyOffset += keySpeed; }
        if (input::Keyboard::key(BANANA_INPUT_KEY_UP)) { yKeyOffset += keySpeed; }
        if (input::Keyboard::key(BANANA_INPUT_KEY_DOWN)) { yKeyOffset -= keySpeed; }

        if (input::Keyboard::keyDown(BANANA_INPUT_KEY_LEFT)) { xKeyOffset -= keySpeed * 100; }
        if (input::Keyboard::keyDown(BANANA_INPUT_KEY_RIGHT)) { xKeyOffset += keySpeed * 100; }

        m_shader->setUniformFloat2("offset", xNorm + xKeyOffset, yNorm + yKeyOffset);
        glm::mat4 model = glm::mat4(1.f);
        model = glm::translate(model, { m_RectPos.x,m_RectPos.y,0.f });
        model = glm::scale(model, { m_RectSize.x,m_RectSize.y,0.f });
        m_shader->setUniformMat4("model",model);
    }

    void render()   override
    {
        Engine::Instance().getRenderManager().submit(BANANA_SUBMIT_RC(RenderTexturedVertexArray, m_Va, m_tex,m_shader));
    }

    void imguiRender()  override
    {
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        if (ImGui::Begin("Rect Controls"))
        {
            ImGui::DragFloat2("Rect Pos ", glm::value_ptr(m_RectPos), 0.01f);
            ImGui::DragFloat2("Rect Size", glm::value_ptr(m_RectSize), 0.01f);
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
                            ImGui::Text("%dx%d", tex->getWidth(),tex->getHeight());
                            ImGui::TextColored(datacol, "Channels : "); ImGui::SameLine();
                            ImGui::Text("%d", tex->getNumOfChannels());
                            ImGui::TextColored(datacol, "Shader Refrence name: "); ImGui::SameLine();
                            ImGui::Text("%s", tex->getName().c_str());
                            ImGui::TextColored(datacol, "Path : "); ImGui::SameLine();
                            ImGui::Text("%s", tex->getPath().c_str());
                            ImVec2 size{ 100.f,(float)tex->getHeight()*100.f/ (float)tex->getWidth() };
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
            ImVec2 size{ 500,300 };
            ImVec2 uv0{ 0,1 };
            ImVec2 uv1{ 1,0 };
            auto& window = Engine::Instance().getWindow();
            ImGui::Image((ImTextureID)(intptr_t)window.getFramebuffer()->getTextureId(), size,uv0,uv1);
        }
        ImGui::End();
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

                    uniform mat4 model = mat4(1.0);
                    
                    void main()
                    {
                        gl_Position = model * vec4(position,1.0);
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

                    uniform mat4 model = mat4(1.0);
                    
                    void main()
                    {

                        uvs = texcoords;
                        gl_Position = model * vec4(position,1.0);
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

        //Textures
        {
            std::shared_ptr<graphics::Texture> BananaTex= std::make_shared<graphics::Texture>("bananaTex", "./res/banana.png", 0);
            m_texLibrary.load("bananaLogo", BananaTex);
        }
        
        {
            std::shared_ptr<graphics::Texture> pirateTex= std::make_shared<graphics::Texture>("pirateTex", "./res/pirate.png", 1);
            m_texLibrary.load("pirate", pirateTex);
        }
    }
};

banana::App* createApp()
{
    return new Editor();
}