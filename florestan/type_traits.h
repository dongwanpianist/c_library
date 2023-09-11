// Florestan's Type Traits 
//
// © dongwanpianist
// September 11th, 2023
// Evanston, Illinois, USA
//
// C doesn't have any template (of C++) for receiving multiple types for parameters,
// but C can still use some Preprocessor features and "_Generic".
// I made this C-style <type_traits>, hoping to be helpful for you to implement some C Preprocessor macros.
// *almost* stable
//
// 1. printtype(VARIABLE) -> printf
// 2. allocated_info(POINTER_OR_ARRAY) -> allocated_record
// 3. allocated_record  (typedef struct __allocated_record)
//      .name           (const char*)
//      .method         (enum: dynamic, allocated, fixed)
//                          * allocated and fixed pointer variable can show the correct sizes,
//                            while dynamic record cannot show any specific size. Have your own count!
//      .pointer_depth  (uint8_t / unsigned char)
//      .size           (size_t)
//      .typesize       (size_t)
//      .arraysize      (size_t)
// 4. __is_{type}(VARIABLE) -> bool(0 or 1)
//      * Supported types: all fundamental types of C
//          bool, char, signed char, unsigned char, short, unsigned short,
//          int, unsigned int, long, unsigned long, long long, unsigned long long
//          double, float, long double
//          pointer of all above types, including void*, upto quadruple pointer
//      * alteration of name for these:
//          unsigned -> u (__is_uchar, __is_ushort, __is_uint, __is_ulong, __is_ullong)
//          long -> l (__is_llong, __is_ullong, __is_ldouble)
// 5. __pointer_depth(VARIABLE) -> bool(0 or 1)
//      * depth is supported until 4th one
// 5. __is_const(VARIABLE) -> bool(0 or 1)
// 6. __is_const_pointer(VARIABLE) -> bool(0 or 1)
//      * constant pointer is read when its the final constant pointer during depth of 4
// 7. __sizeof(VARIABLE) -> size_t
//      * tracks its type at the final pointer
// 8. alloc_sizeof -> size_t
//      * OS-specific function (supports Apple, Win,  but not unix?)
// ( 9. __type_num: for debugging )

#ifndef FLORESTAN_TYPE_TRAITS_H
#define FLORESTAN_TYPE_TRAITS_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define __is_void(...) _Generic((&__VA_ARGS__), \
    void*: 1,     const void*: 1, \
    void**: 1,    const void**: 1, \
    void***: 1,   const void***: 1, \
    void****: 1,  const void****: 1, \
    void*****: 1, const void*****: 1, \
    default: 0 )
#define __is_bool(...) _Generic((__VA_ARGS__), \
    bool: 1,     const bool: 1, \
    bool*: 1,    const bool*: 1, \
    bool**: 1,   const bool**: 1, \
    bool***: 1,  const bool***: 1, \
    bool****: 1, const bool****: 1, \
    default: 0 )
#define __is_char(...) _Generic((__VA_ARGS__), \
    char: 1,     const char: 1, \
    char*: 1,    const char*: 1, \
    char**: 1,   const char**: 1, \
    char***: 1,  const char***: 1, \
    char****: 1, const char****: 1, \
    default: 0 )
#define __is_short(...) _Generic((__VA_ARGS__), \
    short: 1,     const short: 1, \
    short*: 1,    const short*: 1, \
    short**: 1,   const short**: 1, \
    short***: 1,  const short***: 1, \
    short****: 1, const short****: 1, \
    default: 0 )
#define __is_int(...) _Generic((__VA_ARGS__), \
    int: 1,     const int: 1, \
    int*: 1,    const int*: 1, \
    int**: 1,   const int**: 1, \
    int***: 1,  const int***: 1, \
    int****: 1, const int****: 1, \
    default: 0 )
#define __is_long(...) _Generic((__VA_ARGS__), \
    long: 1,     const long: 1, \
    long*: 1,    const long*: 1, \
    long**: 1,   const long**: 1, \
    long***: 1,  const long***: 1, \
    long****: 1, const long****: 1, \
    default: 0 )
#define __is_llong(...) _Generic((__VA_ARGS__), \
    long long: 1,     const long long: 1, \
    long long*: 1,    const long long*: 1, \
    long long**: 1,   const long long**: 1, \
    long long***: 1,  const long long***: 1, \
    long long****: 1, const long long****: 1, \
    default: 0 )
#define __is_double(...) _Generic((__VA_ARGS__), \
    double: 1,    const double: 1, \
    double*: 1,   const double*: 1, \
    double**: 1,  const double**: 1, \
    double***: 1, const double***: 1, \
    double****: 1, const double****: 1, \
    default: 0 )
#define __is_float(...) _Generic((__VA_ARGS__), \
    float: 1,    const float: 1, \
    float*: 1,   const float*: 1, \
    float**: 1,  const float**: 1, \
    float***: 1, const float***: 1, \
    float****: 1, const float****: 1, \
    default: 0 )
#define __is_ldouble(...) _Generic((__VA_ARGS__), \
    long double: 1,    const long double: 1, \
    long double*: 1,   const long double*: 1, \
    long double**: 1,  const long double**: 1, \
    long double***: 1, const long double***: 1, \
    long double****: 1, const long double****: 1, \
    default: 0 )
