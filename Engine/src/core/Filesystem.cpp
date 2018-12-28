#include "dusk/core/Filesystem.hpp"
#include <dusk/core/Util.hpp>
#include <dusk/core/Log.hpp>
#include <codecvt>
#include <locale>
#include <cerrno>

#if defined(WIN32)

#elif defined(__linux__)

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#endif

namespace dusk {

#if defined(WIN32)

const char Path::Sep = '\\';

void HandleDeleter(HandleWrap * wrp)
{
    HANDLE hnd = wrp->handle;
    if (hnd) FindClose(hnd);
    delete wrp;
}

#elif defined(__linux__)

const char Path::Sep = '/';

void DirDeleter(DIR * dir)
{
    if (dir) closedir(dir);
}

#endif

Path::Path(const char * path)
    : _path(path)
{
    //std::string tmp(path);
    //_path.assign(tmp.begin(), tmp.end());
    Clean();
}

Path::Path(const std::string& path)
    : _path(path.begin(), path.end())
{
    Clean();
}

//Path::Path(const std::wstring& path)
//    : _path(path)
//{
//}

const char * Path::GetCStr() const
{
    return _path.c_str();
}

std::string Path::GetStr() const
{
    //std::setlocale(LC_ALL, "");
    //std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> conv;
    //return conv.to_bytes(_path);
    return _path;
}

std::string Path::GetDirname() const
{
    return dusk::GetDirname(GetStr());
}

std::string Path::GetBasename() const
{
    return dusk::GetBasename(GetStr());
}

std::string Path::GetExt() const
{
    return GetExtension(GetStr());
}

bool Path::IsEmpty() const
{
    return _path.empty();
}

Path Path::operator+(const Path& rhs) const
{
    return Path(_path + Path::Sep + rhs._path);
}

void Path::Clean()
{
    if (_path.empty()) return;

    size_t left;
    bool absolute = false;

#if defined(WIN32)
    // Clean mixed '/' '\'
    size_t it = _path.find('/');
    while (it != std::string::npos) {
        _path[it] = Path::Sep;
        it = _path.find('/', it + 1);
    }
#elif defined(__linux__)
    absolute = (_path.front() == Path::Sep);
#endif

    std::vector<std::string> parts = StringSplit(Path::Sep, _path);
    _path.clear();

    //for (size_t i = 0; i < parts.size(); ++i) {
    //    if (i > 0 && parts[i] == "..") {
    //        parts.erase(parts.begin() + i - 1, parts.begin() + i);
    //    }
    //}

    // Linux leading '/'
    if (absolute) {
        _path += Path::Sep;
    }

    _path += StringJoin(Path::Sep, parts);

    if (!_path.empty()) {
        if (_path.back() == Path::Sep) {
            _path.pop_back();
        }
    }
}

DirectoryEntry::DirectoryEntry(const Path& path)
    : _path(path)
{
#if defined(WIN32)

#elif defined(__linux__)
    struct stat st;
    if (stat(_path.GetCStr(), &st) == 0) {
        _directory = (S_ISDIR(st.st_mode) != 0);
        _size = st.st_size;
    }
#endif
}

Path DirectoryEntry::GetPath() const
{
    return _path;
}

bool DirectoryEntry::IsDirectory() const
{
    return _directory;
}

bool DirectoryEntry::IsFile() const
{
    return !_directory;
}

size_t DirectoryEntry::GetSize() const
{
    return _size;
}

DirectoryIterator::DirectoryIterator(Path path)
{
    _basePath = path;

#if defined(WIN32)

    path = path + dusk::Path("*");

    LARGE_INTEGER size;
    WIN32_FIND_DATA ffd;
    _hnd.reset(new HandleWrap(), HandleDeleter);
    _hnd->handle = FindFirstFile(path.GetCStr(), &ffd);

    if (_hnd->handle == INVALID_HANDLE_VALUE) {
        return;
    }
    
    DuskLogInfo("%s", ffd.cFileName);
    
    if (strcmp(ffd.cFileName, ".") == 0) {
        ++(*this);
    }
    else if (strcmp(ffd.cFileName, "..") == 0) {
        ++(*this);
    }
    else {
        size.LowPart = ffd.nFileSizeLow;
        size.HighPart = ffd.nFileSizeHigh;
        _entry = DirectoryEntry(_basePath + Path(ffd.cFileName));
        _entry._size = size.QuadPart;
        _entry._directory = (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY > 0);
        _entry._directory = (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ? true : false);
    }

#elif defined(__linux__)

    struct dirent * ent;

    _dir.reset(opendir(path.GetCStr()), DirDeleter);

    if (!_dir) {
        return;
    }

    ent = readdir(_dir.get());
    _index = telldir(_dir.get());

    if (strcmp(ent->d_name, ".") == 0) {
        ++(*this);
    }
    else if (strcmp(ent->d_name, "..") == 0) {
        ++(*this);
    }
    else {
        //DuskLogInfo("New Entry %s", ent->d_name);
        _entry = DirectoryEntry(_basePath + Path(ent->d_name));
    }

#endif
}

DirectoryIterator::DirectoryIterator(const DirectoryIterator& rhs)
{
    _basePath = rhs._basePath;
    _entry = rhs._entry;

#if defined(WIN32)
    _hnd = rhs._hnd;
#elif defined(__linux__)
    _dir = rhs._dir;
    _index = rhs._index;
#endif
}

DirectoryIterator& DirectoryIterator::operator=(const DirectoryIterator& rhs)
{
    _basePath = rhs._basePath;
    _entry = rhs._entry;

#if defined(WIN32)
    _hnd = rhs._hnd;
#elif defined(__linux__)
    _dir = rhs._dir;
    _index = rhs._index;
#endif

    return *this;
}

DirectoryIterator& DirectoryIterator::operator++()
{
#if defined(WIN32)

    LARGE_INTEGER size;
    WIN32_FIND_DATA ffd;
    
    if (FindNextFile(_hnd->handle, &ffd) == 0) {
        _hnd = nullptr;
        return *this;
    }

    DuskLogInfo("%s", ffd.cFileName);
    
    if (strcmp(ffd.cFileName, ".") == 0) {
        ++(*this);
    }
    else if (strcmp(ffd.cFileName, "..") == 0) {
        ++(*this);
    }
    else {
        size.LowPart = ffd.nFileSizeLow;
        size.HighPart = ffd.nFileSizeHigh;
        _entry = DirectoryEntry(_basePath + Path(ffd.cFileName));
        _entry._size = size.QuadPart;
        _entry._directory = (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ? true : false);
    }

#elif defined(__linux__)

    struct dirent * ent = nullptr;

    seekdir(_dir.get(), _index);
    ent = readdir(_dir.get());

    if (!ent) {
        _dir = nullptr;
        _index = -1;
        return *this;
    }

    _index = telldir(_dir.get());

    if (strcmp(ent->d_name, ".") == 0) {
        ++(*this);
    }
    else if (strcmp(ent->d_name, "..") == 0) {
        ++(*this);
    }
    else {
        //DuskLogInfo("New Entry %s", ent->d_name);
        _entry = DirectoryEntry(_basePath + Path(ent->d_name));
    }

#endif

    return *this;
}

const DirectoryEntry& DirectoryIterator::operator*() const
{
    return _entry;
}

const DirectoryEntry* DirectoryIterator::operator->() const
{
    return &_entry;
}

bool operator==(const DirectoryIterator& lhs, const DirectoryIterator& rhs)
{
#if defined(WIN32)
    return (lhs._hnd.get() == rhs._hnd.get());
#elif defined(__linux__)
    return (lhs._dir.get() == rhs._dir.get() &&
            lhs._index == rhs._index);
#endif
}

bool operator!=(const DirectoryIterator& lhs, const DirectoryIterator& rhs)
{
    return !(lhs == rhs);
}

DirectoryIterator begin(DirectoryIterator iter) noexcept
{
    return iter;
}

DirectoryIterator end(const DirectoryIterator&) noexcept
{
    return DirectoryIterator();
}

} // namespace dusk
