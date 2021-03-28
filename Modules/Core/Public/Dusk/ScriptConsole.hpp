#ifndef DUSK_SCRIPT_CONSOLE_HPP
#define DUSK_SCRIPT_CONSOLE_HPP

#include <Dusk/Config.hpp>
#include <Dusk/String.hpp>

#if defined(DUSK_PLATFORM_WINDOWS)

    #include <conio.h>

#else

    #include <termios.h>
    #include <unistd.h>

#endif // defined(DUSK_PLATFORM_WINDOWS)

#include <vector>

#include <Python.h>

namespace Dusk {

class DUSK_CORE_API ScriptConsole
{
public:

    ScriptConsole() = delete;
    ScriptConsole(const ScriptConsole&) = delete;

    static void Initialize();
    static void Terminate();
    
    static void Update();

private:

    static inline string& GetCurrentLine()
    {
        return _history[_index];
    }

    static void ReadNextCharacter();

    static bool RunCommand(const string& command);

    static void JumpCursorLeft();
    static void JumpCursorRight();

    static void MoveCursorLeft(int amount);
    static void MoveCursorRight(int amount);

    static void HandleUp();
    static void HandleDown();
    static void HandleRight(bool ctrl);
    static void HandleLeft(bool ctrl);
    static void HandleHome();
    static void HandleEnd();
    static void HandleBackspace();
    static void HandleDelete();

    static bool _initialized;

    static PyObject * _locals;

    static std::vector<string> _history;

    static int _cursor;
    static int _index;

#if defined(DUSK_PLATFORM_WINDOWS)

    static HANDLE _stdinHandle;
    static HANDLE _stdoutHandle;

#else

    static struct termios _termios;

#endif

}; // class PyConsole

} // namespace Dusk

#endif // DUSK_SCRIPT_CONSOLE_HPP