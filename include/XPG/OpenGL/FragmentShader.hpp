#ifndef XPGH_FRAGMENTSHADER
#define XPGH_FRAGMENTSHADER

#include "Shader.hpp"

namespace XPG
{
    class FragmentShader : public Shader
    {
        public:
            FragmentShader();
            virtual ~FragmentShader();

        protected:
            FragmentShader(const FragmentShader& inShader) : Shader(inShader)
            {
                // no copying
            }

            FragmentShader& operator=(const FragmentShader& inShader)
            {
                // no copying
                return *this;
            }
    };
}

#endif
