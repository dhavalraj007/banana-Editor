#include"banana/engine.h"
#include"banana/log.h"
#include"sdl2/SDL.h"
#include"banana/graphics/mesh.h"
#include"banana/graphics/shader.h"

namespace banana
{
    //public

    bool Engine::initialize()
    {
        bool ok = false;
        BANANA_ASSERT(!m_isInitialized, "Attempting to call Engine::initialized more than once!");
        if (!m_isInitialized)
        {
            m_LogManager.initialize();
            getInfo();

            if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
            {
                BANANA_ERROR("ERROR Initialzing SDL2: {} ", SDL_GetError())
            }
            else
            {
                SDL_version version;
                SDL_VERSION(&version);
                BANANA_INFO("SDL {}.{}.{}", (int32_t)version.major, (int32_t)version.minor, (int32_t)version.patch);


                if (m_Window.create())
                {
                    //initialize managers
                    m_RenderManager.initialize();

                    ok = true;
                    m_isRunning = true;
                    m_isInitialized = true;
                }
            }
        }
        


        if (!ok)
        {
            BANANA_ERROR("Engine Initializing Failed. Shutting Down.");
            shutdown();
        }

        return ok;
    }

   

    void Engine::run()
    {
        if (initialize())
        {
          
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

                std::shared_ptr<graphics::Mesh> mesh = std::make_shared<graphics::Mesh>(vertices, 4, 3, elements, 6);

                //Test Shader

                const char* vertexShader = R"(
                    #version 410 core
                    layout (location = 0) in vec3 position;
                    out vec3 vpos;
                    void main()
                    {
                        vpos = position + vec3(0.5,0.5,0.0);
                        gl_Position = vec4(position,1.0);
                    }
                      )";

                const char* fragShader = R"(
                    #version 410 core
                    out vec4 outColor;
                    in vec3 vpos;
                    uniform vec3 color = vec3(0.0);

                    void main()
                    {
                        outColor = vec4(vpos,1.0);
                    }
                      )";

                std::shared_ptr<graphics::Shader> shader = std::make_shared<graphics::Shader>(vertexShader, fragShader);
                shader->setUniformFloat3("color", 1, 0, 0);

                m_RenderManager.setWireframeMode(true);
                while (m_isRunning)        // core Game Loop
                {

                    m_Window.pumpEvents();
                    m_Window.beginRender();

                    auto rendercmd = std::make_unique<graphics::rendercommands::RenderMesh>(mesh, shader);
                    m_RenderManager.submit(std::move(rendercmd));
                    m_RenderManager.flush();
                    m_Window.endRender();
                }
            }
        shutdown();
        }
    }

    void Engine::shutdown()
    {
        m_isRunning = false;
        m_isInitialized = false;

        //Managers
        m_RenderManager.shutdown();
        
        //SDL
        m_Window.shutdown();
        SDL_Quit();;
        
        m_LogManager.shutdown();
    }

    //private
    Engine::Engine() 
        :m_isRunning(false)
        ,m_isInitialized(false)
    {}

    void Engine::getInfo()
    {
        
        BANANA_TRACE("Banana Engine v{}.{}", 0, 1);

#ifdef BANANA_CONFIG_DEBUG
        BANANA_DEBUG("Config:DEBUG");
#endif

#ifdef BANANA_CONFIG_RELEASE
        BANANA_DEBUG("Config:RELEASE");
#endif
#ifdef BANANA_PLATFORM_WINDOWS
        BANANA_WARN("Platform: WINDOWS");
#endif
#ifdef BANANA_PLATFORM_MAC
        BANANA_WARN("Platform: MAC");
#endif
#ifdef BANANA_PLATFORM_LINUX
        BANANA_WARN("Platform: LINUX");
#endif
    }
}