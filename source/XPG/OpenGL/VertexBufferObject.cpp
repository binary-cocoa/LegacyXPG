#include <XPG/OpenGL/VertexBufferObject.hpp>

namespace XPG
{
    VertexBufferObject::VertexBufferObject(GLenum inTarget, GLenum inType,
        GLenum inUsage) : mHandle(0), mTarget(inTarget), mType(inType),
        mUsage(inUsage), mValuesPerUnit(1), mContextVersion(0)
    {
        switch (mType)
        {
            case GL_FLOAT:          mTypeSize = sizeof(GLfloat);  break;
            case GL_UNSIGNED_BYTE:  mTypeSize = sizeof(GLubyte);  break;
            case GL_UNSIGNED_SHORT: mTypeSize = sizeof(GLushort); break;
            case GL_UNSIGNED_INT:   mTypeSize = sizeof(GLuint);   break;

            default: {}
        }
    }

    VertexBufferObject::~VertexBufferObject()
    {
        if (mHandle) glDeleteBuffers(1, &mHandle);
    }

    void VertexBufferObject::create()
    {
        if (mContextVersion < getContextVersion())
        {
            mContextVersion = getContextVersion();
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
