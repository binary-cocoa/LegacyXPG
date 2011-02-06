# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

SF := ../../../../source
LOCAL_MODULE    := libgl2jni
LOCAL_CFLAGS    := -w -I../../../include
LOCAL_SRC_FILES := xpg-test.cpp \
    TestModule.cpp \
    $(SF)/zlib/adler32.c \
    $(SF)/zlib/compress.c \
    $(SF)/zlib/crc32.c \
    $(SF)/zlib/deflate.c \
    $(SF)/zlib/gzclose.c \
    $(SF)/zlib/gzlib.c \
    $(SF)/zlib/gzread.c \
    $(SF)/zlib/gzwrite.c \
    $(SF)/zlib/infback.c \
    $(SF)/zlib/inffast.c \
    $(SF)/zlib/inflate.c \
    $(SF)/zlib/inftrees.c \
    $(SF)/zlib/trees.c \
    $(SF)/zlib/uncompr.c \
    $(SF)/zlib/zutil.c \
    $(SF)/png/png.c \
    $(SF)/png/pngerror.c \
    $(SF)/png/pngget.c \
    $(SF)/png/pngmem.c \
    $(SF)/png/pngpread.c \
    $(SF)/png/pngread.c \
    $(SF)/png/pngrio.c \
    $(SF)/png/pngrtran.c \
    $(SF)/png/pngrutil.c \
    $(SF)/png/pngset.c \
    $(SF)/png/pngtest.c \
    $(SF)/png/pngtrans.c \
    $(SF)/png/pngwio.c \
    $(SF)/png/pngwrite.c \
    $(SF)/png/pngwtran.c \
    $(SF)/png/pngwutil.c \
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
LOCAL_LDLIBS    := -llog -lGLESv2

include $(BUILD_SHARED_LIBRARY)

