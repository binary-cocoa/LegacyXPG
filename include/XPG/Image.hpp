#ifndef XPGH_IMAGE
#define XPGH_IMAGE

#include <XPG/DataTypes.hpp>
#include <XPG/OpenGL/Base.hpp>
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
        void loadIntoTexture(GLenum inTarget, GLsizei inIndex) const;

        inline bool isValid() const { return mData; }

        inline uint16 width() const { return mWidth; }
        inline uint16 height() const { return mHeight; }

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
        uint16 mWidth;
        uint16 mHeight;
        uint16 mBitDepth;
        uint16 mChannels;
    };
}

#endif
