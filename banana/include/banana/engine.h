#pragma once

#include"core/window.h"
#include"managers/logmanager.h"

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
        //Contains Main loop
        void run();
        inline void quit() { m_isRunning = false; }

        core::Window m_Window;
        //managers
        managers::LogManager m_LogManager;

        Engine(Engine&) = delete;
        Engine operator=(Engine&) = delete;
    
    private:
        void getInfo();
        //Initialize/Create Subsystems(SDL,Window)
        [[nodiscard]] bool initialize();
        //Shutdown Subsystems(SDL,window)
        void shutdown();
        Engine();

        bool m_isRunning;
        bool m_isInitialized;
    };
}