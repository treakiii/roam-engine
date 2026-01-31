// Engine.cpp - Implementation of our digital deity
// This code is either brilliant or a complete disaster

#include "Engine.h"
#include <iostream>

// Include all the managers - hope they don't fight
#include "Application.h"
#include "Logger.h"
#include "TimeManager.h"
#include "ConfigManager.h"
#include "EventSystem.h"
#include "ResourceManager.h"
#include "ThreadManager.h"

Engine::Engine() : isRunning(false) {
    // Constructor - setting up the throne room
    std::cout << "Engine constructor called - initializing the chaos" << std::endl;
}

Engine::~Engine() {
    // Destructor - cleaning up the mess
    if (isRunning) {
        Shutdown();
    }
    std::cout << "Engine destructor called - the end is nigh" << std::endl;
}

bool Engine::Initialize(const std::string& configFile) {
    // Initialize all managers - this is where things get real
    try {
        logger = std::make_unique<Logger>();
        configManager = std::make_unique<ConfigManager>();
        timeManager = std::make_unique<TimeManager>();
        eventSystem = std::make_unique<EventSystem>();
        resourceManager = std::make_unique<ResourceManager>();
        threadManager = std::make_unique<ThreadManager>();
        application = std::make_unique<Application>();

        // Load config - because defaults are for losers
        if (!configManager->LoadConfig(configFile)) {
            std::cerr << "Failed to load config file: " << configFile << std::endl;
            return false;
        }

        // Initialize application last - it's the boss
        if (!application->Initialize()) {
            std::cerr << "Failed to initialize application" << std::endl;
            return false;
        }

        isRunning = true;
        std::cout << "Engine initialized successfully - let's make some games!" << std::endl;
        return true;

    } catch (const std::exception& e) {
        std::cerr << "Engine initialization failed: " << e.what() << std::endl;
        return false;
    }
}

void Engine::Run() {
    // Main game loop - the heart of the beast
    if (!isRunning) {
        std::cerr << "Engine not initialized - call Initialize() first, you fool!" << std::endl;
        return;
    }

    std::cout << "Engine running - may the games begin!" << std::endl;

    while (isRunning) {
        // Update time - because time flies when you're having fun
        timeManager->Update();

        // Process events - handle the chaos
        eventSystem->ProcessEvents();

        // Update application - do the actual work
        application->Update();

        // Render - make it pretty
        application->Render();

        // Check for quit - don't leave us hanging
        if (application->ShouldQuit()) {
            isRunning = false;
        }
    }

    std::cout << "Engine run loop ended - time for a nap" << std::endl;
}

void Engine::Shutdown() {
    // Shutdown everything - gracefully, we hope
    if (application) {
        application->Shutdown();
    }

    // Shutdown managers in reverse order - like a civilized shutdown
    threadManager.reset();
    resourceManager.reset();
    eventSystem.reset();
    timeManager.reset();
    configManager.reset();
    logger.reset();

    isRunning = false;
    std::cout << "Engine shutdown complete - see you next time!" << std::endl;
}
