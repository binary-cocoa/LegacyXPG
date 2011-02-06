#ifndef XPGH_SHADER
#define XPGH_SHADER

#include "Base.hpp"

namespace XPG
{
    class Shader
    {
        public:
            Shader();
            Shader(const char* inFile, GLenum inType);
            ~Shader();

            inline GLuint handle() const { return mHandle; }
            void loadFromFile(const char* inFile, GLenum inType);
            void loadFromBuffer(const char* inBuffer, GLenum inType);
            void unload();

        private:
            Shader(const Shader& inShader) {} // no copying

            GLuint mHandle;

            static char* fileToBuffer(const char* inFile);
    };
}

#endif
