#pragma once

#include"banana/core/window.h"
#include"banana/managers/logmanager.h"
#include"banana/managers/rendermanager.h"

namespace banana
{   
    class Engine                        // main Class that holds window,Managers
    {
    public:
        static Engine& Instance()
        {
            static Engine instance;             // may be this will be on the heap (if the Engine is on heap)
            return instance;
        }
        ~Engine(){}
        void run();                             //Contains Main loop
        inline void quit() { m_isRunning = false; }

        inline core::Window& getWindow() { return m_Window; }
        //Managers
        inline managers::RenderManager& getRenderManager() { return m_RenderManager; }

        Engine(Engine&) = delete;
        Engine operator=(Engine&) = delete;
    
        core::Window m_Window;
    private:
       
        Engine();                               //initializes isRunning and isInitialized as false
        [[nodiscard]] bool initialize();         //Initialize/Create Subsystems(logManager,SDL,window,renderManager)
        void getInfo();
        
        void shutdown();                        //Shutdown Subsystems(renderManger,window,logManager)
        
        //managers
        managers::LogManager m_LogManager;
        managers::RenderManager m_RenderManager;

        bool m_isRunning;                       // helps in when to close the window
        bool m_isInitialized;                   // to restrict calling Engine::initialize() more than once
    };
}