#ifndef XPGH_VERTEXBUFFEROBJECT
#define XPGH_VERTEXBUFFEROBJECT

#include "Base.hpp"

namespace XPG
{
    class VertexBufferObject
    {
        public:
            friend class VertexArrayObject;

            VertexBufferObject(GLenum inTarget = GL_ARRAY_BUFFER,
                GLenum inType = GL_FLOAT, GLenum inUsage = GL_STATIC_DRAW);
            ~VertexBufferObject();

            inline void bind() const { glBindBuffer(mTarget, mHandle); }
            void loadData(const GLvoid* inData, GLuint inSize,
                GLuint inValuesPerUnit = 1);

        private:
            GLuint mHandle;
            GLenum mTarget;
            GLenum mType;
            GLenum mUsage;
            GLuint mTypeSize;
            GLuint mValuesPerUnit;
    };
}

#endif
