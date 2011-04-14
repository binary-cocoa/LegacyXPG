#ifndef XPGH_VECTOR3D
#define XPGH_VECTOR3D

#include "Platforms.hpp"

#ifdef XPG_PLATFORM_ANDROID
#   include <math.h>
#   include <string.h>
#else
#   include <cmath>
#   include <cstring>
#   include <iostream>
#endif

namespace XPG
{
    template<typename T> class Vector2;
    template<typename T> class Vector3;
    template<typename T> class Vector4;

    /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// ///

    template<size_t N, typename T>
    class VectorN
    {
    public:
        VectorN<N, T>();
        VectorN<N, T>(const VectorN<N, T>& inVector);

        void set(T inValue);
        void normalizeTo(T inLength);
        void normalize();
        void negate();

        inline operator T*() { return mData; }
        inline operator const T*() const { return mData; }

        inline T* array() { return mData; }
        inline const T* array() const { return mData; }

        inline T lengthSquared() const
        {
            T outLength = (mData[0] * mData[0]);

            for (size_t i = 1; i < (N < 4 ? N : 3); ++i)
                outLength += (mData[i] * mData[i]);

            return outLength;
        }

        inline T length() const { return sqrt(lengthSquared()); }

    protected:
        T mData[N];
    };

    template<size_t N, typename T>
    VectorN<N, T>::VectorN()
    {
        memset(mData, 0, sizeof(T) * (N < 4 ? N : 3));
        if (N == 4) mData[3] = static_cast<T>(1);
    }

    template<size_t N, typename T>
    void VectorN<N, T>::set(T inValue)
    {
        for (size_t i = 0; i < N; ++i) mData[i] = inValue;
    }

    template<size_t N, typename T>
    void VectorN<N, T>::normalizeTo(T inLength)
    {
        T l = length();
        l = inLength / l;
        for (size_t i = 0; i < (N < 4 ? N : 3); ++i) mData[i] *= l;
    }

    template<size_t N, typename T>
    void VectorN<N, T>::normalize()
    {
        T l = length();
        for (size_t i = 0; i < (N < 4 ? N : 3); ++i) mData[i] /= l;
    }

    template<size_t N, typename T>
    void VectorN<N, T>::negate()
    {
        for (size_t i = 0; i < N; ++i) mData[i] *= static_cast<T>(-1);
    }

#ifndef XPG_PLATFORM_ANDROID
    template<size_t N, typename T>
    std::istream& operator>>(std::istream& inStream, VectorN<N, T>& inVector)
    {
        for (size_t i = 0; i < N; ++i) inStream >> inVector[i];
        return inStream;
    }

    template<size_t N, typename T>
    std::ostream& operator<<(std::ostream& inStream,
        const VectorN<N, T>& inVector)
    {
        inStream << inVector[0];
        for (size_t i = 1; i < N; ++i) inStream << ' ' << inVector[i];
        return inStream;
    }
#endif

    /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// ///

    template<typename T>
    class Vector2 : public VectorN<2, T>
    {
        public:
            Vector2<T>();
            Vector2<T>(T inX, T inY);
            Vector2<T>(const T* inArray);
            Vector2<T>(const Vector2<T>& inVector);
            Vector2<T>(const Vector3<T>& inVector);
            Vector2<T>(const Vector4<T>& inVector);
    };

    template<typename T>
    Vector2<T>::Vector2() : VectorN<2, T>()
    {
    }

    template<typename T>
    Vector2<T>::Vector2(T inX, T inY)
    {
        VectorN<2, T>::mData[0] = inX;
        VectorN<2, T>::mData[1] = inY;
    }

    template<typename T>
    Vector2<T>::Vector2(const T* inArray)
    {
        memcpy(VectorN<2, T>::mData, inArray, 2 * sizeof(T));
    }

    template<typename T>
    Vector2<T>::Vector2(const Vector2<T>& inVector)
    {
        memcpy(VectorN<2, T>::mData, inVector.array(), 2 * sizeof(T));
    }

