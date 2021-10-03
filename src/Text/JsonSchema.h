#pragma once

#include <stdint.h>

#define JsonSchema_DefineMember(Type, member, memberSchema) \
    (void)sizeof(memberSchema); \
    JsonSchema_DefineMemberRaw(#Type, offsetof(Type, member), #member, #memberSchema)

#define JsonSchema_JsonSchema_DefineMemberEx(Type, member, memberName, memberSchema) \
    (void)sizeof(memberSchema); \
    JsonSchema_DefineMemberRaw(#Type, offsetof(Type, member), memberName, #memberSchema)

#define JsonSchema_JsonSchema_DefineMemberEx2(typeName, Type, member, memberName, memberSchema) \
    (void)sizeof(memberSchema); \
    JsonSchema_DefineMemberRaw(typeName, offsetof(Type, member), memberName, #memberSchema)

typedef void    (JsonSchemaReadFn)(void* data, void* dst, int32_t offset, const struct Json* json);
typedef void    (JsonSchemaWriteFn)(void* data, const void* src, int32_t offset, struct Json* json);

typedef struct JsonSchemaMember
{
    void*               data;
    int32_t             offset;

    JsonSchemaReadFn*   read;
    JsonSchemaWriteFn*  write;
} JsonSchemaMember;

typedef struct JsonSchema
{
    int32_t          memberCount;
    JsonSchemaMember members[32];
} JsonSchema;

void JsonSchema_Define(const char* typeName, void* data, JsonSchemaReadFn readFn, JsonSchemaWriteFn writeFn);
void JsonSchema_DefineMemberRaw(const char* typeName, int32_t offset, const char* memberName, const char* memberSchema);

void JsonScheme_Serialize(const char* typeName, const void* src, const struct Json* json);
void JsonSchema_Deserialize(const char* typeName, void* dst, const struct Json* json);