#define __is_schar(...) _Generic((__VA_ARGS__), \
    signed char: 1,     const signed char: 1, \
    signed char*: 1,    const signed char*: 1, \
    signed char**: 1,   const signed char**: 1, \
    signed char***: 1,  const signed char***: 1, \
    signed char****: 1, const signed char****: 1, \
    default: 0 )
#define __is_uchar(...) _Generic((__VA_ARGS__), \
    unsigned char: 1,     const unsigned char: 1, \
    unsigned char*: 1,    const unsigned char*: 1, \
    unsigned char**: 1,   const unsigned char**: 1, \
    unsigned char***: 1,  const unsigned char***: 1, \
    unsigned char****: 1, const unsigned char****: 1, \
    default: 0 )
#define __is_ushort(...) _Generic((__VA_ARGS__), \
    unsigned short: 1,     const unsigned short: 1, \
    unsigned short*: 1,    const unsigned short*: 1, \
    unsigned short**: 1,   const unsigned short**: 1, \
    unsigned short***: 1,  const unsigned short***: 1, \
    unsigned short****: 1, const unsigned short****: 1, \
    default: 0 )
#define __is_uint(...) _Generic((__VA_ARGS__), \
    unsigned int: 1,     const unsigned int: 1, \
    unsigned int*: 1,    const unsigned int*: 1, \
    unsigned int**: 1,   const unsigned int**: 1, \
    unsigned int***: 1,  const unsigned int***: 1, \
    unsigned int****: 1, const unsigned int****: 1, \
    default: 0 )
#define __is_ulong(...) _Generic((__VA_ARGS__), \
    unsigned long: 1,     const unsigned long: 1, \
    unsigned long*: 1,    const unsigned long*: 1, \
    unsigned long**: 1,   const unsigned long**: 1, \
    unsigned long***: 1,  const unsigned long***: 1, \
    unsigned long****: 1, const unsigned long****: 1, \
    default: 0 )
#define __is_ullong(...) _Generic((__VA_ARGS__), \
    unsigned long long: 1,     const unsigned long long: 1, \
    unsigned long long*: 1,    const unsigned long long*: 1, \
    unsigned long long**: 1,   const unsigned long long**: 1, \
    unsigned long long***: 1,  const unsigned long long***: 1, \
    unsigned long long****: 1, const unsigned long long****: 1, \
    default: 0 )
#define __pointer_depth(...) _Generic((__VA_ARGS__), \
    bool: 0,                const bool: 0, \
    char: 0,                const char: 0, \
    short: 0,               const short: 0, \
    int: 0,                 const int: 0, \
    long: 0,                const long: 0, \
    long long: 0,           const long long: 0, \
    double: 0,              const double: 0, \
    float: 0,               const float: 0, \
    long double: 0,         const long double: 0, \
    signed char: 0,         const signed char: 0, \
    unsigned char: 0,       const unsigned char: 0, \
    unsigned short: 0,      const unsigned short: 0, \
    unsigned int: 0,        const unsigned int: 0, \
    unsigned long: 0,       const unsigned long: 0, \
    unsigned long long: 0,  const unsigned long long: 0, \
    void*: 1,               const void*: 1, \
    bool*: 1,               const bool*: 1, \
    char*: 1,               const char*: 1, \
    short*: 1,              const short*: 1, \
    int*: 1,                const int*: 1, \
    long*: 1,               const long*: 1, \
    long long*: 1,          const long long*: 1, \
    double*: 1,             const double*: 1, \
    float*: 1,              const float*: 1, \
    long double*: 1,        const long double*: 1, \
    signed char*: 1,        const signed char*: 1, \
    unsigned char*: 1,      const unsigned char*: 1, \
    unsigned short*: 1,     const unsigned short*: 1, \
    unsigned int*: 1,       const unsigned int*: 1, \
    unsigned long*: 1,      const unsigned long*: 1, \
    unsigned long long*: 1, const unsigned long long*: 1, \
    void**: 2,               const void**: 2, \
    bool**: 2,               const bool**: 2, \
    char**: 2,               const char**: 2, \
    short**: 2,              const short**: 2, \
    int**: 2,                const int**: 2, \
    long**: 2,               const long**: 2, \
    long long**: 2,          const long long**: 2, \
    double**: 2,             const double**: 2, \
    float**: 2,              const float**: 2, \
    long double**: 2,        const long double**: 2, \
    signed char**: 2,        const signed char**: 2, \
    unsigned char**: 2,      const unsigned char**: 2, \
    unsigned short**: 2,     const unsigned short**: 2, \
    unsigned int**: 2,       const unsigned int**: 2, \
    unsigned long**: 2,      const unsigned long**: 2, \
    unsigned long long**: 2, const unsigned long long**: 2, \
    void***: 3,               const void***: 3, \
    bool***: 3,               const bool***: 3, \
    char***: 3,               const char***: 3, \
    short***: 3,              const short***: 3, \
    int***: 3,                const int***: 3, \
    long***: 3,               const long***: 3, \
    long long***: 3,          const long long***: 3, \
    double***: 3,             const double***: 3, \
    float***: 3,              const float***: 3, \
    long double***: 3,        const long double***: 3, \
    signed char***: 3,        const signed char***: 3, \
    unsigned char***: 3,      const unsigned char***: 3, \
    unsigned short***: 3,     const unsigned short***: 3, \
    unsigned int***: 3,       const unsigned int***: 3, \
    unsigned long***: 3,      const unsigned long***: 3, \
    unsigned long long***: 3, const unsigned long long***: 3, \
    void****: 4,               const void****: 4, \
    bool****: 4,               const bool****: 4, \
    char****: 4,               const char****: 4, \
    short****: 4,              const short****: 4, \
    int****: 4,                const int****: 4, \
    long****: 4,               const long****: 4, \
    long long****: 4,          const long long****: 4, \
    double****: 4,             const double****: 4, \
    float****: 4,              const float****: 4, \
    long double****: 4,        const long double****: 4, \
    signed char****: 4,        const signed char****: 4, \
    unsigned char****: 4,      const unsigned char****: 4, \
    unsigned short****: 4,     const unsigned short****: 4, \
    unsigned int****: 4,       const unsigned int****: 4, \
    unsigned long****: 4,      const unsigned long****: 4, \
    unsigned long long****: 4, const unsigned long long****: 4, \
    default: 0 )
