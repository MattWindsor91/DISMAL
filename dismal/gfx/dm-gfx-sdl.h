/** @file     gfx/dm-gfx-sdl.h
 *  @author   Matt Windsor (captainhayashi)
 *  @version  0.001
 *  @brief    Header for SDL graphical subsystem implementation.
 */

#ifndef __DM_GFX_SDL_H__
#define __DM_GFX_SDL_H__

typedef struct dm_GfxSDLData dm_GfxSDLData;

struct dm_GfxSDLData {
  struct SDL_Surface *screen; /**< Pointer to the screen SDL surface. */
};

/** Register the SDL driver.
 *
 *  @param driver  The driver structure in which to store function
 *  pointers, etc.
 */

void dm_gfx_sdl_register(dm_GfxDriver *driver);


/** Initialise the SDL graphics system.
 *
 *  @param conf  A pointer to a dm_Config structure filled with
 *  initial configuration values.
 *
 *  @return DM_SUCCESS for success, DM_FAILURE for failure;
 */

int dm_gfx_sdl_init(dm_Config *conf);

/** Update the screen.
 */
void dm_gfx_sdl_update(void);

/** De-initialise the SDL graphics system.
 *
 *  This is, at time of writing, a no-operation stub.
 */

void dm_gfx_sdl_cleanup(void);


/** Load an image as a SDL surface.
 *
 *  @param filename  Name of the file to load.
 *
 *  @return  a void pointer to the SDL surface.
 */
void *dm_sdl_load_image_data(const char filename[]);


/** Free an image as a SDL surface.
 *
 *  @param data  A void pointer to the SDL surface to free.
 */
void dm_sdl_free_image_data(void *data);


/** Draw an image on-screen using SDL.
 *
 *  @see dm_draw_image
 *
 *  @param image     Node containing the image data.
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
int dm_sdl_draw_image(struct dm_GfxImageNode *image,
                      unsigned int image_x,
                      unsigned int image_y,
                      unsigned int screen_x,
                      unsigned int screen_y,
                      unsigned int width,
                      unsigned int height);

/** Fill a rectangle with the given RGB colour using SDL.
 *
 *  @see dm_fill_rect_rgb
 *
 *  @param x  X co-ordinate of the top-left corner of the rectangle.
 *  @param y  X co-ordinate of the top-left corner of the rectangle.
 *  @param w  Width of the rectangle.
 *  @param h  Height of the rectangle.
 *  @param r  Red component of the fill colour.
 *  @param g  Green component of the fill colour.
 *  @param b  Blue component of the fill colour.
 */
void dm_sdl_fill_rect_rgb(unsigned int x,
                          unsigned int y,
                          unsigned int w,
                          unsigned int h,
                          unsigned int r,
                          unsigned int g,
                          unsigned int b);

#endif /* __DM_GFX_H__ */
