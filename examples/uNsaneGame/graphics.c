/** @file     /graphics.c
 *  @author   Matt Windsor (captainhayashi)
 *  @version  0.001
 *  @brief    Graphical routines.
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

#include "dismal/dismal.h"

#include "main.h"
#include "graphics.h"

int
init_screen (struct AppCore *core)
{
  if (dm_load_image (FONT_PATH))
    {
      if (dm_load_image (TILES_PATH))
        return TRUE;
      else
        {
          dm_delete_image (FONT_PATH);
          fprintf (stderr, "Error loading %s.\n", TILES_PATH);
        }
    }
  else
    fprintf (stderr, "Error loading %s.\n", FONT_PATH); 

  return FALSE;
}

void
update_screen (struct AppCore *core)
{
  update_scores (core);
  update_tiles (core);

  dm_gfx_update ();
}

void
update_scores (struct AppCore *core)
{
  char buf[256];

  dm_fill_rect_rgb (0, 0, 200, 200, 0, 0, 0);

  sprintf (buf, "Score: %u", core->score);
  write_str (core, SCORE_X, SCORE_Y, 0, ALIGN_LEFT, buf);

  sprintf (buf, "(+ %u)", core->potential_score);
  write_str (core, PSCORE_X, PSCORE_Y, 0, ALIGN_LEFT, buf);
}

void update_tiles(struct AppCore *core)
{
  int i, row, col, type;

  /* Map the grid's coordinates to the centre. */
  core->grid_x = (unsigned short) core->grid_w * TILE_W;
  core->grid_y = (unsigned short) core->grid_h * TILE_H;

  dm_coord_map (&core->grid_x, &core->grid_y, DM_CENTRE);

  for (i = 0; i < (core->grid_w * core->grid_h); i++)
    {
      /* Determine row and column. */
      row = i / core->grid_w;
      col = i % core->grid_w;

      /* Determine the type and then draw the tile. */
      type = core->grid[i];
      draw_tile (core, row, col, type, core->gridmask[i]);
    }
}

void
write_str (struct AppCore *core,
           unsigned int x,
           unsigned int y,
           unsigned int box_width, 
           unsigned char alignment,
           const char *string)
{
  char chr;
  unsigned int i, x1, slength, length, midpoint;
 
  slength = strlen (string);
  length = FONT_W * slength;
  
  /* Set up alignment */
  switch (alignment)
    {
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
  for (i = 0; i < strlen (string); i++)
    {
      chr = string[i];
      dm_draw_image ("font.png", 
                     (chr % 16) * FONT_W, 
                     ((chr - (chr % 16))/16) * FONT_H, 
                     x1, y, FONT_W, FONT_H);

      x1 += FONT_W;
    }  
}

void
draw_tile (struct AppCore *core, int row, int col, int type, int highlight)
{
  int offset;

  /* Use the offset to retrieve the second line of tile images if a 
     highlight is needed. */

  if (highlight)
    offset = TILE_H;
  else
    offset = 0;

  dm_draw_image ("tiles.png",
                 type * TILE_W, offset,
                 (col * TILE_W) + core->grid_x, (row * TILE_H) + core->grid_y,
                 TILE_W, TILE_H);
}
