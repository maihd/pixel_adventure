#pragma once

#include <vectormath/vectormath_types.h>

struct SpriteSheet;

enum struct SpriteBatchState : uint32_t
{
    UnCreated,

    Idle,
    Batching,
};

struct SpriteBatch
{
public: // Constants
    static constexpr const char* TAG = "SpriteBatch";

public: // Fields
    SpriteBatchState    state               = SpriteBatchState::UnCreated;
    
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

    [[deprecated]]
    inline void         DrawSprite(const Sprite* sprite, vec2 position, float rotation, vec2 scale, vec3 color)
    {
        BatchSprite(sprite, position, rotation, scale, color);
    }

    void                BatchSprite(const Sprite* sprite, vec2 position, float rotation, vec2 scale, vec3 color);
};

//! LEAVE AN EMPTY LINE HERE, REQUIRE BY GCC/G++
