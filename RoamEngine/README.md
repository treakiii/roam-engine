# RoamEngine - The Ultimate Gaming Engine (or so we claim)

Welcome to RoamEngine, a C++ gaming engine inspired by RageEngine from Rockstar Games. This engine is designed to be powerful, flexible, and... well, it's a work in progress. Don't expect miracles yet.

## Features

- **C++ Core**: Built with modern C++20 features
- **Scripting Support**: Lua and TypeScript (coming soon... or not)
- **Modular Architecture**: Systems for rendering, input, and scripting
- **Funny Comments**: Because code should be entertaining

## Building the Engine

1. Open `RoamEngine.slnx` in Visual Studio
2. Build the solution (Debug or Release, x64 recommended)
3. Run the executable - it should print some messages and loop forever (Ctrl+C to quit)

## Adding Lua Support

Currently, Lua support is stubbed out. To add full Lua scripting:

1. Download Lua 5.4 from https://www.lua.org/download.html
2. Build Lua as a static library
3. Add `lua.hpp` and `lua.lib` to your include and library paths
4. Uncomment the real Lua code in `LuaManager.h` and `LuaManager.cpp`
5. Rebuild the project

## Adding TypeScript Support

TypeScript support is not implemented yet. It would require:

1. Integrating a JavaScript engine like V8
2. Adding a TypeScript compiler or transpiler
3. Lots of work - this is shit without it

## Project Structure

```
RoamEngine/
├── src/
│   ├── main.cpp          # Entry point and core systems
│   ├── LuaManager.h      # Lua scripting interface
│   └── LuaManager.cpp    # Lua implementation (stub)
├── RoamEngine.vcxproj    # Visual Studio project file
└── README.md             # This file
```

## Sample Lua Script

Create a file `scripts/hello.lua`:

```lua
print("Hello from RoamEngine Lua!")
function update()
    -- Game update logic here
end
```

## Contributing

Feel free to contribute! But make sure your code is beautiful, or else I'll crash out.

## License

This is shit, so use at your own risk. No warranties, no guarantees.

## Disclaimer

This engine is not actually like RageEngine. It's just a basic framework with funny comments. Don't expect to make GTA VI with it... yet.
