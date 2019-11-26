#include <Dusk/Dusk.hpp>

#include <Dusk/Module.hpp>
#include <Dusk/Loaders/TextureLoader.hpp>

int main(int argc, char** argv) 
{
    Dusk::Initialize(argc, argv);

    Dusk::LoadModule("STBI");
    Dusk::LoadModule("OpenGL");

    const auto& data = Dusk::GetTextureLoader()->Load("Assets/models/teapot.png");
    printf("%zu x %zu\n", data.Width, data.Height);

    Dusk::Terminate();

    return 0;
}
