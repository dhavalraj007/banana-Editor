#include <iostream>
#include "banana/engine.h"

int main()
{
    banana::Engine& engine = banana::Engine::Instance();
    engine.run();
    
    std::cout << "Press ENTER to Quit ";
    std::cin.ignore();
    return 0;
}