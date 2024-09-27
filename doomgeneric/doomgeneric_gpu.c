
#include <time.h>
#include <stdio.h>

#include "doomkeys.h"
#include "m_argv.h"
#include "doomgeneric.h"


#define NS_IN_MS 1000000L

void DG_Init(){

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
  return 0;
}

void DG_SetWindowTitle(const char * title)
{

}

int main(int argc, char **argv, char **envp)
{
  // uint32_t thread_id = __builtin_amdgcn_workitem_id_x();
  uint32_t thread_id = __nvvm_read_ptx_sreg_tid_x();
  if (thread_id == 0)
  {
    doomgeneric_Create(argc, argv);

    uint32_t time = DG_GetTicksMs();
    uint32_t last_tick = 0;
    for (int i = 0; ; i++)
    {
        doomgeneric_Tick();

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
