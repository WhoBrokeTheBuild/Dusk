#ifndef DUSK_SHADER_HPP
#define DUSK_SHADER_HPP

#include <dusk/Config.hpp>

#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <memory>

namespace dusk {

class Shader
{
public:

    /// Boilerplate

    DISALLOW_COPY_AND_ASSIGN(Shader);

    std::unique_ptr<Shader> Create();
    std::unique_ptr<Shader> Create(const std::vector<std::string>& filenames);

    /** Create an empty shader, for use with LoadFromFile().
     */
    Shader() = default;

    /** Create and load a shader from the given filenames.
     */
    Shader(const std::vector<std::string>& filenames);

    virtual ~Shader();

    /// Static Methods

    /** Initialize the version string to prepend to all shader files.
     * Uses GL_SHADING_LANGUAGE_VERSION to generate the #version string to
     * prepend onto shader files.
     * e.g. for OpenGL 4.5 and GLSL version 4.50, the string would be
     * '#version 450 core'
     */
    static void InitializeVersionString();

    /** Initialize the structures used to track Uniform Buffer Objects
     */
    static void InitializeUniformBuffers();

    /** Update the Uniform Buffer Object's data for all shaders it exists in.
     * @param name The variable name of the UBO to update.
     * @param data A pointer to the data to load into the UBO, must be properly
     * aligned.
     */
    static void SetUniformBufferData(const std::string& name, GLvoid * data);

    /// Methods

    /** Load and compile the given shader files, then link the program.
     */
    virtual bool LoadFromFiles(const std::vector<std::string>& filenames);

    /** Check if the shader has been successfully loaded.
     * @return True if the shader has successfully loaded.
     */
    inline bool IsLoaded() { return _loaded; }

    /** Bind the shader for use with rendering or update operations.
     * This internally calls glUseProgram()
     */
    void Bind();

    /** Check if the shader has an attribute.
     * @param name The name of the attribute to check.
     * @return True if the attribute exists within the shader.
     */
    bool HasAttribute(const std::string& name) const;

    /** Get the location of an attribute within the shader.
     * @param name The name of the attribute to get the location of.
     * @return The location of the attribute
     */
    GLint GetAttributeLocation(const std::string& name) const;

    /** Check if the shader has an uniform.
     * @param name The name of the uniform to check.
     * @return True if the uniform exists within the shader.
     */
    bool HasUniform(const std::string& name) const;

    /** Get the location of an uniform within the shader.
     * @param name The name of the uniform to get the location of.
     * @return The location of the uniform
     */
    GLint GetUniformLocation(const std::string& name) const;

    #include "Shader.inc.hpp"

protected:

    /** Hash the filenames into a unique identifier for the compiled shader binary.
     * @param filenames The list of filenames to hash.
     * @return The generated filename for the shader binary.
     */
    virtual std::string GetBinaryName(const std::vector<std::string> filenames);

private:

    /// Types

    struct UniformRecord
    {
        GLint  Location;
        GLint  Size;
        GLenum Type;
    };

    struct UniformBufferRecord
    {
        GLuint GLID;
        GLuint Binding;
        size_t Size;
    };

    /// Static Variables

    static std::string _GLSLVersionString;

    static std::queue<GLuint> _AvailableUniformBufferBindings;
    static std::unordered_map<std::string, UniformBufferRecord> _UniformBuffers;

    /// Methods

    /** Get the OpenGL ID of the shader program
     * @return the OpenGL ID of the shader program
     */
    GLuint GetGLId() const { return _glId; };

    /**
     */
    GLuint LoadShader(const std::string& filename);

    /** Apply preprocessing to a shader's source code.
     * Process all preprocessor definitions in the shader's source code.
     * @param type The type of shader, e.g. GL_FRAGMENT_SHADER
     * @param code The original source code.
     * @param basedir The directory the shader was loaded from.
     * @return The processed source code.
     */
    std::string PreProcess(GLuint type, const std::string& code, const std::string& basedir = "");

    /** Print the log for a given shader
     */
    void PrintShaderLog(GLuint id);

    /** Print the log for shader program.
     */
    void PrintProgramLog();

    /** Read all uniforms from the shader and save their locations.
     */
    void CacheUniforms();

    /** Read all attributes from the shader and save their locations.
     */
    void CacheAttributes();

    /// Variables

    bool _loaded = false;
    GLuint _glId = 0;

    std::vector<std::string> _filenames;
    std::unordered_map<std::string, UniformRecord> _uniforms;
    std::unordered_map<std::string, GLuint> _attributes;

};

} // namespace dusk

#endif // DUSK_SHADER_HPP
