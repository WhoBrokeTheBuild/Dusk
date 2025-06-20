#ifndef DUSK_FILE_HPP
#define DUSK_FILE_HPP

#include <Dusk/Macros.hpp>
#include <Dusk/List.hpp>
#include <Dusk/Path.hpp>
#include <Dusk/Optional.hpp>
#include <Dusk/Span.hpp>

#include <fstream>

namespace dusk {

class DUSK_API File
{
public:

    enum class Mode {
        Read  = std::ios::in,
        Write = std::ios::out,

        Default  = Read | Write,
    };

    File() = default;

    File(const Path& path, Mode mode = Mode::Default) {
        Open(path, mode);
    }

    virtual ~File() = default;

    bool Open(const Path& path, Mode mode = Mode::Default);

    void Close();

    inline size_t GetPosition() {
        return _file.tellg();
    }

    inline size_t GetSize() const {
        return _size;
    }

    inline std::fstream& GetStream() {
        return _file;
    }

    inline bool IsOpen() const {
        return _file.is_open();
    }

    inline operator bool() {
        return bool(_file);
    }

    inline bool ReadInto(void * buffer, size_t size) {
        _file.read(reinterpret_cast<char *>(buffer), size);
        return _file.good();
    }

    template <typename T>
    inline bool ReadInto(Span<T> buffer) {
        return ReadInto(buffer.data(), buffer.size_bytes());
    }

    template <typename T = uint8_t>
    Optional<T> ReadOne() {
        T value = {};
        if (not ReadInto(&value, sizeof(value))) {
            return nullopt;
        }
        return value;
    }

    template <typename T = uint8_t>
    inline Optional<List<T>> ReadSome(size_t count) {
        List<T> data(count);
        if (not ReadInto<T>(data)) {
            return nullopt;
        }
        return data;
    }

    template <typename T = uint8_t>
    inline Optional<List<T>> ReadAll() {
        size_t remaining = _size - _file.tellg();
        return ReadSome<T>(remaining / sizeof(T));
    }

    template <typename T = uint8_t>
    static Optional<List<T>> ReadFile(const Path& path) {
        File file(path);
        if (not file) {
            return nullopt;
        }

        return file.ReadAll<T>();
    }


private:

    std::fstream _file;

    size_t _size = 0;

}; // class File

}; // namespace dusk

#endif // DUSK_FILE_HPP