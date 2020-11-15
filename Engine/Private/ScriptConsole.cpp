#include <Dusk/ScriptConsole.hpp>
#include <Dusk/Dusk.hpp>

#include <Python/PyDusk.hpp>

namespace Dusk {

PyObject * ScriptConsole::_locals = nullptr;

std::vector<std::string> ScriptConsole::_history;

int ScriptConsole::_cursor = 0;
int ScriptConsole::_index = 0;

#if defined(DUSK_OS_WINDOWS)

    HANDLE ScriptConsole::_stdinHandle;
    HANDLE ScriptConsole::_stdoutHandle;

#else

    static struct termios ScriptConsole::_termios;

#endif

void ScriptConsole::Initialize()
{
#if defined(DUSK_OS_WINDOWS)

    _stdinHandle = GetStdHandle(STD_INPUT_HANDLE);
    _stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    DWORD mode;
    GetConsoleMode(_stdinHandle, &mode);
    SetConsoleMode(_stdinHandle, mode & ~(ENABLE_PROCESSED_INPUT));

    GetConsoleMode(_stdoutHandle, &mode);
    SetConsoleMode(_stdoutHandle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

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
    _locals = PyDict_New();
    
    PyRun_String("import Dusk", Py_single_input, pyMainDict, _locals);

    _history.push_back(std::string());
    _index = _history.size() - 1;

    printf(">>> ");
    fflush(stdout);
}

void ScriptConsole::Terminate()
{
    Py_XDECREF(_locals);

#if !defined(DUSK_OS_WINDOWS)

    tcsetattr(STDIN_FILENO, TCSANOW, &_termios);

#endif
}

void ScriptConsole::Update()
{
#if defined(DUSK_OS_WINDOWS)

    DWORD result;
    if (!GetNumberOfConsoleInputEvents(_stdinHandle, &result)) {
        return;
    }

    while (result > 0) {
        ReadNextCharacter();

        if (!GetNumberOfConsoleInputEvents(_stdinHandle, &result)) {
            return;
        }
    }

#else

    struct timeval tv = { 0L, 0L };
    fd_set fds;

    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);

    while (select(1, &fds, NULL, NULL, &tv)) {
        ReadNextCharacter();
    }

#endif
}

void ScriptConsole::ReadNextCharacter()
{
    unsigned char c;
    bool ctrlPressed = false;

#if defined(DUSK_OS_WINDOWS)

    const DWORD CTRL_MASK = (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED);
    
    DWORD read;
    struct _INPUT_RECORD record;

    if (!ReadConsoleInput(_stdinHandle, &record, 1, &read)) {
        return;
    }
    
    if (record.EventType != KEY_EVENT || !record.Event.KeyEvent.bKeyDown) {
        return;
    }

    ctrlPressed = ((record.Event.KeyEvent.dwControlKeyState & CTRL_MASK) > 0);
    
    switch (record.Event.KeyEvent.wVirtualKeyCode) {
    case VK_LEFT:   HandleLeft(ctrlPressed);    return;
    case VK_RIGHT:  HandleRight(ctrlPressed);   return;
    case VK_UP:     HandleUp();     return;
    case VK_DOWN:   HandleDown();   return;
    case VK_HOME:   HandleHome();   return;
    case VK_END:    HandleEnd();    return;
    };

    c = record.Event.KeyEvent.uChar.AsciiChar;

#else

    static bool inEscapeSequence = false;

    if (read(STDIN_FILENO, &c, sizeof(c)) < 0) {
        return;
    }

    if (c == 27) {
        inEscapeSequence = true;
        return;
    }
    
    if (inEscapeSequence) {
        if (c == '[' || c == '~') {
            return;
        }

        switch (c) {
        case 'D': HandleLeft();     return;
        case 'C': HandleRight();    return;
        case 'A': HandleUp();       return;
        case 'B': HandleDown();     return;
        case 'H': HandleHome();     return;
        case 'F': HandleEnd();      return;
        }

        inEscapeSequence = false;
    }

#endif

    // Process all control characters, except tab
    if (c != 9 && iscntrl(c)) {
        switch (c) {
        case 3: // Ctrl+C
        case 4: // Ctrl+D
            SetRunning(false);
            break;
        case 10: // Newline
        case 13: // Carriage Return
            printf("\n");
            fflush(stdout);

            // If running a command from history, set the current line to that
            if (_index < _history.size() - 1) {
                _history.back() = GetCurrentLine();
            }

            if (RunCommand(_history.back())) {
                _history.push_back(std::string());
                _index = _history.size() - 1;
            }
            else {
                _history.back().clear();
            }

            _cursor = 0;
            printf(">>> ");
            fflush(stdout);

            break;
        case 8:
        case 127: HandleBackspace(); return;
        }
    }
    else {
        putchar(c);
        fflush(stdout);

        if (_index < _history.size() - 1) {
            _history.back() = GetCurrentLine();
            _index = _history.size() - 1;
        }

        _history.back().insert(_cursor, 1, c);
        ++_cursor;

        printf("%s", _history.back().c_str() + _cursor);
        fflush(stdout);

        if (_cursor < _history.back().size()) {
            MoveCursorLeft(_history.back().size() - _cursor);
        }
    }

}

bool ScriptConsole::RunCommand(const std::string& command)
{
    if (command[0] == '#') {
        return true;
    }

    bool allWhitespace = true;
    for (const auto& c : command) {
        if (!isspace(c)) {
            allWhitespace = false;
        }
    }

    if (allWhitespace) {
        return false;
    }

    PyObject * pyMain = PyImport_AddModule("__main__");
    PyObject * pyMainDict = PyModule_GetDict(pyMain);

    PyObject * result = PyRun_String(command.c_str(), Py_single_input, pyMainDict, _locals);
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

void ScriptConsole::JumpCursorLeft()
{
    const auto& str = GetCurrentLine();

    if (_cursor == 0) {
        return;
    }

    int amount = 0;
    bool alnum = isalnum(str[_cursor - 1]);

    for (int i = _cursor - 1; i >= 0; --i) {
        bool tmp = isalnum(str[i]);
        if (alnum != tmp) {
            break;
        }

        ++amount;
    }

    MoveCursorLeft(amount);
}

void ScriptConsole::JumpCursorRight()
{
    const auto& str = GetCurrentLine();

    if (_cursor == str.length()) {
        return;
    }

    int amount = 0;
    bool alnum = isalnum(str[_cursor]);

    for (int i = _cursor; i < str.length(); ++i) {
        bool tmp = isalnum(str[i]);
        if (alnum != tmp) {
            break;
        }
        
        ++amount;
    }

    MoveCursorRight(amount);
}

void ScriptConsole::MoveCursorLeft(int amount)
{
    if (amount > 0) {
        printf("\033[%dD", amount);
        fflush(stdout);
        _cursor -= amount;
    }
}

void ScriptConsole::MoveCursorRight(int amount)
{
    if (amount > 0) {
        printf("\033[%dC", amount);
        fflush(stdout);
        _cursor += amount;
    }
}

void ScriptConsole::HandleUp()
{
    if (_index > 0) {
        --_index;

        if (_cursor > 0) {
            MoveCursorLeft(_cursor);   
        }
        _cursor = 0;
        printf("\033[K");
        fflush(stdout);

        const auto& str = GetCurrentLine();

        printf("%s", str.c_str());
        fflush(stdout);

        _cursor = str.size();
    }
}

void ScriptConsole::HandleDown()
{
    if (_index < _history.size() - 1) {
        ++_index;

        if (_cursor > 0) {
            MoveCursorLeft(_cursor);   
        }
        _cursor = 0;
        printf("\033[K");
        fflush(stdout);

        const auto& str = GetCurrentLine();

        printf("%s", str.c_str());
        fflush(stdout);

        _cursor = str.size();
    }
}

void ScriptConsole::HandleRight(bool ctrl)
{
    if (_cursor < GetCurrentLine().size()) {
        if (ctrl) {
            JumpCursorRight();
        }
        else {
            MoveCursorRight(1);
        }
    }
}

void ScriptConsole::HandleLeft(bool ctrl)
{
    if (_cursor > 0) {
        if (ctrl) {
            JumpCursorLeft();
        }
        else {
            MoveCursorLeft(1);
        }
    }
}

void ScriptConsole::HandleHome()
{
    MoveCursorLeft(_cursor);
}

void ScriptConsole::HandleEnd()
{
    MoveCursorRight(_history.back().size() - _cursor);
}

void ScriptConsole::HandleBackspace()
{
    if (_cursor > 0) {
        printf("\010\033[K");
        fflush(stdout);
        
        if (_index < _history.size() - 1) {
            _history.back() = GetCurrentLine();
            _index = _history.size() - 1;
        }

        --_cursor;
        _history.back().erase(_cursor, 1);

        printf("%s", _history.back().c_str() + _cursor);
        fflush(stdout);

        if (_cursor < _history.back().size()) {
            MoveCursorLeft(_history.back().size() - _cursor);
        }
    }
}

} // namespace Dusk