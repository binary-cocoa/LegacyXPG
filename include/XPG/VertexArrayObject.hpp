#ifndef XPGH_VERTEXARRAYOBJECT
#define XPGH_VERTEXARRAYOBJECT

#include "OpenGL.hpp"

namespace XPG
{
    class VertexArrayObject
    {
        public:
            VertexArrayObject();
            ~VertexArrayObject();

            inline void bind() const { glBindVertexArray(mHandle); }

        private:
            GLuint mHandle;
    };
}

#endif
