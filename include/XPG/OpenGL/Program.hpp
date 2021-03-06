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

            inline bool isProgram() const
            {
                return glIsProgram(mHandle) == GL_TRUE;
            }

            void attachShader(const Shader& inShader);
            void bindAttribLocation(GLuint inIndex, const GLchar* inName);
            GLint getUniformLocation(const GLchar* inName);
            void link();
            bool validate();

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
