/** @file     gfx/dm-gfx-sdl.c
 *  @author   Matt Windsor (captainhayashi)
 *  @version  0.001
 *  @brief    SDL implementation of the DISMAL graphical subsystem.
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
#include "SDL/SDL_image.h"

#include "../dismal.h"
#include "dm-gfx.h"
#include "dm-gfx-sdl.h"

static dm_GfxSDLData *_dm_gfxsdl;

void dm_gfx_sdl_register(dm_GfxDriver *driver)
{
  driver->init = dm_gfx_sdl_init;
  driver->update = dm_gfx_sdl_update;
  driver->cleanup = dm_gfx_sdl_cleanup;
  driver->load_image_data = dm_sdl_load_image_data;
  driver->free_image_data = dm_sdl_free_image_data;
  driver->draw_image = dm_sdl_draw_image;
  driver->fill_rect_rgb = dm_sdl_fill_rect_rgb;
}


int dm_gfx_sdl_init(dm_Config *conf)
{
  dm_debug("GFX-SDL: Initialising.");

  if (SDL_InitSubSystem(SDL_INIT_VIDEO) == 0) {

    _dm_gfxsdl = malloc(sizeof(dm_GfxSDLData));

    if (_dm_gfxsdl) {
      dm_debug("%u %u %u\n", conf->gfx_screen_width, 
               conf->gfx_screen_height,
               conf->gfx_screen_depth);
      _dm_gfxsdl->screen = SDL_SetVideoMode(conf->gfx_screen_width,
                                            conf->gfx_screen_height,
                                            conf->gfx_screen_depth,
                                            SDL_SWSURFACE|SDL_ANYFORMAT);
      if (_dm_gfxsdl->screen) {
        return DM_SUCCESS;
      } else {
        dm_fatal("GFX-SDL: Could not initialise screen.");
      }
    } else {
      dm_fatal("GFX-SDL: Could not initialise driver structure.");
    }
  } else {
    dm_fatal("GFX-SDL: Could not initialise SDL-GFX subsystem.");
  }
  return DM_FAILURE;
}

void dm_gfx_sdl_update(void)
{
  SDL_Flip(_dm_gfxsdl->screen);
}

void dm_gfx_sdl_cleanup(void)
{
  if (_dm_gfxsdl) {
    free(_dm_gfxsdl);
    _dm_gfxsdl = NULL;
  }

  SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void *dm_sdl_load_image_data(const char filename[])
{ 
  SDL_Surface *surf;

  surf = IMG_Load(filename);

  if (surf) {
    /* TODO: make this flaggable or something */
    SDL_SetColorKey(surf,
                    SDL_SRCCOLORKEY | SDL_RLEACCEL, 
                    SDL_MapRGB(_dm_gfxsdl->screen->format, 255, 0, 255));;
  } else {
    dm_fatal("GFX-SDL: Couldn't load %s!\n", filename);
  }

  return (void*) surf;
}

void dm_sdl_free_image_data(void *data)
{
  if (data) {
    SDL_FreeSurface(data);
    data = NULL;
  }
}

int dm_sdl_draw_image(struct dm_GfxImageNode *image,
                      unsigned int image_x,
                      unsigned int image_y,
                      unsigned int screen_x,
                      unsigned int screen_y,
                      unsigned int width,
                      unsigned int height)
{
  SDL_Rect srcrect, destrect;
  SDL_Surface *ptex;

  ptex = (SDL_Surface*) image->data;

  srcrect.x = image_x;
  srcrect.y = image_y;

  destrect.x = screen_x;
  destrect.y = screen_y;

  srcrect.w = destrect.w = width;
  srcrect.h = destrect.h = height;

  if (ptex) {
    SDL_BlitSurface(ptex, &srcrect, _dm_gfxsdl->screen, &destrect);
    return DM_SUCCESS;
  } else {
    return DM_FAILURE;
  }
}


void dm_sdl_fill_rect_rgb(unsigned int x,
                          unsigned int y,
                          unsigned int w,
                          unsigned int h,
                          unsigned int r,
                          unsigned int g,
                          unsigned int b)
{
  SDL_Rect rect;

  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;

  SDL_FillRect(_dm_gfxsdl->screen, &rect,
               SDL_MapRGB(_dm_gfxsdl->screen->format, 
                          r, g, b));
}
