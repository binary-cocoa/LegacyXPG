#ifndef XPGH_TEXTUREARRAY
#define XPGH_TEXTUREARRAY

#include "Texture.hpp"
#include "../Image.hpp"

namespace XPG
{
    class TextureArray : public Texture
    {
        public:
            TextureArray(GLsizei inWidth, GLsizei inHeight, GLsizei inDepth);
            virtual ~TextureArray();

            void loadImage(GLsizei inIndex, const Image& inImage);

        private:
            GLsizei mWidth;
            GLsizei mHeight;
            GLsizei mDepth;
    };
}

#endif
