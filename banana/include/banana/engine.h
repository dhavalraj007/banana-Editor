#pragma once

#include"banana/core/window.h"
#include"banana/managers/logmanager.h"
#include"banana/managers/rendermanager.h"

namespace banana
{   
    class Engine
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

        core::Window m_Window;

        //Managers
        inline managers::RenderManager& getRenderManager() { return m_RenderManager; }

        Engine(Engine&) = delete;
        Engine operator=(Engine&) = delete;
    
    private:
        void getInfo();
       
        [[nodiscard]] bool initialize();         //Initialize/Create Subsystems(SDL,Window)
        
        void shutdown();                        //Shutdown Subsystems(SDL,window)
        Engine();
        
        //managers
        managers::LogManager m_LogManager;
        managers::RenderManager m_RenderManager;

        bool m_isRunning;
        bool m_isInitialized;
    };
}