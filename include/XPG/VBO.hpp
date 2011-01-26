#ifndef XPGH_VBO
#define XPGH_VBO

#include "OpenGL.hpp"

namespace XPG
{
    class VBO
    {
        public:
            VBO(GLenum inTarget = GL_ARRAY_BUFFER);
            ~VBO();

            inline void bind() const { glBindBuffer(mTarget, mHandle); }
            void setData(const GLvoid* inData, size_t inSize,
                GLenum inType = GL_FLOAT, GLenum inUsage = GL_STATIC_DRAW);

        private:
            GLuint mHandle;
            GLenum mTarget;
            GLenum mType;
            size_t mTypeSize;
    };
}

#endif
