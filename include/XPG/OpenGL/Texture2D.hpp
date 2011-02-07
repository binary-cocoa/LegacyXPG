#ifndef XPGH_TEXTURE2D
#define XPGH_TEXTURE2D

#include <XPG/OpenGL/Texture.hpp>
#include <XPG/Image.hpp>

namespace XPG
{
    class Texture2D : public Texture
    {
        public:
            Texture2D();
            virtual ~Texture2D();

            void loadFromImage(const Image& inImage);
    };
}

#endif
