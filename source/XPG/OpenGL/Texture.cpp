#include <XPG/OpenGL/Texture.hpp>

namespace XPG
{
    void Texture::setTexParams(const GLenum inParameters[])
    {
        for (const GLenum* i = inParameters; *i; i += 2)
            glTexParameteri(mTarget, i[0], i[1]);
    }

    Texture::Texture(GLenum inTarget) : mTarget(inTarget), mHandle(0)
    {
        glGenTextures(1, &mHandle);
    }

    Texture::~Texture()
    {
        if (mHandle) glDeleteTextures(1, &mHandle);
    }
}
