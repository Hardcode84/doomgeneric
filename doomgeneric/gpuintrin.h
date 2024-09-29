#ifndef GPUINTRIN_H
#define GPUINTRIN_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __NVPTX__
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

#endif // GPUINTRIN_H
