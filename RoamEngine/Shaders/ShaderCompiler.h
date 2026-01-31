// ShaderCompiler.h - The shader code chef
// This turns your shader code into something the GPU can understand

#ifndef SHADERCOMPILER_H
#define SHADERCOMPILER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

// Shader languages - what flavor of shader?
enum class ShaderLanguage {
    GLSL,
    HLSL,
    SPIRV,
    Metal,
    VulkanGLSL
};

// Optimization levels - how much do we optimize?
enum class OptimizationLevel {
    None,
    Basic,
    Aggressive,
    Size
};

// Shader stage - what part of the pipeline?
enum class ShaderStage {
    Vertex,
    Fragment,
    Geometry,
    TessellationControl,
    TessellationEvaluation,
    Compute
};

// Compilation result - did it work?
struct CompilationResult {
    bool success;
    std::string compiledCode;
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
    std::unordered_map<std::string, int> uniforms;
    std::unordered_map<std::string, int> attributes;
    ShaderLanguage targetLanguage;
    std::string targetVersion;

    CompilationResult() : success(false) {}
};

// Shader variant - different versions of the same shader
struct ShaderVariant {
    std::string name;
    std::unordered_map<std::string, std::string> defines;
    std::unordered_map<std::string, std::string> options;
    CompilationResult result;

    ShaderVariant(const std::string& variantName) : name(variantName) {}
};

// Include handler - for #include directives
class IncludeHandler {
public:
    virtual ~IncludeHandler() {}
    virtual std::string ResolveInclude(const std::string& includePath, const std::string& currentFile) = 0;
};

// The ShaderCompiler class - our shader cooking expert
class ShaderCompiler {
public:
    static ShaderCompiler& GetInstance();

    // Initialization - set up the kitchen
    bool Initialize();
    void Shutdown();

    // Compilation - cook the shader
    CompilationResult CompileShader(const std::string& source, ShaderStage stage,
                                   ShaderLanguage targetLanguage = ShaderLanguage::GLSL,
                                   const std::string& entryPoint = "main");

    // Source processing - prepare the ingredients
    std::string PreprocessShader(const std::string& source, const std::string& filename = "");
    std::string OptimizeShader(const std::string& source, OptimizationLevel level = OptimizationLevel::Basic);

    // Include handling - resolve dependencies
    void SetIncludeHandler(std::unique_ptr<IncludeHandler> handler);
    std::string ResolveIncludes(const std::string& source, const std::string& currentFile = "");

    // Shader variants - create different versions
    ShaderVariant* CreateVariant(const std::string& baseName, const std::string& variantName);
    void AddVariantDefine(ShaderVariant* variant, const std::string& define, const std::string& value = "");
    void AddVariantOption(ShaderVariant* variant, const std::string& option, const std::string& value);
    CompilationResult CompileVariant(ShaderVariant* variant, ShaderStage stage);

    // Cross-compilation - convert between languages
    CompilationResult CrossCompile(const std::string& source, ShaderLanguage sourceLanguage,
                                  ShaderLanguage targetLanguage, ShaderStage stage);

    // Reflection - analyze the shader
    std::unordered_map<std::string, int> ReflectUniforms(const std::string& compiledCode, ShaderLanguage language);
    std::unordered_map<std::string, int> ReflectAttributes(const std::string& compiledCode, ShaderLanguage language);
    std::vector<std::string> ReflectOutputs(const std::string& compiledCode, ShaderLanguage language);

    // Validation - check if it's correct
    std::vector<std::string> ValidateShader(const std::string& source, ShaderStage stage, ShaderLanguage language);

    // Debugging - help with troubleshooting
    std::string GenerateDebugInfo(const CompilationResult& result);
    std::string DisassembleShader(const std::string& compiledCode, ShaderLanguage language);

    // Settings - configure the compiler
    void SetOptimizationLevel(OptimizationLevel level) { defaultOptimizationLevel = level; }
    void SetTargetVersion(const std::string& version) { targetVersion = version; }
    void EnableDebugSymbols(bool enable) { debugSymbols = enable; }
    void EnableWarningsAsErrors(bool enable) { warningsAsErrors = enable; }

    // Caching - remember compiled shaders
    void EnableCaching(bool enable) { cachingEnabled = enable; }
    void ClearCache();

    // Statistics - how are we doing?
    struct CompilerStats {
        uint32_t shadersCompiled;
        uint32_t compilationErrors;
        uint32_t cacheHits;
        uint32_t cacheMisses;
        float averageCompileTime;

        CompilerStats() : shadersCompiled(0), compilationErrors(0), cacheHits(0), cacheMisses(0), averageCompileTime(0.0f) {}
    };
    const CompilerStats& GetStats() const { return stats; }
    void ResetStats();

    // Error handling - when things go wrong
    std::string GetLastError() const { return lastError; }
    void ClearError();

private:
    ShaderCompiler();
    ~ShaderCompiler();

    // Prevent copying - one compiler is enough
    ShaderCompiler(const ShaderCompiler&) = delete;
    ShaderCompiler& operator=(const ShaderCompiler&) = delete;

    // Internal state
    std::unique_ptr<IncludeHandler> includeHandler;
    std::unordered_map<std::string, ShaderVariant> variants;
    std::unordered_map<std::string, CompilationResult> cache;
    CompilerStats stats;
    std::string lastError;

    // Settings
    OptimizationLevel defaultOptimizationLevel;
    std::string targetVersion;
    bool debugSymbols;
    bool warningsAsErrors;
    bool cachingEnabled;

    // Internal helpers
    std::string ProcessDirectives(const std::string& source);
    std::string ExpandMacros(const std::string& source);
    std::string RemoveComments(const std::string& source);
    std::string MinifyCode(const std::string& source);
    bool ValidateSyntax(const std::string& source, ShaderLanguage language);
    std::string GenerateCacheKey(const std::string& source, ShaderStage stage, ShaderLanguage language);
    void UpdateStats(const CompilationResult& result, float compileTime);
};

#endif // SHADERCOMPILER_H
