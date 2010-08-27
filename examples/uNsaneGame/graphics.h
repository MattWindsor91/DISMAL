/** @file     /graphics.h
 *  @author   Matt Windsor (captainhayashi)
 *  @version  0.001
 *  @brief    Prototypes, constants and datatypes for graphical
 *            routines.
 */

#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#define FONT_PATH "font.png"
#define TILES_PATH "tiles.png"

enum {
  X_RES = 640, /**< X resolution of screen. */
  Y_RES = 400, /**< Y resolution of screen. */
  BPP   = 8,   /**< Bits per pixel of screen. */

  GRID_X = 32, /**< X offset of grid on screen. */
  GRID_Y = 48, /**< Y offset of grid on screen. */

  SCORE_X = 16, /**< X offset of score. */
  SCORE_Y = 16, /**< X offset of score. */

  PSCORE_X = 16, /**< X offset of potential score. */
  PSCORE_Y = 32, /**< X offset of potential score. */

  TILE_W = 16, /**< Width of one tile. */
  TILE_H = 16, /**< Height of one tile. */

  FONT_W = 10, /**< Width of one font character. */
  FONT_H = 10, /**< Height of one font character. */

  ALIGN_LEFT = 0, /**< Left alignment for text. */
  ALIGN_CENTRE,   /**< Centre alignment for text. */
  ALIGN_RIGHT,    /**< Right alignment for text. */

  RGB_ELEMENTS = 3, /**< Number of elements in an RGB triplet. */
  R = 0, /**< Red element. */
  G,     /**< Green element. */
  B,     /**< Blue element. */

  ISL_FONT = 0, /**< Image slot ID for font. */
  ISL_TILES,    /**< Image slot ID for tiles. */
  ISL_BG,       /**< Image slot ID for background. */
  IMAGE_SLOTS   /**< Total image slot count. */
};

struct Gfx {
  struct SDL_Surface *screen; /**< The SDL screen used for
                                 graphics. */
  struct SDL_Surface *images[IMAGE_SLOTS]; /**< Image slots. */
};

/** Initialise the screen.
 *
 *  @param core  Pointer to the application core structure.
 *
 *  @return TRUE for success, FALSE otherwise.
 */

int init_screen(struct AppCore *core);


/** De-initialise the screen (stub).
 *
 *  Since SDL_Quit deletes the screen surface, this function is
 *  actually a no-operation and is reserved for if special screen
 *  cleanup operations are required in the future.
 *
 *  @param core  Pointer to the application core structure.
 */

void cleanup_screen(struct AppCore *core);


/** Update the screen.
 *
 *  This redraws the tile set and the user interface.
 *
 *  @param core  Pointer to the application core structure.
 */

void update_screen(struct AppCore *core);


/** Update the scoreboard.
 *
 *  @param core  Pointer to the application core structure.
 */

void update_scores(struct AppCore *core);


/** Update the grid of tiles.
 *
 *  @param core  Pointer to the application core structure.
 */

void update_tiles(struct AppCore *core);


/** Fill a rectangle on the screen with a colour.
 *
 *  @param core  Pointer to the application core structure.
 *  @param x  X co-ordinate of the top-left corner of the rectangle.
 *  @param y  X co-ordinate of the top-left corner of the rectangle.
 *  @param w  Width of the rectangle.
 *  @param h  Height of the rectangle.
 *  @param r  Red component of the fill colour.
 *  @param g  Green component of the fill colour.
 *  @param b  Blue component of the fill colour.
 */

void fill_rect(struct AppCore *core, 
               unsigned int x,
               unsigned int y,
               unsigned int w,
               unsigned int h,
               unsigned int r,
               unsigned int g,
               unsigned int b);


/** Load an image file.
 *
 *  This places the resultant SDL surface for the image pointed to by
 *  the filename given in the slot denoted by slot_id.
 *
 *  @param core  Pointer to the application core structure.
 *  @param slot_id   Image slot to replace with this image.
 *  @param filename  The path to the file to load from.
 *
 *  @return TRUE for success, FALSE otherwise.
 */

int load_image(struct AppCore *core,
               unsigned char slot_id,
               const char *filename);


/** Display a image file.
 *
 *  This loads an image file from the slot given by slot_id, and displays 
 *  a rectangular area specified by image_x, image_y, width and height 
 *  on screen at co-ordinates specified by screen_x and screen_y.
 *
 *  @param core      Pointer to the application core structure.
 *  @param slot_id   File ID for the file to be loaded.
 *  @param image_x   The X-coordinate of the on-image rectangle to display.
 *  @param image_y   The Y-coordinate of the on-image rectangle to display.
 *  @param screen_x  The X-coordinate on-screen to display the image at.
 *  @param screen_y  The Y-coordinate on-screen to display the image at.
 *  @param width     The width of the rectangle.
 *  @param height    The height of the rectangle.
 *
 *  @return TRUE for success, FALSE otherwise. In most cases, a failure 
 *          will simply cause the image to not appear.
 */

int draw_image(struct AppCore *core,
               unsigned char slot_id,
               unsigned int image_x,
               unsigned int image_y,
               unsigned int screen_x,
               unsigned int screen_y,
               unsigned int width,
               unsigned int height);


/** Write text to the buffer.
 *
 *  A wrapper to the image drawing functions that allows text to be 
 *  left, centre, or right-aligned on a line of length box_width
 *  starting at (x, y).
 *
 *  @param core       Pointer to the in-use GameCore.
 *  @param x          X position of text.
 *  @param y          X position of text.
 *  @param box_width  Width of line to align text on. This need only be given 
 *                    for centre or right-aligned text.
 *  @param alignment  Desired alignment (ALIGN_LEFT, ALIGN_CENTRE or
 *                    ALIGN_RIGHT).
 *  @param string     The string to write.
 *
 */

void write_str(struct AppCore *core,
               unsigned int x,
               unsigned int y,
               unsigned int box_width, 
               unsigned char alignment,
               const char *string);


/** Draw a single tile.
 *
 *  This draws a tile of the given type in the given row and column on
 *  the graphical grid.
 *
 *  If highlight is TRUE, a lighter version of the tile will be
 *  drawn. This is used to signify selection.
 *
 *  @param core       Pointer to the application core structure.
 *  @param row        The grid row in which the tile is to be drawn.
 *  @param col        The grid row in which the tile is to be drawn.
 *  @param type       The tile type of the tile to be drawn.
 *  @param highlight  Whether the tile should be drawn highlighted or
 *                    not.
 */

void draw_tile(struct AppCore *core, int row, int col, int type, int highlight);

#endif /* __GRAPHICS_H__ */