#define __is_const(...) (_Generic((&__VA_ARGS__), \
    void*: 0,               const void*: 1, \
    bool*: 0,               const bool*: 1, \
    char*: 0,               const char*: 1, \
    short*: 0,              const short*: 1, \
    int*: 0,                const int*: 1, \
    long*: 0,               const long*: 1, \
    long long*: 0,          const long long*: 1, \
    double*: 0,             const double*: 1, \
    float*: 0,              const float*: 1, \
    long double*: 0,        const long double*: 1, \
    signed char*: 0,        const signed char*: 1, \
    unsigned char*: 0,      const unsigned char*: 1, \
    unsigned short*: 0,     const unsigned short*: 1, \
    unsigned int*: 0,       const unsigned int*: 1, \
    unsigned long*: 0,      const unsigned long*: 1, \
    unsigned long long*: 0, const unsigned long long*: 1, \
    default: 0 ) || _Generic((__VA_ARGS__), \
    void*: 0,               const void*: 1, \
    bool*: 0,               const bool*: 1, \
    char*: 0,               const char*: 1, \
    short*: 0,              const short*: 1, \
    int*: 0,                const int*: 1, \
    long*: 0,               const long*: 1, \
    long long*: 0,          const long long*: 1, \
    double*: 0,             const double*: 1, \
    float*: 0,              const float*: 1, \
    long double*: 0,        const long double*: 1, \
    signed char*: 0,        const signed char*: 1, \
    unsigned char*: 0,      const unsigned char*: 1, \
    unsigned short*: 0,     const unsigned short*: 1, \
    unsigned int*: 0,       const unsigned int*: 1, \
    unsigned long*: 0,      const unsigned long*: 1, \
    unsigned long long*: 0, const unsigned long long*: 1, \
    void**: 0,               const void**: 1, \
    bool**: 0,               const bool**: 1, \
    char**: 0,               const char**: 1, \
    short**: 0,              const short**: 1, \
    int**: 0,                const int**: 1, \
    long**: 0,               const long**: 1, \
    long long**: 0,          const long long**: 1, \
    double**: 0,             const double**: 1, \
    float**: 0,              const float**: 1, \
    long double**: 0,        const long double**: 1, \
    signed char**: 0,        const signed char**: 1, \
    unsigned char**: 0,      const unsigned char**: 1, \
    unsigned short**: 0,     const unsigned short**: 1, \
    unsigned int**: 0,       const unsigned int**: 1, \
    unsigned long**: 0,      const unsigned long**: 1, \
    unsigned long long**: 0, const unsigned long long**: 1, \
    void***: 0,               const void***: 1, \
    bool***: 0,               const bool***: 1, \
    char***: 0,               const char***: 1, \
    short***: 0,              const short***: 1, \
    int***: 0,                const int***: 1, \
    long***: 0,               const long***: 1, \
    long long***: 0,          const long long***: 1, \
    double***: 0,             const double***: 1, \
    float***: 0,              const float***: 1, \
    long double***: 0,        const long double***: 1, \
    signed char***: 0,        const signed char***: 1, \
    unsigned char***: 0,      const unsigned char***: 1, \
    unsigned short***: 0,     const unsigned short***: 1, \
    unsigned int***: 0,       const unsigned int***: 1, \
    unsigned long***: 0,      const unsigned long***: 1, \
    unsigned long long***: 0, const unsigned long long***: 1, \
    void****: 0,               const void****: 1, \
    bool****: 0,               const bool****: 1, \
    char****: 0,               const char****: 1, \
    short****: 0,              const short****: 1, \
    int****: 0,                const int****: 1, \
    long****: 0,               const long****: 1, \
    long long****: 0,          const long long****: 1, \
    double****: 0,             const double****: 1, \
    float****: 0,              const float****: 1, \
    long double****: 0,        const long double****: 1, \
    signed char****: 0,        const signed char****: 1, \
    unsigned char****: 0,      const unsigned char****: 1, \
    unsigned short****: 0,     const unsigned short****: 1, \
    unsigned int****: 0,       const unsigned int****: 1, \
    unsigned long****: 0,      const unsigned long****: 1, \
    unsigned long long****: 0, const unsigned long long****: 1, \
    default: 0 ))
