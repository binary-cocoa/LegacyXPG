#ifndef XPGH_SHORTINDEXVBO
#define XPGH_SHORTINDEXVBO

#include "VertexBufferObject.hpp"

namespace XPG
{
    class ShortIndexVBO : public VertexBufferObject
    {
        public:
            ShortIndexVBO(GLenum inUsage = GL_STATIC_DRAW);
            virtual ~ShortIndexVBO();

            void draw(GLenum inMode = GL_TRIANGLES) const;
            void drawInstanced(GLsizei inPrimCount,
                GLenum inMode = GL_TRIANGLES) const;

        protected:
        private:
    };
}

#endif
