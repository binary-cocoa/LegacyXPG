#ifndef XPGH_SHADER
#define XPGH_SHADER

#include "Base.hpp"

namespace XPG
{
    class Shader
    {
        public:
            friend class Program;

            Shader(GLenum inType);
            ~Shader();

            void loadFromFile(const char* inFile);
            void loadFromBuffer(const char* inBuffer);

        private:
            Shader(const Shader& inShader) {} // no copying

            Shader& operator=(const Shader& inShader)
            {
                // no copying
                return *this;
            }

            void create();

            GLuint mHandle;
            GLenum mType;
            int16u mContextVersion;

            static char* fileToBuffer(const char* inFile);
    };
}

#endif
