#include <Dusk/OpenGL/ShaderImporter.hpp>

#include <Dusk/Log.hpp>

#include <fstream>

namespace Dusk::OpenGL {

// DUSK_OPENGL_API
// ShaderData && ShaderImporter::LoadFromFile(std::string filename)
// {
//     ShaderData data;

//     std::string possibleFilenames[] = {
//         // Vertex Shader
//         filename + ".vs.glsl",
//         // Fragment Shader
//         filename + ".fs.glsl",
//         // Geometry Shader
//         filename + ".gs.glsl",
//     #if GL_VERSION_4_0
//         // Tesselation Control Shader
//         filename + ".tcs.glsl",
//         // Tesselation Evaluation Shader
//         filename + ".tes.glsl",
//     #endif
//     #if GL_VERSION_4_3
//         // Compute Shader
//         filename + ".cs.glsl",
//     #endif
//     };

    // std::ifstream file;
    // for (const auto& testFilename : possibleFilenames) {
    //     DuskLogVerbose("Trying %s", testFilename);

    //     file.open(testFilename);
    //     if (file.is_open()) {
    //         DuskLogLoad("Loading %s", testFilename);

    //         data.Files.emplace(testFilename, 
    //             std::string(
    //                 (std::istreambuf_iterator<char>(file)), 
    //                 std::istreambuf_iterator<char>()
    //             )
    //         );
    //         file.close();
    //     }
    // }

//     return std::move(data);
// }

} // namespace Dusk::OpenGL