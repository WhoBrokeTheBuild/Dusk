#ifndef DUSK_SHADER_DATA_HPP
#define DUSK_SHADER_DATA_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Macros.hpp>

#include <gsl/gsl>

namespace Dusk {

class IShaderData
{
public:

    DISALLOW_COPY_AND_ASSIGN(IShaderData)

    explicit IShaderData() = default;

    virtual ~IShaderData() = default;

    virtual gsl::span<uint8_t> GetData() const = 0;

    virtual void Bind(int index) const;

}; // class IShaderData

} // namespace Dusk

#endif // DUSK_SHADER_DATA_HPP