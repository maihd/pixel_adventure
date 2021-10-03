// #include "Texture.h"
// #include "OpenGL.h"

// static const uint32_t ConvertTextureWrap[] = { 
//     0,          // None,
//     GL_CLAMP,   // Clamp,
//     GL_REPEAT,  // Repeat,
//     //GL_MIRROR_CLAMP, // MirrorClamp,
//     //GL_MIRROR_REPEAT, // MirrorRepeat,
// };

// static const uint32_t ConvertTextureFilter[] = {
//     0,          // None,
//     GL_LINEAR,  // Linear,
//     GL_NEAREST, // Nearest,
// };

// static const uint32_t ConvertPixelFormat[] = {
//     0,          // None,
//     GL_RGB,     // RGB,
//     GL_RGBA,    // RGBA,
// };

// Texture Texture::Create(void)
// {
//     Texture texture;
//     glGenTextures(1, (GLuint*)&texture.id);

//     GLint bindingTexture;
//     glGetIntegerv(GL_TEXTURE_BINDING_2D, &bindingTexture);

//     glBindTexture(GL_TEXTURE_2D, texture.handle);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//     glBindTexture(GL_TEXTURE_2D, (GLuint)bindingTexture);

//     return texture;
// }

// void Texture::Destroy(Texture& texture)
// {
//     glDeleteTextures(1, (GLuint*)&texture.handle);

//     texture = {}; // Clear all memory
// }

// void Texture::SetWrap(TextureWrap wrap)
// {
//     GLint bindingTexture;
//     glGetIntegerv(GL_TEXTURE_BINDING_2D, &bindingTexture);

//     glBindTexture(GL_TEXTURE_2D, handle);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ConvertTextureWrap(wrap));
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, ConvertTextureWrap(wrap));
//     glBindTexture(GL_TEXTURE_2D, (GLuint)bindingTexture);

//     HandleError();
// }

// void Texture::SetFilter(TextureFilter filter)
// {
//     glTextureParameteri(handle, GL_TEXTURE_MIN_FILTER, ConvertTextureFilter(filter));
//     glTextureParameteri(handle, GL_TEXTURE_MAG_FILTER, ConvertTextureFilter(filter));

//     HandleError();
// }

// void Texture::SetPixels(int w, int h, PixelFormat format, const void* pixels, PixelFormat targetFormat)
// {
//     if (w <= 0 || h <= 0)
//     {
//         return;
//     }

//     this->width  = (float)w;
//     this->height = (float)h;
//     this->format = targetFormat;

//     glTextureImage2DEXT(handle, GL_TEXTURE_2D, 0, ConvertPixelFormat(targetFormat), w, h, 0, ConvertPixelFormat(format), GL_UNSIGNED_BYTE, pixels);
//     HandleError();
// }
