#ifndef DUSK_CORE_FILESYSTEM_HPP
#define DUSK_CORE_FILESYSTEM_HPP

#include <dusk/Config.hpp>

#include <string>
using std::string;

#include <memory>
using std::shared_ptr;

#if defined(WIN32)

struct HandleWrap {
    HANDLE handle;
};

#elif defined(__linux__)

#include <dirent.h>

#endif

namespace dusk {

class Path
{
public:

    static const char Sep;

    Path() = default;
    Path(const Path&) = default;
    Path& operator=(const Path& rhs) = default;

    explicit Path(const char * path);
    explicit Path(const string& path);
    //Path(const wstring& path);

    const char * GetCStr() const;
    string GetStr() const;
    //wstring GetWStr() const;

    string GetDirname() const;
    string GetBasename() const;
    string GetExt() const;

    bool IsEmpty() const;

    Path operator+(const Path& rhs) const;

private:

    void Clean();

    //wstring _path;
    string _path;

};

class DirectoryIterator;

class DirectoryEntry
{
    friend class DirectoryIterator;

public:

    DirectoryEntry() = default;
    DirectoryEntry(const DirectoryEntry&) = default;

    DirectoryEntry(const Path& path);

    Path GetPath() const;

    bool IsDirectory() const;
    bool IsFile() const;

    size_t GetSize() const;

private:

    Path _path;
    bool _directory = false;
    size_t _size = 0;

};

class DirectoryIterator
{
public:

    DirectoryIterator() = default;

    DirectoryIterator(Path path);
    DirectoryIterator(const DirectoryIterator&);

    DirectoryIterator& operator=(const DirectoryIterator&);
    DirectoryIterator& operator++();
    const DirectoryEntry& operator*() const;
    const DirectoryEntry* operator->() const;

    friend void swap(DirectoryIterator& lhs, DirectoryIterator& rhs);

    friend bool operator==(const DirectoryIterator& lhs, const DirectoryIterator& rhs);
    friend bool operator!=(const DirectoryIterator& lhs, const DirectoryIterator& rhs);

private:

    Path _basePath;
    DirectoryEntry _entry;

#if defined(WIN32)

    shared_ptr<HandleWrap> _hnd;

#elif defined(__linux__)

    shared_ptr<DIR> _dir;
    long _index = -1;

#endif

};

DirectoryIterator begin(DirectoryIterator iter) noexcept;
DirectoryIterator end(const DirectoryIterator&) noexcept;

bool operator==(const DirectoryIterator& lhs, const DirectoryIterator& rhs);
bool operator!=(const DirectoryIterator& lhs, const DirectoryIterator& rhs);

class RecursiveDirectoryIterator
{
public:

    RecursiveDirectoryIterator() = default;
    RecursiveDirectoryIterator(const RecursiveDirectoryIterator&) = default;

    RecursiveDirectoryIterator(Path path);

    RecursiveDirectoryIterator& operator=(const RecursiveDirectoryIterator&);
    RecursiveDirectoryIterator& operator++();
    DirectoryEntry& operator*() const;
    DirectoryEntry* operator->() const;

private:

};

RecursiveDirectoryIterator begin(RecursiveDirectoryIterator iter) noexcept;
RecursiveDirectoryIterator end(const RecursiveDirectoryIterator&) noexcept;

} // namespace dusk

#endif // DUSK_CORE_FILESYSTEM_HPP
