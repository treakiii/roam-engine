// Engine.h - The beating heart of RoamEngine
// This is where the magic happens, or the crashes begin

#ifndef ENGINE_H
#define ENGINE_H

#include <memory>
#include <string>

// Forward declarations - because we're fancy like that
class Application;
class Logger;
class TimeManager;
class ConfigManager;
class EventSystem;
class ResourceManager;
class ThreadManager;

// The Engine class - our digital god
class Engine {
public:
    Engine();
    ~Engine();

    // Initialize the engine - hope it doesn't explode
    bool Initialize(const std::string& configFile = "config.ini");

    // Run the main loop - infinite fun or infinite pain
    void Run();

    // Shutdown gracefully - or not so gracefully
    void Shutdown();

    // Get various managers - because encapsulation is overrated sometimes
    Application* GetApplication() { return application.get(); }
    Logger* GetLogger() { return logger.get(); }
    TimeManager* GetTimeManager() { return timeManager.get(); }

private:
    // All our precious managers
    std::unique_ptr<Application> application;
    std::unique_ptr<Logger> logger;
    std::unique_ptr<TimeManager> timeManager;
    std::unique_ptr<ConfigManager> configManager;
    std::unique_ptr<EventSystem> eventSystem;
    std::unique_ptr<ResourceManager> resourceManager;
    std::unique_ptr<ThreadManager> threadManager;

    // Engine state - running or crying in a corner
    bool isRunning;
};

#endif // ENGINE_H
