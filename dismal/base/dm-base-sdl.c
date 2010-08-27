/** @file     base/dm-base-sdl.c
 *  @author   Matt Windsor (captainhayashi)
 *  @version  0.001
 *  @brief    SDL base implementation.
 */

#include "SDL/SDL.h"

#include "../dismal.h"
#include "dm-base-sdl.h"

int dm_base_sdl_init(dm_Config *conf)
{
  if (SDL_Init(0) == 0) {
    return DM_SUCCESS;
  } else {
    dm_fatal("BASE-SDL: Could not initialise SDL.");
    return DM_FAILURE;
  }
}

void dm_base_sdl_cleanup(void)
{
  SDL_Quit();
}
