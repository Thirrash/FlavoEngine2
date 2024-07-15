#pragma once

namespace flavo::utils
{
    template<typename T>
    T* CopyRawArray(const T* src, size_t size)
    {
        if (src == nullptr)
            return nullptr;

        if (size == 0)
            return nullptr;

        T* dst = new T[size];
        for (size_t i = 0; i < size; ++i)
        {
            dst[i] = src[i];
        }

        return dst;
    }
}
