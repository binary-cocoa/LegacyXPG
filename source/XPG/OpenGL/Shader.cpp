#include <XPG/OpenGL/Shader.hpp>

#include <XPG/Platforms.hpp>
#ifdef XPG_PLATFORM_ANDROID
#   include <stdio.h>
#else
#   include <cstdio>
#endif

namespace XPG
{
    Shader::Shader(GLenum inType) : mHandle(0), mType(inType),
        mContextVersion(0)
    {
    }

    Shader::~Shader()
    {
        if (mHandle) glDeleteShader(mHandle);
    }

    void Shader::create()
    {
        if (mContextVersion < getContextVersion())
        {
            mContextVersion = getContextVersion();
            mHandle = glCreateShader(mType);
        }
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
        create();

        if (!mHandle)
        {
            // TODO: report error
            return;
        }

        glShaderSource(mHandle, 1, const_cast<const GLchar**>(&inBuffer), 0);
        glCompileShader(mHandle);

        GLint compiled;
        glGetShaderiv(mHandle, GL_COMPILE_STATUS, &compiled);
        if (!compiled)
        {
            // TODO: convert to XPG exception
            GLchar log[2048];
            GLsizei length;
            glGetShaderInfoLog(mHandle, 2048, &length, log);
            //printf("-- shader compiler errors --\n%s\n", log);

            glDeleteShader(mHandle);
            mHandle = 0;
        }
    }

    char* Shader::fileToBuffer(const char* inFile)
    {
        FILE *f;
        size_t length;
        size_t r;
        char* outBuffer;

        if (!(f = fopen(inFile, "r")))
        {
            // TODO: report error
            return NULL;
        }

        fseek(f, 0, SEEK_END);
        length = ftell(f);

        fseek(f, 0, SEEK_SET);

        outBuffer = static_cast<char*>(malloc((length + 1) * sizeof(char)));
        if (!outBuffer)
        {
            // TODO: report error
            return NULL;
        }

        r = fread(outBuffer, sizeof(char), length, f);
        outBuffer[length] = '\0';
        fclose(f);

        return outBuffer;
    }
}
