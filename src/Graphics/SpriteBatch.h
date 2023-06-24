#pragma once

#include <vectormath/vectormath_types.h>

struct Sprite;
struct SpriteSheet;

enum struct SpriteBatchState : uint32_t
{
    Idle,
    Batching,
};

// @todo: convert to C ABI
// @note: about default values, is all zero, so { 0 } fit the purpose
struct SpriteBatch
{
public: // Constants
    static constexpr const char* TAG = "SpriteBatch";

public: // Fields
    SpriteBatchState    state               = SpriteBatchState::Idle;
    
    uint32_t            textureId           = 0;
    uint32_t            vertexArrayId       = 0;
    uint32_t            verticesBufferId    = 0;
    uint32_t            uvsBufferId         = 0;
    uint32_t            colorsBufferId      = 0;
    
    int32_t             count               = 0;
    int32_t             capacity            = 0;

    vec2*               vertices            = nullptr;
    vec2*               uvs                 = nullptr;
    vec3*               colors              = nullptr;

public: // Methods
    void                Create(const SpriteSheet* sheet, int32_t vertexCapacity);
    void                Destroy(void);

    void                Begin(void);
    void                End(void);

    void                DrawSprite(const Sprite* sprite, vec2 position, float rotation, vec2 scale, vec3 color);
};
