#include <iostream>
#include <vector>
#include "banana/engine.h"
#include"banana/app.h"
#include"banana/main.h"
#include"banana/log.h"

#include"banana/graphics/mesh.h"
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
    float mixVal = 0.2f;
    glm::vec2 m_RectPos, m_RectSize;

    std::shared_ptr<graphics::Mesh> m_mesh;
    std::shared_ptr<graphics::Shader> m_shader;
    std::shared_ptr<graphics::Texture> m_BananaTex;
    std::shared_ptr<graphics::Texture> m_ContTex;
    

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
        //Test Mesh
        float vertices[] = {
             0.5f,  0.5f, 0.f,      //top right
             0.5f, -0.5f, 0.f,      //bottom right
            -0.5f, -0.5f, 0.f,       //bottom left
            -0.5f,  0.5f, 0.f      //top left
        };
        uint32_t elements[] = {
            0, 1, 3,
            3, 1, 2
        };
        float texcoords[] = {
            1.f,1.f,
            1.f,0.f,
            0.f,0.f,
            0.f,1.f
        };


        m_BananaTex = std::make_shared<graphics::Texture>("bananaTex", "./res/banana.png", 0);
        m_ContTex = std::make_shared<graphics::Texture>("pirateTex", "./res/pirate.png", 1);
        m_mesh = std::make_shared<graphics::Mesh>(vertices, 4, 3,texcoords, elements, 6);
        //Test Shader

        const char* vertexShader = R"(
                    #version 410 core
                    layout (location = 0) in vec3 position;
                    layout (location = 1) in vec2 tex;
                    out vec3 vpos;
                    out vec2 uvs;
                    uniform vec2 offset = vec2(0.5);
                    uniform mat4 model = mat4(1.0);
                    
                    void main()
                    {
                        uvs = tex;
                        vpos = position + vec3(offset,0.0);
                        gl_Position = model * vec4(position,1.0);
                    }
                      )";

        const char* fragShader = R"(
                    #version 410 core
                    out vec4 outColor;
                    in vec3 vpos;
                    in vec2 uvs;
                    uniform float mixVal=0.2;
                    uniform sampler2D bananaTex;
                    uniform sampler2D pirateTex;
                    void main()
                    {
                       // outColor = vec4(vpos,1.0);
                        outColor = mix(texture(bananaTex,uvs),mix(texture(pirateTex,uvs),vec4(vpos,1.0),0.3),mixVal);
                    }
                      )";

        m_shader = std::make_shared<graphics::Shader>(vertexShader, fragShader);
        m_RectPos = glm::vec2(0.f);
        m_RectSize = glm::vec2(1.f);
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
        m_shader->setUniformFloat("mixVal", mixVal);
    }

    void render()   override
    {
        auto texs = graphics::multipleTexures({ m_BananaTex,m_ContTex });
        Engine::Instance().getRenderManager().submit(BANANA_SUBMIT_RC(RenderMultiTexturedMesh, m_mesh, texs,m_shader));
    }

    void imguiRender()  override
    {
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        if (ImGui::Begin("Rect"))
        {
            ImGui::DragFloat2("Rect Pos ", glm::value_ptr(m_RectPos), 0.01f);
            ImGui::DragFloat2("Rect Size", glm::value_ptr(m_RectSize), 0.01f);
        }
        ImGui::End();
        if (ImGui::Begin("Textures blend"))
        {
            ImGui::DragFloat("Texture blend val", &mixVal, 0.01f,0.f,1.f);
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
};

banana::App* createApp()
{
    return new Editor();
}