#define __is_const_pointer(...) _Generic((&__VA_ARGS__), \
    void*: 0,               const void*: 0, \
    bool*: 0,               const bool*: 0, \
    char*: 0,               const char*: 0, \
    short*: 0,              const short*: 0, \
    int*: 0,                const int*: 0, \
    long*: 0,               const long*: 0, \
    long long*: 0,          const long long*: 0, \
    double*: 0,             const double*: 0, \
    float*: 0,              const float*: 0, \
    long double*: 0,        const long double*: 0, \
    signed char*: 0,        const signed char*: 0, \
    unsigned char*: 0,      const unsigned char*: 0, \
    unsigned short*: 0,     const unsigned short*: 0, \
    unsigned int*: 0,       const unsigned int*: 0, \
    unsigned long*: 0,      const unsigned long*: 0, \
    unsigned long long*: 0, const unsigned long long*: 0, \
    void**: 0,               const void**: 0,               void* const*: 1,               const void* const*: 1, \
    bool**: 0,               const bool**: 0,               bool* const*: 1,               const bool* const*: 1, \
    char**: 0,               const char**: 0,               char* const*: 1,               const char* const*: 1, \
    short**: 0,              const short**: 0,              short* const*: 1,              const short* const*: 1, \
    int**: 0,                const int**: 0,                int* const*: 1,                const int* const*: 1, \
    long**: 0,               const long**: 0,               long* const*: 1,               const long* const*: 1, \
    long long**: 0,          const long long**: 0,          long long* const*: 1,          const long long* const*: 1, \
    double**: 0,             const double**: 0,             double* const*: 1,             const double* const*: 1, \
    float**: 0,              const float**: 0,              float* const*: 1,              const float* const*: 1, \
    long double**: 0,        const long double**: 0,        long double* const*: 1,        const long double* const*: 1, \
    signed char**: 0,        const signed char**: 0,        signed char* const*: 1,        const signed char* const*: 1, \
    unsigned char**: 0,      const unsigned char**: 0,      unsigned char* const*: 1,      const unsigned char* const*: 1, \
    unsigned short**: 0,     const unsigned short**: 0,     unsigned short* const*: 1,     const unsigned short* const*: 1, \
    unsigned int**: 0,       const unsigned int**: 0,       unsigned int* const*: 1,       const unsigned int* const*: 1, \
    unsigned long**: 0,      const unsigned long**: 0,      unsigned long* const*: 1,      const unsigned long* const*: 1, \
    unsigned long long**: 0, const unsigned long long**: 0, unsigned long long* const*: 1, const unsigned long long* const*: 1, \
    void***: 0,               const void***: 0,               void** const*: 1,               const void** const*: 1, \
    bool***: 0,               const bool***: 0,               bool** const*: 1,               const bool** const*: 1, \
    char***: 0,               const char***: 0,               char** const*: 1,               const char** const*: 1, \
    short***: 0,              const short***: 0,              short** const*: 1,              const short** const*: 1, \
    int***: 0,                const int***: 0,                int** const*: 1,                const int** const*: 1, \
    long***: 0,               const long***: 0,               long** const*: 1,               const long** const*: 1, \
    long long***: 0,          const long long***: 0,          long long** const*: 1,          const long long** const*: 1, \
    double***: 0,             const double***: 0,             double** const*: 1,             const double** const*: 1, \
    float***: 0,              const float***: 0,              float** const*: 1,              const float** const*: 1, \
    long double***: 0,        const long double***: 0,        long double** const*: 1,        const long double** const*: 1, \
    signed char***: 0,        const signed char***: 0,        signed char** const*: 1,        const signed char** const*: 1, \
    unsigned char***: 0,      const unsigned char***: 0,      unsigned char** const*: 1,      const unsigned char** const*: 1, \
    unsigned short***: 0,     const unsigned short***: 0,     unsigned short** const*: 1,     const unsigned short** const*: 1, \
    unsigned int***: 0,       const unsigned int***: 0,       unsigned int** const*: 1,       const unsigned int** const*: 1, \
    unsigned long***: 0,      const unsigned long***: 0,      unsigned long** const*: 1,      const unsigned long** const*: 1, \
    unsigned long long***: 0, const unsigned long long***: 0, unsigned long long** const*: 1, const unsigned long long** const*: 1, \
    void****: 0,               const void****: 0,               void*** const*: 1,               const void*** const*: 1, \
    bool****: 0,               const bool****: 0,               bool*** const*: 1,               const bool*** const*: 1, \
    char****: 0,               const char****: 0,               char*** const*: 1,               const char*** const*: 1, \
    short****: 0,              const short****: 0,              short*** const*: 1,              const short*** const*: 1, \
    int****: 0,                const int****: 0,                int*** const*: 1,                const int*** const*: 1, \
    long****: 0,               const long****: 0,               long*** const*: 1,               const long*** const*: 1, \
    long long****: 0,          const long long****: 0,          long long*** const*: 1,          const long long*** const*: 1, \
    double****: 0,             const double****: 0,             double*** const*: 1,             const double*** const*: 1, \
    float****: 0,              const float****: 0,              float*** const*: 1,              const float*** const*: 1, \
    long double****: 0,        const long double****: 0,        long double*** const*: 1,        const long double*** const*: 1, \
    signed char****: 0,        const signed char****: 0,        signed char*** const*: 1,        const signed char*** const*: 1, \
    unsigned char****: 0,      const unsigned char****: 0,      unsigned char*** const*: 1,      const unsigned char*** const*: 1, \
    unsigned short****: 0,     const unsigned short****: 0,     unsigned short*** const*: 1,     const unsigned short*** const*: 1, \
    unsigned int****: 0,       const unsigned int****: 0,       unsigned int*** const*: 1,       const unsigned int*** const*: 1, \
    unsigned long****: 0,      const unsigned long****: 0,      unsigned long*** const*: 1,      const unsigned long*** const*: 1, \
    unsigned long long****: 0, const unsigned long long****: 0, unsigned long long*** const*: 1, const unsigned long long*** const*: 1, \
    void*****: 0,               const void*****: 0,               void**** const*: 1,               const void**** const*: 1, \
    bool*****: 0,               const bool*****: 0,               bool**** const*: 1,               const bool**** const*: 1, \
    char*****: 0,               const char*****: 0,               char**** const*: 1,               const char**** const*: 1, \
    short*****: 0,              const short*****: 0,              short**** const*: 1,              const short**** const*: 1, \
    int*****: 0,                const int*****: 0,                int**** const*: 1,                const int**** const*: 1, \
    long*****: 0,               const long*****: 0,               long**** const*: 1,               const long**** const*: 1, \
    long long*****: 0,          const long long*****: 0,          long long**** const*: 1,          const long long**** const*: 1, \
    double*****: 0,             const double*****: 0,             double**** const*: 1,             const double**** const*: 1, \
    float*****: 0,              const float*****: 0,              float**** const*: 1,              const float**** const*: 1, \
    long double*****: 0,        const long double*****: 0,        long double**** const*: 1,        const long double**** const*: 1, \
    signed char*****: 0,        const signed char*****: 0,        signed char**** const*: 1,        const signed char**** const*: 1, \
    unsigned char*****: 0,      const unsigned char*****: 0,      unsigned char**** const*: 1,      const unsigned char**** const*: 1, \
    unsigned short*****: 0,     const unsigned short*****: 0,     unsigned short**** const*: 1,     const unsigned short**** const*: 1, \
    unsigned int*****: 0,       const unsigned int*****: 0,       unsigned int**** const*: 1,       const unsigned int**** const*: 1, \
    unsigned long*****: 0,      const unsigned long*****: 0,      unsigned long**** const*: 1,      const unsigned long**** const*: 1, \
    unsigned long long*****: 0, const unsigned long long*****: 0, unsigned long long**** const*: 1, const unsigned long long**** const*: 1, \
