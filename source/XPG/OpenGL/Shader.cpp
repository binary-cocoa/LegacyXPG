#include <XPG/OpenGL/Shader.hpp>

#include <XPG/Platforms.hpp>
#ifdef XPG_PLATFORM_ANDROID
#   include <stdlib.h>
#   include <stdio.h>
#else
#   include <cstdlib>
#   include <cstdio>
#endif

namespace XPG
{
    Shader::Shader(GLenum inType) : mHandle(0), mType(inType)
    {
        mHandle = glCreateShader(mType);
        if (!mHandle) printf("Failed to make shader!\n");
    }

    Shader::~Shader()
    {
        glDeleteShader(mHandle);
    }

    void Shader::loadFromFile(const char* inFile)
    {
        if (!inFile || !*inFile) return;

        char* source = fileToBuffer(inFile);
        if (!source)
        {
            // TODO: report error
            return;
        }

        loadFromBuffer(source);

        delete [] source;
    }

    void Shader::loadFromBuffer(const char* inBuffer)
    {
        if (!mHandle)
        {
            // TODO: report error
            return;
        }

        glShaderSource(mHandle, 1, &inBuffer, 0);
        glCompileShader(mHandle);

        GLint compiled;
        glGetShaderiv(mHandle, GL_COMPILE_STATUS, &compiled);
        if (!compiled)
        {
            // TODO: convert to XPG exception
            GLint logLength;
            glGetShaderiv(mHandle, GL_INFO_LOG_LENGTH, &logLength);
            GLchar* log = new GLchar[logLength];

            GLsizei length;
            glGetShaderInfoLog(mHandle, logLength, &length, log);
            printf("-- shader compiler errors --\n%s\n", log);

            delete [] log;
        }
    }

    char* Shader::fileToBuffer(const char* inFile)
    {
        FILE* f = fopen(inFile, "r");
        if (!f)
        {
            // TODO: report error
            return NULL;
        }

        fseek(f, 0, SEEK_END);
        size_t length = ftell(f);

        fseek(f, 0, SEEK_SET);

        char* outBuffer = new char[length + 1];
        if (!outBuffer)
        {
            // TODO: report error
            return NULL;
        }

        size_t r = fread(outBuffer, sizeof(char), length, f);
        outBuffer[length] = '\0';
        fclose(f);

        if (r != length && ferror(f))
        {
            // TODO: report error
        }

        return outBuffer;
    }
}
