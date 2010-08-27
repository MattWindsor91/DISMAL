/** @file     input/dm-input-sdl.h
 *  @author   Matt Windsor (captainhayashi)
 *  @version  0.001
 *  @brief    Header for SDL input driver.
 */

#ifndef __DM_INPUT_SDL_H__
#define __DM_INPUT_SDL_H__

#include "SDL/SDL.h"
#include "../dismal.h"

/** Initialise the compiled input module.
 *
 *  This will initialise the internal 
 *
 *  @param conf  A pointer to a dm_Config structure filled with
 *  initial configuration values.
 *
 *  @return non-zero (DM_SUCCESS) for success, zero (DM_FAILURE) for
 *  failure.
 */
int dm_input_sdl_init(struct dm_Config *conf);


/** De-initialise the compiled input module. */
void dm_input_sdl_cleanup(void);


/** Process one frame of input.
 *
 *  This function calls the base-specific input routines to handle any
 *  pending input events, and trigger any relevant callbacks.
 *
 */
void dm_input_sdl_process(void);

/** Handle a SDL mouse motion event.
 *
 *  @param event     Event union to populate with information.
 *  @param sdlevent  SDL event to extract motion data from.
 */
void dm_sdl_mouse_motion(dm_InputEvent *event, SDL_Event *sdlevent);

#endif /* __DM_INPUT_SDL_H__ */