default: 0 )
#define __sizeof(...) _Generic((__VA_ARGS__), \
    bool: sizeof(bool),                             const bool: sizeof(bool), \
    char: sizeof(char),                             const char: sizeof(char), \
    short: sizeof(short),                           const short: sizeof(short), \
    int: sizeof(int),                               const int: sizeof(int), \
    long: sizeof(long),                             const long: sizeof(long), \
    long long: sizeof(long long),                   const long long: sizeof(long long), \
    double: sizeof(double),                         const double: sizeof(double), \
    float: sizeof(float),                           const float: sizeof(float), \
    long double: sizeof(long double),               const long double: sizeof(long double), \
    signed char: sizeof(signed char),               const signed char: sizeof(signed char), \
    unsigned char: sizeof(unsigned char),           const unsigned char: sizeof(unsigned char), \
    unsigned short: sizeof(unsigned short),         const unsigned short: sizeof(unsigned short), \
    unsigned int: sizeof(unsigned int),             const unsigned int: sizeof(unsigned int), \
    unsigned long: sizeof(unsigned long),           const unsigned long: sizeof(unsigned long), \
    unsigned long long: sizeof(unsigned long long), const unsigned long long: sizeof(unsigned long long), \
    void*: 0,                                        const void*: 0, \
    bool*: sizeof(bool),                             const bool*: sizeof(bool), \
    char*: sizeof(char),                             const char*: sizeof(char), \
    short*: sizeof(short),                           const short*: sizeof(short), \
    int*: sizeof(int),                               const int*: sizeof(int), \
    long*: sizeof(long),                             const long*: sizeof(long), \
    long long*: sizeof(long long),                   const long long*: sizeof(long long), \
    double*: sizeof(double),                         const double*: sizeof(double), \
    float*: sizeof(float),                           const float*: sizeof(float), \
    long double*: sizeof(long double),               const long double*: sizeof(long double), \
    signed char*: sizeof(signed char),               const signed char*: sizeof(signed char), \
    unsigned char*: sizeof(unsigned char),           const unsigned char*: sizeof(unsigned char), \
    unsigned short*: sizeof(unsigned short),         const unsigned short*: sizeof(unsigned short), \
    unsigned int*: sizeof(unsigned int),             const unsigned int*: sizeof(unsigned int), \
    unsigned long*: sizeof(unsigned long),           const unsigned long*: sizeof(unsigned long), \
    unsigned long long*: sizeof(unsigned long long), const unsigned long long*: sizeof(unsigned long long), \
    void**: 0,                                        const void**: 0, \
    bool**: sizeof(bool),                             const bool**: sizeof(bool), \
    char**: sizeof(char),                             const char**: sizeof(char), \
    short**: sizeof(short),                           const short**: sizeof(short), \
    int**: sizeof(int),                               const int**: sizeof(int), \
    long**: sizeof(long),                             const long**: sizeof(long), \
    long long**: sizeof(long long),                   const long long**: sizeof(long long), \
    double**: sizeof(double),                         const double**: sizeof(double), \
    float**: sizeof(float),                           const float**: sizeof(float), \
    long double**: sizeof(long double),               const long double**: sizeof(long double), \
    signed char**: sizeof(signed char),               const signed char**: sizeof(signed char), \
    unsigned char**: sizeof(unsigned char),           const unsigned char**: sizeof(unsigned char), \
    unsigned short**: sizeof(unsigned short),         const unsigned short**: sizeof(unsigned short), \
    unsigned int**: sizeof(unsigned int),             const unsigned int**: sizeof(unsigned int), \
    unsigned long**: sizeof(unsigned long),           const unsigned long**: sizeof(unsigned long), \
    unsigned long long**: sizeof(unsigned long long), const unsigned long long**: sizeof(unsigned long long), \
    void***: 0,                                        const void***: 0, \
    bool***: sizeof(bool),                             const bool***: sizeof(bool), \
    char***: sizeof(char),                             const char***: sizeof(char), \
    short***: sizeof(short),                           const short***: sizeof(short), \
    int***: sizeof(int),                               const int***: sizeof(int), \
    long***: sizeof(long),                             const long***: sizeof(long), \
    long long***: sizeof(long long),                   const long long***: sizeof(long long), \
    double***: sizeof(double),                         const double***: sizeof(double), \
    float***: sizeof(float),                           const float***: sizeof(float), \
    long double***: sizeof(long double),               const long double***: sizeof(long double), \
    signed char***: sizeof(signed char),               const signed char***: sizeof(signed char), \
    unsigned char***: sizeof(unsigned char),           const unsigned char***: sizeof(unsigned char), \
    unsigned short***: sizeof(unsigned short),         const unsigned short***: sizeof(unsigned short), \
    unsigned int***: sizeof(unsigned int),             const unsigned int***: sizeof(unsigned int), \
    unsigned long***: sizeof(unsigned long),           const unsigned long***: sizeof(unsigned long), \
    unsigned long long***: sizeof(unsigned long long), const unsigned long long***: sizeof(unsigned long long), \
    void****: 0,                                        const void****: 0, \
    bool****: sizeof(bool),                             const bool****: sizeof(bool), \
    char****: sizeof(char),                             const char****: sizeof(char), \
    short****: sizeof(short),                           const short****: sizeof(short), \
    int****: sizeof(int),                               const int****: sizeof(int), \
    long****: sizeof(long),                             const long****: sizeof(long), \
    long long****: sizeof(long long),                   const long long****: sizeof(long long), \
    double****: sizeof(double),                         const double****: sizeof(double), \
    float****: sizeof(float),                           const float****: sizeof(float), \
    long double****: sizeof(long double),               const long double****: sizeof(long double), \
    signed char****: sizeof(signed char),               const signed char****: sizeof(signed char), \
    unsigned char****: sizeof(unsigned char),           const unsigned char****: sizeof(unsigned char), \
    unsigned short****: sizeof(unsigned short),         const unsigned short****: sizeof(unsigned short), \
    unsigned int****: sizeof(unsigned int),             const unsigned int****: sizeof(unsigned int), \
    unsigned long****: sizeof(unsigned long),           const unsigned long****: sizeof(unsigned long), \
    unsigned long long****: sizeof(unsigned long long), const unsigned long long****: sizeof(unsigned long long), \
