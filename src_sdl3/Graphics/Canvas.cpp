// #include "Canvas.h"
// #include "OpenGL.h"

// void Canvas::Create(int32_t width, int32_t height)
// {
//     assert(textureId == 0);
//     assert(frameBufferId == 0);
//     assert(renderBufferId == 0);

//     const int32_t pixelsWidth  = (int)width;
//     const int32_t pixelsHeight = (int)height;

//     texture.Create();
//     texture.SetPixels(pixelsWidth, pixelsHeight, PixelFormat::RGB, nullptr, PixelFormat::RGB);

//     // Failed to generate texture
//     if (!texture.handle)
//     {
//         return;
//     }

//     // Generate depth stencil buffer
//     //glGenTextures(1, &renderTarget.depthStencilTexture);
//     //if (!renderTarget.depthStencilTexture)
//     //{
//     //    return renderTarget;
//     //}
//     //glBindTexture(GL_TEXTURE_2D, renderTarget.colorTexture);
//     //glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
//     //glBindTexture(GL_TEXTURE_2D, 0);

//     // Generate render buffer
//     glGenRenderbuffers(1, (GLuint*)&renderTarget.renderBuffer);
//     glNamedRenderbufferStorage(renderTarget.renderBuffer, GL_DEPTH24_STENCIL8, pixelsWidth, pixelsHeight);

//     // Generate frame buffer
//     GLint bindingFramebuffer;
//     glGetIntegerv(GL_FRAMEBUFFER_BINDING, &bindingFramebuffer);

//     glGenFramebuffers(1, (GLuint*)&frameBufferId);
//     glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
//     glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTarget.texture.handle, 0); 
//     glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderTarget.renderBuffer);
//     //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, renderTarget.depthStencilTexture, 0);

//     GLenum status;
//     status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
//     switch (status)
//     {
//     case GL_FRAMEBUFFER_COMPLETE:
//         break;
    
//     default:
//         return renderTarget;
//     }
    
//     glBindFramebuffer(GL_FRAMEBUFFER, bindingFramebuffer);

//     return renderTarget;
// }

// void Canvas::Destroy()
// {
//     glDeleteVertexArrays(1, (GLuint*)&array.handle);
// }
