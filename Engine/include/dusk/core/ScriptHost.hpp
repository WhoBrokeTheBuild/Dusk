#ifndef DUSK_SCRIPT_HOST_HPP
#define DUSK_SCRIPT_HOST_HPP

#include <dusk/Config.hpp>

#include <string>
#include <vector>

namespace dusk {

class ScriptHost
{
public:

    DISALLOW_COPY_AND_ASSIGN(ScriptHost)

    ScriptHost();
    virtual ~ScriptHost();

    //void Serialize(nlohmann::json& data);
    //void Deserialize(nlohmann::json& data);

    bool LoadFile(const std::string& filename);

    void Run();

private:

}; // class ScriptHost

} // namespace dusk

#endif // DUSK_SCRIPT_HOST_HPP
