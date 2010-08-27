/** @file     /dismal.h
 *  @author   Matt Windsor (captainhayashi)
 *  @version  0.001
 *  @brief    Main header for the DISMAL engine.
 */

/* Driver logic based heavily on MegaZeux's rendering system. */

#ifndef __DISMAL_H__
#define __DISMAL_H__

#include <stdlib.h>
#include <stdio.h>

enum {
  /** Boolean logic */
  DM_SUCCESS = 1, /**< An operation concluded successfully; this is the
                      same as DM_TRUE. */
  DM_FAILURE = 0, /**< An operation concluded unsuccessfully; this is
                     the same as DM_FALSE. */
  DM_TRUE = 1, /**< Boolean true. */
  DM_FALSE = 0 /**< Boolean false. */
};

typedef struct dm_Master dm_Master;
typedef struct dm_Config dm_Config;


struct dm_Master {
  dm_Config *conf; /**< Current configuration. */
};

struct dm_Config {
  /* NOTE: On lo-res (Amiga AGA, DOS VGA) targets, the screen
  resolution will always be 320x200 8bpp. 

  If you are supporting lo-res platforms, ensure that you check for
  the flag dm_is_lores and double-check the dm_scr_w, dm_scr_h and
  dm_scr_d values before using the graphics system, and that your
  program will run fine in 320x200 8bpp. */

  FILE *debug_file; /**< File to send debug information through. */

  int gfx_screen_width; /**< Desired screen width on hi-res (SDL, OpenGL)
                           targets. */
  int gfx_screen_height; /**< Desired screen height on hi-res (SDL,
                            OpenGL) targets. */
  int gfx_screen_depth; /**< Desired screen depth on hi-res (SDL,
                           OpenGL) targets. */
  int gfx_flags; /**< Bit-field of flags. */
};

/** Initialise DISMAL.
 *
 *  @return non-zero (DM_SUCCESS) for success, zero (DM_FAILURE) for
 *  failure.
 */

int dm_init();


/** Initialise the configuration structure.
 *
 *  This will, if given a null pointer, create a configuration
 *  structure at _conf and populate it with default values. If a
 *  non-null pointer is given, the function will assume it is already
 *  initialised and return DM_SUCCESS without doing anything.
 *
 *  @param conf  A pointer to a dm_Config structure to be filled with
 *  initial configuration values.
 *
 *  @return DM_SUCCESS if the configuration is initialised or assumed
 *  initialised, DM_SUCCESS if there was an error (eg out of memory).
 */

int dm_config_init();

/** De-initialise DISMAL.
 */

void dm_cleanup(void);


/** Show a fatal error message.
 *
 *  @param str  The printf format to use.
 *  @param ...  The variables used in the printf format.
 */

void dm_fatal(const char *str, ...);

/** Show a debug message.
 *
 *  @param str  The printf format to use.
 *  @param ...  The variables used in the printf format.
 */

void dm_debug(const char *str, ...);


/** Set the resolution.
 *
 *  @param width   Width of the screen (must be at least 320).
 *  @param height  Height of the screen (must be at least 200).
 */
void dm_set_resolution(unsigned short width, unsigned short height);

/* Include other headers for convenience. */
#include "base/dm-base.h"
#include "gfx/dm-gfx.h"
#include "input/dm-input.h"

#endif /* __DISMAL_H__ */
