#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstdio>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

const std::regex CLASS_REGEX("DUSK_CLASS\\((.*)\\)");
const std::regex PROPERTY_REGEX("DUSK_PROPERTY\\((.*)\\)\\n\\s*(.*)\\n");

std::string StringTrim(const std::string& str)
{
    size_t f = 0;
    size_t b = str.size() - 1;

    while (f < b && (str[f] == ' ' || str[f] == '\t' ||
                     str[f] == '\n' || str[f] == '\r')) ++f;

    while (b > f && (str[b] == ' ' || str[b] == '\t' ||
                     str[b] == '\n' || str[b] == '\r')) --b;

    return str.substr(f, f - b);
}

std::vector<std::string> SplitString(std::string str, const char& sep)
{
    std::vector<std::string> parts;

    size_t i = str.find(sep);
    while (i != std::string::npos) {
        parts.push_back(str.substr(0, i));
        str = str.substr(i + 1);
        i = str.find(sep);
    }

    if (!str.empty()) {
        parts.push_back(str);
    }

    return parts;
}

/*
const std::regex PROPERTY_REGEX(R"#(DUSK_PROPERTY\((.*)\)\n\s*(.*)\n)#");
const std::regex SHADER_REGEX  (R"#(DUSK_SHADER\((.*)\))#");

void FindProperties(const std::string& filename, std::string buffer)
{
    std::smatch match;
    std::string remaining = buffer;
    while (std::regex_search(remaining, match, PROPERTY_REGEX)) {
        if (match.size() == 3) {
            std::stringstream paramss(match[1].str());
            std::stringstream defnss(match[2].str());

            std::unordered_map<std::string, std::string> params;
            std::string param;
            while (getline(paramss, param, ",")) {
                char * pch = param.c_str();
                while (*pch == ' ' || *pch == '\t') ++pch;

            }

            // params, split ',', strip whitespace
            // check for =, parse out halves, strip "



            size_t lastSpace = defn.rfind(' ');
            if (lastSpace == std::string::npos) {
                lastSpace = defn.rfind('\t');
            }
            if (lastSpace != std::string::npos) {
                std::string varName = defn.substr(lastSpace + 1);
                size_t semicolon = varName.rfind(';');
                if (semicolon != std::string::npos) {
                    varName = varName.substr(0, semicolon);
                }

                printf("// Variable Name: %s\n", varName.c_str());
            }
        }

        remaining = match.suffix().str();
    }
}

void FindActors(const std::string& filename, std::string buffer)
{
    std::smatch match;
    std::string remaining = buffer;
    while (std::regex_search(remaining, match, ACTOR_REGEX)) {
        if (match.size() == 2) {
            printf("dusk::Actor::RegisterSubclass<%s>(\"%s\");\n", match[1].str().c_str(), match[1].str().c_str());
            FindProperties(filename, buffer);
        }

        remaining = match.suffix().str();
    }
}

*/

