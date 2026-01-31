// Application.h - The face of our engine
// This is what users see, so make it pretty or they'll hate us

#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include <memory>

// Forward declarations - keeping it classy
class Window;
class Renderer;
class InputManager;
class AudioEngine;
class SceneManager;

// The Application class - our public persona
class Application {
public:
    Application();
    ~Application();

    // Initialize the application - set up the party
    bool Initialize();

    // Shutdown - clean up after the party
    void Shutdown();

    // Update - do stuff every frame
    void Update();

    // Render - make it look good
    void Render();

    // Should we quit? - the eternal question
    bool ShouldQuit() const;

    // Get window title - for that personal touch
    void SetWindowTitle(const std::string& title);
    std::string GetWindowTitle() const;

    // Window size - because size matters
    void SetWindowSize(int width, int height);
    void GetWindowSize(int& width, int& height) const;

private:
    // Our window - the gateway to the user
    std::unique_ptr<Window> window;

    // Rendering system - make it beautiful
    std::unique_ptr<Renderer> renderer;

    // Input handling - listen to the users
    std::unique_ptr<InputManager> inputManager;

    // Audio - because games need sound
    std::unique_ptr<AudioEngine> audioEngine;

    // Scene management - organize the chaos
    std::unique_ptr<SceneManager> sceneManager;

    // Application state
    bool initialized;
    bool quitRequested;
    std::string windowTitle;
    int windowWidth;
    int windowHeight;

    // Frame rate limiting - don't melt the CPU
    double targetFrameRate;
    double frameTime;
};

#endif // APPLICATION_H
