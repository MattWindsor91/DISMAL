/** @file     gfx/dm-gfx.c
 *  @author   Matt Windsor (captainhayashi)
 *  @version  0.001
 *  @brief    Generic section of DISMAL graphics subsystem.
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

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../dismal.h"
#include "dm-gfx.h"

#ifdef DM_GFX_SDL
#include "dm-gfx-sdl.h"
#endif

static const dm_GfxDriverSpec dm_driver_specs[] = {

  /* -- Drivers using SDL base -- */
#ifdef DM_BASE_SDL

#ifdef DM_GFX_SDL
  {"sdl", dm_gfx_sdl_register},
#endif /* DM_GFX_SDL */

#ifdef DM_GFX_SDL_OPENGL
  {"sdl-opengl", dm_gfx_sdl_register},
#endif /* DM_GFX_SDL_OPENGL */

#endif /* DM_BASE_SDL */

  /* -- Drivers using Classic Amiga base -- */

  /* -- Drivers using PC/DOS base -- */

  /* -- Null driver (used to trap compilation errors) -- */
  {NULL, NULL}
};

dm_GfxData *dm_gfxdata;

int
dm_gfx_init (dm_Config *conf)
{
  /* Prepare the internal GFX structure. */
  dm_gfxdata = malloc (sizeof (dm_GfxData));

  if (dm_gfxdata == NULL)
    {
      dm_fatal("GFX: Could not allocate graphics structure.");
      return DM_FAILURE;
    }

  dm_gfxdata->conf = conf;

  dm_gfxdata->driver = malloc (sizeof (dm_GfxDriver));

  if (dm_gfxdata->driver == NULL)
    {
      dm_fatal ("GFX: Could not allocate graphics driver structure.");
      return DM_FAILURE;
    }

  if (dm_gfx_select_driver () == DM_FAILURE)
    {
      dm_fatal ("GFX: Could not select graphics driver.");
      return DM_FAILURE;
    }

  if (dm_gfxdata->driver->init (conf) == DM_FAILURE)
    {
      dm_fatal ("GFX: Could not init driver.");
      return DM_FAILURE;
    }

  /* Initialise image slots */

  memset (dm_gfxdata->images, (int) NULL, 
          sizeof (struct dm_GfxImageNode*) * DM_GFX_HASH_VALS);

  return DM_SUCCESS;
}

int
dm_gfx_select_driver (void)
{
  int i, max, firstdri, prefdri;

  prefdri = firstdri = -1;

  max = sizeof dm_driver_specs / sizeof (dm_GfxDriverSpec);

  dm_debug ("GFX: There are %d drivers defined (including null).", max);

  /* Find first non-null driver and preferred driver if possible. */
  for (i = 0; i < max; i++)
    {
      if (dm_driver_specs[i].name != NULL
          && dm_driver_specs[i].reg != NULL)
        {
          if (firstdri == -1)
            firstdri = i;
      

          /* TEST CODE - replace me */
          if (strcmp (dm_driver_specs[i].name, "sdl") == 0)
            {
              dm_debug ("Selected driver: %s", dm_driver_specs[i].name);
              prefdri = i;
            }
        }
    }

  /* No preferred driver - select first driver defined instead. */
  if (prefdri == -1)
    {
      /* No first driver available - this means no drivers are available! */
      if (firstdri == -1)
        {
          dm_fatal ("GFX: No graphics drivers compiled in!");
          return DM_FAILURE;
        }

      prefdri = firstdri;
    }

  dm_driver_specs[prefdri].reg (dm_gfxdata->driver);

  return DM_SUCCESS;
}

DM_INLINE void
dm_gfx_update (void)
{
  dm_gfxdata->driver->update();
}

void
dm_gfx_cleanup (void)
{
  if (dm_gfxdata) {
    dm_clear_images();

    if (dm_gfxdata->driver) {
      dm_gfxdata->driver->cleanup();

      free(dm_gfxdata->driver);
    }

    free(dm_gfxdata);
    dm_gfxdata = NULL;
  }
}

struct dm_GfxImageNode *dm_load_image(const char filename[])
{
  struct dm_GfxImageNode *ptr;

  ptr = malloc(sizeof(struct dm_GfxImageNode));

  if (ptr) {
    /* Load data. */
    strncpy(ptr->name, filename, DM_GFX_HASH_NAME_LEN);
    ptr->data = dm_gfxdata->driver->load_image_data(filename);

    if (ptr->data) {
      /* Store the image. */
      return dm_get_image(filename, ptr);
    } else {
      dm_fatal("GFX: Could not load data for image %s", filename);
      return NULL;
    }
  } else {
    dm_fatal("GFX: Could not allocate space for image %s", 
             filename);
    return NULL;
  }
}

void dm_free_image(struct dm_GfxImageNode *node)
{
  dm_gfxdata->driver->free_image_data(node->data);
  free(node);
}

