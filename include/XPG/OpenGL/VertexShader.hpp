#ifndef XPGH_VERTEXSHADER
#define XPGH_VERTEXSHADER

#include "Shader.hpp"

namespace XPG
{
    class VertexShader : public Shader
    {
        public:
            VertexShader();
            virtual ~VertexShader();

        protected:
            VertexShader(const VertexShader& inShader) : Shader(inShader)
            {
                // no copying
            }

            VertexShader& operator=(const Shader& inShader)
            {
                // no copying
                return *this;
            }
    };
}

#endif
