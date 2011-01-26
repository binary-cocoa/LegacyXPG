#include <XPG/Image.hpp>
#include <cstdlib>

using namespace std;

namespace XPG
{
    Image::Image(const char* inFile) : mData(NULL)
    {
        loadFile(inFile);
    }

    Image::~Image()
    {
        if (mData) free(mData);
    }

    void Image::loadFile(const char* inFile)
    {
        if (!inFile || !*inFile) return;

        // TODO: parse by file extension

        FILE* f = fopen(inFile, "rb");
        if (!f)
        {
            cout << "failed to open " << inFile << endl;
            return;
        }

        loadPNG(f);

        fclose(f);
    }

    void Image::loadIntoTexture(GLenum inTarget) const
    {
        if (!mData) return;

        glTexImage2D(inTarget, 0, GL_RGBA, mWidth, mHeight, 0, mFormat,
            GL_UNSIGNED_BYTE, mData);
    }
}
