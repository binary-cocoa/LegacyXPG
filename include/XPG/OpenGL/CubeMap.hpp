#ifndef XPGH_CUBEMAP
#define XPGH_CUBEMAP

#include <XPG/OpenGL/Texture.hpp>
#include <XPG/Image.hpp>

namespace XPG
{
    class CubeMap : public Texture
    {
        public:
            CubeMap();
            virtual ~CubeMap();

            void loadFromImages(const Image* inImages[]);
    };
}

#endif

