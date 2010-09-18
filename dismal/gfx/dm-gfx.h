/** @file     gfx/dm-gfx.h
 *  @author   Matt Windsor (captainhayashi)
 *  @version  0.001
 *  @brief    Header for generic graphical subsystem.
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

#ifndef __DM_GFX_H__
#define __DM_GFX_H__

#include "../dismal.h"

typedef struct dm_GfxImageNode dm_GfxImageNode;
typedef struct dm_GfxData dm_GfxData;
typedef struct dm_GfxDriver dm_GfxDriver;
typedef struct dm_GfxDriverSpec dm_GfxDriverSpec;

/* Global variables */
extern dm_GfxData *dm_gfxdata;

enum {
  /* Low-res reference screen dimensions. */
  DM_LOWRES_WIDTH = 320, /**< Width of LowRes screen. */
  DM_LOWRES_HEIGHT = 200, /**< Height of LowRes screen. */
  /* Flags */
  DM_GFX_AUTO_TRANSLATE = (1<<0), /**< If set, then all coordinates and
                                     images will be assumed low-res and 
                                     automatically scaled up to fill a
                                     high-res screen. */

  DM_GFX_HASH_NAME_LEN = 100, /**< Maximum size of the part of the
                                 image filename used by the hashing
                                 function. If the filename (relative
                                 to the image directory) is larger,
                                 then an error will be tripped. */

  DM_GFX_HASH_VALS = 10, /**< Number of hash values used in the image
                            hash table, and therefore the number of
                            separate linked lists. Increasing this
                            may increase performance. */

  DM_GFX_HASH_MUL  = 31, /**< Hash value used in the image hash table
                            algorithms. This value is taken from
                            Kernighan and Pike's ``The Practice of
                            Programming'', as is the algorithmic
                            concept. */

  /* Coordinate reference point IDs.*/

  DM_TOP_LEFT     = 0, /**< Top-left of screen reference point. */
  DM_TOP_RIGHT    = 1, /**< Top-right of screen reference point. */
  DM_BOTTOM_RIGHT = 2, /**< Bottom-right of screen reference point. */
  DM_BOTTOM_LEFT  = 3, /**< Bottom-left of screen reference point. */
  DM_CENTRE       = 4  /**< Centre of screen reference point. */
};

/** An image node in the image hash table.
 *
 *  Since the contents of data will vary between graphics drivers, it
 *  is strongly recommended that nothing outside DISMAL directly
 *  manipulates an ImageNode.
 */
struct dm_GfxImageNode
{
  char name[DM_GFX_HASH_NAME_LEN]; /**< Name used to identify the
                                      image. */
  void *data;                   /**< Driver-dependent image data. */
  struct dm_GfxImageNode *next; /**< The next node, if any. */
};


struct dm_GfxData
{
  dm_Config *conf;      /**< Pointer to the configuration structure. */
  dm_GfxDriver *driver; /**< Pointer to the driver function table. */
  dm_GfxImageNode *images[DM_GFX_HASH_VALS]; /**< Image hash table. */
};


struct dm_GfxDriverSpec
{
  const char *name; /**< Name of the driver, used to identify it in 
                       debugging and configuration systems.*/
  void (*reg)
  (dm_GfxDriver *driver); /**< Pointer to the function that
                             will populate the driver
                             function table with this
                             driver's functions. */
};


/** A function table for a graphics driver.
 */
struct dm_GfxDriver
{
  int   
  (*init) (dm_Config *conf);
  void
  (*update) (void);
  void
  (*cleanup) (void);
  void*
  (*load_image_data) (const char filename[]);
  void
  (*free_image_data) (void *data);
  int
  (*draw_image) (struct dm_GfxImageNode *image, 
                 unsigned int image_x,
                 unsigned int image_y,
                 unsigned int screen_x, 
                 unsigned int screen_y,
                 unsigned int width, 
                 unsigned int height);
  void
  (*fill_rect_rgb) (unsigned int x, 
                    unsigned int y, 
                    unsigned int w,
                    unsigned int h,
                    unsigned int r,
                    unsigned int g,
                    unsigned int b);
  void
  (*fill_rect_pal) (unsigned int x, 
                    unsigned int y, 
                    unsigned int w,
                    unsigned int h,
                    unsigned int index); 
};


/** Select the driver and register it.
 *
 *  @return non-zero (DM_SUCCESS) for success, zero (DM_FAILURE) for
 *  failure.
 */

