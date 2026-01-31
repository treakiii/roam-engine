// TypeScriptManager.h - The TypeScript sorcerer
// This brings the power of TypeScript to our engine

#ifndef TYPESCRIPTMANAGER_H
#define TYPESCRIPTMANAGER_H

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>

// TypeScript compilation result
struct TypeScriptCompileResult {
    bool success;
    std::string compiledCode;
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
};

// TypeScript module - a compiled script
class TypeScriptModule {
public:
    TypeScriptModule(const std::string& name, const std::string& source);
    ~TypeScriptModule();

    // Module info
    const std::string& GetName() const { return name; }
    const std::string& GetSource() const { return source; }
    bool IsCompiled() const { return compiled; }

    // Compilation
    bool Compile();
    const TypeScriptCompileResult& GetCompileResult() const { return compileResult; }

    // Execution
    bool Execute();
    bool CallFunction(const std::string& functionName, const std::vector<std::string>& args = {});

    // Hot reload
    bool Reload(const std::string& newSource);

private:
    std::string name;
    std::string source;
    bool compiled;
    TypeScriptCompileResult compileResult;

    // Runtime data
    void* runtimeContext; // Placeholder for actual TypeScript runtime
};

// TypeScript class binding - expose C++ classes to TypeScript
template<typename T>
class TypeScriptClassBinding {
public:
    TypeScriptClassBinding(const std::string& className) : name(className) {}

    // Property binding
    template<typename PropertyType>
    TypeScriptClassBinding& BindProperty(const std::string& propertyName, PropertyType T::* property) {
        // Bind property getter/setter
        return *this;
    }

    // Method binding
    template<typename ReturnType, typename... Args>
    TypeScriptClassBinding& BindMethod(const std::string& methodName, ReturnType (T::* method)(Args...)) {
        // Bind method
        return *this;
    }

    // Constructor binding
    template<typename... Args>
    TypeScriptClassBinding& BindConstructor() {
        // Bind constructor
        return *this;
    }

    // Register the binding
    void Register() {
        // Register with TypeScript runtime
    }

private:
    std::string name;
};

// TypeScript engine integration
class TypeScriptEngine {
public:
    TypeScriptEngine();
    ~TypeScriptEngine();

    // Engine lifecycle
    bool Initialize();
    void Shutdown();
    void Update(float deltaTime);

    // Module management
    bool LoadModule(const std::string& name, const std::string& source);
    bool UnloadModule(const std::string& name);
    TypeScriptModule* GetModule(const std::string& name);

    // Compilation
    TypeScriptCompileResult CompileSource(const std::string& source, const std::string& filename = "");

    // Execution
    bool ExecuteString(const std::string& code);
    bool CallGlobalFunction(const std::string& functionName, const std::vector<std::string>& args = {});

    // Class binding
    template<typename T>
    TypeScriptClassBinding<T> BindClass(const std::string& className) {
        return TypeScriptClassBinding<T>(className);
    }

    // Global object access
    void SetGlobal(const std::string& name, int value);
    void SetGlobal(const std::string& name, float value);
    void SetGlobal(const std::string& name, bool value);
    void SetGlobal(const std::string& name, const std::string& value);

    int GetGlobalInt(const std::string& name);
    float GetGlobalFloat(const std::string& name);
    bool GetGlobalBool(const std::string& name);
    std::string GetGlobalString(const std::string& name);

    // Event system
    void RegisterEventHandler(const std::string& eventName, std::function<void(const std::vector<std::string>&)> handler);
    void TriggerEvent(const std::string& eventName, const std::vector<std::string>& args = {});

    // Debugging
    void EnableDebugging(bool enable);
    std::string GetStackTrace();
    std::vector<std::string> GetBreakpoints();
    void SetBreakpoint(const std::string& file, int line);
    void ClearBreakpoint(const std::string& file, int line);

    // Hot reload
    bool EnableHotReload(bool enable);
    void CheckForChanges();

private:
    // Module storage
    std::unordered_map<std::string, std::unique_ptr<TypeScriptModule>> modules;

    // Event handlers
    std::unordered_map<std::string, std::vector<std::function<void(const std::vector<std::string>&)>>> eventHandlers;

    // Runtime state
    bool initialized;
    bool debuggingEnabled;
    bool hotReloadEnabled;

    // File watching for hot reload
    std::unordered_map<std::string, std::string> fileContents;
    std::unordered_map<std::string, std::chrono::system_clock::time_point> fileTimestamps;
};

// The TypeScriptManager class - our TypeScript overlord
class TypeScriptManager {
public:
    static TypeScriptManager& GetInstance();

    // Manager lifecycle
    bool Initialize();
    void Shutdown();
    void Update(float deltaTime);

    // Script loading
    bool LoadScript(const std::string& name, const std::string& filename);
    bool LoadScriptFromString(const std::string& name, const std::string& source);
    bool UnloadScript(const std::string& name);

    // Execution
    bool ExecuteScript(const std::string& name);
    bool CallScriptFunction(const std::string& scriptName, const std::string& functionName,
                           const std::vector<std::string>& args = {});

    // Engine access
    TypeScriptEngine* GetEngine() { return &engine; }

    // Utility functions for scripts
    void RegisterUtilityFunctions();

    // Error handling
    std::string GetLastError() const { return lastError; }
    void ClearError() { lastError.clear(); }

    // Debugging
    void EnableDebugMode(bool enable) { debugMode = enable; }
    bool IsDebugMode() const { return debugMode; }

private:
    TypeScriptManager();
    ~TypeScriptManager();

    // Prevent copying
    TypeScriptManager(const TypeScriptManager&) = delete;
    TypeScriptManager& operator=(const TypeScriptManager&) = delete;

    // Engine
    TypeScriptEngine engine;

    // State
    bool initialized;
    std::string lastError;
    bool debugMode;
};

#endif // TYPESCRIPTMANAGER_H
