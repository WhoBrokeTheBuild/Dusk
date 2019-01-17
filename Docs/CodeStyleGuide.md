# Dusk Code Style Guidelines

## Indentation

The indent size is 4 spaces. Tabs should never be used.

**Right**
```cpp
int func() {
    return 0;
}
```

**Wrong**
```cpp
int func() {
  return 0;
}
int func() {
        return 0;
}
```

Namespaces should have no indent.

**Right**
```cpp
namespace dusk {

namespace glTF2 {

void func() {

}

} // namespace glTF2

} // namespace dusk
```

**Wrong**
```cpp
namespace dusk {
    namespace glTF2 {
        void func() {

        }
    }
}
```

Case labels should line up with their switch statements. The case statements are indented.

**Right**
```cpp
switch (condition) {
case 0:
case 1:
    ++i;
    break;
case 2:
    --i;
    break;
default:
    i = 0;
}
```

**Wrong**
```cpp
switch (condition) {
    case 0:
    case 1:
        ++i;
        break;
    case 2:
        --i;
    break;
    default:
}
```

Boolean expressions that span multiple lines should have their operators on the left side.

**Right**
```cpp
return checkFirstCase() 
    || checkSecondCase()
    || (checkThirdCase() && checkEdgeCase());
```

**Wrong**
```cpp
return checkFirstCase() ||
    checkSecondCase() ||
    (checkThirdCase() && checkEdgeCase());
```

## Spacing

Do not put spaces around unary operators.

**Right**
```cpp
++i;
int * j = &i;
```

**Wrong**
```cpp
++ i;
int * j = & i;
```

Do put spaces around binary and ternary operators.

**Right**
```cpp
y = m * x + b;
f(a, b);
c = a | b;
return (condition ? 1 : 0);
```

**Wrong**
```cpp
y = m*x+b;
f(a,b);
c = a|b;
return (condition?1:0);
```

Do put spaces around the colon in a range-based for loop.

**Right**
```cpp
std::vector<glm::vec2> sizes;
for (auto& size : sizes) {
    DuskLog("%s", glm::to_string(size));
}
```

**Wrong**
```cpp
std::vector<glm::vec2> sizes;
for (auto& size: sizes) {
    DuskLog("%s", glm::to_string(size));
}
```

Do not put spaces before commas or semicolons.

**Right**
```cpp
for (int i = 0; i < 10; ++i) {
    doSomething();
}

f(a, b);
```

**Wrong**
```cpp
for (int i = 0 ; i < 10 ; ++i) {
    doSomething();
}

f(a , b) ;
```

Do put spaces between control statements and parenthesis.

**Right**
```cpp
if (condition) {
    doSomething();
}
```

**Wrong**
```cpp
if(condition) {
    doSomething();
}
```

When brace-initializing an object, do put spaces between the object name, and the concents of the braces.

**Right**
```cpp
glm::vec2 size { 0.f, 10.f };
```

**Wrong**
```cpp
glm::vec2 size{ 0.f, 10.f };
glm::vec2 size {0.f, 10.f};
```

## Line Breaking

Every statement gets its own line.

**Right**
```cpp
++x;
++y;
if (condition) {
    doSomething();
}
```

**Wrong**
```cpp
++x; ++y;
if (condition) { doSomething(); }
```

An else should go on the same line as the previous end-bracket.

**Right**
```cpp
if (condition) {
    doSomething();
} else if (otherCondition) {
    doSomethingElse();
} else {
    doNothing();
}
```

**Wrong**
```cpp
if (condition) {
    doSomething();
} 
else if (otherCondition) {
    doSomethingElse();
}
else {
    doNothing();
}
```

Namespaces should have one line of padding after opening and before closing.

**Right**
```cpp
namespace dusk {

namespace glTF2 {

void doSomething();

} // namespace glTF2

} // namespace dusk
```

**Wrong**
```cpp
namespace dusk {
namespace glTF2 {
void doSomething();
} // namespace glTF2
} // namespace dusk
```

## Braces

Functions, and Classes should have their braces on their own line.

**Right**
```cpp
int main()
{
    return 0;
}

class Test 
{

}; // class Test
```

**Wrong**
```cpp
int main() {
    return 0;
}

class Test {

}; // class Test
```

Namespaces should have braces on the same line.

**Right**
```cpp
namespace dusk {

} // namespace dusk
```

**Wrong**
```cpp
namespace dusk 
{

} // namespace dusk
```

All control statements must use braces.

**Right**
```cpp
if (condition) {
    doSomething();
} else {
    doSomethingElse();
}

while (condition) {
    doSomething();
}
```

**Wrong**
```cpp
if (condition)
    doSomething();
else
    doSomethingElse();

if (condition) 
    return;

while (condition) doSomething();
```

