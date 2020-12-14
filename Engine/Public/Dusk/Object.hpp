#ifndef DUSK_OBJECT_HPP
#define DUSK_OBJECT_HPP

#include <Dusk/Config.hpp>

#include <string>

namespace Dusk {

class DUSK_ENGINE_API Object
{
public:

    virtual ~Object() = default;

    // Normal new operator

    void * operator new(size_t size);

    void * operator new[](size_t size);

    // Placement new operator

    void * operator new(size_t size, int line, const char * filename);

    void * operator new[](size_t size, int line, const char * filename);

    // Normal delete operator

    void operator delete(void * ptr);

    void operator delete[](void * ptr);

    // Required because of placement new operator, should not be used
    
    inline void operator delete(void * ptr, int, const char *) {
        ::operator delete(ptr);
    }

    inline void operator delete[](void * ptr, int, const char *) {
        ::operator delete[](ptr);
    }

}; // class Object

void InitMemoryTracking();

void TermMemoryTracking();

} // namespace Dusk

#if defined(DUSK_BUILD_DEBUG)

    #define New new(__LINE__, __FILE__)

#else

    #define New new

#endif

#endif // DUSK_OBJECT_HPP