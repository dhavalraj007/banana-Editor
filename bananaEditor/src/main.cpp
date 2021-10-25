#include <iostream>
#include "banana/engine.h"
#include"banana/app.h"
#include"banana/main.h"
#include"banana/log.h"

#include"banana/graphics/mesh.h"
#include"banana/graphics/shader.h"
#include"banana/graphics/framebuffer.h"

#include"banana/input/mouse.h"
#include"banana/input/keyboard.h"
#include"external/imgui/imgui.h"

using namespace banana;

class Editor :public banana::App
{
public:

    float xKeyOffset = 0.f;
    float yKeyOffset = 0.f;
    float keySpeed = 0.001f;
    std::shared_ptr<graphics::Mesh> m_mesh;
    std::shared_ptr<graphics::Shader> m_shader;

    core::WindowProperties GetWindowProperties()
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

        m_mesh = std::make_shared<graphics::Mesh>(vertices, 4, 3, elements, 6);

        //Test Shader

        const char* vertexShader = R"(
                    #version 410 core
                    layout (location = 0) in vec3 position;
                    out vec3 vpos;
                    uniform vec2 offset = vec2(0.5);
                    
                    void main()
                    {
                        vpos = position + vec3(offset,0.0);
                        gl_Position = vec4(position,1.0);
                    }
                      )";

        const char* fragShader = R"(
                    #version 410 core
                    out vec4 outColor;
                    in vec3 vpos;

                    void main()
                    {
                        outColor = vec4(vpos,1.0);
                    }
                      )";

        m_shader = std::make_shared<graphics::Shader>(vertexShader, fragShader);

    }

    void shutdown() override
    {

    }

    void update()   override
    {
        int windowW = 0;
        int windowH = 0;
        Engine::Instance().getWindow().getSize(windowW, windowH);

        float xNorm = (float)input::Mouse::X() / (float)windowW;
        float yNorm = (float)(windowH - input::Mouse::Y()) / (float)windowH;

        if (input::Keyboard::key(BANANA_INPUT_KEY_LEFT)) { xKeyOffset -= keySpeed; }
        if (input::Keyboard::key(BANANA_INPUT_KEY_RIGHT)) { xKeyOffset += keySpeed; }
        if (input::Keyboard::key(BANANA_INPUT_KEY_UP)) { yKeyOffset += keySpeed; }
        if (input::Keyboard::key(BANANA_INPUT_KEY_DOWN)) { yKeyOffset -= keySpeed; }

        if (input::Keyboard::keyDown(BANANA_INPUT_KEY_LEFT)) { xKeyOffset -= keySpeed * 100; }
        if (input::Keyboard::keyDown(BANANA_INPUT_KEY_RIGHT)) { xKeyOffset += keySpeed * 100; }

        m_shader->setUniformFloat2("offset", xNorm + xKeyOffset, yNorm + yKeyOffset);
    }

    void render()   override
    {
        auto rendercmd = std::make_unique<graphics::rendercommands::RenderMesh>(m_mesh, m_shader);
        Engine::Instance().getRenderManager().submit(std::move(rendercmd));
        Engine::Instance().getRenderManager().flush();
    }

    void imguiRender()  override
    {
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        if (ImGui::Begin("RectPos x"))
        {
            ImGui::DragFloat("Rect Pos X", &xKeyOffset, 0.01f);
        }
        ImGui::End();

        if (ImGui::Begin("RectPos y"))
        {
            ImGui::DragFloat("Rect Pos Y", &yKeyOffset, 0.01f);
        }
        ImGui::End();

        if (ImGui::Begin("Game View"))
        {
            if (ImGui::IsWindowHovered())
            {
                ImGui::CaptureMouseFromApp(false);
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