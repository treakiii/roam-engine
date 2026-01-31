// ShaderManager.h - The shader sorcerer
// This manages all our magical shader programs

#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

// Shader types - vertex, fragment, etc.
enum class ShaderType {
    Vertex,
    Fragment,
    Geometry,
    Compute,
    TessellationControl,
    TessellationEvaluation
};

// Shader program - a collection of shaders
class ShaderProgram {
public:
    ShaderProgram();
    ~ShaderProgram();

    // Create from source - write the magic
    bool CreateFromSource(const std::string& vertexSource, const std::string& fragmentSource);

    // Create from files - load the spells
    bool CreateFromFiles(const std::string& vertexFile, const std::string& fragmentFile);

    // Use this program - cast the spell
    void Use();

    // Set uniforms - configure the magic
    void SetUniform(const std::string& name, int value);
    void SetUniform(const std::string& name, float value);
    void SetUniform(const std::string& name, const Vector3& value);
    void SetUniform(const std::string& name, const Matrix4x4& value);

    // Get program ID - the magical identifier
    unsigned int GetProgramID() const { return programID; }

    // Is valid? - does the magic work?
    bool IsValid() const { return programID != 0; }

private:
    // Compile shader - forge the component
    unsigned int CompileShader(ShaderType type, const std::string& source);

    // Link program - combine the components
    bool LinkProgram(unsigned int vertexShader, unsigned int fragmentShader);

    // Program ID - the heart of the magic
    unsigned int programID;

    // Shader IDs - the components
    unsigned int vertexShaderID;
    unsigned int fragmentShaderID;
};

// The ShaderManager class - our shader librarian
class ShaderManager {
public:
    ShaderManager();
    ~ShaderManager();

    // Load shader program - get it from the library
    ShaderProgram* LoadShader(const std::string& name, const std::string& vertexFile, const std::string& fragmentFile);

    // Get shader program - retrieve from cache
    ShaderProgram* GetShader(const std::string& name);

    // Unload shader - return to the library
    void UnloadShader(const std::string& name);

    // Reload all shaders - refresh the magic
    void ReloadAllShaders();

    // Get shader names - list the spells
    std::vector<std::string> GetShaderNames() const;

private:
    // Our shader collection - the library
    std::unordered_map<std::string, std::unique_ptr<ShaderProgram>> shaders;

    // Shader directory - where the spells are stored
    std::string shaderDirectory;
};

#endif // SHADERMANAGER_H
