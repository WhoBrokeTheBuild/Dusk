#include <Dusk/Dusk.hpp>

#include <Dusk/Log.hpp>
#include <Dusk/Module.hpp>

#include <Python/PyDusk.hpp>

#include <Python.h>
#include <frameobject.h>

#if defined(DUSK_OS_WINDOWS)

#include <Windows.h>
#include <conio.h>

#else

#include <termios.h>
#include <unistd.h>

#endif // defined(DUSK_OS_WINDOWS)

#include <string>
#include <vector>
#include <cstdio>

namespace Dusk {

DUSK_ENGINE_API
void Initialize(int argc, char ** argv) {
    PyImport_AppendInittab("Dusk", PyInit_Dusk);

    wchar_t *program = Py_DecodeLocale(argv[0], NULL);
    if (program) {
        Py_SetProgramName(program);
    }

    PyMem_RawFree(program);
    
    Py_Initialize();
    PyImport_ImportModule("Dusk");

    DuskLogVerbose("Dusk Version: %s", GetVersion().GetString());
    DuskLogVerbose("Application Name: %s", GetApplicationName());
    DuskLogVerbose("Application Version: %s", GetApplicationVersion().GetString());
}

DUSK_ENGINE_API
void Terminate() {
    Py_Finalize();

    FreeModules();
}

static bool _Running = false;

DUSK_ENGINE_API
void SetRunning(bool running) {
    _Running = running;
}

DUSK_ENGINE_API
bool IsRunning() {
    return _Running;
}

DUSK_ENGINE_API
bool RunScriptFile(const std::string& filename) {
    FILE * file = fopen(filename.c_str(), "rt");
    if (!file) {
        return false;
    }

    PyObject * pyMain = PyImport_AddModule("__main__");
    PyObject * pyMainDict = PyModule_GetDict(pyMain);
    PyObject * pyLocalDict = PyDict_New();

    PyRun_File(file, filename.c_str(), Py_file_input, pyMainDict, pyLocalDict);

    PyCheckError();

    fclose(file);
    return true;
}

DUSK_ENGINE_API
bool RunScriptString(const std::string& code)
{
    PyObject * pyMain = PyImport_AddModule("__main__");
    PyObject * pyMainDict = PyModule_GetDict(pyMain);
    PyObject * pyLocalDict = PyDict_New();

    PyRun_String(code.c_str(), Py_single_input, pyMainDict, pyLocalDict);

    PyCheckError();
    return true;
}

static PyObject * _PyScriptConsoleLocals = NULL;

static std::vector<std::string> _ScriptConsoleHistory;

static int _ScriptConsoleCursor = 0;
static int _ScriptConsoleHistoryIndex = 0;

bool _ScriptConsoleRunCommand(const std::string&);
void _ScriptConsoleMoveCursorLeft(int);
void _ScriptConsoleMoveCursorRight(int);
void _ScriptConsoleNextCharacter();

#if defined(DUSK_OS_WINDOWS)

HANDLE _StdinHandle;
HANDLE _StdoutHandle;

#else

static struct termios _Termios;

#endif

DUSK_ENGINE_API
void InitScriptConsole()
{
#if defined(DUSK_OS_WINDOWS)

    _StdinHandle = GetStdHandle(STD_INPUT_HANDLE);
    _StdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    DWORD mode;
    GetConsoleMode(_StdinHandle, &mode);
    SetConsoleMode(_StdinHandle, mode & ~(ENABLE_PROCESSED_INPUT));

    GetConsoleMode(_StdoutHandle, &mode);
    SetConsoleMode(_StdoutHandle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

#else

    struct termios tmp;

    tcgetattr(STDIN_FILENO, &_Termios);
    tmp = _Termios;
    tmp.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    tmp.c_cc[VMIN] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &tmp);

#endif

    PyObject * pyMain = PyImport_AddModule("__main__");
    PyObject * pyMainDict = PyModule_GetDict(pyMain);
    _PyScriptConsoleLocals = PyDict_New();

    PyRun_String("import Dusk", Py_single_input, pyMainDict, _PyScriptConsoleLocals);

    _ScriptConsoleHistory.push_back(std::string());
    _ScriptConsoleHistoryIndex = _ScriptConsoleHistory.size() - 1;

    printf(">>> ");
    fflush(stdout);
}

DUSK_ENGINE_API
void TermScriptConsole()
{
    Py_XDECREF(_PyScriptConsoleLocals);

#if defined(DUSK_OS_WINDOWS)

#else

    tcsetattr(STDIN_FILENO, TCSANOW, &_Termios);
    
#endif
}

bool _ScriptConsoleRunCommand(const std::string& command)
{
    if (command[0] == '#') {
        return true;
    }

    bool whitespace = true;
    for (const auto& c : command) {
        if (!isspace(c)) {
            whitespace = false;
        }
    }

    if (whitespace) {
        return false;
    }

    PyObject * pyMain = PyImport_AddModule("__main__");
    PyObject * pyMainDict = PyModule_GetDict(pyMain);

    PyObject * result = PyRun_String(command.c_str(), Py_single_input, pyMainDict, _PyScriptConsoleLocals);
    if (PyCheckError()) {
        return false;
    }
    else if (result && result != Py_None) {
        PyObject * resultRepr = PyObject_Repr(result);
        if (resultRepr) {
            PyObject * resultStr = PyUnicode_AsEncodedString(resultRepr, "utf-8", "~E~");

            printf("%s\n", PyBytes_AS_STRING(resultStr));

            Py_XDECREF(resultStr);
            Py_XDECREF(resultRepr);
        }

        Py_XDECREF(result);
    }

    return true;
}

void _ScriptConsoleMoveCursorLeft(int amount)
{
    printf("\033[%dD", amount);
    fflush(stdout);
}

void _ScriptConsoleMoveCursorRight(int amount)
{
    printf("\033[%dC", amount);
    fflush(stdout);
}

void _ScriptConsoleNextCharacter() 
{
    static bool inEscape = false;

    unsigned char c;

#if defined(DUSK_OS_WINDOWS)

    DWORD read;
    struct _INPUT_RECORD record;
    if (!ReadConsoleInput(_StdinHandle, &record, 1, &read) 
        || record.EventType != KEY_EVENT
        || !record.Event.KeyEvent.bKeyDown) {
        return;
    }

    switch (record.Event.KeyEvent.wVirtualKeyCode) {
    case VK_LEFT:
        inEscape = true;
        c = 'D';
        break;
    case VK_RIGHT:
        inEscape = true;
        c = 'C';
        break;
    case VK_UP:
        inEscape = true;
        c = 'A';
        break;
    case VK_DOWN:
        inEscape = true;
        c = 'B';
        break;
    case VK_HOME:
        inEscape = true;
        c = 'H';
        break;
    case VK_END:
        inEscape = true;
        c = 'F';
        break;
    default:
        c = record.Event.KeyEvent.uChar.AsciiChar;
        break;
    };
    
#else

    if (read(STDIN_FILENO, &c, sizeof(c)) < 0) {
        return;
    }

#endif

    if (inEscape) {
        if (c == '[' || c == '~') {
            return;
        }

        switch (c) {
        case 'A': // up
            if (_ScriptConsoleHistoryIndex > 0) {
                --_ScriptConsoleHistoryIndex;

                if (_ScriptConsoleCursor > 0) {
                    _ScriptConsoleMoveCursorLeft(_ScriptConsoleCursor);   
                }
                _ScriptConsoleCursor = 0;
                printf("\033[K");
                fflush(stdout);

                const auto& str = _ScriptConsoleHistory[_ScriptConsoleHistoryIndex];

                printf("%s", str.c_str());
                fflush(stdout);

                _ScriptConsoleCursor = str.size();
            }

            break;
        case 'B': // down
            if (_ScriptConsoleHistoryIndex < _ScriptConsoleHistory.size() - 1) {
                ++_ScriptConsoleHistoryIndex;

                if (_ScriptConsoleCursor > 0) {
                    _ScriptConsoleMoveCursorLeft(_ScriptConsoleCursor);   
                }
                _ScriptConsoleCursor = 0;
                printf("\033[K");
                fflush(stdout);

                const auto& str = _ScriptConsoleHistory[_ScriptConsoleHistoryIndex];

                printf("%s", str.c_str());
                fflush(stdout);

                _ScriptConsoleCursor = str.size();
            }

            break;
        case 'C': // right
            if (_ScriptConsoleCursor < _ScriptConsoleHistory.back().size()) {
                _ScriptConsoleMoveCursorRight(1);
                ++_ScriptConsoleCursor;
            }
            break;
        case 'D': // left
            if (_ScriptConsoleCursor > 0) {
                _ScriptConsoleMoveCursorLeft(1);
                --_ScriptConsoleCursor;
            }
            break;
        case 'H': // home
            if (_ScriptConsoleCursor > 0) {
            _ScriptConsoleMoveCursorLeft(_ScriptConsoleCursor);
                _ScriptConsoleCursor = 0;
            }
            break;
        case 'F': // end
            if (_ScriptConsoleCursor < _ScriptConsoleHistory.back().size()) {
                _ScriptConsoleMoveCursorRight(_ScriptConsoleHistory.back().size() - _ScriptConsoleCursor);
                _ScriptConsoleCursor = _ScriptConsoleHistory.back().size();
            }
            break;
        }

        inEscape = false;
    }
    else if (c != 9 && iscntrl(c)) {
        switch (c) {
        case 3: // Ctrl+C
        case 4: // Ctrl+D
            SetRunning(false);
            break;
        case 10: // Newline
        case 13: // Carriage Return
            printf("\n");
            fflush(stdout);

            if (_ScriptConsoleHistoryIndex < _ScriptConsoleHistory.size() - 1) {
                _ScriptConsoleHistory.back() = _ScriptConsoleHistory[_ScriptConsoleHistoryIndex];
            }

            if (_ScriptConsoleRunCommand(_ScriptConsoleHistory.back())) {
                _ScriptConsoleHistory.push_back(std::string());
                _ScriptConsoleHistoryIndex = _ScriptConsoleHistory.size() - 1;
            }
            else {
                _ScriptConsoleHistory.back().clear();
            }

            _ScriptConsoleCursor = 0;
            printf(">>> ");
            fflush(stdout);

            break;
        case 27: // Escape
            inEscape = true;
            break;
        case 8:
        case 127: // Backspace
            if (_ScriptConsoleCursor > 0) {
                printf("\010\033[K");
                fflush(stdout);

                if (_ScriptConsoleHistoryIndex < _ScriptConsoleHistory.size() - 1) {
                    _ScriptConsoleHistory.back() = _ScriptConsoleHistory[_ScriptConsoleHistoryIndex];
                    _ScriptConsoleHistoryIndex = _ScriptConsoleHistory.size() - 1;
                }

                --_ScriptConsoleCursor;
                _ScriptConsoleHistory.back().erase(_ScriptConsoleCursor, 1);

                printf("%s", _ScriptConsoleHistory.back().c_str() + _ScriptConsoleCursor);
                fflush(stdout);

                if (_ScriptConsoleCursor < _ScriptConsoleHistory.back().size()) {
                    _ScriptConsoleMoveCursorLeft(_ScriptConsoleHistory.back().size() - _ScriptConsoleCursor);
                }
            }
            break;
        }
    }
    else {
        putchar(c);
        fflush(stdout);

        if (_ScriptConsoleHistoryIndex < _ScriptConsoleHistory.size() - 1) {
            _ScriptConsoleHistory.back() = _ScriptConsoleHistory[_ScriptConsoleHistoryIndex];
            _ScriptConsoleHistoryIndex = _ScriptConsoleHistory.size() - 1;
        }

        _ScriptConsoleHistory.back().insert(_ScriptConsoleCursor, 1, c);
        ++_ScriptConsoleCursor;

        printf("%s", _ScriptConsoleHistory.back().c_str() + _ScriptConsoleCursor);
        fflush(stdout);

        if (_ScriptConsoleCursor < _ScriptConsoleHistory.back().size()) {
            _ScriptConsoleMoveCursorLeft(_ScriptConsoleHistory.back().size() - _ScriptConsoleCursor);
        }
    }
}

DUSK_ENGINE_API
void UpdateScriptConsole()
{
#if defined(DUSK_OS_WINDOWS)

    DWORD result;
    if (!GetNumberOfConsoleInputEvents(_StdinHandle, &result)) {
        return;
    }

    while (result > 0) {
        _ScriptConsoleNextCharacter();

        if (!GetNumberOfConsoleInputEvents(_StdinHandle, &result)) {
            return;
        }
    }

#else

    struct timeval tv = { 0L, 0L };
    fd_set fds;

    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);

    while (select(1, &fds, NULL, NULL, &tv)) {
        _ScriptConsoleNextCharacter();
    }

#endif
}

static std::string _ApplicationName;

DUSK_ENGINE_API
void SetApplicationName(const std::string& name)
{
    _ApplicationName = name;
}

DUSK_ENGINE_API
std::string GetApplicationName()
{
    return _ApplicationName;
}

static Version _ApplicationVersion;

DUSK_ENGINE_API
void SetApplicationVersion(unsigned major, unsigned minor, unsigned patch)
{
    _ApplicationVersion = Version(major, minor, patch);
}

DUSK_ENGINE_API
void SetApplicationVersion(const Version& version)
{
    _ApplicationVersion = version;
}

DUSK_ENGINE_API
Version GetApplicationVersion()
{
    return _ApplicationVersion;
}

}
