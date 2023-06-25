#pragma once

#include <stdint.h>
#include <assert.h>

#include "Misc/Compiler.h"
#include "Graphics/Texture.h"

// @todo: convert to C ABI
// @note: about default values, is all zero, so { 0 } fit the purpose
struct Canvas
{
    Texture     texture             __default_init({});
    uint32_t    frameBufferId       __default_init(0);
    uint32_t    renderBufferId      __default_init(0);
};

#ifdef __cplusplus
extern "C" {
#endif

Canvas      Canvas_Create(int32_t width, int32_t height);
void        Canvas_Destroy(Canvas* canvas);
void        Canvas_Detach(Canvas* canvas);

#ifdef __cplusplus
}
#endif

//! LEAVE AN EMPTY LINE HERE, REQUIRE BY GCC/G++
