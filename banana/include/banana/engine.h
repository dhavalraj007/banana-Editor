#pragma once

#include"banana/core/window.h"
#include"banana/managers/logmanager.h"
#include"banana/managers/rendermanager.h"

namespace banana
{   
    class App;
    class Engine                        // main Class that holds window,Managers
    {
    public:
        static Engine& Instance()
        {
            static Engine instance;             // may be this will be on the heap (if the Engine is on heap)
            return instance;
        }
        ~Engine(){}
        void run(App* app);                             //Contains Main loop
        inline void quit() { m_isRunning = false; }

        inline App& getApp() { return *m_App; }
        inline core::Window& getWindow() { return m_Window; }
        inline managers::RenderManager& getRenderManager() { return m_RenderManager; }
        uint32_t getInitTime();
        Engine(Engine&) = delete;
        Engine operator=(Engine&) = delete;
    
        core::Window m_Window;
    private:
       
        Engine();                               //initializes isRunning and isInitialized as false
        [[nodiscard]] bool initialize();         //Initialize/Create Subsystems(logManager,SDL,window,renderManager)
        void getInfo();
        void shutdown();                        //Shutdown Subsystems(renderManger,window,logManager)
        void update(float deltaTime);
        void render();

        //managers
        managers::LogManager m_LogManager;
        managers::RenderManager m_RenderManager;
        App* m_App;

        bool m_isRunning;                       // helps in when to close the window
        bool m_isInitialized;                   // to restrict calling Engine::initialize() more than once
    };
}