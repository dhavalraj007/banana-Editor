#include <iostream>
#include "banana/engine.h"

int main()
{
    std::cout << "Heyyya World" << std::endl;
    banana::getInfo();
    int a = banana::add(10,5);
    int b = banana::sub(10,5);
    std::cout<<a<<" ,"<<b<<std::endl;

    return 0;
}