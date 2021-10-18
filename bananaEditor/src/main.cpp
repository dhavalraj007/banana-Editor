#include <iostream>
#include "banana/engine.h"
#include"banana/app.h"
#include"banana/main.h"
#include"banana/log.h"
#include"banana/graphics/mesh.h"
#include"banana/graphics/shader.h"
#include"banana/input/mouse.h"
#include"banana/input/keyboard.h"

using namespace banana;

class Editor :public banana::App
{
public:
    std::shared_ptr<graphics::Mesh> m_mesh;
    std::shared_ptr<graphics::Shader> m_shader;
    void initialize()
    {
        BANANA_TRACE("Editor::Intitalize");
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

    void shutdown()
    {
        BANANA_TRACE("Editor::shutdown");
    }

    void update()
    {
        BANANA_TRACE("Editor::update");
        int windowW, windowH;
        Engine::Instance().getWindow().getSize(windowW, windowH);

        float normX = (float)input::Mouse::X() / (float)windowW;
        float normY = (float)(windowH - input::Mouse::Y()) / (float)windowH;

        if (input::Keyboard::keyUp(BANANA_INPUT_KEY_R))
        {
            normX += 1.f;
            BANANA_TRACE("R");
        }

        m_shader->setUniformFloat2("offset", normX, normY);

    }

    void render()
    {
        BANANA_TRACE("Editor::render");
        auto rendercmd = std::make_unique<graphics::rendercommands::RenderMesh>(m_mesh, m_shader);
        Engine::Instance().getRenderManager().submit(std::move(rendercmd));
        Engine::Instance().getRenderManager().flush();

    }
};

banana::App* createApp()
{
    return new Editor();
}