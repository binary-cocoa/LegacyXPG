#include <XPG/OpenGL/Program.hpp>
#include <XPG/OpenGL/Shader.hpp>

namespace XPG
{
    Program::Program() : mHandle(0), mSize(0), mLinked(false)
    {
        mHandle = glCreateProgram();
    }

    Program::~Program()
    {
        for (size_t i = 0; i < mSize; ++i)
            glDetachShader(mHandle, mShaders[i]);

        glDeleteProgram(mHandle);
    }

    void Program::attachShader(const Shader& inShader)
    {
        if (mLinked)
        {
            // More shaders cannot be attached if the program is already linked.
            return;
        }

        if (mSize >= MaxShaders)
        {
            return;
        }

        mShaders[mSize] = inShader.mHandle;
        ++mSize;

        glAttachShader(mHandle, inShader.mHandle);
    }

    void Program::bindAttribLocation(GLuint inIndex, const GLchar* inName)
    {
        glBindAttribLocation(mHandle, inIndex, inName);
    }

    void Program::link()
    {
        if (mLinked)
        {
            // TODO: report error through XPG exception
            return;
        }

        if (mSize < 2 || !mHandle)
        {
            // TODO: report error through XPG exception
            return;
        }

        glLinkProgram(mHandle);

        GLint linked;
        glGetProgramiv(mHandle, GL_LINK_STATUS, &linked);

        if (!linked)
        {
            // TODO: report error through XPG exception
            return;
        }

        mLinked = true;
        use();
    }

    GLint Program::getUniformLocation(const GLchar* inName)
    {
        return glGetUniformLocation(mHandle, inName);
    }
}
