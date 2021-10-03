#include"engine.h"
#include"log.h"
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
            while (m_isRunning)        // core Game Loop
            {
                m_Window.pumpEvents();
                m_Window.beginRender();
                m_Window.endRender();
            }
        shutdown();
        }
    }

    void Engine::shutdown()
    {
        m_isRunning = false;
        m_isInitialized = false;
        //Managers
        m_LogManager.shutdown();
        //SDL
        m_Window.shutdown();
        SDL_Quit();;
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