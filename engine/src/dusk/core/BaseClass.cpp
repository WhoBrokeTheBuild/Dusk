#include "dusk/core/BaseClass.hpp"

namespace dusk {

std::unordered_map<std::string, std::type_index> BaseClass::Types;
std::unordered_map<std::string, std::function<std::unique_ptr<BaseClass>()>> BaseClass::Initializers;
std::unordered_map<std::string, std::function<void(BaseClass*, nlohmann::json&)>> BaseClass::Serializers;
std::unordered_map<std::string, std::function<void(BaseClass*, nlohmann::json&)>> BaseClass::Deserializers;

}
