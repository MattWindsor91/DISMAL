/** @file     /graphics.c
 *  @author   Matt Windsor (captainhayashi)
 *  @version  0.001
 *  @brief    Graphical routines.
 */

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include "dismal/gfx/dm-gfx.h"

#include "main.h"
#include "graphics.h"

int init_screen(struct AppCore *core)
{
  if (dm_load_image(FONT_PATH)) {
    if (dm_load_image(TILES_PATH)) {
      return TRUE;
    } else {
      dm_delete_image(FONT_PATH);
      fprintf(stderr, "Error loading %s.\n", TILES_PATH);
    }
    fprintf(stderr, "Error loading %s.\n", FONT_PATH);
  }

  return FALSE;
}

void update_screen(struct AppCore *core)
{
  update_scores(core);
  update_tiles(core);

  dm_gfx_update();
}

void update_scores(struct AppCore *core)
{
  char buf[256];

  dm_fill_rect_rgb(0, 0, 200, 200, 0, 0, 0);
  sprintf(buf, "Score: %u", core->score);
  write_str(core, SCORE_X, SCORE_Y, 0, ALIGN_LEFT, buf);
  sprintf(buf, "(+ %u)", core->potential_score);
  write_str(core, PSCORE_X, PSCORE_Y, 0, ALIGN_LEFT, buf);
}

void update_tiles(struct AppCore *core)
{
  int i, row, col, type;

  for (i = 0; i < (core->grid_w * core->grid_h); i++) {
    /* Determine row and column. */
    row = i / core->grid_w;
    col = i % core->grid_w;

    /* Determine the type and then draw the tile. */
    type = core->grid[i];
    draw_tile(core, row, col, type, core->gridmask[i]);
  }
}

void write_str(struct AppCore *core,
               unsigned int x,
               unsigned int y,
               unsigned int box_width, 
               unsigned char alignment,
               const char *string)
{
  char chr;
  unsigned int i, x1, slength, length, midpoint;
 
  slength = strlen(string);
  length = FONT_W * slength;
  
  /* Set up alignment */
  switch (alignment) {
    case ALIGN_LEFT:
      x1 = x;
      break;
    case ALIGN_RIGHT:
      x1 = box_width - slength;
      break;
    case ALIGN_CENTRE:   
      midpoint = x + (box_width / 2);
      x1 = midpoint - (length / 2);
      break;
  }

  /* Draw each character using the font image. */
  for (i = 0; i < strlen(string); i++) {
    chr = string[i];
    dm_draw_image("font.png", 
               (chr % 16) * FONT_W, 
               ((chr - (chr % 16))/16) * FONT_H, 
               x1, y, FONT_W, FONT_H);

    x1 += FONT_W;
  }  
}

void draw_tile(struct AppCore *core, int row, int col, int type, int highlight)
{
  if (highlight) {
    dm_draw_image("tiles.png",
                  type * TILE_W, TILE_H,
                  (col * TILE_W) + GRID_X, (row * TILE_H) + GRID_Y,
                  TILE_W, TILE_H);
  } else {
    dm_draw_image("tiles.png",
                  type * TILE_W, 0,
                  (col * TILE_W) + GRID_X, (row * TILE_H) + GRID_Y,
                  TILE_W, TILE_H);
  }
}