Empty control statements must use empty braces.

**Right**
```cpp
for (; wait < 100; wait++) { }
while (condition) { }
```

**Wrong**
```cpp
for (; wait < 100; wait++);
while (condition);
```

## Null, false, and zero

In C++, null should be represented by `nullptr`. In C, null should be represented by `NULL`.

In C++ and C, `bool` values should be represented by `true` or `false`.

In library-specific code, you should use the boolean defines for that library, e.g. `GL_TRUE`, `SDL_FALSE`

Tests for true/false, null/non-null, and zero/non-zero should all be done without equality expressions.

**Right**
```cpp
if (condition) {

}

if (!ptr) {

}

if (!count) {

}
```

**Wrong**
```cpp
if (condition == true) {

}

if (ptr == NULL) {

}

if (count == 0) {
    
}
```

## Floating Point Literals

Always append `f` or `.f` to floating point literals. Do not append `.0f`.

**Right**
```cpp
const float angle = glm::radians(90.f);

const float pi = 3.14159f;
```

**Wrong**
```cpp
const float angle = glm::radians(90.0f);

const float pi = 3.14159;
```

## Names

Use CamelCase for classes, and structs. Capitalize all letters of an acronym as appropraite.

**Right**
```cpp
struct Data;
class JSONElement;
class glTF2;
```

**Wrong**
```cpp
struct data;
class jsonElement;
class GLTF2;
```

Use snakeCase for namespaces. Capitalize all letters of an acronym as appropraite.

**Right**
```cpp
namespace dusk {

namespace glTF2 {

} // namespace glTF2

} // namespace dusk
```

**Wrong**
```cpp
namespace Dusk {

namespace gltf2 {

} // namespace gltf2

} // namespace Dusk
```

Use full words, except for common abbreviations and iterators.

**Right**
```cpp
size_t length;
MeshComponent * meshComponent = new MeshComponent();
for (int i = 0; i 10; ++i) { }
void parseActors();
```

**Wrong**
```cpp
size_t len, l;
MeshComponent * comp = new MeshComponent();
void pActors();
void parseA();
```

Private member variables should be perfixed with `_`. Regular member variables should use snakeCase, static member variables should use CamelCase.

**Right**
```cpp
class Test
{
public:

    ...

private:

    static int _Count;

    int _count;

}
```

**Wrong**
```cpp
class Test
{
public:

    ...

private:

    static int _count;
    static int Count;
    static int _sCount;
    static int s_Count;

    int count;
    int Count;
    int mCount;
    int _mCount;
    int m_Count;

}
```

Prefix boolean variables with a past- or current-tense verbs.

**Right**
```cpp
bool isValid;
bool didSendData;
```

**Wrong**
```cpp
bool valid;
bool sendData;
```

Prefix setters with `Set` and getters with `Get`.

**Right**
```cpp
void SetPosition(const glm::vec3& position);
glm::mat4 GetTransform() const;
```

**Wrong**
```cpp
void Position(const glm::vec3& position);
glm::mat4 Transform() const;
```

Use descriptive verbs in function names.

**Right**
```cpp
std::uint8_t * readDataFromFile();
bool convertToJSON();
```

**Wrong**
```cpp
std::uint8_t * getData();
bool toJSON();
```


Prefer enums to booleans as parameters, except for setter functions.

**Right**
```cpp
loadFromFile("test.txt", AllowNotFound);
doSomething(something, AllowFooBar);
setResizable(false);
```

**Wrong**
```cpp
loadFromFile("test.txt", true);
doSomething(something, false);
setResizable(NotResizable);
```


Prefer inline functions to macros.

Enum members should be CamelCase.

Prefer const to #define.

`#define`'d constants should be ALL_CAPS.

Macros that expand like functions should be named in CamelCase.

**Right**
```cpp
#define DuskLogInfo(M) PrintLog(M, __LINE__, __FILE__);
```

**Wrong**
```cpp
#define DUSK_LOG_INFO(M) PrintLog(M, __LINE__, __FILE__);
```

## Classes 

All non-trivial classes should disable the copy constructor and assignment operator, except when desired. Use the macro `DISALLOW_COPY_AND_ASSIGN` from `Config.hpp`.

```cpp
class Test 
{
public:

    DISALLOW_COPY_AND_ASSIGN(Test)

}; // class Test
```

Constructors should initialize all members either in their declarations, or with initializer syntax. One per line, preceeded by `:` or `,` depending.

**Right**
```cpp
class Test 
{
public:

    Test(const std::string& name, const glm::vec2& size) 
        : _name(name)
        , _size(size)
    { }

private:

    int _count = 0;

    std::string _name;

    glm::vec2 _size;

}; // class Test
```

