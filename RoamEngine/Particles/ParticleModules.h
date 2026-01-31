// ParticleModules.h - The particle module collection
// Extra settings for your particle parties

#ifndef PARTICLEMODULES_H
#define PARTICLEMODULES_H

#include <vector>
#include <functional>
#include "Math/Vector3.h"

// Module types - what can we add to particles?
enum class ModuleType {
    Main,
    Emission,
    Shape,
    Velocity,
    Color,
    Size,
    Rotation,
    Texture,
    Renderer,
    Custom
};

// Curve - for smooth value changes over time
struct Curve {
    enum class CurveType { Constant, Linear, EaseIn, EaseOut, EaseInOut, Custom };
    CurveType type;
    float startValue;
    float endValue;
    std::function<float(float)> customFunction; // time -> value

    Curve() : type(CurveType::Constant), startValue(0), endValue(1) {}

    float Evaluate(float t) const {
        switch (type) {
            case CurveType::Constant: return startValue;
            case CurveType::Linear: return startValue + (endValue - startValue) * t;
            case CurveType::EaseIn: return startValue + (endValue - startValue) * (t * t);
            case CurveType::EaseOut: return startValue + (endValue - startValue) * (1 - (1 - t) * (1 - t));
            case CurveType::EaseInOut: {
                float t2 = t * t;
                return startValue + (endValue - startValue) * (3 * t2 - 2 * t * t2);
            }
            case CurveType::Custom: return customFunction ? customFunction(t) : startValue;
            default: return startValue;
        }
    }
};

// Module settings base - common settings for all modules
struct ModuleSettings {
    bool enabled;
    ModuleType type;

    ModuleSettings(ModuleType t) : enabled(true), type(t) {}
    virtual ~ModuleSettings() {}
};

// Main module settings - overall system control
struct MainModuleSettings : ModuleSettings {
    int maxParticles;
    float duration;
    float startDelay;
    bool looping;
    bool playOnAwake;
    bool useUnscaledTime;
    float gravityModifier;
    Vector3 gravity;
    float simulationSpeed;

    MainModuleSettings() : ModuleSettings(ModuleType::Main), maxParticles(1000), duration(5),
                          startDelay(0), looping(false), playOnAwake(true), useUnscaledTime(false),
                          gravityModifier(1), gravity(0, -9.81f, 0), simulationSpeed(1) {}
};

// Emission module settings - how particles are created
struct EmissionModuleSettings : ModuleSettings {
    enum class EmissionShape { Point, Sphere, Box, Cone, Circle, Rectangle };
    EmissionShape shape;
    float emissionRate;
    float duration;
    bool looping;
    int maxParticles;
    float startDelay;
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;

    // Shape parameters
    float radius;
    float angle;
    Vector3 boxSize;

    // Randomization
    Curve positionVariance;
    Curve velocityVariance;
    Curve sizeVariance;
    Curve lifeVariance;

    EmissionModuleSettings() : ModuleSettings(ModuleType::Emission), shape(EmissionShape::Point),
                              emissionRate(10), duration(5), looping(false), maxParticles(100),
                              startDelay(0), position(0, 0, 0), rotation(0, 0, 0), scale(1, 1, 1),
                              radius(1), angle(45), boxSize(1, 1, 1) {}
};

// Shape module settings - emission area definition
struct ShapeModuleSettings : ModuleSettings {
    enum class ShapeType { Point, Sphere, Box, Cone, Circle, Rectangle };
    ShapeType shape;
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    bool alignToDirection;
    float radius;
    float angle;
    float length;
    Vector3 boxSize;
    bool emitFromShell;
    bool emitFromEdge;

    ShapeModuleSettings() : ModuleSettings(ModuleType::Shape), shape(ShapeType::Point),
                           position(0, 0, 0), rotation(0, 0, 0), scale(1, 1, 1),
                           alignToDirection(false), radius(1), angle(45), length(1),
                           boxSize(1, 1, 1), emitFromShell(false), emitFromEdge(false) {}
};

