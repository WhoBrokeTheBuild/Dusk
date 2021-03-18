#ifndef DUSK_OBJECT_HPP
#define DUSK_OBJECT_HPP

#include <Dusk/Config.hpp>

#include <atomic>
#include <string>
#include <cassert>

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

//     inline void AddReference() {
//         ++_refCount;
//     }

//     inline void RemoveReference() {
//         --_refCount;
//     }

//     inline long GetReferenceCount() {
//         return _refCount;
//     }

// private:

//     std::atomic<long> _refCount = 0;

}; // class Object

// template <class T>
// class DUSK_ENGINE_API Pointer
// {
// public:

//     static_assert(std::is_base_of<Object, T>::value);

//     Pointer() 
//         : _ptr(nullptr)
//     { }

//     Pointer(T * ptr)
//         : _ptr(ptr)
//     {
//         AddReference();
//     }

//     Pointer(const Pointer& other)
//         : _ptr(other._ptr)
//     {
//         AddReference();
//     }

//     Pointer(Pointer && other)
//         : _ptr(other._ptr)
//     {
//         other._ptr = nullptr;
//     }

//     virtual ~Pointer() {
//         RemoveReference();
//     }

//     Pointer& operator=(const Pointer& other) {
//         RemoveReference();
//         _ptr = other._ptr;
//         AddReference();

//         return *this;
//     }

//     Pointer& operator=(Pointer && other) {
//         if (this != &rhs) {
//             RemoveReference();
//             _ptr = other._ptr;
//             other._ptr = nullptr;
//         }
        
//         return *this;
//     }

//     inline void AddReference() {
//         if (_ptr) {
//             _ptr->AddReference();
//         }
//     }

//     inline void RemoveReference() {
//         if (_ptr) {
//             _ptr->RemoveReference();
//         }
//     }

//     explicit operator bool() const {
//         return (_ptr != nullptr);
//     }

//     inline T* Get() const {
//         return _ptr;
//     }

//     inline void Reset(T * ptr = nullptr) {
//         RemoveReference();
//         _ptr = ptr;
//         AddReference();
//     }

//     inline T& operator*() const {
//         return *_ptr;
//     }

//     inline T* operator->() const {
//         return _ptr;
//     }

//     template <class O>
//     O * As() const {
//         return dynamic_cast<O *>(_ptr);
//     }

// private:

//     T * _ptr;

// };

void InitMemoryTracking();

void TermMemoryTracking();

} // namespace Dusk

#if defined(DUSK_BUILD_DEBUG)

    #define New new(__LINE__, __FILE__)

#else

    #define New new

#endif

#endif // DUSK_OBJECT_HPP