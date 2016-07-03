#ifndef _ALIGNMENT_ALLOCATOR_H
#define _ALIGNMENT_ALLOCATOR_H

#include <immintrin.h>

template <class T, int N>
class AlignedAllocator
{
    public:

    typedef T value_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef size_t size_type;

    template <class U>
    struct rebind
    {
        typedef AlignedAllocator<U,N> other;
    };

    inline AlignedAllocator() throw() {}
    inline AlignedAllocator(const AlignedAllocator&) throw() {}

    template <class U>
    inline AlignedAllocator(const AlignedAllocator<U,N>&) throw() {}

    inline ~AlignedAllocator() throw() {}

    inline pointer address(reference r) { return &r; }
    inline const_pointer address(const_reference r) const { return &r; }

    pointer allocate(size_type n, typename std::allocator<void>::const_pointer hint = 0);
    inline void deallocate(pointer p, size_type);

    inline void construct(pointer p, const_reference value) { new (p) value_type(value); }
    inline void destroy(pointer p) { p->~value_type(); }

    inline size_type max_size() const throw() { return size_type(-1) / sizeof(T); }

    inline bool operator==(const AlignedAllocator&) { return true; }
    inline bool operator!=(const AlignedAllocator& rhs) { return !operator==(rhs); }
};

namespace detail
{
    inline void* _aligned_malloc(size_t size, size_t alignment)
    {
        void* res = 0;
        void* ptr = malloc(size+alignment);
        if(ptr != 0)
        {
            res = reinterpret_cast<void*>((reinterpret_cast<size_t>(ptr) & ~(size_t(alignment-1))) + alignment);
            *(reinterpret_cast<void**>(res) - 1) = ptr;
        }
        return res;
    }
}

inline void* aligned_malloc(size_t size, size_t alignment)
{
    #if MALLOC_ALREADY_ALIGNED
        return malloc(size);
    #elif HAS_MM_MALLOC
        return _mm_malloc(size,alignment);
    #elif HAS_POSIX_MEMALIGN
        void* res;
        const int failed = posix_memalign(&res,size,alignment);
        if(failed) res = 0;
        return res;
    #elif (defined _MSC_VER)
        return _aligned_malloc(size, alignment);
    #else
        return detail::_aligned_malloc(size,alignment);
    #endif
}

namespace detail
{
    inline void _aligned_free(void* ptr)
    {
        if(ptr != 0)
            free(*(reinterpret_cast<void**>(ptr)-1));
    }
}

inline void aligned_free(void* ptr)
{
    #if MALLOC_ALREADY_ALIGNED
        free(ptr);
    #elif HAS_MM_MALLOC
        _mm_free(ptr);
    #elif HAS_POSIX_MEMALIGN
        free(ptr);
    #elif defined(_MSC_VER)
        _aligned_free(ptr);
    #else
        detail::_aligned_free(ptr);
    #endif
}

template <class T, int N>
typename AlignedAllocator<T,N>::pointer
    AlignedAllocator<T,N>::allocate(size_type n, typename std::allocator<void>::const_pointer hint)
    {
        pointer res = reinterpret_cast<pointer>(aligned_malloc(sizeof(T)*n,N));
        if(res == 0)
            throw std::bad_alloc();
        return res;
    }

template <class T, int N>
void AlignedAllocator<T,N>::deallocate(pointer p, size_type)
{
    aligned_free (p);
}

#endif // _ALIGNMENT_ALLOCATOR_H