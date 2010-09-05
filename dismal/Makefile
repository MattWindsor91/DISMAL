##########################################################################
#                                                                        #
#  Copyright 2010       CaptainHayashi etc.                              #
#                                                                        #
#  This file is part of DISMAL.                                          #
#                                                                        #
#  DISMAL is free software: you can redistribute it and/or modify        #
#  it under the terms of the GNU General Public License as published by  #
#  the Free Software Foundation, either version 3 of the License, or     #
#  (at your option) any later version.                                   #
#                                                                        #
#  DISMAL is distributed in the hope that it will be useful,             #
#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
#  GNU General Public License for more details.                          #
#                                                                        #
#  You should have received a copy of the GNU General Public License     #
#  along with DISMAL.  If not, see <http://www.gnu.org/licenses/>.       #
#                                                                        #
##########################################################################

SHELL = /bin/sh

ifndef DM_BASE
  DM_BASE = sdl
endif

# By default, init everything

ifndef DM_INCLUDE_GFX
  DM_INCLUDE_GFX = yes
endif

ifndef DM_INCLUDE_INPUT
  DM_INCLUDE_INPUT = yes
endif

SOURCES  += $(DISMALROOT)dismal/dismal.c \
            $(DISMALROOT)dismal/gfx/dm-gfx.c \
            $(DISMALROOT)dismal/base/dm-base.c \
            $(DISMALROOT)dismal/input/dm-input.c

OBJ       = $(subst .c,.o,$(SOURCES))
DEPFILES  = $(subst .c,.d,$(SOURCES))

# Assume CC and cflags inherited from main program

# Set up conditional compiles for modules

ifeq ($(DM_INCLUDE_GFX), yes)
  CFLAGS   += -DDM_GFX
endif

ifeq ($(DM_INCLUDE_INPUT), yes)
  CFLAGS   += -DDM_INPUT
endif

# SDL - use SDL and/or OpenGL driver
ifeq ($(DM_BASE), sdl)
  CFLAGS   += -DDM_BASE_SDL
  SOURCES  += $(DISMALROOT)dismal/base/dm-base-sdl.c

  ifeq ($(DM_INCLUDE_GFX), yes)
    CFLAGS   += -DDM_GFX_SDL
    SOURCES  += $(DISMALROOT)dismal/gfx/dm-gfx-sdl.c
    ifdef DM_OPENGL
      CFLAGS   += -DDM_GFX_SDL_OPENGL
    endif
  endif

  ifeq ($(DM_INCLUDE_INPUT), yes)
    CFLAGS   += -DDM_INPUT_SDL
    SOURCES  += $(DISMALROOT)dismal/input/dm-input-sdl.c
  endif

endif

# Amiga 68k - use AGA driver
ifeq ($(DM_BASE), amiga68k)
  CFLAGS   += -DDM_BASE_AMIGA68K -DDM_GFX_AGA
endif

# IBM PC with DOS - use VGA driver
ifeq ($(DM_BASE), dos)
  CFLAGS   += -DDM_BASE_DOS -DDM_GFX_VGA
endif
