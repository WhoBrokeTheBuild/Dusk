#ifndef DUSK_SHADER_HPP
#define DUSK_SHADER_HPP

#include <dusk/Config.hpp>
#include <dusk/core/Math.hpp>
#include <dusk/core/OpenGL.hpp>

#include <initializer_list>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace dusk {

class Shader
{
public:

    using filenames_initializer_t   = std::initializer_list<std::string>;
    using filenames_vector_t        = std::vector<std::string>;
    using define_map_t              = std::unordered_map<std::string, std::string>;
    using uniform_map_t             = std::unordered_map<std::string, GLint>;
    using attribute_map_t           = std::unordered_map<std::string, GLuint>;

    /// Boilerplate

    DISALLOW_COPY_AND_ASSIGN(Shader)

    /** Create an empty shader, for use with LoadFromFile().
     */
    Shader() = default;

    /** Create and load a shader from the given filenames.
     */
    Shader(const filenames_initializer_t& filenames, const define_map_t& defines = { });

    /** Create and load a shader from the given filenames.
     */
    Shader(const filenames_vector_t& filenames, const define_map_t& defines = { });

    virtual ~Shader();

    /// Static Methods

    /** Initialize the version string to prepend to all shader files.
     * Uses GL_SHADING_LANGUAGE_VERSION to generate the #version string to
     * prepend onto shader files.
     * e.g. for OpenGL 4.5 and GLSL version 4.50, the string would be
     * '#version 450 core'
     */
    static void InitializeVersionString();

    /// Methods

    /** Load and compile the given shader files, then link the program.
     */
    virtual bool LoadFromFiles(const filenames_initializer_t& filenames, const define_map_t& defines = { });
    
    /** Load and compile the given shader files, then link the program.
     */
    virtual bool LoadFromFiles(const filenames_vector_t& filenames, const define_map_t& defines = { });

    /** Bind the shader for use with rendering or update operations.
     * This internally calls glUseProgram()
     */
    void Bind();
    
    define_map_t GetDefines() {
        return _defines;
    }

    template <typename T>
    void SetDefine(std::string name, T value) {
        _defines.emplace(name, std::to_string(value));
    }

    /**
     */
    std::vector<std::string> GetFilenames() const {
        return _filenames;
    }

    /**
     */
    GLuint GetGLID() const { 
        return _glID; 
    }

    /** Get the location of an attribute within the shader.
     * @param name The name of the attribute to get the location of.
     * @return The location of the attribute
     */
    GLint GetAttributeLocation(const std::string& name) const;

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

    /// Static Variables

    static inline std::string _GLSLVersionString = "";

    /// Variables

    GLuint _glID = 0;

    std::vector<std::string> _filenames;

	define_map_t _defines = {
		{ "M_PI", std::to_string(M_PI) },
	};

    uniform_map_t _uniforms;

    attribute_map_t _attributes;

    /// Methods

    /**
     */
    GLuint LoadShader(const std::string& filename, const define_map_t& defines);

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

}; // class Shader

} // namespace dusk

#endif // DUSK_SHADER_HPP
