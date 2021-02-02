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

const std::regex SCENE_REGEX("DUSK_SCENE\\((.*)\\)");
const std::regex ACTOR_REGEX("DUSK_ACTOR\\((.*)\\)");
const std::regex COMPONENT_REGEX("DUSK_COMPONENT\\((.*)\\)");
const std::regex PROPERTY_REGEX("DUSK_PROPERTY\\((.*)\\)\\n\\s*(.*)\\n");

void CleanSlashes(string& path)
{
    for (unsigned int i = 0; i < path.size(); ++i)
    {
        if (path[i] == '\\')
        {
            path[i] = '/';
        }
    }
}

string GetDirname(string path)
{
    CleanSlashes(path);
    size_t pivot = path.find_last_of('/');
    return (pivot == string::npos
        ? "./"
        : path.substr(0, pivot));
}

string GetExtension(string path)
{
    size_t pivot = path.find_last_of('.');
    return (pivot == string::npos
        ? string()
        : path.substr(pivot + 1));
}

string StringTrim(const string& str)
{
    size_t f = 0;
    size_t b = str.size() - 1;

    while (f < b && (str[f] == ' ' || str[f] == '\t' ||
                     str[f] == '\n' || str[f] == '\r')) ++f;

    while (b > f && (str[b] == ' ' || str[b] == '\t' ||
                     str[b] == '\n' || str[b] == '\r')) --b;

    return str.substr(f, f - b);
}

