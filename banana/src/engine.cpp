#include"engine.h"
#include<iostream>
#include"sdl2/SDL.h"


namespace banana
{
    void getInfo()
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

    bool initialize()
    {
        bool ret = true;

        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            std::cout << "ERROR Initialzing SDL2: " << SDL_GetError() << std::endl;
            ret = false;
        }
        else
        {
            SDL_version version;
            SDL_VERSION(&version);
            std::cout << "SDL " << (int32_t)version.major << "." << (int32_t)version.minor << "." << (int32_t)version.patch << std::endl;
        }
        return ret; 
    }

    void shutdown()
    {
        SDL_Quit();
    }



}