int dm_draw_image(const char filename[],
                  unsigned short image_x,
                  unsigned short image_y,
                  unsigned short screen_x,
                  unsigned short screen_y,
                  unsigned short width,
                  unsigned short height)
{
  struct dm_GfxImageNode *img;

  img = dm_get_image(filename, NULL);

  /* Image not preloaded - try to load it now. */

  if (img == NULL) {
    img = dm_load_image(filename);
    if (img == NULL) {
      dm_fatal("GFX: Cannot load non-preloaded image.");
      return DM_FAILURE;
    }
  }

  /* Perform coordinate translation. */

  dm_coord_translate(&screen_x, &screen_y, DM_TRUE);
  dm_coord_translate(&image_x, &image_y, DM_FALSE);
  dm_coord_translate(&width, &height, DM_FALSE);

  /* Then draw the image. >_> */

  return dm_gfxdata->driver->draw_image(img,
                                        image_x,
                                        image_y,
                                        screen_x,
                                        screen_y,
                                        width,
                                        height);
}

void dm_fill_rect_rgb(unsigned short x,
                      unsigned short y,
                      unsigned short w,
                      unsigned short h,
                      unsigned char r,
                      unsigned char g,
                      unsigned char b)
{
  dm_coord_translate(&x, &y, DM_TRUE);
  dm_coord_translate(&w, &h, DM_FALSE);

  dm_gfxdata->driver->fill_rect_rgb(x, y, w, h, r, g, b);
}

int dm_ascii_hash(const char string[])
{
  unsigned char *p;
  int h;

  h = 0;

  /* For each character in the string, multiply the current hash value by the 
   * hash multiplier and then add the value of the next character.
   *
   *  (derived from Kernighan and Pike, ``The Practice of Programming'')
   */

  for (p = (unsigned char*) string; *p != '\0'; p++) {
    h = (DM_GFX_HASH_MUL * h) + *p;
  }

  /* Return the modulus so that the value is in between 0 and the hash 
     value upper bound. */
  return h % DM_GFX_HASH_VALS;
}

int dm_delete_image(const char name[])
{
  int h;
  struct dm_GfxImageNode *img, *prev;

  h = dm_ascii_hash(name);
  prev = NULL;

  /* Iterate through the hash bucket to find the correct image, then 
     delete its data and node. */
  for (img = dm_gfxdata->images[h]; img != NULL; img = prev->next) {
    if (strcmp(name, img->name) == 0) {
      if (prev == NULL) {
        dm_gfxdata->images[h] = img->next;
      } else {
        prev->next = img->next;
      }
      dm_free_image(img);
      return DM_SUCCESS;
    }
  }
  return DM_FAILURE;
}

/* Delete all images. */
void dm_clear_images(void)
{
  int i;
  struct dm_GfxImageNode *p, *next;
  
  for (i = 0; i < DM_GFX_HASH_VALS; i++) {
    for (p = dm_gfxdata->images[i]; p != NULL; p = next) {
      next = p->next;
      /* Delete the image data and node */
      dm_free_image(p);
    }
    dm_gfxdata->images[i] = NULL;
  }
}

struct dm_GfxImageNode *dm_get_image(const char name[], 
                                     struct dm_GfxImageNode *add_pointer)
{
  int h; 
  struct dm_GfxImageNode *img;

  /* Get the hash of the image's filename so we can search in the correct 
     bucket. */
  h = dm_ascii_hash(name);

  /* Now try to find the image. */
  for (img = dm_gfxdata->images[h]; img != NULL; img = img->next) {
    if (strcmp(name, img->name) == 0) {

      /* If there is a pointer to add, then replace the found image's 
         data with the new pointer's data. */

      if (add_pointer) {
        dm_free_image(img->data);
        img->data = add_pointer->data;
        dm_debug("GFX: Found existing image but have new pointer, overwriting.");
      }

      return img;
    }
  }

  /* If we are given a pointer to add, and the image doesn't already 
     exist, then add the image to the start of the linked list. */
  if (add_pointer) {
    add_pointer->next = dm_gfxdata->images[h];
    dm_gfxdata->images[h] = add_pointer;
    return dm_gfxdata->images[h];
  }

  /* Return NULL, if all else fails. */
  return NULL;
}

void dm_coord_translate(unsigned short *xp, unsigned short *yp, 
                        unsigned short centre)
{
  /* Work out screen width and height multiples of low-res width and 
     height. */

  div_t wdiv, hdiv;

  wdiv = div(dm_gfxdata->conf->gfx_screen_width, DM_LOWRES_WIDTH);
  hdiv = div(dm_gfxdata->conf->gfx_screen_height, DM_LOWRES_HEIGHT);

  if (centre) {
    /* Multiply and centre the coordinates. */
    *xp = (*xp * wdiv.quot) + (wdiv.rem / 2);
    *yp = (*yp * hdiv.quot) + (hdiv.rem / 2);
  } else {
    /* Just multiply the coordinates. */
    *xp *= wdiv.quot;
    *yp *= hdiv.quot;
  }
}

void dm_coord_detranslate(unsigned short *xp, unsigned short *yp, 
                          unsigned short decentre)
{
  /* Work out screen width and height multiples of low-res width and 
     height. */

  div_t wdiv, hdiv;

  wdiv = div(dm_gfxdata->conf->gfx_screen_width, DM_LOWRES_WIDTH);
  hdiv = div(dm_gfxdata->conf->gfx_screen_height, DM_LOWRES_HEIGHT);

  if (decentre) {
    /* De-centre and divide the coordinates. */
    *xp = (*xp - (wdiv.rem / 2)) / wdiv.quot;
    *yp = (*yp - (hdiv.rem / 2)) / hdiv.quot;
  } else {
    /* Just divide the coordinates. */
    *xp /= wdiv.quot;
    *xp /= hdiv.quot;
  }
}
