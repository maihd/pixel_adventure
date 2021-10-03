#pragma once

/* START OF EXTERN "C" */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// Define boolean type if needed
#if !defined(__cplusplus)
#include <stdbool.h>
#endif

/// JSON type of json value
typedef enum JsonType
{
    JsonType_Null,
    JsonType_Array,
    JsonType_Object,
    JsonType_Number,
    JsonType_String,
    JsonType_Boolean,
} JsonType;

/// JSON error code
typedef enum JsonError
{
    JsonError_None,

    /* Parsing error */

    JsonError_WrongFormat,
    JsonError_UnmatchToken,
    JsonError_UnknownToken,
    JsonError_UnexpectedToken,
    JsonError_UnsupportedToken,

    /* Runtime error */

    JsonError_OutOfMemory,
    JsonError_InvalidValue,
    JsonError_InternalFatal,

} JsonError;

/// Json parse flags
typedef enum JsonFlags
{
    JsonFlags_None              = 0,
    JsonFlags_SupportComment    = 1 << 0,
} JsonFlags;

typedef struct Json             Json;
typedef struct JsonAllocator    JsonAllocator;
typedef struct JsonObjectEntry  JsonObjectEntry;

Json*           Json_Parse(const char* jsonCode, int32_t jsonCodeLength, JsonFlags flags);
Json*           Json_ParseEx(const char* jsonCode, int32_t jsonCodeLength, JsonAllocator allocator, JsonFlags flags);

void            Json_Destroy(Json* rootValue);

JsonError       Json_GetError(const Json* rootValue);
const char*     Json_GetErrorMessage(const Json* rootValue);

bool            Json_Equals(const Json* a, const Json* b);

const Json*     Json_Find(const Json* x, const char* name);

struct Json
{
    JsonType                type;                      /* Type of value: number, boolean, string, array, object    */
    int32_t                 length;                    /* Length of value, always 1 on primitive types             */
    union
    {
        double              number;
        bool                boolean;

        const char*         string;

        Json*               array;

        JsonObjectEntry*    object;
    };
};

struct JsonObjectEntry
{
    const char*             name;
    Json                    value;
};

struct JsonAllocator
{
    void*                   data;
    void                    (*free)(void* data, void* ptr);
    void*                   (*alloc)(void* data, int size);
    //void* (*realloc)(void* data, void* ptr, int size);
};

/* END OF EXTERN "C" */
#ifdef __cplusplus
}
#endif
/* * */
