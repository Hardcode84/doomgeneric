
#include <time.h>
#include <stdio.h>

#include "doomkeys.h"
#include "m_argv.h"
#include "doomgeneric.h"

#define NS_IN_MS 1000000L

#define PROTECTED_VIS __attribute__ ((visibility ("protected")))

int DG_GPU_ResX PROTECTED_VIS;
int DG_GPU_Resy PROTECTED_VIS;
void* DG_GPU_ScreenBuffer PROTECTED_VIS;

#define KEYQUEUE_SIZE 16
unsigned short* DG_GPU_KeyQueue PROTECTED_VIS;
#define KeyQueueReadIndex (DG_GPU_KeyQueue[KEYQUEUE_SIZE])
#define KeyQueueWriteIndex (DG_GPU_KeyQueue[KEYQUEUE_SIZE + 1])

void DG_Init()
{
  DG_GPU_ResX = DOOMGENERIC_RESX;
  DG_GPU_Resy = DOOMGENERIC_RESY;
  DG_GPU_ScreenBuffer = DG_ScreenBuffer;
}

void DG_DrawFrame()
{

}

void DG_SleepMs(uint32_t ms)
{
  struct timespec tim;
  tim.tv_sec  = ms / 1000;
  tim.tv_nsec = (NS_IN_MS * ms) % (NS_IN_MS * 1000L);
  nanosleep(&tim, NULL);
}

uint32_t DG_GetTicksMs()
{
  struct timespec tim;
  clock_gettime(CLOCK_MONOTONIC, &tim);
  return (uint32_t)(tim.tv_sec * 1000 + tim.tv_nsec / NS_IN_MS);
}

int DG_GetKey(int* pressed, unsigned char* doomKey)
{
  if (KeyQueueReadIndex == KeyQueueWriteIndex){
    //key queue is empty
    return 0;
  } else {
    unsigned short keyData = DG_GPU_KeyQueue[KeyQueueReadIndex];
    KeyQueueReadIndex++;
    KeyQueueReadIndex %= KEYQUEUE_SIZE;

    *pressed = keyData >> 8;
    *doomKey = keyData & 0xFF;

    return 1;
  }

  return 0;
}

void DG_SetWindowTitle(const char * title)
{

}

extern void _gpu_host_barrier();

int main(int argc, char **argv, char **envp)
{
  // uint32_t thread_id = __builtin_amdgcn_workitem_id_x();
  uint32_t thread_id = __nvvm_read_ptx_sreg_tid_x();
  if (thread_id == 0)
  {
    doomgeneric_Create(argc, argv);
    _gpu_host_barrier();

    uint32_t time = DG_GetTicksMs();
    uint32_t last_tick = 0;
    for (int i = 0; ; i++)
    {
        doomgeneric_Tick();
        _gpu_host_barrier();
        doomgeneric_Draw();
        _gpu_host_barrier();

        int interval = 10;
        if (i % interval == 0)
        {
          uint32_t new_time = DG_GetTicksMs();
          uint32_t diff = (new_time - time);
          if (diff > 2000)
          {
            float fps = (float)(i - last_tick) / (diff / 1000.0f);
            last_tick = i;
            time = new_time;
            printf("fps %f\n", fps);
          }
        }
    }
    
  }

    return 0;
}
