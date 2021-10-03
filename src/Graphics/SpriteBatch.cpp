#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#include "OpenGL.h"
#include "Graphics.h"
#include "SpriteBatch.h"
#include "Native/Memory.h"
#include "Math/VectorMath.h"

void SpriteBatch::Create(const SpriteSheet* sheet, int32_t capacity)
{
    this->textureId = sheet->textureId;

    glGenBuffers(3, &this->verticesBufferId);
    glGenVertexArrays(1, &this->vertexArrayId);

    glBindVertexArray(this->vertexArrayId);

    glBindBuffer(GL_ARRAY_BUFFER, this->verticesBufferId);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(vec2), NULL);

    glBindBuffer(GL_ARRAY_BUFFER, this->uvsBufferId);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(vec2), NULL);

    glBindBuffer(GL_ARRAY_BUFFER, this->colorsBufferId);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, true, sizeof(vec3), NULL);

    this->count = 0;
    this->capacity = capacity;

    int vertexCapacity = capacity * 6;

    this->vertices  = (vec2*)MemoryAlloc(vertexCapacity * sizeof(vec2), alignof(vec2));
    this->uvs       = (vec2*)MemoryAlloc(vertexCapacity * sizeof(vec2), alignof(vec2));
    this->colors    = (vec3*)MemoryAlloc(vertexCapacity * sizeof(vec3), alignof(vec3));
}

void SpriteBatch::Destroy(void)
{
    assert(this->state == SpriteBatchState::Idle);

    this->state = SpriteBatchState::Idle;

    glDeleteBuffers(3, &this->verticesBufferId);
    glDeleteVertexArrays(1, &this->vertexArrayId);

    MemoryFree(this->vertices);
    MemoryFree(this->uvs);
    MemoryFree(this->colors);

    this->count             = 0;
    this->capacity          = 0;

    this->textureId         = 0;
    this->vertexArrayId     = 0;

    this->verticesBufferId  = 0;
    this->uvsBufferId       = 0;
    this->colorsBufferId    = 0;

    this->vertices          = nullptr;
    this->uvs               = nullptr;
    this->colors            = nullptr;
}

void SpriteBatch::Begin(void)
{
    assert(this->state == SpriteBatchState::Idle);

    this->count = 0;
    this->state = SpriteBatchState::Batching;
}

void SpriteBatch::End(void)
{
    assert(this->state == SpriteBatchState::Batching);

    this->state = SpriteBatchState::Idle;
}

void SpriteBatch::DrawSprite(const Sprite* sprite, vec2 position, float rotation, vec2 scale, vec3 color)
{
    assert(this->count < this->capacity);
    assert(sprite != nullptr);

    const mat4 model = mat4_transform2(position, rotation, vec2_mul(scale, vec2_new(sprite->width, sprite->height)));

    const vec2 pos0 = mat4_mul_vec2(model, vec2_new(-0.5f, -0.5f));
    const vec2 pos1 = mat4_mul_vec2(model, vec2_new( 0.5f,  0.5f));

    const vec2 uv0 = sprite->uv0;
    const vec2 uv1 = sprite->uv1;

    int vertexIndex = 6 * this->count;

    this->vertices[vertexIndex + 0] = vec2_new(pos0.x, pos0.y);
    this->vertices[vertexIndex + 1] = vec2_new(pos1.x, pos1.y);
    this->vertices[vertexIndex + 2] = vec2_new(pos0.x, pos1.y);

    this->vertices[vertexIndex + 3] = vec2_new(pos0.x, pos0.y);
    this->vertices[vertexIndex + 4] = vec2_new(pos1.x, pos0.y);
    this->vertices[vertexIndex + 5] = vec2_new(pos1.x, pos1.y);

    this->uvs[vertexIndex + 0] = vec2_new(uv0.x, uv0.y);
    this->uvs[vertexIndex + 1] = vec2_new(uv1.x, uv1.y);
    this->uvs[vertexIndex + 2] = vec2_new(uv0.x, uv1.y);

    this->uvs[vertexIndex + 3] = vec2_new(uv0.x, uv0.y);
    this->uvs[vertexIndex + 4] = vec2_new(uv1.x, uv0.y);
    this->uvs[vertexIndex + 5] = vec2_new(uv1.x, uv1.y);

    this->colors[vertexIndex + 0] = color;
    this->colors[vertexIndex + 1] = color;
    this->colors[vertexIndex + 2] = color;

    this->colors[vertexIndex + 3] = color;
    this->colors[vertexIndex + 4] = color;
    this->colors[vertexIndex + 5] = color;

    this->count++;
}
