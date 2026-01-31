// LuaManager.h - Because scripting is cool, or so they say
// This is shit, but it'll make the engine "scriptable"

#ifndef LUAMANAGER_H
#define LUAMANAGER_H

#include <string>

// LuaManager - handles all that Lua nonsense
// Note: Full Lua support requires installing Lua library (lua.hpp, lua.lib)
// For now, this is a stub - make it beautiful or else I'll crash out
class LuaManager {
public:
    LuaManager();
    ~LuaManager();

    // Initialize Lua - hope it doesn't crash the whole engine
    bool Initialize();

    // Execute a Lua script - make it beautiful or else the scripts will rage quit
    bool ExecuteScript(const std::string& script);

    // Call a Lua function - because why not?
    bool CallFunction(const std::string& funcName);

private:
    // Placeholder for Lua state - this is shit without actual Lua
    void* luaState; // In real implementation, this would be lua_State*
};

#endif // LUAMANAGER_H
