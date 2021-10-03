#include "../test_framework.h"

#include "Text/Json.h"
#include "Text/JsonSchema.h"

DEFINE_UNIT_TEST("Json unit tests: object")
{
    const char jsonCode[] = "{}";
    Json* json = Json_Parse(jsonCode, sizeof(jsonCode) - 1, JsonFlags_None);
    TEST(json != nullptr && json->type == JsonType_Object && Json_GetError(json) == JsonError_None);
    
    Json_Destroy(json);
}

DEFINE_UNIT_TEST("Json unit tests: array")
{
    const char jsonCode[] = "[]";
    Json* json = Json_Parse(jsonCode, sizeof(jsonCode) - 1, JsonFlags_None);
    TEST(json != nullptr && json->type == JsonType_Array && Json_GetError(json) == JsonError_None);

    Json_Destroy(json);
}

DEFINE_UNIT_TEST("Json unit tests: error")
{
    const char jsonCode[] = "";
    Json* json = Json_Parse(jsonCode, sizeof(jsonCode) - 1, JsonFlags_None);
    TEST(json == nullptr);

    Json_Destroy(json);
}

DEFINE_UNIT_TEST("JsonSchema unit tests")
{
#if 0
    JsonSchema_DefineMember(vec2, x, float);
    JsonSchema_DefineMember(vec2, y, float);

    const char jsonCode[] = "{\"x\": 1.0, \"y\": 2.0}";
    Json* json = Json_Parse(jsonCode, sizeof(jsonCode) - 1, JsonFlags_None);
    TEST(json != nullptr && json->type == JsonType_Object && Json_GetError(json) == JsonError_None);

    vec2 value;
    JsonSchema_Deserialize("vec2", &value, json);

    Json_Destroy(json);
#endif
}