int dm_gfx_select_driver(void);


/** Initialise DISMAL's graphics subsystem.
 *
 *  This should NOT be called outside dm_init.  Instead, call dm_init
 *  with the flag DM_USE_GFX (included in DM_DEFAULT_FLAGS) passed.
 *
 *  @param conf  A pointer to a dm_Config structure filled with
 *  initial configuration values.
 *
 *  @return non-zero (DM_SUCCESS) for success, zero (DM_FAILURE) for
 *  failure.
 */

int dm_gfx_init(dm_Config *conf);

/** Update the screen.
 */
void dm_gfx_update(void);


/** Clean up the graphics subsystem.
 */
void dm_gfx_cleanup(void);

/** Load an image.
 *
 *  This loads the image of given filename from the predesignated
 *  image directory.
 *
 *  @warning  The returned node data should not be directly accessed,
 *            as it will vary wildly from GFX driver to GFX driver.
 *
 *  @param filename  The name of the image file (eg boom.png).
 *
 *  @return  A pointer to the image node encapsulating the image
 *           data.
 *
 */

struct dm_GfxImageNode *dm_load_image(const char filename[]);


/** De-allocate an image node.
 *
 *  This should be used instead of free, to ensure that the
 *  driver-specific data is correctly freed.
 *
 *  @param node  A pointer to the image node to be freed.
 */

void dm_free_image(struct dm_GfxImageNode *node);


/** Draw an image on-screen.
 *
 *  As with all drawing functions, if the flag DM_MULTIPLY_COORDS is
 *  set, the screen co-ordinates will be treated as relative to a
 *  320x200 screen, and upscaled to higher resolutions.  Otherwise, it
 *  is up to the calling code to ensure the coordinates are correct
 *  for the display resolution.
 *
 *  This loads an image file from the slot given by slot_id, and displays 
 *  a rectangular area specified by image_x, image_y, width and height 
 *  on screen at co-ordinates specified by screen_x and screen_y.
 *
 *  @note  If the image has not yet been loaded into memory with
 *  dm_load_image, the image will be automatically loaded.  This may
 *  cause a delay as the disk is accessed.
 *
 *  @param filename  Filename of the image.
 *  @param image_x   The X-coordinate of the on-image rectangle to
 *  display.
 *  @param image_y   The Y-coordinate of the on-image rectangle to
 *  display.
 *  @param screen_x  The X-coordinate on-screen to display the image at.
 *  @param screen_y  The Y-coordinate on-screen to display the image at.
 *  @param width     The width of the rectangle.
 *  @param height    The height of the rectangle.
 *
 *  @return  DM_SUCCESS for success, DM_FAILURE otherwise. In most
 *           cases, a failure will simply cause the image to not appear.
 */

int dm_draw_image(const char filename[],
                  unsigned short image_x,
                  unsigned short image_y,
                  unsigned short screen_x,
                  unsigned short screen_y,
                  unsigned short width,
                  unsigned short height);


/** Fill a rectangle with the given RGB colour.
 *
 *  When working in 8-bit, the colour used for the fill will instead
 *  be that of the logical palette entry on average closest to the
 *  given colour.  To directly use a logical palette colour to fill, 
 *  use the function dm_fill_image_pal() instead.
 *
 *  @param x  X co-ordinate of the top-left corner of the rectangle.
 *  @param y  X co-ordinate of the top-left corner of the rectangle.
 *  @param w  Width of the rectangle.
 *  @param h  Height of the rectangle.
 *  @param r  Red component of the fill colour.
 *  @param g  Green component of the fill colour.
 *  @param b  Blue component of the fill colour.
 */

void dm_fill_rect_rgb(unsigned short x,
                      unsigned short y,
                      unsigned short w,
                      unsigned short h,
                      unsigned char r,
                      unsigned char g,
                      unsigned char b);


/** Perform a basic hash on an ASCII string.
 *
 *  This uses the algorithm documented in Kernighan and Pike's ``The
 *  Practice of Programming''.
 *
 *  @param string  String to be hashed.
 *
 *  @return an integer hash of the string, guaranteed to be between 0
 *  and DM_GFX_HASH_VALS.
 */

int dm_ascii_hash(const char string[]);


/** Delete an image from the hash table.
 * 
 *  @param name  The filename of the image.
 *
 *  @return DM_SUCCESS for success, DM_FAILURE otherwise.
 */
