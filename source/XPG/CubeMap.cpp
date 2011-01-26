#include <XPG/CubeMap.hpp>

namespace XPG
{
    const GLenum defaultCubeMapTexParam[] = {
        GL_TEXTURE_MAG_FILTER, GL_LINEAR,
        GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE,
        GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE,
        GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE,
        0 };

    const GLenum defaultCubeMapTexParam2[] = {
        //GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR,
        GL_TEXTURE_MIN_FILTER, GL_LINEAR,
        0 };

    CubeMap::CubeMap() : Texture(GL_TEXTURE_CUBE_MAP)
    {
    }

    CubeMap::~CubeMap()
    {
    }

    void CubeMap::loadFromImages(const Image* inImages[])
    {
        static const GLenum cubeFaces[6] = {
            GL_TEXTURE_CUBE_MAP_POSITIVE_X,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

        for (size_t i = 0; i < 6; ++i)
            if (!inImages[i] || !inImages[i]->isValid())
                return;

        bind();
        setTexParams(mTarget, defaultCubeMapTexParam);

        for (size_t i = 0; i < 6; ++i)
            inImages[i]->loadIntoTexture(cubeFaces[i]);

         glGenerateMipmap(mTarget);
         setTexParams(mTarget, defaultCubeMapTexParam2);
    }
}
