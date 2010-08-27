/** @file     dismal.c
 *  @author   Matt Windsor (captainhayashi)
 *  @version  0.001
 *  @brief    Initialising routines for the DISMAL engine.
 */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "dismal.h"
#include "gfx/dm-gfx.h"

dm_Config *_conf = NULL;

int dm_init(void)
{  
  dm_debug("CORE: Initialising DISMAL.\n");

  /* If config has not already been created, make it now. */
  if (dm_config_init() == DM_SUCCESS) {
    if (dm_base_init(_conf) == DM_FAILURE) {
      return DM_FAILURE;
    }

#ifdef DM_GFX
    if (dm_gfx_init(_conf) == DM_FAILURE) {
      return DM_FAILURE;
    }
#endif /* DM_GFX */

    /* Initialise input last because otherwise fun stuff like 
       SDL forgetting to use unicode can happen. */

#ifdef DM_INPUT
    if (dm_input_init(_conf) == DM_FAILURE) {
      return DM_FAILURE;
    }
#endif /* DM_INPUT */

  } else {
    dm_fatal("CORE: Unable to init config. (Out of memory?)");
    return DM_FAILURE;
  }

  return DM_SUCCESS;
}

void dm_cleanup(void)
{
  dm_debug("Closing DISMAL. Have a nice day.\n");

  if (_conf) {

    free(_conf);
  }

#ifdef DM_INPUT
  dm_input_cleanup();
#endif /* DM_INPUT */

#ifdef DM_GFX
  dm_gfx_cleanup();
#endif /* DM_GFX */

  dm_base_cleanup();
}

int dm_config_init(void)
{
  /* If config does not exist, create it. */

  if (_conf == NULL) {
    _conf = malloc(sizeof(struct dm_Config));

    if (_conf) {
      /* Pre-initialise here. */
      _conf->debug_file = stderr;
      _conf->gfx_screen_width = 640;
      _conf->gfx_screen_height = 400;
      _conf->gfx_screen_depth = 32;
      _conf->gfx_flags = DM_GFX_AUTO_RESIZE;
    } else {
      return DM_FAILURE;
    }
  }

  return DM_SUCCESS;
}

void dm_set_resolution(unsigned short width, unsigned short height)
{
  if (dm_config_init() == DM_SUCCESS) {
    _conf->gfx_screen_width = width;
    _conf->gfx_screen_height = height;
  }
}


void dm_fatal(const char *str, ...)
{
  va_list ap;

  va_start(ap, str);
  fprintf(stderr, "FATAL: ");
  vfprintf(stderr, str, ap);
  fprintf(stderr, "\n");
  va_end(ap);

  fflush(stderr);
}

void dm_debug(const char *str, ...)
{
#ifdef DEBUG
  va_list ap;
  FILE *df;

  if (_conf && _conf->debug_file) {
    df = _conf->debug_file;
  } else {
    df = stderr;
  }

  va_start(ap, str);

  fprintf(df, "DEBUG: ");
  vfprintf(df, str, ap);
  fprintf(df, "\n");
  
  va_end(ap);
#endif /* DEBUG */
}