**Wrong**
```cpp
class Test 
{
public:

    Test(const std::string& name, const glm::vec2& size) : _name(name)
    { 
        _count = 0;
        _size = size;
    }

private:

    int _count;

    std::string _name;

    glm::vec2 _size;

}; // class Test
```

All destructors of virtual classes must be marked virtual.

When overriding virtual functions, the function declaration must have the `override` or `final` keyword. When `override` or `final` are specified, you should not prefix the function with `virtual`.

Use `explicit` to stop unwatned type conversions.

## Best Practices

Source files should wrap contents in the same namespace as their corresponding header.

**Right**
```cpp
#include "header.hpp"

namespace dusk {

Test::Test() {

}

} // namespace dusk
```

**Wrong**
```cpp
#include "header.hpp"

dusk::Test::Test() {
    
}
```

Use range-based for when index is not needed. Use auto for iterators.

**Right**
```cpp
std::vector<std::unique_ptr<Actor>> actors;
for (auto& actor : actors) {
    actor->Update(ctx);
}

// Index is used
for (size_t i = 0; i < actors.size(); ++i) {
    DuskLogInfo("Updating actor #%zu", i);
    actors[i]->Update(ctx);
}
```

**Wrong**
```cpp
std::vector<std::unique_ptr<Actor>> actors;

std::vector<std::unique_ptr<Actor>> it = actors.begin();
for (; it != actors.end(); ++it) {
    actor->Update(ctx);
}

// Index is not used
for (size_t i = 0; i < actors.size(); ++i) {
    actors[i]->Update(ctx);
}
```

All pointers should be written such that the * is spaced from the type and name.

**Right**
```cpp
int * x;
std::vector<Actor *> actors;
```

**Wrong**
```cpp
int* x;
int *y;
std::vector<Actor*> actors;
```

`#include` statements should be grouped into the following categories: Dusk, System, and Third Party. Within those categories they should be alphabetized, except when order is important.

**Right**
```cpp
#include <dusk/Config.hpp>
#include <dusk/asset/Mesh.hpp>
#include <dusk/scene/Actor.hpp>

#include <memory>
#include <vector>

#include <SDL.h>
```

**Wrong**
```cpp
#include <dusk/scene/Actor.hpp>
#include <dusk/asset/Mesh.hpp>
#include <dusk/Config.hpp>
#include <SDL.h>
#include <vector>
#include <memory>
```

Using statements are only allowed inside functions or source files. Do not pollute the global or dusk namespaces.

**Right**
```cpp
void doTimeStuff() {
    using std::chrono;
    ...
}
```

**Wrong**
```cpp
using namespace std::chrono;

void doTimeStuff() {
    ...
}
```

Omit unnecessary parameter names from function declarations where the size or function name allow you to infer the use.

**Right**
```cpp
void SetWidth(int);
void AddActor(std::unique_ptr<Actor>&&);

void doSomething(int amountToAdd);
```

**Wrong**
```cpp
void SetWidth(int width);
void AddActor(std::unique_ptr<Actor>&& actor);

void doSomething(int);
```

Use `auto` when needed, and when helpful. Do not use it to infer basic types like ints or floats.

**Right**
```cpp
const auto& actor = GetFirstActor();
auto it = _array.begin();
auto mesh = std::make_unique<Mesh>();
```

**Wrong**
```cpp
const std::unique_ptr<Actor>& actor = GetFirstActor();
std::vector<std::unique_ptr<Mesh>>::iterator it = _array.begin();
std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>();
```

Use `unsigned` instead of `unsigned int`.

Use `uint8_t *` over `unsigned char *` for binary data.

Avoid `using namespace x`, instead prefix all types with `x::`. (e.g. `std::`, `glm::`). 

All angles will be stored in radians. When storing constants that are better read as degrees, e.g. 90d, store them as `glm::radians(90.f)`.

Use `::` to explicitly request the global scope when referencing C functions with conflicting names:

```
void Socket::send() {
    ::send(_socket, _buffer.data(), _buffer.size(), 0);
}
```

Prefix resource variables from Third Party libraries with an identifier. (e.g. `_sdlWindow`, `_glVAO`).

All header files should use `#ifndef` guards named `DUSK_FILENAME_HPP`, where FILENAME is an all-caps representation of the filename.

## Comments

Classes and Namespaces should end with a comment `// class name` or `// namespace name`.

**Right**
```cpp
namespace dusk {

namespace glTF2 {

class Test 
{

}; // class Test

} // namespace glTF2

} // namespace dusk
```

**Wrong**
```cpp
namespace dusk {

namespace glTF2 {

class Test 
{

};

}

}
```

Use `// TODO` or `// FIXME` comments to indicate a section needs a new feature, or is broken respectively.
