#pragma once

#include <stdint.h>
#include <assert.h>
#include "Texture.h"

struct Canvas
{
    Texture     texture             = {};
    uint32_t    frameBufferId       = 0;
    uint32_t    renderBufferId      = 0;

    void        Create(int32_t width, int32_t height);
    void        Destroy();
    void        Detach();
};
