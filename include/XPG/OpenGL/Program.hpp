#ifndef XPGH_PROGRAM
#define XPGH_PROGRAM

#include "Base.hpp"

#ifdef XPG_PLATFORM_ANDROID
#   include <stdlib.h>
#else
#   include <cstdlib>
#endif

namespace XPG
{
    const size_t MaxShaders = 16;

    class Shader;

    class Program
    {
        public:
            Program();
            ~Program();

            void attachShader(const Shader& inShader);
            void bindAttribLocation(GLuint inIndex, const GLchar* inName);
            void bindFragDataLocation(GLuint inColorNumber, const char* inName);
            GLint getUniformLocation(const GLchar* inName);
            void link();

            inline void use() { glUseProgram(mHandle); }

        private:
            void create();

            GLuint mHandle;
            GLuint mShaders[MaxShaders];
            size_t mSize;
            bool mLinked;
    };
}

#endif
