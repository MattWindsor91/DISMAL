/** @file     base/dm-base-sdl.c
 *  @author   Matt Windsor (captainhayashi)
 *  @version  0.001
 *  @brief    SDL base implementation.
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
