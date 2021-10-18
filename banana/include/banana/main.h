#pragma once
#include"banana/engine.h"
#include"banana/app.h"

//to be implemented in client app
//the function returns a pointer to a instance derived from banana::App
//ownership belongs to Banana
banana::App* createApp();

int main()
{
    banana::App* app = createApp();
    banana::Engine::Instance().run(app);
    delete app;
    return 0;
}
