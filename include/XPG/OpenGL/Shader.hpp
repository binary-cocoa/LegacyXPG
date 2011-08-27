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
            virtual ~Shader();

            void loadFromFile(const char* inFile);
            void loadFromBuffer(const char* inBuffer);

        protected:
            Shader(const Shader& inShader) {} // no copying

            Shader& operator=(const Shader& inShader)
            {
                // no copying
                return *this;
            }

        private:

            GLuint mHandle;
            GLenum mType;

            static char* fileToBuffer(const char* inFile);
    };
}

#endif
