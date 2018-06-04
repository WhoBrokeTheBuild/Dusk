#ifndef DUSK_TEXTURE_HPP
#define DUSK_TEXTURE_HPP

#include <dusk/Config.hpp>

#include <dusk/asset/Asset.hpp>
#include <dusk/core/Event.hpp>
#include <string>
#include <vector>

namespace dusk {

class Texture : public Asset
{
public:

    /// Class Boilerplate

    DISALLOW_COPY_AND_ASSIGN(Texture)

    Texture() = default;
    Texture(const std::string& filename);
    virtual ~Texture();

    /// Methods

    bool LoadFromFile(const std::string& filename);

    void Bind();

private:

    virtual void Load() override;
    virtual void Free() override;

    GLuint _glID = 0;

}; // class Texture

} // namespace dusk

#endif // DUSK_TEXTURE_HPP
