#include <XPG/Texture2D.hpp>

namespace XPG
{
    const GLenum defaultTexParam[] = {
        GL_TEXTURE_WRAP_S, GL_REPEAT,
        GL_TEXTURE_WRAP_T, GL_REPEAT,
        GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR,
        GL_TEXTURE_MAG_FILTER, GL_LINEAR,
        0 };

    Texture2D::Texture2D() : Texture(GL_TEXTURE_2D)
    {
    }

    Texture2D::~Texture2D()
    {
    }

    void Texture2D::loadFromImage(const Image& inImage)
    {
        if (!inImage.isValid()) return;

        bind();
        setTexParams(mTarget, defaultTexParam);
        inImage.loadIntoTexture(mHandle);
        glGenerateMipmap(mTarget);
    }
}
