#include <XPG/OpenGL/ShortIndexVBO.hpp>

namespace XPG
{
    ShortIndexVBO::ShortIndexVBO(GLenum inUsage)
        : VertexBufferObject(GL_ELEMENT_ARRAY_BUFFER, GL_UNSIGNED_SHORT,
        inUsage)
    {
    }

    ShortIndexVBO::~ShortIndexVBO()
    {
    }

    void ShortIndexVBO::draw(GLenum inMode) const
    {
        bind();
        glDrawElements(inMode, mSize * mValuesPerUnit, mType, 0);
    }

    void ShortIndexVBO::drawInstanced(GLsizei inPrimCount, GLenum inMode) const
    {
        bind();
        glDrawElementsInstanced(inMode, mSize * mValuesPerUnit, mType, 0,
            inPrimCount);
    }
}
