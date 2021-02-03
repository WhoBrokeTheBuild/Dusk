#include <Dusk/ScriptConsole.hpp>
#include <Dusk/Dusk.hpp>

#include <Dusk/Log.hpp>
#include <Python/PyDusk.hpp>

#include <algorithm>

namespace Dusk {

bool ScriptConsole::_initialized = false;

PyObject * ScriptConsole::_locals = nullptr;

std::vector<string> ScriptConsole::_history;

int ScriptConsole::_cursor = 0;
int ScriptConsole::_index = 0;

#if defined(DUSK_PLATFORM_WINDOWS)

    HANDLE ScriptConsole::_stdinHandle;
    HANDLE ScriptConsole::_stdoutHandle;

#else

    struct termios ScriptConsole::_termios;

#endif

void ScriptConsole::Initialize()
{
#if defined(DUSK_PLATFORM_WINDOWS)

    if (!_isatty(_fileno(stdin))) {
        return;
    }

    _stdinHandle = GetStdHandle(STD_INPUT_HANDLE);
    _stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    DWORD mode;
    GetConsoleMode(_stdinHandle, &mode);
    SetConsoleMode(_stdinHandle, mode & ~(ENABLE_PROCESSED_INPUT));

    GetConsoleMode(_stdoutHandle, &mode);
    SetConsoleMode(_stdoutHandle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

#else

    if (!isatty(STDIN_FILENO)) {
        return;
    }

    struct termios tmp;

    tcgetattr(STDIN_FILENO, &_termios);
    tmp = _termios;
    tmp.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    tmp.c_cc[VMIN] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &tmp);

#endif

    PyObject * pyMain = PyImport_AddModule("__main__");
    PyObject * pyMainDict = PyModule_GetDict(pyMain);
    _locals = PyDict_New();
    
    PyRun_String("import Dusk", Py_single_input, pyMainDict, _locals);

    _history.push_back(string());
    _index = _history.size() - 1;

    printf(">>> ");
    fflush(stdout);

    _initialized = true;
}

void ScriptConsole::Terminate()
{
    if (!_initialized) {
        return;
    }
    
    Py_XDECREF(_locals);

#if !defined(DUSK_PLATFORM_WINDOWS)

    tcsetattr(STDIN_FILENO, TCSANOW, &_termios);

#endif

    _initialized = false;
}

void ScriptConsole::Update()
{
    if (!_initialized) {
        return;
    }

#if defined(DUSK_PLATFORM_WINDOWS)

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

    while (select(1, &fds, NULL, NULL, &tv) > 0) {
        ReadNextCharacter();
    }

#endif
}

void ScriptConsole::ReadNextCharacter()
{
    if (!_initialized) {
        return;
    }

    // TODO: Support UTF-8
    
    unsigned char c;

#if defined(DUSK_PLATFORM_WINDOWS)

    const DWORD CTRL_MASK = (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED);
    
    DWORD read;
    struct _INPUT_RECORD record;

    if (!ReadConsoleInput(_stdinHandle, &record, 1, &read)) {
        return;
    }
    
    if (record.EventType != KEY_EVENT || !record.Event.KeyEvent.bKeyDown) {
        return;
    }

    bool ctrlPressed = ((record.Event.KeyEvent.dwControlKeyState & CTRL_MASK) > 0);
    
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

    static char modifier = '\0';
    
    if (inEscapeSequence) {       
        // static string escapeSequence;
        // escapeSequence += c;

        if (c == '[' || c == ';') {
            return;
        }

        if (isdigit(c)) {
            modifier = c;
            return;
        }

        switch (c) {
        case 'D': 
            HandleLeft(modifier == '5');
            break;
        case 'C':
            HandleRight(modifier == '5');
            break;
        case 'A':
            HandleUp();
            break;
        case 'B':
            HandleDown();
            break;
        case 'H':
            HandleHome();
            break;
        case 'F':
            HandleEnd();
            break;
        case '~':
            if (modifier == '3') {
                HandleDelete();
            }
            break;
        }

        modifier = '\0';
        inEscapeSequence = false;

        // for (char c : escapeSequence) {
        //     DuskLogVerbose("%c", c);
        // }
        // escapeSequence.clear();

        return;
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
                _history.push_back(string());
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

        GetCurrentLine().insert(_cursor, 1, c);
        ++_cursor;

        printf("%s", GetCurrentLine().c_str() + _cursor);
        fflush(stdout);

        if (_cursor < GetCurrentLine().size()) {
            printf("\033[%zuD", GetCurrentLine().size() - _cursor);
            fflush(stdout);
        }
    }

}

bool ScriptConsole::RunCommand(const string& command)
{
    if (!_initialized) {
        return false;
    }

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
    if (!_initialized) {
        return;
    }

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
    if (!_initialized) {
        return;
    }
    
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
    if (!_initialized) {
        return;
    }
    
    amount = std::clamp(amount, 0, _cursor);
    if (amount > 0) {
        printf("\033[%dD", amount);
        fflush(stdout);
        _cursor -= amount;
    }
}

void ScriptConsole::MoveCursorRight(int amount)
{
    if (!_initialized) {
        return;
    }
    
    int maxLen = static_cast<int>(GetCurrentLine().size());
    amount = std::clamp(amount, 0, maxLen - _cursor);
    if (amount > 0) {
        printf("\033[%dC", amount);
        fflush(stdout);
        _cursor += amount;
    }
}

void ScriptConsole::HandleUp()
{
    if (!_initialized) {
        return;
    }
    
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
    if (!_initialized) {
        return;
    }
    
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
    if (!_initialized) {
        return;
    }
    
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
    if (!_initialized) {
        return;
    }
    
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
    if (!_initialized) {
        return;
    }
    
    MoveCursorLeft(_cursor);
}

void ScriptConsole::HandleEnd()
{
    if (!_initialized) {
        return;
    }
    
    MoveCursorRight(_history.back().size() - _cursor);
}

void ScriptConsole::HandleBackspace()
{
    if (!_initialized) {
        return;
    }
    
    if (_cursor > 0) {
        printf("\010\033[K");
        fflush(stdout);
        
        if (_index < _history.size() - 1) {
            _history.back() = GetCurrentLine();
            _index = _history.size() - 1;
        }

        --_cursor;
        GetCurrentLine().erase(_cursor, 1);

        printf("%s", GetCurrentLine().c_str() + _cursor);
        fflush(stdout);

        if (_cursor < GetCurrentLine().size()) {
            printf("\033[%zuD", GetCurrentLine().size() - _cursor);
            fflush(stdout);
        }
    }
}

void ScriptConsole::HandleDelete()
{
    if (!_initialized) {
        return;
    }
    
    if (_cursor < GetCurrentLine().size()) {
        printf("\033[K");
        fflush(stdout);
        
        if (_index < _history.size() - 1) {
            _history.back() = GetCurrentLine();
            _index = _history.size() - 1;
        }

        GetCurrentLine().erase(_cursor, 1);

        printf("%s", GetCurrentLine().c_str() + _cursor);
        fflush(stdout);

        if (_cursor < GetCurrentLine().size()) {
            printf("\033[%zuD", GetCurrentLine().size() - _cursor);
            fflush(stdout);
        }
    }
}

} // namespace Dusk