#include <XPG/Image.hpp>
#include <png.h>

#include <XPG/Platforms.hpp>
#ifdef XPG_PLATFORM_ANDROID
#   include <stdlib.h>
#   include <stdio.h>
#else
#   include <cstdlib>
#   include <cstdio>
#endif

namespace XPG
{
    void Image::loadPNG(FILE* inFile)
    {
        FILE* f = inFile;

        int8u buffer[8];
        size_t r = fread(buffer, 1, 8, f);
        ++r;

        bool isPNG = !png_sig_cmp(buffer, 0, 8);
        if (!isPNG)
        {
            //cout << "not a PNG file" << endl;
            return;
        }

        //cout << "found a PNG file" << endl;
        //rewind(f);

        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
            NULL, NULL, NULL);

        if (!png_ptr)
        {
            //cout << "failed to create png_ptr" << endl;
            return;
        }

        png_infop info_ptr = png_create_info_struct(png_ptr);

        if (!info_ptr)
        {
            png_destroy_read_struct(&png_ptr, NULL, NULL);
            //cout << "failed to create info_ptr" << endl;
            return;
        }

        png_infop end_info = png_create_info_struct(png_ptr);

        if (!end_info)
        {
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            //cout << "failed to create info_ptr" << endl;
            return;
        }

        if (setjmp(png_jmpbuf(png_ptr)))
        {
            png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
            //cout << "PNG error" << endl;
            return;
        }

        png_init_io(png_ptr, f);
        png_set_sig_bytes(png_ptr, 8);
        png_read_info(png_ptr, info_ptr);

        png_uint_32 width;
        png_uint_32 height;
        int bitDepth;
        int colorType;
        int interlaceMethod;
        int compressionMethod;
        int filterMethod;

        png_get_IHDR(png_ptr, info_ptr, &width, &height, &bitDepth, &colorType,
            &interlaceMethod, &compressionMethod, &filterMethod);

        //cout << width << " x " << height << " x " << bitDepth << endl;

        png_byte channels = png_get_channels(png_ptr, info_ptr);
        //cout << int(channels) << " channels" << endl;

        // strip 16-bit colors to 8-bit colors
        png_set_strip_16(png_ptr);

        // extract bit depths of 1, 2, and 4 into separate bytes
        png_set_packing(png_ptr);

        if (colorType == PNG_COLOR_TYPE_GRAY)
            png_set_expand(png_ptr);

        if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        {
            //cout << "hmmmmmmm" << endl;
        }

        png_read_update_info(png_ptr, info_ptr);
        png_uint_32 rowbytes = png_get_rowbytes(png_ptr, info_ptr);

        //cout << "rowbytes == " << rowbytes << endl;

        png_bytep imageData = (png_bytep)
            malloc(rowbytes * height * sizeof(png_byte));
        png_bytepp rowPointers = (png_bytepp)
            malloc(height * sizeof(png_bytep));

        if (!rowPointers)
        {
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            //cout << "failed to allocate row pointers" << endl;
            return;
        }

        for (png_uint_32 i = 0; i < height; ++i)
            rowPointers[height - 1 - i] = imageData + i * rowbytes;

        png_read_image(png_ptr, rowPointers);

        /// transfer results to the object

        mData = imageData;
        mWidth = width;
        mHeight = height;
        mBitDepth = bitDepth;
        mChannels = channels;

        //cout << "format: ";
        switch (colorType)
        {
            case PNG_COLOR_TYPE_GRAY:
            {
                //cout << "PNG_COLOR_TYPE_GRAY";
                break;
            }

            case PNG_COLOR_TYPE_GRAY_ALPHA:
            {
                // same as PNG_COLOR_MASK_ALPHA
                //cout << "PNG_COLOR_TYPE_GRAY_ALPHA";
                break;
            }

            case PNG_COLOR_TYPE_PALETTE:
            {
                //cout << "PNG_COLOR_TYPE_PALETTE";
                break;
            }

            case PNG_COLOR_TYPE_RGB:
            {
                // same as PNG_COLOR_MASK_COLOR
                //cout << "PNG_COLOR_TYPE_RGB";
                mFormat = GL_RGB;
                break;
            }

            case PNG_COLOR_TYPE_RGB_ALPHA:
            {
                // same as PNG_COLOR_TYPE_RGBA
                //cout << "PNG_COLOR_TYPE_RGB_ALPHA";
                mFormat = GL_RGBA;
                break;
            }

            case PNG_COLOR_MASK_PALETTE:
            {
                //cout << "PNG_COLOR_MASK_PALETTE";
                break;
            }

            default:
            {
                //cout << "lolwut";
            }
        }

        //cout << endl;

        /// final cleanup

        free(rowPointers);
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    }
}