default: sizeof(__VA_ARGS__) )

#define __type_num(...) _Generic((__VA_ARGS__), \
    bool: 1,                 const bool: 2, \
    char: 3,                 const char: 4, \
    short: 5,                const short: 6, \
    int: 7,                  const int: 8, \
    long: 9,                 const long: 10, \
    long long: 11,           const long long: 12, \
    double: 13,              const double: 14, \
    float: 15,               const float: 16, \
    long double: 17,         const long double: 18, \
    signed char: 19,         const signed char: 20, \
    unsigned char: 21,       const unsigned char: 22, \
    unsigned short: 23,      const unsigned short: 24, \
    unsigned int: 25,        const unsigned int: 26, \
    unsigned long: 27,       const unsigned long: 28, \
    unsigned long long: 29,  const unsigned long long: 30, \
    void*: 31,               const void*: 32,               void* const: 33,               const void* const: 34, \
    bool*: 35,               const bool*: 36,               bool* const: 37,               const bool* const: 38, \
    char*: 39,               const char*: 40,               char* const: 41,               const char* const: 42, \
    short*: 43,              const short*: 44,              short* const: 45,              const short* const: 46, \
    int*: 47,                const int*: 48,                int* const: 49,                const int* const: 50, \
    long*: 51,               const long*: 52,               long* const: 53,               const long* const: 54, \
    long long*: 55,          const long long*: 56,          long long* const: 57,          const long long* const: 58, \
    double*: 59,             const double*: 60,             double* const: 61,             const double* const: 62, \
    float*: 63,              const float*: 64,              float* const: 65,              const float* const: 66, \
    long double*: 67,        const long double*: 68,        long double* const: 69,        const long double* const: 70, \
    signed char*: 71,        const signed char*: 72,        signed char* const: 73,        const signed char* const: 74, \
    unsigned char*: 75,      const unsigned char*: 76,      unsigned char* const: 77,      const unsigned char* const: 78, \
    unsigned short*: 79,     const unsigned short*: 80,     unsigned short* const: 81,     const unsigned short* const: 82, \
    unsigned int*: 83,       const unsigned int*: 84,       unsigned int* const: 85,       const unsigned int* const: 86, \
    unsigned long*: 87,      const unsigned long*: 88,      unsigned long* const: 89,      const unsigned long* const: 90, \
    unsigned long long*: 91, const unsigned long long*: 92, unsigned long long* const: 93, const unsigned long long* const: 94, \
    void**: 95,                const void**: 96,                void** const: 97,                const void** const: 98, \
    bool**: 99,                const bool**: 100,               bool** const: 101,               const bool** const: 102, \
    char**: 103,               const char**: 104,               char** const: 105,               const char** const: 106, \
    short**: 107,              const short**: 108,              short** const: 109,              const short** const: 110, \
    int**: 111,                const int**: 112,                int** const: 113,                const int** const: 114, \
    long**: 115,               const long**: 116,               long** const: 117,               const long** const: 118, \
    long long**: 119,          const long long**: 120,          long long** const: 121,          const long long** const: 122, \
    double**: 123,             const double**: 124,             double** const: 125,             const double** const: 126, \
    float**: 127,              const float**: 128,              float** const: 129,              const float** const: 130, \
    long double**: 131,        const long double**: 132,        long double** const: 133,        const long double** const: 134, \
    signed char**: 135,        const signed char**: 136,        signed char** const: 137,        const signed char** const: 138, \
    unsigned char**: 139,      const unsigned char**: 140,      unsigned char** const: 141,      const unsigned char** const: 142, \
    unsigned short**: 143,     const unsigned short**: 144,     unsigned short** const: 145,     const unsigned short** const: 146, \
    unsigned int**: 147,       const unsigned int**: 148,       unsigned int** const: 149,       const unsigned int** const: 150, \
    unsigned long**: 151,      const unsigned long**: 152,      unsigned long** const: 153,      const unsigned long** const: 154, \
    unsigned long long**: 155, const unsigned long long**: 156, unsigned long long** const: 157, const unsigned long long** const: 158, \
    void***: 159,               const void***: 160,               void*** const: 161,               const void*** const: 162, \
    bool***: 163,               const bool***: 164,               bool*** const: 165,               const bool*** const: 166, \
    char***: 167,               const char***: 168,               char*** const: 169,               const char*** const: 170, \
    short***: 171,              const short***: 172,              short*** const: 173,              const short*** const: 174, \
    int***: 175,                const int***: 176,                int*** const: 177,                const int*** const: 178, \
    long***: 179,               const long***: 180,               long*** const: 181,               const long*** const: 182, \
    long long***: 183,          const long long***: 184,          long long*** const: 185,          const long long*** const: 186, \
    double***: 187,             const double***: 188,             double*** const: 189,             const double*** const: 190, \
    float***: 191,              const float***: 192,              float*** const: 193,              const float*** const: 194, \
    long double***: 195,        const long double***: 196,        long double*** const: 197,        const long double*** const: 198, \
    signed char***: 199,        const signed char***: 200,        signed char*** const: 201,        const signed char*** const: 202, \
    unsigned char***: 203,      const unsigned char***: 204,      unsigned char*** const: 205,      const unsigned char*** const: 206, \
    unsigned short***: 207,     const unsigned short***: 208,     unsigned short*** const: 209,     const unsigned short*** const: 210, \
    unsigned int***: 211,       const unsigned int***: 212,       unsigned int*** const: 213,       const unsigned int*** const: 214, \
    unsigned long***: 215,      const unsigned long***: 216,      unsigned long*** const: 217,      const unsigned long*** const: 218, \
    unsigned long long***: 219, const unsigned long long***: 220, unsigned long long*** const: 221, const unsigned long long*** const: 222, \
    void****: 223,               const void****: 224,               void**** const: 225,               const void**** const: 226, \
    bool****: 227,               const bool****: 228,               bool**** const: 229,               const bool**** const: 230, \
    char****: 231,               const char****: 232,               char**** const: 233,               const char**** const: 234, \
    short****: 235,              const short****: 236,              short**** const: 237,              const short**** const: 238, \
    int****: 239,                const int****: 240,                int**** const: 241,                const int**** const: 242, \
    long****: 243,               const long****: 244,               long**** const: 245,               const long**** const: 246, \
    long long****: 247,          const long long****: 248,          long long**** const: 249,          const long long**** const: 250, \
    double****: 251,             const double****: 252,             double**** const: 253,             const double**** const: 254, \
    float****: 255,              const float****: 256,              float**** const: 257,              const float**** const: 258, \
    long double****: 259,        const long double****: 260,        long double**** const: 261,        const long double**** const: 262, \
    signed char****: 263,        const signed char****: 264,        signed char**** const: 265,        const signed char**** const: 266, \
    unsigned char****: 267,      const unsigned char****: 268,      unsigned char**** const: 269,      const unsigned char**** const: 270, \
    unsigned short****: 271,     const unsigned short****: 272,     unsigned short**** const: 273,     const unsigned short**** const: 274, \
    unsigned int****: 275,       const unsigned int****: 276,       unsigned int**** const: 277,       const unsigned int**** const: 278, \
    unsigned long****: 279,      const unsigned long****: 280,      unsigned long**** const: 281,      const unsigned long**** const: 282, \
    unsigned long long****: 283, const unsigned long long****: 284, unsigned long long**** const: 285, const unsigned long long**** const: 286, \
    default: 0 )

