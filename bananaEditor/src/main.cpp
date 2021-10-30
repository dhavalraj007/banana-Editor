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
    std::shared_ptr<graphics::Mesh> m_mesh;
    std::shared_ptr<graphics::Shader> m_shader;
    glm::vec2 m_RectPos, m_RectSize;

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

        m_mesh = std::make_shared<graphics::Mesh>(vertices, 4, 3, elements, 6);

        //Test Shader
        m_shader = std::make_shared<graphics::Shader>("./shaders/vertexShader.glsl", "./shaders/fragmentShader.glsl");

        m_RectPos=glm::vec2(0.f);
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

        if (ImGui::Begin("Rect Pos"))
        {
            ImGui::DragFloat2("Rect Pos X", glm::value_ptr(m_RectPos), 0.01f);
        }
        ImGui::End();

        if (ImGui::Begin("Rect Size"))
        {
            ImGui::DragFloat2("Rect Pos Y", glm::value_ptr(m_RectSize), 0.01f);
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