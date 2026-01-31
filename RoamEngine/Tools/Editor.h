// Editor.h - The level designer
// This lets you build worlds without writing code

#ifndef EDITOR_H
#define EDITOR_H

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>
#include "Math/Vector3.h"

// Editor modes - what are we doing?
enum class EditorMode {
    Edit,
    Play,
    Pause,
    Simulate
};

// Tool types - what are we using?
enum class ToolType {
    Select,
    Move,
    Rotate,
    Scale,
    Terrain,
    Paint,
    Place,
    Erase,
    Measure,
    Custom
};

// Gizmo - the 3D manipulation widget
struct Gizmo {
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    enum class GizmoType { Translate, Rotate, Scale };
    GizmoType type;
    bool visible;
    bool selected;

    Gizmo() : position(0, 0, 0), rotation(0, 0, 0), scale(1, 1, 1),
             type(GizmoType::Translate), visible(true), selected(false) {}
};

// Grid - the reference surface
struct Grid {
    Vector3 position;
    Vector3 normal;
    float size;
    float spacing;
    bool visible;
    uint32_t color;

    Grid() : position(0, 0, 0), normal(0, 1, 0), size(100), spacing(1),
            visible(true), color(0xFF808080) {}
};

// Viewport - the camera view
struct Viewport {
    int x, y, width, height;
    enum class ViewType { Perspective, Orthographic, Top, Front, Side };
    ViewType type;
    float fov; // for perspective
    float orthoSize; // for orthographic
    bool active;

    Viewport() : x(0), y(0), width(800), height(600),
                type(ViewType::Perspective), fov(60), orthoSize(10), active(true) {}
};

// Snap settings - for precise placement
struct SnapSettings {
    bool enabled;
    float positionSnap;
    float rotationSnap;
    float scaleSnap;
    bool snapToGrid;
    bool snapToObjects;

    SnapSettings() : enabled(false), positionSnap(0.5f), rotationSnap(15.0f),
                    scaleSnap(0.1f), snapToGrid(true), snapToObjects(false) {}
};

// Undo/Redo action
struct EditorAction {
    std::string description;
    std::function<void()> doAction;
    std::function<void()> undoAction;
    uint64_t timestamp;

    EditorAction(const std::string& desc, std::function<void()> doFunc, std::function<void()> undoFunc)
        : description(desc), doAction(doFunc), undoAction(undoFunc),
          timestamp(std::chrono::duration_cast<std::chrono::milliseconds>(
              std::chrono::system_clock::now().time_since_epoch()).count()) {}
};

// Selection - what objects are selected?
struct Selection {
    std::vector<std::string> objectIds;
    Vector3 selectionCenter;
    Vector3 selectionBounds;

    bool IsEmpty() const { return objectIds.empty(); }
    void Clear() { objectIds.clear(); selectionCenter = Vector3(0, 0, 0); selectionBounds = Vector3(0, 0, 0); }
    void AddObject(const std::string& id);
    void RemoveObject(const std::string& id);
    bool ContainsObject(const std::string& id) const;
};

// The Editor class - our world builder
class Editor {
public:
    static Editor& GetInstance();

    // Initialization
    bool Initialize();
    void Shutdown();
    void Update(float deltaTime);
    void Render();

    // Mode management
    void SetMode(EditorMode mode);
    EditorMode GetMode() const { return currentMode; }
    bool IsInEditMode() const { return currentMode == EditorMode::Edit; }
    bool IsPlaying() const { return currentMode == EditorMode::Play; }

    // Tool management
    void SetActiveTool(ToolType tool);
    ToolType GetActiveTool() const { return activeTool; }
    void RegisterTool(ToolType type, const std::string& name, std::function<void()> activateFunc);

    // Selection
    Selection& GetSelection() { return selection; }
    void SelectObject(const std::string& objectId);
    void SelectObjects(const std::vector<std::string>& objectIds);
    void ClearSelection();
    void SelectAll();
    void InvertSelection();

