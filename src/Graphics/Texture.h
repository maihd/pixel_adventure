#pragma once

#include <stdint.h>

enum struct TextureFilter : uint32_t
{
    None,
    Linear,
    Nearest,
};

enum struct TextureWrap : uint32_t
{
    None,
    Clamp,
    Repeat,
    MirrorClamp,
    MirrorRepeat,
};

enum struct PixelFormat : uint32_t
{
    None,

    RGB,
    RGBA,
};

struct Texture
{
    uint32_t        id      = 0;
    float           width   = 0.0f;
    float           height  = 0.0f;

    PixelFormat     format  = PixelFormat::None;
    TextureWrap     wrap    = TextureWrap::None;
    TextureFilter   filter  = TextureFilter::None;

    // NO mipmap support now
    //int32_t         mipmaps = 0;

    Texture         Create(void);
    void            Destroy(void);

    void            SetWrap(TextureWrap wrap);
    void            SetFilter(TextureFilter filter);
    void            SetPixels(int32_t width, int32_t height, PixelFormat format, const void* pixels, PixelFormat targetFormat = PixelFormat::RGBA);
};
