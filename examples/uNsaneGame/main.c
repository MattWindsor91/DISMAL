/** @file     /main.c
 *  @author   Matt Windsor (captainhayashi)
 *  @version  0.001
 *  @brief    Main code file.
 */

#include <stdio.h>
#include <time.h>
#include <math.h>

#include "SDL/SDL.h"
#include "dismal/dismal.h"

#include "main.h"
#include "graphics.h"

struct AppCore *dcore;

int main(int argc, char **argv)
{
  dcore = init_core();
  
  if (dcore) {

    /* Seed the random number generator */

    srand(time(NULL));

    dm_set_resolution(X_RES, Y_RES);

    /* Init DISMAL, the Display/Image/Sound Meta-Abstraction Layer. */
    if (dm_init() == DM_SUCCESS) {
      if (init_screen(dcore)) {
        if (init_grid(dcore)) {
          if (init_callbacks()) {
            main_loop(dcore);
          
            cleanup_grid(dcore);
            cleanup_callbacks();
          } else {
            fprintf(stderr, "Could not initialise input!\n");
          }
        } else {
          fprintf(stderr, "Could not initialise game grid.\n");
        }

      } else {
        fprintf(stderr, "Could not initialise screen.\n");
      }

      cleanup_core(dcore);
    }
    
    dm_cleanup();
  }
  return 0;
}

int init_callbacks(void)
{
  dcore->mousemotioncb = dm_input_install_callback(on_mouse_motion, DM_MOUSE_MOTION_EVENT);
  dcore->mouseupcb = dm_input_install_callback(on_mouse_up, DM_MOUSE_BUTTON_UP_EVENT);
  dcore->asciikeycb = dm_input_install_callback(on_ascii_key, DM_ASCII_KEY_DOWN_EVENT);
  dcore->quitcb = dm_input_install_callback(on_quit, DM_QUIT_EVENT);

  if (dcore->mousemotioncb && dcore->mouseupcb && dcore->asciikeycb && dcore->quitcb) {
    return TRUE;
  } else {
    /* Clean up callbacks if any failed. */
    cleanup_callbacks();
    return FALSE;
  }
}

void cleanup_callbacks(void)
{
  if (dcore->mousemotioncb) {
    dm_input_unload_callback(dcore->mousemotioncb);
  }

  if (dcore->mouseupcb) {
    dm_input_unload_callback(dcore->mouseupcb);
  }

  if (dcore->asciikeycb) {
    dm_input_unload_callback(dcore->asciikeycb);
  }

  if (dcore->quitcb) {
    dm_input_unload_callback(dcore->quitcb);
  }
}

struct AppCore *init_core(void)
{
  struct AppCore *core;

  core = malloc(sizeof(struct AppCore));

  if (core) {
    /* Initialise defaults. */
    core->running = TRUE;

    /* Nullify callback placeholders for later init. */
    core->mousemotioncb = NULL;
    core->mouseupcb = NULL;
    core->asciikeycb = NULL;
    core->quitcb = NULL;

    core->current_tile = TILE_DESELECTED;
    core->num_selected_tiles = core->score = core->potential_score  = 0;

    memset(core->tile_count, 0, sizeof(int) * TILE_TYPES);

    core->grid_w = core->cols_in_play = DEFAULT_GRID_W;
    core->grid_h = DEFAULT_GRID_H;

    core->grid = core->gridmask = NULL;

    core->gfx = NULL;

  } else {
    fprintf(stderr, "Could not allocate memory for core.\n");
  }

  return core;
}

void cleanup_core(struct AppCore *core)
{
  free(core);
  core = NULL;
}


