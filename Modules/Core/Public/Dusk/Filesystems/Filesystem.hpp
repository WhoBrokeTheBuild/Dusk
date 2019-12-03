#ifndef DUSK_FILESYSTEM_HPP
#define DUSK_FILESYSTEM_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Macros.hpp>

#include <cstdlib>
#include <cstdint>
#include <limits>
#include <string>

namespace Dusk {

class DUSK_CORE_API IFile
{
public:

    enum class Seek {
        Begin,
        End,
        Current,
    };

    virtual size_t GetSize() = 0;

    virtual size_t Read(size_t size, uint8_t * buffer) = 0;

    virtual void Write(uint8_t * data, size_t length) = 0;

    virtual void Seek(size_t offset, Seek where) = 0;

    virtual void Close() = 0;

    virtual inline void Rewind() {
        Seek(0, Seek::Begin);
    }

    virtual uint8_t Get8() {
        uint8_t data;
        Read(1, &data);
        return data;
    }

    virtual uint16_t Get16BigEndian() {
        uint16_t data = ((uint16_t)Get8() << 8) + Get8();
        return data;
    }

    virtual uint32_t Get32BigEndian() {
        uint32_t data = ((uint32_t)Get16BigEndian() << 16) + Get16BigEndian();
        return data;
    }

    virtual uint64_t Get64BigEndian() {
        uint64_t data = ((uint64_t)Get32BigEndian() << 32) + Get32BigEndian();
        return data;
    }

    virtual uint16_t Get16LittleEndian() {
        uint16_t data = Get8() + ((uint16_t)Get8() << 8);
        return data;
    }

    virtual uint32_t Get32LittleEndian() {
        uint32_t data = Get16LittleEndian() + ((uint32_t)Get16LittleEndian() << 16);
        return data;
    }

    virtual uint64_t Get64LittleEndian() {
        uint64_t data = Get32LittleEndian() + ((uint64_t)Get32LittleEndian() << 32);
        return data;
    }

}; // class IFile

class DUSK_CORE_API IFilesystem
{
public:

    enum class Mode {
        Read        = 1 << 0,
        Write       = 1 << 1,
        Append      = 1 << 2,
        Binary      = 1 << 3,
        Truncate    = 1 << 4,
    };

    explicit IFilesystem() = default;

    virtual ~IFilesystem() = default;

    virtual IFile Open(const std::string& filename, Mode mode) = 0;

}; // class IFilesystem

} // namespace Dusk

#endif // DUSK_FILESYSTEM_HPP
