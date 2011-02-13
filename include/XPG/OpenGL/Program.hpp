#ifndef XPGH_PROGRAM
#define XPGH_PROGRAM

#include "Base.hpp"

#include <XPG/Platforms.hpp>
#ifdef XPG_PLATFORM_ANDROID
#   include <stdlib.h>
#else
#   include <cstdlib>
#endif

namespace XPG
{
    const size_t MaxShaders = 3;

    class Shader;

    class Program
    {
        public:
            Program();
            ~Program();

            void attachShader(const Shader& inShader);
            void bindAttribLocation(GLuint inIndex, const GLchar* inName);
            void link();
            GLint getUniformLocation(const GLchar* inName);
            void clear();

            inline void use() { glUseProgram(mHandle); }

        private:
            void create();

            GLuint mHandle;
            GLuint mShaders[MaxShaders];
            size_t mSize;
            bool mLinked;
            OpenGL::Context mContext;
    };
}

#endif
