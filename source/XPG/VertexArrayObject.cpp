#include <XPG/VertexArrayObject.hpp>

namespace XPG
{
    VertexArrayObject::VertexArrayObject()
    {
        glGenVertexArrays(1, &mHandle);
    }

    VertexArrayObject::~VertexArrayObject()
    {
        glDeleteVertexArrays(1, &mHandle);
    }
}
