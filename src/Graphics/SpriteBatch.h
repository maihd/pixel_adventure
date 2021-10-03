#pragma once

#include "Math/VectorTypes.h"

struct SpriteSheet;

enum struct SpriteBatchState : uint32_t
{
    Idle,
    Batching,
};

struct SpriteBatch
{
    SpriteBatchState    state;
    
    uint32_t            textureId;
    uint32_t            vertexArrayId;
    uint32_t            verticesBufferId;
    uint32_t            uvsBufferId;
    uint32_t            colorsBufferId;
    
    int32_t             count;
    int32_t             capacity;

    vec2*               vertices;
    vec2*               uvs;
    vec3*               colors;
};

#ifdef __cplusplus
extern "C" {
#endif

void    SpriteBatch_Create(SpriteBatch* spriteBatch, const SpriteSheet* sheet, int32_t vertexCapacity);
void    SpriteBatch_Destroy(SpriteBatch* spriteBatch);

void    SpriteBatch_Begin(SpriteBatch* spriteBatch);
void    SpriteBatch_End(SpriteBatch* spriteBatch);

void    SpriteBatch_DrawSprite(SpriteBatch* spriteBatch, const Sprite* sprite, vec2 position, float rotation, vec2 scale, vec3 color);

#ifdef __cplusplus
}
#endif
