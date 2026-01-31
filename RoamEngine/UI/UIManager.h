// UIManager.h - The user interface master
// This creates buttons, menus, and all that clickable stuff

#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include <functional>
#include "Math/Vector3.h"

// UI element types - what can we click?
enum class UIElementType {
    Panel,
    Button,
    TextField,
    Label,
    Image,
    Slider,
    Checkbox,
    Dropdown,
    Scrollbar,
    Window,
    Menu,
    Tooltip
};

// UI event types - what can happen?
enum class UIEventType {
    Click,
    Hover,
    Press,
    Release,
    Focus,
    Blur,
    ValueChanged,
    TextChanged,
    SelectionChanged
};

// UI alignment - where to put things
enum class UIAlignment {
    TopLeft,
    TopCenter,
    TopRight,
    MiddleLeft,
    MiddleCenter,
    MiddleRight,
    BottomLeft,
    BottomCenter,
    BottomRight
};

// UI size mode - how to size elements
enum class UISizeMode {
    Fixed,
    Percentage,
    Auto
};

// Base UI element - the foundation
class UIElement {
public:
    UIElement(UIElementType type);
    virtual ~UIElement();

    // Basic properties
    void SetPosition(const Vector3& position) { this->position = position; UpdateTransform(); }
    void SetSize(const Vector3& size) { this->size = size; UpdateTransform(); }
    void SetVisible(bool visible) { this->visible = visible; }
    void SetEnabled(bool enabled) { this->enabled = enabled; }

    const Vector3& GetPosition() const { return position; }
    const Vector3& GetSize() const { return size; }
    bool IsVisible() const { return visible; }
    bool IsEnabled() const { return enabled; }
    UIElementType GetType() const { return type; }

    // Hierarchy
    void AddChild(std::shared_ptr<UIElement> child);
    void RemoveChild(std::shared_ptr<UIElement> child);
    void RemoveFromParent();
    std::shared_ptr<UIElement> GetParent() const { return parent.lock(); }
    const std::vector<std::shared_ptr<UIElement>>& GetChildren() const { return children; }

    // Events
    void AddEventListener(UIEventType eventType, std::function<void(UIElement*)> callback);
    void RemoveEventListener(UIEventType eventType);
    void TriggerEvent(UIEventType eventType);

    // Styling
    void SetBackgroundColor(uint32_t color) { backgroundColor = color; }
    void SetBorderColor(uint32_t color) { borderColor = color; }
    void SetBorderWidth(float width) { borderWidth = width; }
    void SetCornerRadius(float radius) { cornerRadius = radius; }

    // Layout
    void SetAlignment(UIAlignment alignment) { this->alignment = alignment; }
    void SetSizeMode(UISizeMode mode) { sizeMode = mode; }
    void SetMargin(float margin) { this->margin = margin; }
    void SetPadding(float padding) { this->padding = padding; }

    // Update and render
    virtual void Update(float deltaTime);
    virtual void Render();

    // Hit testing - did we click this?
    virtual bool ContainsPoint(const Vector3& point) const;

    // Focus management
    void SetFocusable(bool focusable) { this->focusable = focusable; }
    bool IsFocusable() const { return focusable; }
    void Focus();
    void Blur();
    bool HasFocus() const { return hasFocus; }

protected:
    virtual void UpdateTransform();
    virtual void OnChildAdded(std::shared_ptr<UIElement> child) {}
    virtual void OnChildRemoved(std::shared_ptr<UIElement> child) {}

    UIElementType type;
    Vector3 position;
    Vector3 size;
    Vector3 worldPosition;
    Vector3 worldSize;
    bool visible;
    bool enabled;
    bool focusable;
    bool hasFocus;

    // Hierarchy
    std::weak_ptr<UIElement> parent;
    std::vector<std::shared_ptr<UIElement>> children;

    // Events
    std::unordered_map<UIEventType, std::vector<std::function<void(UIElement*)>>> eventListeners;

