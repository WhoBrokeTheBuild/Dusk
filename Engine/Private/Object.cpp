#include <Dusk/Object.hpp>
#include <Dusk/Log.hpp>

#include <unordered_map>

namespace Dusk {

struct ObjectAllocation
{
    int Line;

    size_t Size;

    string Filename;

    string ClassID;
};

bool _trackingAllocations = false;

std::unordered_map<void *, ObjectAllocation> _allocations;

void * Object::operator new(size_t size)
{
    return ::operator new(size);
}

void * Object::operator new[](size_t size)
{
    return ::operator new[](size);
}

void * Object::operator new(size_t size, int line, const char * filename)
{
    void * ptr = ::operator new(size);

    _allocations[ptr] = ObjectAllocation{line, size, filename};

    return ptr;
}

void * Object::operator new[](size_t size, int line, const char * filename)
{
    void * ptr = ::operator new[](size);

    _allocations[ptr] = ObjectAllocation{line, size, filename};

    return ptr;
}

void Object::operator delete(void * ptr)
{
    if (_trackingAllocations) {
        auto it = _allocations.find(ptr);
        if (it != _allocations.end()) {
            _allocations.erase(it);
        }
    }

    ::operator delete(ptr);
}

void Object::operator delete[](void * ptr)
{
    if (_trackingAllocations) {
        auto it = _allocations.find(ptr);
        if (it != _allocations.end()) {
            _allocations.erase(it);
        }
    }

    ::operator delete[](ptr);
}

void InitMemoryTracking()
{
    _trackingAllocations = true;
}

void TermMemoryTracking()
{
    _trackingAllocations = false;

    if (!_allocations.empty()) {
        DuskLogError("Remaining Memory Allocations:");
        for (const auto& it : _allocations) {
            void * ptr = it.first;
            const ObjectAllocation& allocation = it.second;

            DuskLogError("%p: %zu byte(s) allocated at %s:%d",
                ptr,
                allocation.Size,
                allocation.Filename,
                allocation.Line);
        }
    }

    _allocations.clear();
}

} // namespace Dusk