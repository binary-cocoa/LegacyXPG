#include <XPG/OpenGL/VertexBufferObject.hpp>
#include <cassert>

namespace XPG
{
    VertexBufferObject::VertexBufferObject(GLenum inTarget, GLenum inType,
        GLenum inUsage) : mIndex(0), mTarget(inTarget), mType(inType),
        mUsage(inUsage), mValuesPerUnit(1), mSize(0)
    {
        glGenBuffers(1, &mHandle);

        switch (mType)
        {
            case GL_FLOAT:          mTypeSize = sizeof(GLfloat);  break;
            case GL_UNSIGNED_BYTE:  mTypeSize = sizeof(GLubyte);  break;
            case GL_UNSIGNED_SHORT: mTypeSize = sizeof(GLushort); break;
            case GL_UNSIGNED_INT:   mTypeSize = sizeof(GLuint);   break;

            default:
                mType = GL_FLOAT;
                mTypeSize = sizeof(GLfloat);
        }
    }

    VertexBufferObject::~VertexBufferObject()
    {
        glDeleteBuffers(1, &mHandle);
    }

    void VertexBufferObject::loadData(const GLvoid* inData, GLuint inSize,
        GLuint inValuesPerUnit)
    {
        assert(inData != NULL);
        assert(inSize > 0);
        assert(inValuesPerUnit > 0);

        bind();
        mSize = inSize;
        mValuesPerUnit = inValuesPerUnit;
        glBufferData(mTarget, mSize * mValuesPerUnit * mTypeSize, inData,
            mUsage);
    }

    void VertexBufferObject::editData(const GLvoid* inData, GLuint inFirst,
        GLuint inVertexCount)
    {
        assert(inData != NULL);
        assert(inVertexCount > 0);

        bind();
        glBufferSubData(mTarget, mTypeSize * mValuesPerUnit * inFirst,
            mTypeSize * mValuesPerUnit * inVertexCount, inData);
    }

    // enableVAA == enable vertex attrib array
    void VertexBufferObject::enableVAA(GLuint inIndex)
    {
        mIndex = inIndex;

        bind();
        glVertexAttribPointer(inIndex, mValuesPerUnit, mType, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(mIndex);
    }

    // disableVAA == disable vertex attrib array
    void VertexBufferObject::disableVAA()
    {
        glDisableVertexAttribArray(mIndex);
    }
}