int init_grid(struct AppCore *core)
{
  int i;

  /* Dispose of any existing grid. */
  cleanup_grid(core);

  /* Allocate enough memory for each tile. */
  core->grid = malloc(sizeof(char) * core->grid_w * core->grid_h);
  core->gridmask = malloc(sizeof(char) * core->grid_w * core->grid_h);

  if (core->grid && core->gridmask) {
    /* Set up the tiles randomly, excluding 0 (null tile) */
    for (i = 0; i < (core->grid_w * core->grid_h); i++) {
      core->grid[i] = rand() % (TILE_TYPES - 1) + 1;
      core->tile_count[core->grid[i]]++;
    }

    reset_mask(core);

    return TRUE;
  } else {
    /* Run cleanup, in case one was initialised and the other wasn't. */
    cleanup_grid(core);
    return FALSE;
  }
}

void cleanup_grid(struct AppCore *core)
{
  if (core->grid) {
    free(core->grid);
    core->grid = NULL;
  }

  if (core->gridmask) {
    free(core->gridmask);
    core->gridmask = NULL;
  }

  /* Reset game counters too. */
  core->cols_in_play = core->grid_w;
  core->score = 0;
}

void main_loop(struct AppCore *core)
{
  while (core->running) {
    dm_input_process();
    update_screen(core);
    SDL_Delay(30);
  }
}

void on_quit(dm_InputEvent *event)
{
  printf("Woo, quit event.\n");
  fflush(stdout);
  dcore->running = FALSE;
}

void on_mouse_motion(dm_InputEvent *event)
{
  update_selection(dcore, event->motion.x, event->motion.y);
}

void on_mouse_up(dm_InputEvent *event)
{
  kill_tiles(dcore);
}

void on_ascii_key(dm_InputEvent *event)
{
  switch (event->ascii.code) {
  case 'n':
    /* New grid. */
    init_grid(dcore);
    break;
  }
}

void update_selection(struct AppCore *core, int x, int y)
{
  int row, col, tile;

  /* Calculate the row and column of the tile first. */
  row = (y - GRID_Y) / TILE_H;
  col = (x - GRID_X) / TILE_W;

  /* Check to see if the tile position is sane */
  if (row < core->grid_h &&
      row >= 0 &&
      col < core->grid_w &&
      col >= 0 &&
      core->grid[(row * core->grid_w) + col] != 0) {
    tile = (row * core->grid_w) + col;
  
  /* Only change things if the selected tile has changed, to save effort. */
    if (tile != core->current_tile) {
      core->current_tile = tile;

      /* Zero out the mask thus deselecting everything. */
      reset_mask(core);
      flood_fill(core, tile, core->grid[tile]);

      /* Deselect the tiles again if there is only one of them usable. 
         (Two or more tiles are needed in order to start killing tiles) */
      if (core->num_selected_tiles < 2) {
        reset_mask(core);
      } else {
        core->potential_score = pow(core->num_selected_tiles - 1, 2);
      }
    }
  } else {
    /* Deselect everything if the mouse has only just left the playing field. */
    if (core->current_tile != TILE_DESELECTED) {
      core->current_tile = TILE_DESELECTED;
    }
    reset_mask(core);
  }
}

void flood_fill(struct AppCore *core, int tile, int type)
{
  int row, col;

  /* Do nothing if this tile has already been flagged, 
   is the wrong type or is of null type. */
  if (core->gridmask[tile] == FALSE && 
      core->grid[tile] == type && type != 0) {
    core->gridmask[tile] = TRUE;
    core->num_selected_tiles++;

    /* Propagate in the four cardinal directions, if the 
       result doesn't wrap around or go out of bounds. */

    row = tile / core->grid_w;
    col = tile % core->grid_w;

    /*-- Left */
    if (col > 0) {
      flood_fill(core, tile - 1, type);
    }

    /*-- Right */
    if (col < core->grid_w - 1) {
      flood_fill(core, tile + 1, type);
    }

    /*-- Up */
    if (row > 0) {
      flood_fill(core, tile - core->grid_w, type);
    }

    /*-- Down */
    if (row < core->grid_h - 1) {
      flood_fill(core, tile + core->grid_w, type);
    }
  }
}