#define printtype(...) printf("The type of the variable \"%s\": %s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s\n", #__VA_ARGS__, \
    (__is_const(__VA_ARGS__)? "const " : ""), \
    (__is_void(__VA_ARGS__)? "void" : ""), \
    (__is_bool(__VA_ARGS__)? "bool" : ""), \
    (__is_char(__VA_ARGS__)? "char" : ""), \
    (__is_short(__VA_ARGS__)? "short" : ""), \
    (__is_int(__VA_ARGS__)? "int" : ""), \
    (__is_long(__VA_ARGS__)? "long" : ""), \
    (__is_llong(__VA_ARGS__)? "long long" : ""), \
    (__is_double(__VA_ARGS__)? "double" : ""), \
    (__is_float(__VA_ARGS__)? "float" : ""), \
    (__is_ldouble(__VA_ARGS__)? "long double" : ""), \
    (__is_schar(__VA_ARGS__)? "signed char" : ""), \
    (__is_uchar(__VA_ARGS__)? "unsigned char" : ""), \
    (__is_ushort(__VA_ARGS__)? "unsigned short" : ""), \
    (__is_uint(__VA_ARGS__)? "unsigned int" : ""), \
    (__is_ulong(__VA_ARGS__)? "unsigned long" : ""), \
    (__is_ullong(__VA_ARGS__)? "unsigned long long" : ""), \
    (__pointer_depth(__VA_ARGS__) == 1? " *" : (__pointer_depth(__VA_ARGS__) == 2? " **" : (__pointer_depth(__VA_ARGS__) == 3? " ***" : ""))), \
    (__is_const_pointer(__VA_ARGS__)? " const" : ""))