    // Gizmo
    Gizmo& GetGizmo() { return gizmo; }
    void ShowGizmo(bool show) { gizmo.visible = show; }
    void UpdateGizmo();

    // Grid
    Grid& GetGrid() { return grid; }
    void ShowGrid(bool show) { grid.visible = show; }

    // Viewports
    void AddViewport(const Viewport& viewport);
    void RemoveViewport(size_t index);
    Viewport& GetViewport(size_t index);
    const std::vector<Viewport>& GetViewports() const { return viewports; }
    Viewport& GetActiveViewport();

    // Snap settings
    SnapSettings& GetSnapSettings() { return snapSettings; }
    Vector3 SnapPosition(const Vector3& position) const;
    float SnapRotation(float rotation) const;
    Vector3 SnapScale(const Vector3& scale) const;

    // Undo/Redo
    void PerformAction(const std::string& description, std::function<void()> doAction, std::function<void()> undoAction);
    void Undo();
    void Redo();
    bool CanUndo() const { return !undoStack.empty(); }
    bool CanRedo() const { return !redoStack.empty(); }
    void ClearHistory();

    // Object management
    void CreateObject(const std::string& type, const Vector3& position = Vector3(0, 0, 0));
    void DeleteObject(const std::string& objectId);
    void DuplicateObject(const std::string& objectId);
    void MoveObject(const std::string& objectId, const Vector3& newPosition);
    void RotateObject(const std::string& objectId, const Vector3& newRotation);
    void ScaleObject(const std::string& objectId, const Vector3& newScale);

    // Scene management
    void NewScene();
    void LoadScene(const std::string& filename);
    void SaveScene(const std::string& filename);
    void SaveSceneAs(const std::string& filename);
    std::string GetCurrentSceneName() const { return currentSceneName; }

    // Prefabs
    void CreatePrefab(const std::string& objectId, const std::string& prefabName);
    void InstantiatePrefab(const std::string& prefabName, const Vector3& position = Vector3(0, 0, 0));

    // Input handling
    void HandleMouseMove(const Vector3& position);
    void HandleMouseClick(bool pressed);
    void HandleMouseScroll(float delta);
    void HandleKeyPress(int keyCode, bool pressed);

    // UI
    void ShowMainMenu();
    void ShowToolbar();
    void ShowInspector();
    void ShowHierarchy();
    void ShowConsole();
    void ShowAssetBrowser();

    // Settings
    void LoadSettings(const std::string& filename);
    void SaveSettings(const std::string& filename);

    // Plugins
    void LoadPlugin(const std::string& filename);
    void UnloadPlugin(const std::string& pluginName);

    // Debugging
    void EnableDebugDraw(bool enable) { debugDraw = enable; }
    void DrawDebugInfo();

private:
    Editor();
    ~Editor();

    // Prevent copying
    Editor(const Editor&) = delete;
    Editor& operator=(const Editor&) = delete;

    // Core state
    EditorMode currentMode;
    ToolType activeTool;
    Selection selection;
    Gizmo gizmo;
    Grid grid;
    std::vector<Viewport> viewports;
    SnapSettings snapSettings;

    // Undo/Redo
    std::vector<EditorAction> undoStack;
    std::vector<EditorAction> redoStack;
    const size_t maxHistorySize = 100;

    // Scene
    std::string currentSceneName;
    std::string currentScenePath;

    // Tools
    std::unordered_map<ToolType, std::pair<std::string, std::function<void()>>> tools;

    // Plugins
    std::unordered_map<std::string, void*> loadedPlugins;

    // Debug
    bool debugDraw;

    // Internal helpers
    void UpdateEditMode(float deltaTime);
    void UpdatePlayMode(float deltaTime);
    void RenderEditMode();
    void RenderPlayMode();
    void ProcessInput();
    void UpdateSelection();
    void UpdateGizmoTransform();
    std::string GenerateObjectId() const;
};

#endif // EDITOR_H
