#include"engine.h"
#include<iostream>

namespace banana
{
    int add(int a,int b)
    {
        return a+b;
    }
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
}