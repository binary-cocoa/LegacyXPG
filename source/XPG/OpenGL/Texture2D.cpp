#include <XPG/OpenGL/Texture2D.hpp>

namespace XPG
{
    Texture2D::Texture2D() : Texture(GL_TEXTURE_2D)
    {
    }

    Texture2D::~Texture2D()
    {
    }

    void Texture2D::loadFromImage(const Image& inImage)
    {
        if (!inImage.isValid()) return;

        const GLenum params1[] = {
            GL_TEXTURE_WRAP_S, GL_REPEAT,
            GL_TEXTURE_WRAP_T, GL_REPEAT,
            GL_TEXTURE_MAG_FILTER, GL_LINEAR,
            0 };

        const GLenum params2[] = {
            GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR,
            0 };

        bind();
        setTexParams(params1);
        inImage.loadIntoTexture(mTarget);
        glGenerateMipmap(mTarget);
        setTexParams(params2);
    }
}
