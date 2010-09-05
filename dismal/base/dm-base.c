/** @file     base/dm-base.c
 *  @author   Matt Windsor (captainhayashi)
 *  @version  0.001
 *  @brief    Base interfacing code.
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

#include "../dismal.h"
#include "dm-base.h"

/* Include the relevant base header. */

#ifdef DM_BASE_SDL
#include "dm-base-sdl.h"
#else /* !DM_BASE_SDL */

#ifdef DM_BASE_AMIGA68K
#include "dm-base-amiga68k.h"
#else /* !DM_BASE_AMIGA68K */

#ifdef DM_BASE_DOS
#include "dm-base-dos.h"
#else /* !DM_BASE_DOS */

#error No base selected!

#endif /* DM_BASE_DOS */
#endif /* DM_BASE_AMIGA68K */
#endif /* DM_BASE_SDL */


int dm_base_init(dm_Config *conf)
{
#ifdef DM_BASE_SDL
  return dm_base_sdl_init(conf);
#else /* !DM_BASE_SDL */

#ifdef DM_BASE_AMIGA68K
  return dm_base_amiga68k_init(conf);
#else /* !DM_BASE_AMIGA68K */

#ifdef DM_BASE_DOS
  return dm_base_dos_init(conf);
#else /* !DM_BASE_DOS */

#error No base selected!

#endif /* DM_BASE_DOS */
#endif /* DM_BASE_AMIGA68K */
#endif /* DM_BASE_SDL */
}

void dm_base_cleanup(void)
{
#ifdef DM_BASE_SDL
  dm_base_sdl_cleanup();
#else /* !DM_BASE_SDL */

#ifdef DM_BASE_AMIGA68K
  dm_base_amiga68k_cleanup();
#else /* !DM_BASE_AMIGA68K */

#ifdef DM_BASE_DOS
  dm_base_dos_cleanup();
#else /* !DM_BASE_DOS */

#error No base selected!

#endif /* DM_BASE_DOS */
#endif /* DM_BASE_AMIGA68K */
#endif /* DM_BASE_SDL */
}


int dm_get_base_id(void)
{
#ifdef DM_BASE_SDL
  return DM_SDL;
#else /* !DM_BASE_SDL */

#ifdef DM_BASE_AMIGA68K
  return DM_AMIGA68K;
#else /* !DM_BASE_AMIGA68K */

#ifdef DM_BASE_DOS
  return DM_DOS;
#else /* !DM_BASE_DOS */

#error No base selected!

#endif /* DM_BASE_DOS */
#endif /* DM_BASE_AMIGA68K */
#endif /* DM_BASE_SDL */
}