std::vector<string> SplitString(string str, const char& sep)
{
    std::vector<string> parts;

    size_t i = str.find(sep);
    while (i != string::npos) {
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

void FindProperties(const string& filename, string buffer)
{
    std::smatch match;
    string remaining = buffer;
    while (std::regex_search(remaining, match, PROPERTY_REGEX)) {
        if (match.size() == 3) {
            stringstream paramss(match[1].str());
            stringstream defnss(match[2].str());

            std::unordered_map<string, string> params;
            string param;
            while (getline(paramss, param, ",")) {
                char * pch = param.c_str();
                while (*pch == ' ' || *pch == '\t') ++pch;

            }

            // params, split ',', strip whitespace
            // check for =, parse out halves, strip "



            size_t lastSpace = defn.rfind(' ');
            if (lastSpace == string::npos) {
                lastSpace = defn.rfind('\t');
            }
            if (lastSpace != string::npos) {
                string varName = defn.substr(lastSpace + 1);
                size_t semicolon = varName.rfind(';');
                if (semicolon != string::npos) {
                    varName = varName.substr(0, semicolon);
                }

                printf("// Variable Name: %s\n", varName.c_str());
            }
        }

        remaining = match.suffix().str();
    }
}

void FindActors(const string& filename, string buffer)
{
    std::smatch match;
    string remaining = buffer;
    while (std::regex_search(remaining, match, ACTOR_REGEX)) {
        if (match.size() == 2) {
            printf("dusk::Actor::RegisterSubclass<%s>(\"%s\");\n", match[1].str().c_str(), match[1].str().c_str());
            FindProperties(filename, buffer);
        }

        remaining = match.suffix().str();
    }
}

*/

void PreProcess(const string& filename, nlohmann::json& data)
{
    std::ifstream file(filename);
    string buffer((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
    file.close();

    auto end = std::sregex_iterator();
    auto sceneIt = std::sregex_iterator(buffer.begin(), buffer.end(), SCENE_REGEX);

    for (; sceneIt != end; ++sceneIt) {
        std::smatch match = *sceneIt;
        string name = match[1].str();

        data["Scenes"].push_back({
            { "Name", name },
        });
    }

}

void GenerateCache(string outputFile, string projectName, string projectFile, std::vector<string> sourceFiles)
{
    json data;
    data["ProjectName"] = projectName;
    data["ProjectFile"] = projectFile;
    data["Files"] = sourceFiles;
    data["Scenes"] = nlohmann::json::array();
    data["Actors"] = nlohmann::json::array();
    data["Components"] = nlohmann::json::array();
    data["Cameras"] = nlohmann::json::array();
    data["Lights"] = nlohmann::json::array();

    string dir = GetDirname(projectFile);

    for (string& source : sourceFiles) {
        PreProcess(dir + "/" + source, data);
    }

    std::ofstream file(outputFile);
    file << std::setw(4) << data << "\n";
}

void GenerateEngineMain(string outputFile, string cacheFile)
{
    json data;
    std::ifstream cache(cacheFile);
    cache >> data;

    std::ofstream file(outputFile);
    file << "#include <dusk/Main.hpp>\n";

    for (auto inc : data["Files"]) {
        string tmp = inc.get<string>();
        if (GetExtension(tmp) == "hpp") {
            file << "#include <" << tmp << ">\n";
        }
    }

    file << "const string PROJECT_FILENAME = \"" << data["ProjectFile"].get<string>() << "\";\n";
    file << R"#(
int main(int argc, char** argv) {
    dusk::SetAssetPath(DUSK_ASSET_PATH);
    dusk::App app(argc, argv);
)#";

    for (auto type : data["Scenes"]) {
        string typeName = type["Name"].get<string>();

        file << "dusk::Scene::RegisterType(\"" << typeName << "\", [](const string& id, const string& filename) -> dusk::Scene * { return static_cast<dusk::Scene*>(new " << typeName << "(id, filename)); });\n";
    }

    file << R"#(
    app.LoadConfig(PROJECT_FILENAME);
    _DuskSetup(&app);
    app.Start();
    return 0;
}
)#";
}

void GenerateEditorMain(string outputFile, string cacheFile)
{
    json data;
    std::ifstream cache(cacheFile);
    cache >> data;

    std::ofstream file(outputFile);
    file << "#include <dusk/Main.hpp>\n";
    file << "#include <Editor.hpp>\n";

    // TODO: Just headers
    for (auto inc : data["Files"]) {
        string tmp = inc.get<string>();
        if (GetExtension(tmp) == "hpp") {
            file << "#include <" << tmp << ">\n";
        }
    }

    file << "const string PROJECT_FILENAME = \"" << data["ProjectFile"].get<string>() << "\";\n";

#ifdef __linux__
    file << "const string PROJECT_LIBRARY = \"./lib" << data["ProjectName"].get<string>() << "-Library.so\";\n";
    file << R"#(
#include <dlfcn.h>
void * libraryHandle = nullptr;
void loadLibrary() {
    dusk::App * app = dusk::App::Inst();

    if (libraryHandle) {
        dlclose(libraryHandle);
    }

    DuskLogLoad("Loading library '%s'", PROJECT_LIBRARY.c_str());
    libraryHandle = dlopen(PROJECT_LIBRARY.c_str(), RTLD_LOCAL | RTLD_LAZY);

    if (!libraryHandle) {
        DuskLogError("Failed to load '%s', %s", PROJECT_LIBRARY.c_str(), dlerror());
        return;
    }

    DuskSetupFn setup;
    *(void **) (&setup) = dlsym(libraryHandle, "_DuskSetup");

    if (!setup) {
        DuskLogError("Failed to find '_DuskSetup' in '%s', %s", PROJECT_LIBRARY.c_str(), dlerror());
        return;
    }

    DuskLogInfo("%p", app);
    (*setup)(app);
    app->LoadConfig(PROJECT_FILENAME);
}
)#";
#endif

file << R"#(
int main(int argc, char** argv) {
    dusk::SetAssetPath(DUSK_ASSET_PATH);
    Editor app(argc, argv);
    app.SetConfigFilename(PROJECT_FILENAME);
)#";

    for (auto type : data["Scenes"]) {
        string typeName = type["Name"].get<string>();

        file << "dusk::Scene::RegisterType(\"" << typeName << "\", [](const string& id, const string& filename) -> dusk::Scene * { return static_cast<dusk::Scene*>(new " << typeName << "(id, filename)); });\n";
    }

#if defined(WIN32)
    file << R"#(
    app.TrackCallback(app.OnReset.AddStatic([]() {
        dusk::App * app = dusk::App::Inst();
        _DuskSetup(app);
        app->LoadConfig(PROJECT_FILENAME);
    }));
)#";
#elif defined(__linux__)
file << R"#(
    app.TrackCallback(app.OnStart.AddStatic([]() {
        loadLibrary();
    }));
    app.TrackCallback(app.OnReset.AddStatic([]() {
        loadLibrary();
    }));
)#";
#endif

    file << R"#(
    app.Start();
    return 0;
}
)#";
}

void Usage() {
    printf(
R"#(Usage: DuskPP COMMAND [OPTIONS]
Commands:
    cache OUTPUT PROJECT_NAME PROJECT_FILE [SOURCE_FILES]
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

    string command = argv[1];

    if (command == "cache") {
        if (argc < 5) {
            Usage();
            return 1;
        }

        string outputFile(argv[2]);
        string projectName(argv[3]);
        string projectFile(argv[4]);
        std::vector<string> sourceFiles;
        for (int i = 5; i < argc; ++i) {
            sourceFiles.push_back(argv[i]);
        }

        GenerateCache(outputFile, projectName, projectFile, sourceFiles);
    }
    else if (command == "engine-main") {
        if (argc < 4) {
            Usage();
            return 1;
        }

        string outputFile(argv[2]);
        string cacheFile(argv[3]);

        GenerateEngineMain(outputFile, cacheFile);
    }
    else if (command == "editor-main") {
        if (argc < 4) {
            Usage();
            return 1;
        }

        string outputFile(argv[2]);
        string cacheFile(argv[3]);

        GenerateEditorMain(outputFile, cacheFile);
    }


    return 0;
}
