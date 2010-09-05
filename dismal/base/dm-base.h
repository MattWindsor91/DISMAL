/** @file     base/dm-base.h
 *  @author   Matt Windsor (captainhayashi)
 *  @version  0.001
 *  @brief    Header for base interfacing code.
 *
 *  In DISMAL, a base is an interface to a particular platform or
 *  library, used to initialise the generic libraries required for
 *  DISMAL and its modules.
 *
 *  An example is the SDL base, which is used to initialise the main
 *  SDL library.
 *
 *  Specific graphics, sound and input modules depend on a specific
 *  base module - for example, SDL graphics will only work with the
 *  SDL base initialised.
 *
 *  Only one base can be compiled into a program - and therefore only
 *  one input system can be compiled - but a mixture of graphics and
 *  sound drivers can be selected so long as they are compatible with
 *  the base.
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

#ifndef __DM_BASE_H__
#define __DM_BASE_H__

enum {
  DM_SDL = 1,  /**< SDL base ID */
  DM_AMIGA68K, /**< Classic Amiga base ID */
  DM_DOS       /**< MS-DOS base ID */
};

/** Initialise the compiled base.
 *
 *  This will set the value in conf->base_loaded to the ID of the
 *  loaded base.
 *
 *  @param conf  A pointer to a dm_Config structure filled with
 *  initial configuration values.
 *
 *  @return non-zero (DM_SUCCESS) for success, zero (DM_FAILURE) for
 *  failure.
 */
int dm_base_init(struct dm_Config *conf);


/** De-initialise the compiled base. */
void dm_base_cleanup(void);


/** Return the ID of the compiled base.
 *
 *  The base does not have to be loaded in order to call this function
 *  - it merely returns the compile-time base ID.
 *
 *  @return the ID of the base.
 */
int dm_get_base_id(void);

#endif /* __DM_BASE_H__ */
