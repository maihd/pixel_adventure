#pragma once

// -------------------------------------------------------------
// Function call conventions
// -------------------------------------------------------------

#if defined(__GNUC__)
#   define __vectorcall  /* NO VECTORCALL SUPPORTED */
#   define __forceinline static __attribute__((always_inline))
#endif

#if !defined(__cplusplus) && !defined(constexpr)
#define constexpr static const
#endif

#if !defined(__cplusplus) && !defined(nullptr)
#define nullptr ((void*)0)
#endif

#if !defined(__deprecated)
#if defined(_MSC_VER)
#define __deprecated(msg) __declspec(deprecated)
#else
#define __deprecated(msg) __attribute__((deprecated("" msg)))
#endif
#endif

#if !defined(__enum_type)
#if defined(__cplusplus)
#define __enum_type(T) : T
#else
#define __enum_type(T)
#endif
#endif

#if !defined(__name_of)
#define __name_of(T) (__name_of_impl(), __typename(T))
__deprecated("Use __typename instead!")
static inline void __name_of_impl(void){}
#endif

#if !defined(__default_init)
#ifdef __cplusplus
#define __default_init(...) = ##__VA_ARGS__
#else
#define __default_init(...)
#endif
#endif

/// __typename
/// @spoiler: long definition
#if !defined(__typename)
#if !defined(__cplusplus) || defined(USE_FAST_TYPENAME)
#define __typename(T) #T
#else
#define __typename(T) __typename_impl<T>()

template <typename T>
constexpr auto __typename_GetTypeNameHelper();

template <typename T>
constexpr const char* __typename_impl(void)
{
    static auto string = __typename_GetTypeNameHelper<T>();
    return string.data;
}

template <typename T>
constexpr auto __typename_GetTypeNameHelper()
{
    #if defined(_MSC_VER)
    #define __FUNC__ __FUNCSIG__
    #else
    #define __FUNC__ __PRETTY_FUNCTION__
    #endif

    int head = 0;
    int tail = 0;
    for (int i = 0; i < sizeof(__FUNC__); i++)
    {
        if (__FUNC__[i] == '<')
        {
            if (head == 0)
            {
                head = i + 1;
            }
        }
        else if (__FUNC__[i] == '>')
        {
            tail = i - 1;
        }
    }
    
    /// Result of __typename
    /// Subset typeof StringBuffer
    /// @note: need manually change when StringBuffer changed
    struct Result 
    {
        int     memtag                  = 0x2e9497f4U;      // WEAK tag
        int     memref                  = 0;
        int     length                  = 0;
        int     capacity                = sizeof(__FUNC__);        
        char    data[sizeof(__FUNC__)]  = "";
    };

    Result result;
    
    int index = head;
    int& length = result.length;
    char* typeName = result.data;
    const char* funcName = __FUNC__ + head;
    while (*funcName != '\0' && index <= tail)
    {
        // Skip 'struct' modifier
        if (true
            && funcName[0] == 's'
            && funcName[1] == 't'
            && funcName[2] == 'r'
            && funcName[3] == 'u'
            && funcName[4] == 'c'
            && funcName[5] == 't'
            && funcName[6] == ' ')
        {
            funcName += 7;
            index += 7;
            continue;
        }

        // Skip 'class' modifier
        if (true
            && funcName[0] == 'c'
            && funcName[1] == 'l'
            && funcName[2] == 'a'
            && funcName[3] == 's'
            && funcName[4] == 's'
            && funcName[5] == ' ')
        {
            funcName += 6;
            index += 6;
            continue;
        }

        // Skip 'union' modifier
        if (true
            && funcName[0] == 'u'
            && funcName[1] == 'n'
            && funcName[2] == 'i'
            && funcName[3] == 'o'
            && funcName[4] == 'n'
            && funcName[5] == ' ')
        {
            funcName += 6;
            index += 6;
            continue;
        }

        // Skip 'enum' modifier
        if (true
            && funcName[0] == 'e'
            && funcName[1] == 'n'
            && funcName[2] == 'u'
            && funcName[3] == 'm'
            && funcName[4] == ' ')
        {
            funcName += 5;
            index += 5;
            continue;
        }

        *typeName++ = *funcName++;
        length++;    
        index++;
    }

    #undef __FUNC__ // Release __FUNC__ to other usages
    return result;
}
#endif
#endif

// assert_type_is_memcopyable();
// assert_type_is_trivial();
// assert_type_is_pod();
// assert_type_size();
// assert_type_alignment();
// assert_not_null();
// assert_in_range();
// assert_enough_memory();

//! LEAVE AN EMPTY LINE HERE, REQUIRE BY GCC/G++
