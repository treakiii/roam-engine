// InputManager.h - The input handler
// This listens to all your button mashing and mouse wiggling

#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <unordered_map>
#include <vector>
#include <functional>
#include "Math/Vector3.h"

// Key codes - what keys are being pressed?
enum class KeyCode {
    Unknown = -1,
    Space = 32,
    Apostrophe = 39,
    Comma = 44,
    Minus = 45,
    Period = 46,
    Slash = 47,
    Num0 = 48,
    Num1 = 49,
    Num2 = 50,
    Num3 = 51,
    Num4 = 52,
    Num5 = 53,
    Num6 = 54,
    Num7 = 55,
    Num8 = 56,
    Num9 = 57,
    Semicolon = 59,
    Equal = 61,
    A = 65,
    B = 66,
    C = 67,
    D = 68,
    E = 69,
    F = 70,
    G = 71,
    H = 72,
    I = 73,
    J = 74,
    K = 75,
    L = 76,
    M = 77,
    N = 78,
    O = 79,
    P = 80,
    Q = 81,
    R = 82,
    S = 83,
    T = 84,
    U = 85,
    V = 86,
    W = 87,
    X = 88,
    Y = 89,
    Z = 90,
    LeftBracket = 91,
    Backslash = 92,
    RightBracket = 93,
    GraveAccent = 96,
    Escape = 256,
    Enter = 257,
    Tab = 258,
    Backspace = 259,
    Insert = 260,
    Delete = 261,
    Right = 262,
    Left = 263,
    Down = 264,
    Up = 265,
    PageUp = 266,
    PageDown = 267,
    Home = 268,
    End = 269,
    CapsLock = 280,
    ScrollLock = 281,
    NumLock = 282,
    PrintScreen = 283,
    Pause = 284,
    F1 = 290,
    F2 = 291,
    F3 = 292,
    F4 = 293,
    F5 = 294,
    F6 = 295,
    F7 = 296,
    F8 = 297,
    F9 = 298,
    F10 = 299,
    F11 = 300,
    F12 = 301,
    F13 = 302,
    F14 = 303,
    F15 = 304,
    F16 = 305,
    F17 = 306,
    F18 = 307,
    F19 = 308,
    F20 = 309,
    F21 = 310,
    F22 = 311,
    F23 = 312,
    F24 = 313,
    F25 = 314,
    LeftShift = 340,
    LeftControl = 341,
    LeftAlt = 342,
    LeftSuper = 343,
    RightShift = 344,
    RightControl = 345,
    RightAlt = 346,
    RightSuper = 347,
    Menu = 348
};

// Mouse buttons - which clicks matter?
enum class MouseButton {
    Left = 0,
    Right = 1,
    Middle = 2,
    Button4 = 3,
    Button5 = 4,
    Button6 = 5,
    Button7 = 6,
    Button8 = 7
};

// Input event types - what happened?
enum class InputEventType {
    KeyPressed,
    KeyReleased,
    KeyRepeated,
    MouseButtonPressed,
    MouseButtonReleased,
    MouseMoved,
    MouseScrolled,
    JoystickConnected,
    JoystickDisconnected
};

// Input event - the details of what happened
struct InputEvent {
    InputEventType type;
    KeyCode key;
    MouseButton mouseButton;
    Vector3 mousePosition;
    Vector3 mouseDelta;
    float scrollDelta;
    int joystickId;
    // Add more fields as needed for different event types
};

// Input action - a named input binding
struct InputAction {
    std::string name;
    std::vector<KeyCode> keys;
    std::vector<MouseButton> mouseButtons;
    float axisValue;
    bool isPressed;
    bool wasPressed;
};

// The InputManager class - our input overlord
class InputManager {
public:
    static InputManager& GetInstance();

    // Initialization
    bool Initialize();
    void Shutdown();
    void Update();

    // Keyboard input
    bool IsKeyPressed(KeyCode key) const;
    bool IsKeyReleased(KeyCode key) const;
    bool IsKeyDown(KeyCode key) const;
    bool IsKeyUp(KeyCode key) const;

    // Mouse input
    bool IsMouseButtonPressed(MouseButton button) const;
    bool IsMouseButtonReleased(MouseButton button) const;
    bool IsMouseButtonDown(MouseButton button) const;
    bool IsMouseButtonUp(MouseButton button) const;
    Vector3 GetMousePosition() const;
    Vector3 GetMouseDelta() const;
    float GetMouseScrollDelta() const;

    // Joystick/Gamepad input
    bool IsJoystickConnected(int joystickId) const;
    float GetJoystickAxis(int joystickId, int axis) const;
    bool IsJoystickButtonPressed(int joystickId, int button) const;

    // Input actions
    void CreateAction(const std::string& name);
    void BindKeyToAction(const std::string& actionName, KeyCode key);
    void BindMouseButtonToAction(const std::string& actionName, MouseButton button);
    bool IsActionPressed(const std::string& actionName) const;
    bool IsActionReleased(const std::string& actionName) const;
    bool IsActionDown(const std::string& actionName) const;
    float GetActionAxis(const std::string& actionName) const;

    // Event handling
    void AddEventListener(std::function<void(const InputEvent&)> listener);
    void RemoveEventListener(std::function<void(const InputEvent&)> listener);

    // Text input
    void StartTextInput();
    void StopTextInput();
    bool IsTextInputActive() const;
    std::string GetTextInput() const;
    void ClearTextInput();

    // Cursor control
    void ShowCursor(bool show);
    void LockCursor(bool lock);
    bool IsCursorVisible() const;
    bool IsCursorLocked() const;

    // Vibration (for controllers)
    void SetVibration(int joystickId, float leftMotor, float rightMotor);
    void StopVibration(int joystickId);

private:
    InputManager();
    ~InputManager();

    // Prevent copying
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;

    // Internal state
    std::unordered_map<KeyCode, bool> keyStates;
    std::unordered_map<KeyCode, bool> previousKeyStates;
    std::unordered_map<MouseButton, bool> mouseButtonStates;
    std::unordered_map<MouseButton, bool> previousMouseButtonStates;
    Vector3 mousePosition;
    Vector3 previousMousePosition;
    float mouseScrollDelta;

    // Joystick state
    std::vector<bool> joystickConnected;
    std::vector<std::vector<float>> joystickAxes;
    std::vector<std::vector<bool>> joystickButtons;

    // Input actions
    std::unordered_map<std::string, InputAction> actions;

    // Event listeners
    std::vector<std::function<void(const InputEvent&)>> eventListeners;

    // Text input
    bool textInputActive;
    std::string textInputBuffer;

    // Cursor state
    bool cursorVisible;
    bool cursorLocked;

    // Internal helpers
    void ProcessKeyboardInput();
    void ProcessMouseInput();
    void ProcessJoystickInput();
    void DispatchEvent(const InputEvent& event);
};

#endif // INPUTMANAGER_H
