#include <XPG/VBO.hpp>

namespace XPG
{
    VBO::VBO(GLenum inTarget) : mTarget(inTarget), mType(0), mTypeSize(0)
    {
        glGenBuffers(1, &mHandle);
    }

    VBO::~VBO()
    {
        glDeleteBuffers(1, &mHandle);
    }

    void VBO::setData(const GLvoid* inData, size_t inSize, GLenum inType,
        GLenum inUsage)
    {
        switch (inType)
        {
            case GL_FLOAT:          mTypeSize = sizeof(GLfloat);  break;
            case GL_UNSIGNED_BYTE:  mTypeSize = sizeof(GLubyte);  break;
            case GL_UNSIGNED_SHORT: mTypeSize = sizeof(GLushort); break;
            case GL_UNSIGNED_INT:   mTypeSize = sizeof(GLuint);   break;
            default: return;
        }

        mType = inType;

        bind();
        glBufferData(mTarget, inSize * mTypeSize, inData, inUsage);
    }
}
