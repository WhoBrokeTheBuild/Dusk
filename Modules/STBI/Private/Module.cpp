#include <Dusk/Module.hpp>
#include <Dusk/STBI/Importers/TextureImporter.hpp>

namespace Dusk::STBI {

void ModuleInit() {
    auto importer = std::shared_ptr<ITextureImporter>(new TextureImporter());
    SetTextureImporter("jpg",  importer);
    SetTextureImporter("jpeg", importer);
    SetTextureImporter("png",  importer);
    SetTextureImporter("tga",  importer);
    SetTextureImporter("bmp",  importer);
    SetTextureImporter("gif",  importer);
}

void ModuleTerm() {
    FreeTextureImporter("jpg");
    FreeTextureImporter("jpeg");
    FreeTextureImporter("png");
    FreeTextureImporter("tga");
    FreeTextureImporter("bmp");
    FreeTextureImporter("gif");
}

DUSK_MODULE() {
    "STBI",
    ModuleInit,
    ModuleTerm,
};

}
