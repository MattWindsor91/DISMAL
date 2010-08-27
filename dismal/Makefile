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

SOURCES  += dismal/dismal.c dismal/gfx/dm-gfx.c dismal/base/dm-base.c dismal/input/dm-input.c

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
  SOURCES  += dismal/base/dm-base-sdl.c

  ifeq ($(DM_INCLUDE_GFX), yes)
    CFLAGS   += -DDM_GFX_SDL
    SOURCES  += dismal/gfx/dm-gfx-sdl.c
    ifdef DM_OPENGL
      CFLAGS   += -DDM_GFX_SDL_OPENGL
    endif
  endif

  ifeq ($(DM_INCLUDE_INPUT), yes)
    CFLAGS   += -DDM_INPUT_SDL
    SOURCES  += dismal/input/dm-input-sdl.c
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
