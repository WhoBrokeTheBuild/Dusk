#ifndef DUSK_MACROS_HPP
#define DUSK_MACROS_HPP

#define DISALLOW_COPY_AND_ASSIGN(TypeName)              \
    TypeName(const TypeName&) = delete;                 \
    TypeName& operator=(const TypeName&) = delete;

#define DUSK_STRINGIFY(x) _DUSK_STRINGIFY(x)
#define _DUSK_STRINGIFY(x) #x

#endif // DUSK_MACROS_HPP
