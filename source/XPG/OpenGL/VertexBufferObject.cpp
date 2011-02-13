#include <XPG/OpenGL/VertexBufferObject.hpp>

namespace XPG
{
    VertexBufferObject::VertexBufferObject(GLenum inTarget, GLenum inType,
        GLenum inUsage) : mHandle(0), mTarget(inTarget), mType(inType),
        mUsage(inUsage), mValuesPerUnit(1)
    {
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
        if (!mContext.isOutdated() && mHandle)
            glDeleteBuffers(1, &mHandle);
    }

    void VertexBufferObject::create()
    {
        if (mContext.isOutdated())
        {
            mContext.update();
            glGenBuffers(1, &mHandle);
        }
    }

    void VertexBufferObject::loadData(const GLvoid* inData, GLuint inSize,
        GLuint inValuesPerUnit)
    {
        create();
        bind();
        glBufferData(mTarget, inSize * inValuesPerUnit * mTypeSize, inData,
            mUsage);

        mValuesPerUnit = inValuesPerUnit;
    }
}
