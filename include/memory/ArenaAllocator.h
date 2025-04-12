#pragma once

#include <new>
#include <cstdlib>
#include <utility>
#include <raylib.h>

class ArenaAllocator
{
public:
    ArenaAllocator(size_t size) : m_size(size), m_offset(0)
    {
        m_buffer = static_cast<char*>(std::malloc(size));
        TraceLog(LOG_INFO, TextFormat("ArenaAllocator: allocated buffer of %d", size));
    }

    ~ArenaAllocator()
    {
        std::free(m_buffer);
        TraceLog(LOG_INFO, TextFormat("ArenaAllocator: released buffer of %d", m_size));
    }

    void* Allocate(size_t size)
    {
        if (m_offset + size > m_size)
        {
            return nullptr;
        }

        void* ptr = m_buffer + m_offset;
        m_offset += size;

        TraceLog(LOG_INFO, TextFormat("ArenaAllocator: allocated size of %d, total allocation is %d", size, m_offset));

        return ptr;
    }

    template <typename T, typename... Args>
    T* Allocate(Args&&... args)
    {
        void* memory = Allocate(sizeof(T));
        if (memory != nullptr)
        {
            return new (memory) T(std::forward<Args>(args)...);
        }

        return nullptr;
    }

    template <typename T, typename... Args>
    T* AllocateArray(size_t size, Args&&... args)
    {
        void* memory = Allocate(sizeof(T) * size);
        if (memory != nullptr)
        {
            T* array = static_cast<T*>(memory);
            for (size_t i = 0; i < size; ++i)
            {
                new (&array[i]) T(std::forward<Args>(args)...);
            }

            return array;
        }

        return nullptr;
    }

    void Reset()
    {
        m_offset = 0;
        TraceLog(LOG_INFO, "ArenaAllocator: reset buffer");
    }

private:
    size_t m_size;
    size_t m_offset;
    char* m_buffer;
};