void PreProcess(const std::string& filename, nlohmann::json& data)
{
    std::ifstream file(filename);
    std::string buffer((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());

    std::smatch match;
    if (std::regex_search(buffer, match, CLASS_REGEX) && match.size() == 2) {
        std::string info = match[1].str();

        const std::vector<std::string>& parts = SplitString(info, ',');

        std::string className = StringTrim(parts[0]);
        std::string parent = StringTrim(parts[1]);
        json fields = json::array();

        std::string remain = buffer;
        while (std::regex_search(remain, match, PROPERTY_REGEX)) {
            if (match.size() < 3) continue;

            std::unordered_map<std::string, std::string> params;
            const std::vector<std::string>& paramList = SplitString(match[1].str(), ',');
            std::vector<std::string> defn = SplitString(match[2].str(), ' ');

            for (const auto& p : paramList) {
                params.emplace(StringTrim(p), "");
            }

            defn.erase(defn.begin());
            if (defn.front() == "*") defn.erase(defn.begin());
            if (defn.front().back() == ';') defn.front().pop_back();

            fields.push_back({
                { "Name", defn.front() },
                { "Params", params },
            });

            remain = match.suffix().str();
        }

        data.push_back({
            { "Name", className },
            { "Parent", parent },
            { "Fields", fields },
        });
    }

    file.close();
}

void GenerateCache(std::string outputFile, std::string projectFile, std::vector<std::string> sourceFiles)
{
    json data;
    data["ProjectFile"] = projectFile;
    data["Files"] = sourceFiles;
    data["Types"] = nlohmann::json::array();

    for (std::string& source : sourceFiles) {
        PreProcess(source, data["Types"]);
    }

    std::ofstream file(outputFile);
    file << std::setw(4) << data << "\n";
}

void GenerateEngineMain(std::string outputFile, std::string cacheFile)
{
    json data;
    std::ifstream cache(cacheFile);
    cache >> data;

    std::ofstream file(outputFile);
    file << R"#(
#include <dusk/Main.hpp>

)#";

    for (auto inc : data["Files"]) {
        file << "#include <" << inc.get<std::string>() << ">\n";
    }

    file << R"#(
const std::string PROJECT_FILENAME = ")#" << data["ProjectFile"].get<std::string>() << R"#(";

int main(int argc, char** argv) {
    dusk::App app(argc, argv);

)#";

    for (auto type : data["Types"]) {
        std::string typeName = type["Name"].get<std::string>();
        std::string parent = type["Parent"].get<std::string>();

        file << "dusk::BaseClass::Types.emplace(\"" << typeName << "\", typeid(\"" << typeName << "\"));\n";
        file << "dusk::BaseClass::Initializers.emplace(\"" << typeName << "\","
             << "[]() -> dusk::BaseClass* { return dynamic_cast<dusk::BaseClass*>(new " << typeName << "()); }"
             <<");\n";
        file << "dusk::BaseClass::Serializers.emplace(\"" << typeName << "\","
             << "[](dusk::BaseClass* base, nlohmann::json& data) { \n"
             << "    dusk::BaseClass::Serializers[\"" << parent << "\"](base, data);\n"
             << "    " << typeName << "* inst = dynamic_cast<" << typeName << "*>(base);\n";
        for (auto field : type["Fields"]) {
            std::string fieldName = field["Name"].get<std::string>();
            auto& params = field["Params"];
            if (params.find("INT") != params.end() ||
                params.find("FLOAT") != params.end()) {
                file << "    inst->" << fieldName << " = data[\"" << fieldName << "\"];\n";
            }
            else if (params.find("STRING") != params.end()) {
                file << "    inst->" << fieldName << " = data[\"" << fieldName << "\"].get<std::string>();\n";
            }
            else if (params.find("VEC2") != params.end()) {
                file << "    inst->" << fieldName << " = { data[\"" << fieldName << "\"][0], data[\"" << fieldName << "\"][1] };\n";
            }
            else if (params.find("VEC3") != params.end()) {
                file << "    inst->" << fieldName << " = { data[\"" << fieldName << "\"][0], data[\"" << fieldName << "\"][1], data[\"" << fieldName << "\"][2] };\n";
            }
            else if (params.find("VEC4") != params.end()) {
                file << "    inst->" << fieldName << " = { data[\"" << fieldName << "\"][0], data[\"" << fieldName << "\"][1], data[\"" << fieldName << "\"][2], data[\"" << fieldName << "\"][3] };\n";
            }
        }
        file <<"});\n";
        file << "dusk::BaseClass::Deserializers.emplace(\"" << typeName << "\","
             << "[](dusk::BaseClass* base, nlohmann::json& data) { \n"
             << "    dusk::BaseClass::Deserializers[\"" << parent << "\"](base, data);\n"
             << "    " << typeName << "* inst = dynamic_cast<" << typeName << "*>(base);\n";
        for (auto field : type["Fields"]) {
            std::string fieldName = field["Name"].get<std::string>();
            auto& params = field["Params"];
            if (params.find("INT") != params.end() ||
                params.find("FLOAT") != params.end() ||
                params.find("STRING") != params.end()) {
                file << "    data[\"" << fieldName << "\"] = inst->" << fieldName << ";\n";
            }
            else if (params.find("VEC2") != params.end()) {
                file << "    data[\"" << fieldName << "\"] = { inst->" << fieldName << "[0], inst->" << fieldName << "[1] };\n";
            }
            else if (params.find("VEC3") != params.end()) {
                file << "    data[\"" << fieldName << "\"] = { inst->" << fieldName << "[0], inst->" << fieldName << "[1], inst->" << fieldName << "[2] };\n";
            }
            else if (params.find("VEC4") != params.end()) {
                file << "    data[\"" << fieldName << "\"] = { inst->" << fieldName << "[0], inst->" << fieldName << "[1], inst->" << fieldName << "[2], inst->" << fieldName << "[3] };\n";
            }
        }
        file <<"});\n";
    }

    file << R"#(
    app.EvtLoadConfig.AddStatic([&](std::string) {
        _DuskSetup(&app);
    });
    app.LoadConfig(PROJECT_FILENAME);

    app.Start();
    return 0;
}
)#";

}

void GenerateEditorMain(std::string outputFile, std::string cacheFile)
{
    nlohmann::json data;
    std::ifstream cache(cacheFile);
    cache >> data;

    std::ofstream file(outputFile);
    file << R"#(
#include <dusk/Main.hpp>
#include <Editor.hpp>

const std::string PROJECT_FILENAME = ")#" << data["ProjectFile"].get<std::string>() << R"#(";

int main(int argc, char** argv) {
    Editor app(argc, argv);

    app.EvtLoadConfig.AddStatic([&](std::string) {
        _DuskSetup(&app);
    });
    app.LoadConfig(PROJECT_FILENAME);

    app.Start();
    return 0;
}
)#";
}

void Usage() {
    printf(
R"#(Usage: DuskPP COMMAND [OPTIONS]
Commands:
    cache OUTPUT PROJECT_FILE [SOURCE_FILES]
    engine-main OUTPUT CACHE_FILE
    editor-main OUTPUT CACHE_FILE
)#");
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        Usage();
        return 1;
    }

    std::string command = argv[1];

    if (command == "cache") {
        if (argc < 4) {
            Usage();
            return 1;
        }

        std::string outputFile(argv[2]);
        std::string projectFile(argv[3]);
        std::vector<std::string> sourceFiles;
        for (int i = 4; i < argc; ++i) {
            sourceFiles.push_back(argv[i]);
        }

        GenerateCache(outputFile, projectFile, sourceFiles);
    }
    else if (command == "engine-main") {
        if (argc < 4) {
            Usage();
            return 1;
        }

        std::string outputFile(argv[2]);
        std::string cacheFile(argv[3]);

        GenerateEngineMain(outputFile, cacheFile);
    }
    else if (command == "editor-main") {
        if (argc < 4) {
            Usage();
            return 1;
        }

        std::string outputFile(argv[2]);
        std::string cacheFile(argv[3]);

        GenerateEditorMain(outputFile, cacheFile);
    }


    return 0;
}
