/** @file     dismal.c
 *  @author   Matt Windsor (captainhayashi)
 *  @version  0.001
 *  @brief    Initialising routines for the DISMAL engine.
 */

/**************************************************************************
 *                                                                        *
 *  Copyright 2010       CaptainHayashi etc.                              *
 *                                                                        *
 *  This file is part of DISMAL.                                          *
 *                                                                        *
 *  DISMAL is free software: you can redistribute it and/or modify        *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 3 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  DISMAL is distributed in the hope that it will be useful,             *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with DISMAL.  If not, see <http://www.gnu.org/licenses/>.       *
 *                                                                        *
 **************************************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "dismal.h"
#include "gfx/dm-gfx.h"

dm_Config *_conf = NULL;

int
dm_init (void)
{  
  dm_debug ("CORE: Initialising DISMAL.\n");

  /* If config has not already been created, make it now. */
  if (dm_config_init () == DM_SUCCESS) 
    {

    if (dm_base_init (_conf) == DM_FAILURE)
      return DM_FAILURE;

    if (DM_GFX)
      {
        if (dm_gfx_init (_conf) == DM_FAILURE)
          return DM_FAILURE;
      }

    /* Initialise input last because otherwise fun stuff like 
       SDL forgetting to use unicode can happen. */

    if (DM_INPUT)
      {
        if (dm_input_init (_conf) == DM_FAILURE)
          return DM_FAILURE;
      }

    }
  else
    {
      dm_fatal("CORE: Unable to init config. (Out of memory?)");
      return DM_FAILURE;
    }

  return DM_SUCCESS;
}

void
dm_cleanup (void)
{
  dm_debug("Closing DISMAL. Have a nice day.\n");

  if (_conf)
    free (_conf);

  if (DM_INPUT)
    dm_input_cleanup ();

  if (DM_GFX)
    dm_gfx_cleanup ();

  dm_base_cleanup ();
}

int
dm_config_init (void)
{
  /* If config does not exist, create it. */

  if (_conf == NULL)
    {
      _conf = malloc (sizeof (struct dm_Config));

      if (_conf)
        {
          /* Pre-initialise here. */
          _conf->debug_file = stderr;
          _conf->gfx_screen_width = 640;
          _conf->gfx_screen_height = 400;
          _conf->gfx_screen_depth = 32;
          _conf->gfx_flags = DM_GFX_AUTO_TRANSLATE;
        }
      else
        {
          return DM_FAILURE;
        }
    }

  return DM_SUCCESS;
}

void
dm_set_resolution (unsigned short width, unsigned short height)
{
  if (dm_config_init () == DM_SUCCESS)
    {
    _conf->gfx_screen_width = width;
    _conf->gfx_screen_height = height;
    }
}

void
dm_set_gfx_flag (unsigned short flag_id, unsigned short value)
{
  if (value)
    _conf->gfx_flags |= flag_id;
  else
    _conf->gfx_flags &= ~flag_id;
}

unsigned short
dm_get_gfx_flag (unsigned short flag_id)
{
  if (_conf->gfx_flags & flag_id)
    return DM_TRUE;
  else
    return DM_FALSE;
}


DM_INLINE void
dm_fatal (const char *str, ...)
{
  va_list ap;

  va_start (ap, str);
  fprintf (stderr, "FATAL: ");
  vfprintf (stderr, str, ap);
  fprintf (stderr, "\n");
  va_end (ap);

  fflush (stderr);
}

DM_INLINE void
dm_debug (const char *str, ...)
{
  if (DEBUG)
    {
      va_list ap;
      FILE *df;

      /* Select debug log file. */
      if (_conf && _conf->debug_file)
        df = _conf->debug_file;
      else
        df = stderr;

      va_start (ap, str);

      fprintf (df, "DEBUG: ");
      vfprintf (df, str, ap);
      fprintf (df, "\n");
  
      va_end (ap);
    }
}
