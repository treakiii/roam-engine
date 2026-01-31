// LuaManager.cpp - The implementation of Lua scripting
// This code is a masterpiece, or so we tell ourselves

#include "LuaManager.h"
#include <iostream>

LuaManager::LuaManager() : luaState(nullptr) {
    // Constructor - nothing to see here
}

LuaManager::~LuaManager() {
    // Destructor - clean up the mess
    if (luaState) {
        lua_close(luaState);
        luaState = nullptr;
    }
}

bool LuaManager::Initialize() {
    // Create Lua state - fingers crossed
    luaState = luaL_newstate();
    if (!luaState) {
        std::cerr << "Failed to create Lua state - this is shit" << std::endl;
        return false;
    }

    // Open standard libraries - because scripts need them
    luaL_openlibs(luaState);

    // This is beautiful, or else the engine will crash out
    std::cout << "LuaManager initialized successfully" << std::endl;
    return true;
}

bool LuaManager::ExecuteScript(const std::string& script) {
    // Load and execute the script - hope it's not malicious
    if (luaL_dostring(luaState, script.c_str()) != LUA_OK) {
        std::cerr << "Failed to execute Lua script: " << lua_tostring(luaState, -1) << std::endl;
        lua_pop(luaState, 1); // Pop the error message
        return false;
    }
    return true;
}

bool LuaManager::CallFunction(const std::string& funcName) {
    // Get the function from the global table
    lua_getglobal(luaState, funcName.c_str());
    if (!lua_isfunction(luaState, -1)) {
        std::cerr << "Function '" << funcName << "' not found - this is shit" << std::endl;
        lua_pop(luaState, 1);
        return false;
    }

    // Call the function - no arguments, no returns for now
    if (lua_pcall(luaState, 0, 0, 0) != LUA_OK) {
        std::cerr << "Failed to call function '" << funcName << "': " << lua_tostring(luaState, -1) << std::endl;
        lua_pop(luaState, 1);
        return false;
    }

    return true;
}
