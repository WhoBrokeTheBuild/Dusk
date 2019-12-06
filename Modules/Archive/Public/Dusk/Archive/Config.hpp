#ifndef DUSK_ARCHIVE_HPP
#define DUSK_ARCHIVE_HPP

#include <Dusk/Config.hpp>

#if defined(DUSK_ARCHIVE_EXPORT)
    #define DUSK_ARCHIVE_API DUSK_API_EXPORT
#else
    #define DUSK_ARCHIVE_API DUSK_API_IMPORT
#endif

#endif // DUSK_ARCHIVE_HPP