int has_neighbours(struct AppCore *core, int tile)
{
  int row, col;

  /* Do nothing if this tile is of null type. */
  if (core->grid[tile] != 0) {
    core->gridmask[tile] = TRUE;
    core->num_selected_tiles++;

    /* Propagate in the four cardinal directions, if the 
       result doesn't wrap around or go out of bounds. */

    row = tile / core->grid_w;
    col = tile % core->grid_w;

    /*-- Left */
    if (col > 0 &&
        core->grid[tile - 1] == core->grid[tile]) {
      return TRUE;
    }

    /*-- Right */
    if (col < core->grid_w - 1 &&
        core->grid[tile + 1] == core->grid[tile]) {
      return TRUE;
    }

    /*-- Up */
    if (row > 0 &&
        core->grid[tile - core->grid_w] == core->grid[tile]) {
      return TRUE;
    }

    /*-- Down */
    if (row < core->grid_h - 1 &&
        core->grid[tile + core->grid_w] == core->grid[tile]) {
      return TRUE;
    }
  }

  return FALSE;
}

void reset_mask(struct AppCore *core)
{
  memset(core->gridmask, FALSE, sizeof (char) * core->grid_w * core->grid_h);
  core->potential_score = 0;
  core->num_selected_tiles = 0;
}

void kill_tiles(struct AppCore *core)
{
  int i, ctile;

  for (i = 0; i < (core->grid_w * core->grid_h); i++) {
    if (core->gridmask[i] == TRUE) {
      core->gridmask[i] = FALSE;
      core->tile_count[core->grid[i]]--;
      core->grid[i] = 0;
    }
  }

  core->score += core->potential_score;

  do_gravity(core);
  if (is_playable(core) == FALSE) {
    init_grid(core);
  }

  /* The block under the cursor has either changed or disappeared, 
     so rerun the selection mechanic to update the highlights. */

  ctile = core->current_tile;
  core->current_tile = TILE_DESELECTED;

  update_selection(core, 
                   ((ctile % core->grid_w) * TILE_W) + GRID_X,
                   ((ctile / core->grid_w) * TILE_H) + GRID_Y);
}

void do_gravity(struct AppCore *core)
{
  int tile, row, col, nofalls, nocoltiles;

  /* Gravity is checked with the grid somewhat transposed 
     (bottom to top, left to right). */
  for (col = 0; col < core->cols_in_play; col++) {
    nofalls = FALSE;

    /* Make multiple gravity passes until everything has fallen 
       to its final position. */

    while (nofalls == FALSE) {
      nofalls = TRUE;
      nocoltiles = TRUE;

      for (row = core->grid_h - 1; row >= 0; row--) {
        tile = (row * core->grid_w) + col;

        /* If there is a tile here, then we don't need to liquidate 
           the column, and we can check gravity. */
        if (core->grid[tile] != 0) {
          nocoltiles = FALSE;

          /* We need a null tile below us in order to fall */
          if (tile / core->grid_w < core->grid_h - 1) {
            if (core->grid[tile + core->grid_w] == 0) {
              nofalls = FALSE;
              core->grid[tile + core->grid_w] = core->grid[tile];
              core->grid[tile] = 0;
            }
          }
        }
      }
    }

    if (nocoltiles) {
      /* If this column is empty, liquidate it and then redo its
         gravity pass. */
      kill_column(core, col);
      col--;
    }
  }
}

void kill_column(struct AppCore *core, int col)
{
  int row, ccol, tile;
  for (ccol = col; ccol < core->cols_in_play; ccol++) { 
    for (row = 0; row < core->grid_h; row++) {
 
      tile = (row * core->grid_w) + ccol;

      /* Fill in the last column with null tiles. */
      if (ccol == core->grid_w - 1) {
        core->grid[tile] = 0;
      } else {
        core->grid[tile] = core->grid[tile + 1];
      }
    }
  }
  core->cols_in_play--;
}

int is_playable(struct AppCore *core)
{
  int i;

  /* If there are any pairs of tiles, game is still playable. */

  for (i = 0; i < (core->grid_w * core->grid_h); i++) {
    if (has_neighbours(core, i)) {
      return TRUE;
    }
  }

  return FALSE;
}


