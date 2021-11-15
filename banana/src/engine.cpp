#include"banana/engine.h"
#include"banana/log.h"
#include"banana/app.h"
#include"banana/input/mouse.h"
#include"banana/input/keyboard.h"
#include"sdl2/SDL.h"

namespace banana 
{
    //public

    bool Engine::initialize()
    {
        bool ok = false;
        BANANA_ASSERT(!m_isInitialized, "Attempting to call Engine::initialized more than once!");
        if (!m_isInitialized)
        {
            
            getInfo();

            if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
            {
                BANANA_ERROR("ERROR Initialzing SDL2: {} ", SDL_GetError());
            }
            else
            {
                SDL_version version;
                SDL_VERSION(&version);
                BANANA_INFO("SDL {}.{}.{}", (int32_t)version.major, (int32_t)version.minor, (int32_t)version.patch);

                core::WindowProperties props = m_App->getWindowProperties();
                if (m_Window.create(props))
                {
                    //initialize managers
                    m_RenderManager.initialize();

                    ok = true;
                    m_isRunning = true;
                    m_isInitialized = true;

                    //input
                    input::Mouse::initialize();
                    input::Keyboard::initialize();

                    //init client
                    m_App->initialize();
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

   

    void Engine::run(App* app)
    {
        m_LogManager.initialize();
        BANANA_ASSERT(!m_App, "Attempting to call Engine::run when a valid app exists");
        if (m_App)
        {
            return;
        }
        m_App = app;
        if (initialize())       //init engine
        {
            float deltaTime, lastFrameTime=0;
            float currTime;
                while (m_isRunning)        // core Game Loop
                {
                    currTime = SDL_GetTicks()*0.001f;
                    deltaTime = currTime - lastFrameTime;
                    lastFrameTime = currTime;
                    m_Window.pumpEvents();
                    update(deltaTime);
                    render();
                }
        shutdown();
        }  
    }

    uint32_t Engine::getInitTime()
    {
        return SDL_GetTicks();
    }

    void Engine::shutdown()
    {
        m_isRunning = false;
        m_isInitialized = false;

        //client
        m_App->shutdown();
        //Managers
        m_RenderManager.shutdown();
        
        //SDL
        m_Window.shutdown();
        SDL_Quit();;
        
        m_LogManager.shutdown();
    }

    //private

    void Engine::update(float deltaTime)
    {
        m_App->update(deltaTime);
    }


    void Engine::render()
    {
        m_Window.beginRender();

        m_App->render();

        m_Window.endRender();
    }

    Engine::Engine() 
        :m_isRunning(false)
        ,m_isInitialized(false)
        ,m_App(nullptr)
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