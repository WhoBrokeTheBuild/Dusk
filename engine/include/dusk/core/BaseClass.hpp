#ifndef DUSK_BASE_CLASS_HPP
#define DUSK_BASE_CLASS_HPP

#include <dusk/Config.hpp>
#include <unordered_map>
#include <typeindex>

namespace dusk {

class BaseClass
{
public:

    virtual ~BaseClass() = default;

    static std::unordered_map<std::string, std::type_index> Types;
    static std::unordered_map<std::string, std::function<BaseClass*()>> Initializers;
    static std::unordered_map<std::string, std::function<void(BaseClass*, nlohmann::json&)>> Serializers;
    static std::unordered_map<std::string, std::function<void(BaseClass*, nlohmann::json&)>> Deserializers;

};

} // namespace dusk

#endif // DUSK_BASE_CLASS_HPP
