#ifndef XPGH_IMAGE
#define XPGH_IMAGE

#include <XPG/DataTypes.hpp>
#include <XPG/OpenGL.hpp>
#include <cstdio>

namespace XPG
{
    class Image
    {
        public:
            Image(const char* inFile = NULL);
            ~Image();

            void loadFile(const char* inFile);
            void loadIntoTexture(GLenum inTarget = GL_TEXTURE_2D) const;

            inline bool isValid() const { return !!mData; }

        private:
            Image(const Image& inImage) {} // disallowed

            Image& operator=(const Image& inImage)
            {
                // disallowed
                return *this;
            }

            void loadPNG(FILE* inFile);

            void* mData;
            GLenum mFormat;
            int16u mWidth;
            int16u mHeight;
            int16u mBitDepth;
            int16u mChannels;
    };
}

#endif
