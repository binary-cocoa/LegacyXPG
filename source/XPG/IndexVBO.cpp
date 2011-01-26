#include <XPG/IndexVBO.hpp>

namespace XPG
{
    IndexVBO::IndexVBO() : mSize(0)
    {
        glGenBuffers(1, &mBuffer);
    }

    IndexVBO::~IndexVBO()
    {
        glDeleteBuffers(1, &mBuffer);
    }

    void IndexVBO::loadData(GLenum inMode, GLuint inSize, const OGLI* inData,
        GLenum inUsage)
    {
        mMode = inMode;
        mSize = inSize;
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, inSize * sizeof(OGLI),
            inData, inUsage);
    }

    void IndexVBO::setMode(GLenum inMode)
    {
        mMode = inMode;
    }

    void IndexVBO::draw() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffer);
        glDrawElements(mMode, mSize, OGLIT, 0);
    }
}
