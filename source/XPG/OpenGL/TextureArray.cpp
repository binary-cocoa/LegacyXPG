#include <XPG/OpenGL/TextureArray.hpp>

namespace XPG
{
    TextureArray::TextureArray(GLsizei inWidth, GLsizei inHeight,
        GLsizei inDepth) : Texture(GL_TEXTURE_2D_ARRAY), mWidth(inWidth),
        mHeight(inHeight), mDepth(inDepth)
    {
        glTexImage3D(mTarget, 0, GL_RGBA, mWidth, mHeight, mDepth, 0,
            GL_BGRA, GL_UNSIGNED_BYTE, NULL);
    }

    TextureArray::~TextureArray()
    {
    }

    void TextureArray::loadImage(GLsizei inIndex, const Image& inImage)
    {
        if (!inImage.isValid() || inImage.width() != mWidth
            || inImage.height() != mHeight)
        {
            return;
        }

        const GLenum params[] = {
            GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE,
            GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE,
            GL_TEXTURE_MAG_FILTER, GL_LINEAR,
            GL_TEXTURE_MIN_FILTER, GL_LINEAR,
            0 };

        bind();
        setTexParams(params);
        inImage.loadIntoTexture(mTarget, inIndex);
    }
}