#ifndef alloc_sizeof
    #if defined(__linux__)
        #include <malloc.h>
        #define alloc_sizeof(p) malloc_usable_size((void*)p)
    #elif defined(__APPLE__) // what about unix in general..?
        #include <malloc/malloc.h>
        #define alloc_sizeof(p) malloc_size(p)
    #elif defined(_WIN32)
        #include <malloc.h>
        #define alloc_sizeof(p) _msize((void *)p)
    #else
        #define alloc_sizeof(p) 0
    #endif
#endif
#define __fixed_arraysize(x) ((size_t)sizeof(x) / (size_t)sizeof(x[0]))
#define __is_allocated(x) (alloc_sizeof(x) > 0)
#define __is_fixed_array(x)  ((size_t)sizeof(x) != (size_t)sizeof(void*))
// Node: the checking method of __is_fixed_array(x) CONFUSES the cases of fixed 8-byte arrays like char[8], short[4], and int[2],
// because it is ambiguous that 8 bytes are the same size of one pointer variable.
// But, you must already know what kind of 8-byte array you have!

typedef struct __allocated_record {
    const char* name;
    enum methods: unsigned char { dynamic, allocated, fixed } method;
    unsigned char pointer_depth;
    size_t typesize;
    size_t totalsize;
    size_t arraysize;
} allocated_record;
#define __make_allocated_record(...) ((allocated_record){ \
    #__VA_ARGS__, \
    (__is_allocated(__VA_ARGS__) ? allocated : (__is_fixed_array(__VA_ARGS__) ? fixed : dynamic)), \
    __pointer_depth(__VA_ARGS__), \
    (__is_fixed_array(__VA_ARGS__) ? sizeof(*__VA_ARGS__) : __sizeof(*__VA_ARGS__)), \
    (__is_fixed_array(__VA_ARGS__) ? sizeof(__VA_ARGS__) : alloc_sizeof(__VA_ARGS__)), \
    (__is_fixed_array(__VA_ARGS__) ? __fixed_arraysize(__VA_ARGS__) : alloc_sizeof(__VA_ARGS__) / __sizeof(*__VA_ARGS__)) })
#define allocated_info(...) __make_allocated_record(__VA_ARGS__)

#endif
