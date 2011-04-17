LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libxpgm
LOCAL_CFLAGS    := -w -I../../include
SF := ../../../source
LOCAL_SRC_FILES := xpgm.cpp \
    TestModule.cpp \
    zlib/adler32.c \
    zlib/compress.c \
    zlib/crc32.c \
    zlib/deflate.c \
    zlib/gzclose.c \
    zlib/gzlib.c \
    zlib/gzread.c \
    zlib/gzwrite.c \
    zlib/infback.c \
    zlib/inffast.c \
    zlib/inflate.c \
    zlib/inftrees.c \
    zlib/trees.c \
    zlib/uncompr.c \
    zlib/zutil.c \
    libpng/png.c \
    libpng/pngerror.c \
    libpng/pngget.c \
    libpng/pngmem.c \
    libpng/pngpread.c \
    libpng/pngread.c \
    libpng/pngrio.c \
    libpng/pngrtran.c \
    libpng/pngrutil.c \
    libpng/pngset.c \
    libpng/pngtrans.c \
    libpng/pngwio.c \
    libpng/pngwrite.c \
    libpng/pngwtran.c \
    libpng/pngwutil.c \
    $(SF)/XPG/Module.cpp \
    $(SF)/XPG/Event.cpp \
    $(SF)/XPG/Image.cpp \
    $(SF)/XPG/image/png.cpp \
    $(SF)/XPG/unix/Timer.cpp \
    $(SF)/XPG/OpenGL/Base.cpp \
    $(SF)/XPG/OpenGL/Shader.cpp \
    $(SF)/XPG/OpenGL/Program.cpp \
    $(SF)/XPG/OpenGL/Texture.cpp \
    $(SF)/XPG/OpenGL/Texture2D.cpp \
    $(SF)/XPG/OpenGL/VertexBufferObject.cpp
LOCAL_LDLIBS    := -lGLESv2

include $(BUILD_SHARED_LIBRARY)

