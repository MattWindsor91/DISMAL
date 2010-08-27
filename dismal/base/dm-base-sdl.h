/** @file     base/dm-base-sdl.h
 *  @author   Matt Windsor (captainhayashi)
 *  @version  0.001
 *  @brief    Header for SDL base.
 */

#ifndef __DM_BASE_SDL_H__
#define __DM_BASE_SDL_H__


/** Initialise the SDL base.
 *
 *  @param conf  A pointer to a dm_Config structure filled with
 *  initial configuration values.
 *
 *  @return non-zero (DM_SUCCESS) for success, zero (DM_FAILURE) for
 *  failure.
 */

int dm_base_sdl_init(struct dm_Config *conf);


/** De-initialise the SDL base. */
void dm_base_sdl_cleanup();

#endif /* __DM_BASE_SDL_H__ */
