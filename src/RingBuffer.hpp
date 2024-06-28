#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>
#include <atomic>
#include <type_traits>

namespace app
{

    template <typename T, size_t BufferSize, bool fake_tso = true, size_t cacheline_size = 1, typename index_t = size_t>
    struct RingBuffer
    {

    public:
       
       
        // T &new_back()
        // {
        //     _end += 1;
        //     if (_end >= _buf.size())
        //     {
        //         _end = 0;
        //     }
        //     return _buf[_end];
        // }

        void push(const T &v)
        {
            index_t end = _end.load(std::memory_order_relaxed);
            _buf[end++&BufferMask] = v;
            std::atomic_signal_fence(std::memory_order_release);
            _end.store(end, index_release_barrier);

            // auto new_end = _end + 1;
            // if (new_end >= Tsize)
            // {
            //     new_end = 0;
            // }
            // _end = new_end;
            // _buf[new_end] = v;
        }

        T pop()
        {
            index_t beg = _beg.load(std::memory_order_relaxed);
            std::atomic_signal_fence(std::memory_order_release);
            _beg.store(beg+1, index_release_barrier);
            return _buf[beg&BufferMask];

            // size_t idx = _beg;
            // _beg += 1;
            // if (_beg >= _buf.size())
            // {
            //     _beg = 0;
            // }
            // return _buf[idx];
        }

        bool empty() const
        {
            return size() == 0;
        }

        bool full() const
        {
            return writeAvailable() == 0;
        }

        index_t size() const
        {
            return _end.load(index_acquire_barrier) - _beg.load(std::memory_order_relaxed);
        }

        static constexpr index_t capacity()
        {
            return BufferSize;
        }

        index_t writeAvailable() const
        {
            return BufferSize - (_end.load(index_acquire_barrier) - _beg.load(std::memory_order_relaxed));
        }

    private:
        static constexpr index_t BufferMask = BufferSize - 1;
        static constexpr std::memory_order index_acquire_barrier = fake_tso ? std::memory_order_relaxed
                                                                            : std::memory_order_acquire; // do not load from, or store to buffer before confirmed by the opposite side
        static constexpr std::memory_order index_release_barrier = fake_tso ? std::memory_order_relaxed
                                                                            : std::memory_order_release; // do not update own side before all operations on data_buff committed
        using stored_index_t = std::atomic<index_t>;
        alignas(cacheline_size) stored_index_t _beg{0}; //!< head index
        alignas(cacheline_size) stored_index_t _end{0}; //!< tail index
        alignas(cacheline_size) T _buf[BufferSize];

        static_assert((BufferSize != 0), "buffer cannot be of zero size");
        static_assert((BufferSize & BufferMask) == 0, "buffer size is not a power of 2");
        static_assert(sizeof(index_t) <= sizeof(size_t),
                      "indexing type size is larger than size_t, operation is not lock free and doesn't make sense");
        static_assert(std::numeric_limits<index_t>::is_integer, "indexing type is not integral type");
		static_assert(!(std::numeric_limits<index_t>::is_signed), "indexing type must not be signed");
		static_assert(BufferMask <= ((std::numeric_limits<index_t>::max)() >> 1),
			"buffer size is too large for a given indexing type (maximum size for n-bit type is 2^(n-1))");

    };

} // namespace app