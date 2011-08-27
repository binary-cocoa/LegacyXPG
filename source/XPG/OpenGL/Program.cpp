#include <XPG/OpenGL/Program.hpp>
#include <XPG/OpenGL/Shader.hpp>

#include <iostream>

namespace XPG
{
    Program::Program() : mHandle(0), mSize(0), mLinked(false)
    {
        mHandle = glCreateProgram();
        if (!mHandle) std::cerr << "failed to make program\n";
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
            std::cerr << "already linked\n";
            // More shaders cannot be attached if the program is already linked.
            return;
        }

        if (mSize >= MaxShaders)
        {
            std::cerr << "too many shaders\n";
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
            std::cerr << "already linked\n";
            return;
        }

        if (mSize < 2 || !mHandle)
        {
            // TODO: report error through XPG exception
            std::cerr << "not enough shaders linked\n";
            return;
        }

        glLinkProgram(mHandle);

        GLint linked;
        glGetProgramiv(mHandle, GL_LINK_STATUS, &linked);

        if (!linked)
        {
            // TODO: report error through XPG exception
            std::cerr << "failed to link program\n";
            return;
        }

        mLinked = true;
        use();
    }

    GLint Program::getUniformLocation(const GLchar* inName)
    {
        return glGetUniformLocation(mHandle, inName);
    }

    bool Program::validate()
    {
        glValidateProgram(mHandle);

        GLint status = 0;
        glGetProgramiv(mHandle, GL_VALIDATE_STATUS, &status);
        return status == GL_TRUE;
    }
}
