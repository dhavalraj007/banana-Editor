#include <iostream>
#include "banana/engine.h"

int main()
{
    std::cout << "Heyyya World" << std::endl;
    banana::getInfo();
    banana::initialize();
    banana::shutdown();

    return 0;
}