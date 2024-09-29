#ifndef GPUINTRIN_H
#define GPUINTRIN_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __NVPTX__
#define _private  __attribute__((opencl_private))
#define _constant __attribute__((opencl_constant))
#define _local    __attribute__((opencl_local))
#define _global   __attribute__((opencl_global))

static inline uint32_t _get_thread_id_x()
{
    return __nvvm_read_ptx_sreg_tid_x();
}

static inline uint32_t _get_num_threads_x()
{
    return __nvvm_read_ptx_sreg_ntid_x();
}

static inline void _sync_threads()
{
    __syncthreads();
}
#endif


#define BROADCAST_FROM_THREAD0(var, T)                                         \
    do                                                                         \
    {                                                                          \
        static T _local var##_local_temp;                                      \
        if (_get_thread_id_x() == 0)                                           \
            var_##_local_temp = var;                                           \
        _sync_threads();                                                       \
        var = var_##_local_temp;                                               \
    } while (0)

#endif // GPUINTRIN_H