    template<typename T>
    Vector2<T>::Vector2(const Vector3<T>& inVector)
    {
        memcpy(VectorN<2, T>::mData, inVector.array(), 2 * sizeof(T));
    }

    template<typename T>
    Vector2<T>::Vector2(const Vector4<T>& inVector)
    {
        memcpy(VectorN<2, T>::mData, inVector.array(), 2 * sizeof(T));
    }

    /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// ///

    template<typename T>
    class Vector3 : public VectorN<3, T>
    {
        public:
            Vector3<T>();
            Vector3<T>(T inX, T inY, T inZ);
            Vector3<T>(const T* inArray);
            Vector3<T>(const Vector2<T>& inVector);
            Vector3<T>(const Vector3<T>& inVector);
            Vector3<T>(const Vector4<T>& inVector);
    };

    template<typename T>
    Vector3<T>::Vector3() : VectorN<3, T>()
    {
    }

    template<typename T>
    Vector3<T>::Vector3(T inX, T inY, T inZ)
    {
        VectorN<3, T>::mData[0] = inX;
        VectorN<3, T>::mData[1] = inY;
        VectorN<3, T>::mData[2] = inZ;
    }

    template<typename T>
    Vector3<T>::Vector3(const T* inArray)
    {
        memcpy(VectorN<3, T>::mData, inArray, 3 * sizeof(T));
    }

    template<typename T>
    Vector3<T>::Vector3(const Vector2<T>& inVector)
    {
        memcpy(VectorN<3, T>::mData, inVector.array(), 2 * sizeof(T));
        VectorN<3, T>::mData[2] = static_cast<T>(0);
    }

    template<typename T>
    Vector3<T>::Vector3(const Vector3<T>& inVector)
    {
        memcpy(VectorN<3, T>::mData, inVector.array(), 3 * sizeof(T));
    }

    template<typename T>
    Vector3<T>::Vector3(const Vector4<T>& inVector)
    {
        memcpy(VectorN<3, T>::mData, inVector.array(), 3 * sizeof(T));
    }

    /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// ///

    template<typename T>
    class Vector4 : public VectorN<4, T>
    {
        public:
            Vector4<T>();
            Vector4<T>(T inX, T inY, T inZ, T inW);
            Vector4<T>(const T* inArray);
            Vector4<T>(const Vector2<T>& inVector);
            Vector4<T>(const Vector3<T>& inVector);
            Vector4<T>(const Vector4<T>& inVector);
    };

    template<typename T>
    Vector4<T>::Vector4() : VectorN<4, T>()
    {
    }

    template<typename T>
    Vector4<T>::Vector4(T inX, T inY, T inZ, T inW)
    {
        VectorN<4, T>::mData[0] = inX;
        VectorN<4, T>::mData[1] = inY;
        VectorN<4, T>::mData[2] = inZ;
        VectorN<4, T>::mData[3] = inW;
    }

    template<typename T>
    Vector4<T>::Vector4(const T* inArray)
    {
        memcpy(VectorN<4, T>::mData, inArray, 4 * sizeof(T));
    }

    template<typename T>
    Vector4<T>::Vector4(const Vector2<T>& inVector)
    {
        memcpy(VectorN<4, T>::mData, inVector.array(), 2 * sizeof(T));
        VectorN<4, T>::mData[2] = static_cast<T>(0);
        VectorN<4, T>::mData[3] = static_cast<T>(1);
    }

    template<typename T>
    Vector4<T>::Vector4(const Vector3<T>& inVector)
    {
        memcpy(VectorN<4, T>::mData, inVector.array(), 3 * sizeof(T));
        VectorN<4, T>::mData[3] = static_cast<T>(1);
    }

    template<typename T>
    Vector4<T>::Vector4(const Vector4<T>& inVector)
    {
        memcpy(VectorN<4, T>::mData, inVector.array(), 4 * sizeof(T));
    }
}

#endif