int dm_delete_image(const char name[]);


/** Delete all images. */
void dm_clear_images(void);


/** Retrieve an image in the image hash table.
 *
 *  If add_pointer is non-NULL, this function is changed into an
 *  addition operation, which will overwrite an existing image file of
 *  the same name.
 *
 *  For convenience, two wrapper functions are provided,
 *  dm_find_image() and dm_load_image(), which call this function.  It
 *  is perfectly fine to use dm_get_image directly.
 *
 *  @param name  The filename of the image, used to locate the file in
 *  the hash table.
 *
 *  @param add_pointer  Pointer to a dm_GfxImageNode to add to the
 *  hash table, overwriting any existing node.  If this is NULL, then
 *  the function will just attempt to find the given name without
 *  overwriting or adding.
 *
 *  @return NULL if the given name cannot be found and add_pointer is
 *  NULL; a pointer to the installed node otherwise.
 */

struct dm_GfxImageNode *dm_get_image(const char name[],
                                     struct dm_GfxImageNode *add_pointer);

/** Perform any co-ordinate transformation necessary.
 *
 *  Given an X co-ordinate and a Y co-ordinate by pointer (normalised
 *  within the low-res 320x200 "logical" screen), this will perform 
 *  any transformations necessary (such as low-res to high-res  
 *  scaling and centring).
 *
 *  This can also be used (ie with centre as DM_FALSE) to upscale 
 *  widths and heights.
 *
 *  @param xp      Pointer to the X co-ordinate to change.
 *  @param yp      Pointer to the Y co-ordinate to change.
 *  @param centre  Whether or not to centre as part of the 
 *                 translation.  Generally, pass DM_TRUE to this if 
 *                 the translation is being used on screen X and Y 
 *                 coordinates, and DM_FALSE if the translation is 
 *                 being used on width and height, or image X and 
 *                 Y coordinates.
 */

void
dm_coord_translate (unsigned short *xp,
                    unsigned short *yp, 
                    unsigned short centre);

/** Perform any co-ordinate transformation necessary.
 *
 *  Given an X co-ordinate and a Y co-ordinate by pointer, this will
 *  reverse the lowres->highres transformation to provide the
 *  coordinates normalised within the 320x200 "logical" screen.
 *
 *  This can also be used (ie with centre as DM_FALSE) to downscale 
 *  widths and heights.
 *
 *  @param xp  Pointer to the X co-ordinate to change.
 *  @param yp  Pointer to the Y co-ordinate to change.
 *  @param centre  Whether or not to centre as part of the 
 *                 translation.  Generally, pass DM_TRUE to this if 
 *                 the translation is being used on screen X and Y 
 *                 coordinates, and DM_FALSE if the translation is 
 *                 being used on width and height, or image X and 
 *                 Y coordinates.
 */

void
dm_coord_detranslate (unsigned short *xp, unsigned short *yp, 
                      unsigned short centre);

/** Remap the coordinates given to another reference point.
 *
 *  The coordinates given by pointer will be changed in-place.
 *
 *  If the reference point is given as DM_TOP_LEFT, then the 
 *  coordinate will be unchanged.
 *
 *  If the reference point is given as DM_BOTTOM_RIGHT, then 
 *  x and y coordinates will be taken as distances leftwards 
 *  and upwards from the bottom-right corner of the screen, 
 *  respectively.
 *
 *  DM_TOP_RIGHT and DM_BOTTOM_LEFT are given as intermediates 
 *  between the other two, and flip the X and Y coordinate 
 *  references from DM_TOP_LEFT respectively.
 *
 *  DM_CENTRE will transform the coordinates as if they were the 
 *  width and height of an object to be centred on the screen - 
 *  that is, half the coordinates and offset them to the left 
 *  and upwards to refer to the top-left of the object.
 *
 *  The coordinates are expected to be relative to the logical 
 *  320x200 coordinate system if DM_GFX_AUTO_TRANSLATE is enabled.
 *
 *  @param xp        Pointer to the X co-ordinate to change.
 *  @param yp        Pointer to the Y co-ordinate to change.
 *  @param refpoint  The ID of the reference point from which to 
 *                   base the coordinate mapping.
 */

void
dm_coord_map (unsigned short *xp, unsigned short *yp, 
              unsigned short refpoint);

#endif /* __DM_GFX_H__ */