    // Styling
    uint32_t backgroundColor;
    uint32_t borderColor;
    float borderWidth;
    float cornerRadius;

    // Layout
    UIAlignment alignment;
    UISizeMode sizeMode;
    float margin;
    float padding;
};

// Button - clickable thing
class UIButton : public UIElement {
public:
    UIButton();
    ~UIButton();

    void SetText(const std::string& text) { this->text = text; }
    void SetFontSize(float size) { fontSize = size; }
    void SetTextColor(uint32_t color) { textColor = color; }

    const std::string& GetText() const { return text; }

    // Button states
    void SetPressed(bool pressed) { isPressed = pressed; }
    bool IsPressed() const { return isPressed; }

    void Update(float deltaTime) override;
    void Render() override;

protected:
    void OnClick();

    std::string text;
    float fontSize;
    uint32_t textColor;
    bool isPressed;
    bool isHovered;
};

// Text field - typeable thing
class UITextField : public UIElement {
public:
    UITextField();
    ~UITextField();

    void SetText(const std::string& text) { this->text = text; }
    void SetPlaceholder(const std::string& placeholder) { this->placeholder = placeholder; }
    void SetMaxLength(int length) { maxLength = length; }
    void SetReadOnly(bool readOnly) { this->readOnly = readOnly; }

    const std::string& GetText() const { return text; }
    const std::string& GetPlaceholder() const { return placeholder; }

    void Update(float deltaTime) override;
    void Render() override;

    // Text input
    void InsertText(const std::string& newText);
    void DeleteText(int count = 1);
    void SetCursorPosition(int position);
    int GetCursorPosition() const { return cursorPosition; }

protected:
    void OnTextChanged();
    void OnFocusGained();
    void OnFocusLost();

    std::string text;
    std::string placeholder;
    int maxLength;
    bool readOnly;
    int cursorPosition;
    float cursorBlinkTimer;
    bool showCursor;
};

// The UIManager class - our UI overlord
class UIManager {
public:
    UIManager();
    ~UIManager();

    // Initialization
    bool Initialize();
    void Shutdown();

    // Root element management
    void SetRootElement(std::shared_ptr<UIElement> root);
    std::shared_ptr<UIElement> GetRootElement() const { return rootElement; }

    // Element creation helpers
    std::shared_ptr<UIButton> CreateButton(const std::string& text = "");
    std::shared_ptr<UITextField> CreateTextField(const std::string& placeholder = "");
    std::shared_ptr<UIElement> CreateElement(UIElementType type);

    // Update and render
    void Update(float deltaTime);
    void Render();

    // Input handling
    void HandleMouseMove(const Vector3& position);
    void HandleMouseClick(bool pressed);
    void HandleMouseScroll(float delta);
    void HandleKeyPress(int keyCode, bool pressed);
    void HandleTextInput(const std::string& text);

    // Focus management
    void SetFocusedElement(std::shared_ptr<UIElement> element);
    std::shared_ptr<UIElement> GetFocusedElement() const { return focusedElement; }

    // Styling
    void SetDefaultFont(const std::string& fontPath);
    void SetDefaultStyle(const std::string& styleName);

    // Layout
    void SetScreenSize(const Vector3& size) { screenSize = size; }
    const Vector3& GetScreenSize() const { return screenSize; }

    // Debug
    void EnableDebugDraw(bool enable) { debugDraw = enable; }
    void DrawDebugInfo();

private:
    // Element finding
    std::shared_ptr<UIElement> FindElementAtPoint(const Vector3& point);

    // Input state
    Vector3 mousePosition;
    bool mousePressed;
    int lastKeyPressed;

    // UI state
    std::shared_ptr<UIElement> rootElement;
    std::shared_ptr<UIElement> focusedElement;
    std::shared_ptr<UIElement> hoveredElement;

    // Styling
    std::string defaultFont;
    Vector3 screenSize;

    // Debug
    bool debugDraw;
    bool initialized;
};

#endif // UIMANAGER_H
