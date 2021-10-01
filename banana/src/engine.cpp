#include"engine.h"
#include<iostream>
#include"sdl2/SDL.h"


namespace banana
{
    //public

    bool Engine::initialize()
    {
        bool ok = false;

        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            std::cout << "ERROR Initialzing SDL2: " << SDL_GetError() << std::endl;
        }
        else
        {
            SDL_version version;
            SDL_VERSION(&version);
            std::cout << "SDL " << (int32_t)version.major << "." << (int32_t)version.minor << "." << (int32_t)version.patch << std::endl;

            if (m_Window.create())
            {
                ok = true;
                m_isRunning = true;
            }
        }


        if (!ok)
        {
            std::cout << "Engine Initializing Failed. Shutting Down." << std::endl;
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
            }
        shutdown();
        }
    }

    void Engine::shutdown()
    {
        m_isRunning = false;
        m_Window.shutdown();
        SDL_Quit();;
    }

    //private
    Engine::Engine():m_isRunning(false)
    {
        getInfo();
    }

    void Engine::getInfo()
    {
#ifdef BANANA_CONFIG_DEBUG
        std::cout<<"Config:DEBUG"<<std::endl;
#endif
#ifdef BANANA_CONFIG_RELEASE
        std::cout<<"Config:RELEASE"<<std::endl;
#endif
#ifdef BANANA_PLATFORM_WINDOWS
        std::cout<<"Platform: WINDOWS"<<std::endl;
#endif
#ifdef BANANA_PLATFORM_MAC
        std::cout<<"Platform: MAC"<<std::endl;
#endif
#ifdef BANANA_PLATFORM_LINUX
        std::cout<<"Platform: LINUX"<<std::endl;
#endif
    }
}