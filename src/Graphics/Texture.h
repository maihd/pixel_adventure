#pragma once

#include <stdint.h>

enum TextureFilter : uint32_t
{
    TextureFilter_None,
    TextureFilter_Linear,
    TextureFilter_Nearest,
};

enum TextureWrap : uint32_t
{
    TextureWrap_None,
    TextureWrap_Clamp,
    TextureWrap_Repeat,
    TextureWrap_MirrorClamp,
    TextureWrap_MirrorRepeat,
};

enum PixelFormat : uint32_t
{
    PixelFormat_None,

    PixelFormat_RGB,
    PixelFormat_RGBA,
};

struct Texture
{
    uint32_t        id      = 0;
    float           width   = 0.0f;
    float           height  = 0.0f;

    PixelFormat     format  = PixelFormat_None;
    TextureWrap     wrap    = TextureWrap_None;
    TextureFilter   filter  = TextureFilter_None;

    // NO mipmap support now
    //int32_t         mipmaps = 0;

    Texture         Create(void);
    void            Destroy(void);

    void            SetWrap(TextureWrap wrap);
    void            SetFilter(TextureFilter filter);
    void            SetPixels(int32_t width, int32_t height, PixelFormat format, const void* pixels, PixelFormat targetFormat = PixelFormat_RGBA);
};