// Velocity module settings - particle movement
struct VelocityModuleSettings : ModuleSettings {
    Vector3 linearVelocity;
    Curve speedModifier;
    bool useLocalSpace;
    Curve velocityCurveX;
    Curve velocityCurveY;
    Curve velocityCurveZ;

    VelocityModuleSettings() : ModuleSettings(ModuleType::Velocity), linearVelocity(0, 0, 0),
                              useLocalSpace(false) {}
};

// Color module settings - particle colors
struct ColorModuleSettings : ModuleSettings {
    uint32_t startColor;
    uint32_t endColor;
    Curve colorCurveR;
    Curve colorCurveG;
    Curve colorCurveB;
    Curve colorCurveA;

    ColorModuleSettings() : ModuleSettings(ModuleType::Color), startColor(0xFFFFFFFF),
                           endColor(0x00FFFFFF) {}
};

// Size module settings - particle sizes
struct SizeModuleSettings : ModuleSettings {
    float startSize;
    float endSize;
    Curve sizeCurve;

    SizeModuleSettings() : ModuleSettings(ModuleType::Size), startSize(1), endSize(1) {}
};

// Rotation module settings - particle spinning
struct RotationModuleSettings : ModuleSettings {
    float startRotation;
    float endRotation;
    Curve angularVelocity;

    RotationModuleSettings() : ModuleSettings(ModuleType::Rotation), startRotation(0),
                              endRotation(0) {}
};

// Texture module settings - animated particles
struct TextureModuleSettings : ModuleSettings {
    int tilesX;
    int tilesY;
    int animationSpeed;
    bool loop;
    int startFrame;
    Curve frameCurve;

    TextureModuleSettings() : ModuleSettings(ModuleType::Texture), tilesX(1), tilesY(1),
                             animationSpeed(1), loop(true), startFrame(0) {}
};

// Renderer module settings - how particles look
struct RendererModuleSettings : ModuleSettings {
    enum class RenderMode { Billboard, Stretched, Mesh, Trail };
    RenderMode renderMode;
    float stretchFactor;
    std::string meshName;
    bool castShadows;
    bool receiveShadows;
    int renderQueue;
    float sortingFudge;

    RendererModuleSettings() : ModuleSettings(ModuleType::Renderer), renderMode(RenderMode::Billboard),
                              stretchFactor(1), castShadows(false), receiveShadows(false),
                              renderQueue(3000), sortingFudge(0) {}
};

// Custom module settings - for special effects
struct CustomModuleSettings : ModuleSettings {
    std::string moduleName;
    std::unordered_map<std::string, float> floatParams;
    std::unordered_map<std::string, Vector3> vectorParams;
    std::unordered_map<std::string, std::string> stringParams;
    std::function<void(void*, float)> updateFunction; // custom update logic

    CustomModuleSettings(const std::string& name) : ModuleSettings(ModuleType::Custom), moduleName(name) {}
};

// Module collection - all modules for a particle system
struct ParticleModuleCollection {
    MainModuleSettings main;
    EmissionModuleSettings emission;
    ShapeModuleSettings shape;
    VelocityModuleSettings velocity;
    ColorModuleSettings color;
    SizeModuleSettings size;
    RotationModuleSettings rotation;
    TextureModuleSettings texture;
    RendererModuleSettings renderer;
    std::vector<CustomModuleSettings> customModules;

    // Module management
    void AddCustomModule(const CustomModuleSettings& module) {
        customModules.push_back(module);
    }

    void RemoveCustomModule(const std::string& name) {
        customModules.erase(
            std::remove_if(customModules.begin(), customModules.end(),
                [&name](const CustomModuleSettings& m) { return m.moduleName == name; }),
            customModules.end()
        );
    }

    CustomModuleSettings* GetCustomModule(const std::string& name) {
        for (auto& module : customModules) {
            if (module.moduleName == name) {
                return &module;
            }
        }
        return nullptr;
    }
};

#endif // PARTICLEMODULES_H
