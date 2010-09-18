/** @file     /main.h
 *  @author   Matt Windsor (captainhayashi)
 *  @version  0.001
 *  @brief    Prototypes, constants and datatypes for main code file.
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

#ifndef __MAIN_H__
#define __MAIN_H__

#include "dismal/dismal.h"

enum
  {
    DEFAULT_GRID_W = 15, /**< Default width of the grid. */
    DEFAULT_GRID_H = 10, /**< Default height of the grid. */

    TILE_TYPES = 5, /**< Number of different tile types, including 0 (null) */

    TILE_DESELECTED = -1, /**< Indicates if the mouse isn't on a tile. */

    SDL_SUCCESS = 0, /**< Return value for successful SDL init. */
    TRUE = 1,        /**< Truth (nonzero) value. */
    FALSE = 0,       /**< Falsehood (zero) value. */

    BUFFER_SIZE = 256 /**< Default size of character buffers. */
  };



/** The core game structure.
 *
 *  This maintains all volatile data for the game.
 */
struct AppCore
{
  int running;  /**< Whether the game is running or not. */

  int current_tile; /**< Tile currently being moused over. */
  int num_selected_tiles; /**< Number of tiles currently selected. */
  int tile_count[TILE_TYPES]; /**< Stores counts of tile types for
                                 gameover detection. */

  int grid_w;   /**< Width of the grid. */
  int grid_h;   /**< Height of the grid. */
  unsigned short grid_x; /**< X position of the grid on-screen. */
  unsigned short grid_y; /**< Y position of the grid on-screen. */
  unsigned char *grid; /**< Array for the grid itself. */
  unsigned char *gridmask; /**< Array for the grid connected tiles mask. */

  int cols_in_play; /**< Number of columns being used. */
  unsigned int score; /**< Score. */
  unsigned int potential_score; /**< Score gained from clicking the
                                   screen now. */

  dm_InputCallback *mousemotioncb; /**< Mouse motion input callback. */
  dm_InputCallback *mouseupcb;     /**< Mouse button up input callback. */
  dm_InputCallback *asciikeycb;    /**< ASCII key input callback. */
  dm_InputCallback *quitcb;        /**< Quit input callback. */

  struct Gfx *gfx; /**< Graphics subsystem structure. */
};

/** Main function.
 *
 *  @param argc  Argument count.
 *  @param argv  Argument vector.
 *
 *  @return zero for success, non-zero for error.
 */

int
main (int argc, char **argv);

/** Initialise core DISMAL input callbacks.
 *
 *  @return  TRUE for success, FALSE otherwise.
 */

int
init_callbacks (void);

/** De-initialise core DISMAL input callbacks.
 */

void
cleanup_callbacks (void);

/** Allocate the core structure.
 *
 *  @return  a pointer to the initialised AppCore.
 */

struct AppCore *
init_core (void);

/** Free the core structure.
 *
 *  @param core  A pointer to the AppCore to de-initialise.
 */

void
cleanup_core (struct AppCore *core);

/** (Re)allocate the game grid.
 *
 *  This should be run if the grid size changes, the grid is 
 *  deemed unplayable at start or a new game is created.
 *
 *  This attempts to allocate both the grid of tiles and the selected
 *  tile grid mask according to core->grid_w and core->grid_h.
 *
 *  This auto-calls cleanup_grid(), so a separate call is not
 *  required.
 *
 *  @param core  Pointer to the application core structure.
 *
 *  @return  TRUE if the grid and grid mask could both be allocated,
 *  FALSE otherwise.
 */

int
init_grid (struct AppCore *core);

/** Free the game grid.
 *
 *  This also frees the grid-mask.
 *
 *  @param core  Pointer to the application core structure.
 */

void 
cleanup_grid (struct AppCore *core);

/** The main game loop.
 *
 *  This calls handle_events() to handle SDL events, update_screen()
 *  to handle graphical updates, and then delays the game for a few
 *  milliseconds.
 *
 *  @param core  Pointer to the application core structure.
 */

void
main_loop (struct AppCore *core);

/** Handle SDL events.
 *
 *  This checks for mouse motion, keyboard input and windowing system
 *  requests (eg quit).
 *
 *  @param core  Pointer to the application core structure.
 */

void
handle_events (struct AppCore *core);

/** Callback for quit. 
 *
 *  @param event  The DISMAL event produced by the quit.
 */

void
on_quit (dm_InputEvent *event);

/** Callback for mouse motion. 
 *
 *  @param event  The DISMAL event produced by the mouse motion.
 */

void
on_mouse_motion (dm_InputEvent *event);

/** Callback for mouse button up. 
 *
 *  @param event  The DISMAL event produced by the mouse up.
 */

void
on_mouse_up (dm_InputEvent *event);

/** Callback for ASCII keys. 
 *
 *  @param event  The DISMAL event produced by the ASCII key.
 */

void
on_ascii_key (dm_InputEvent *event);

/** Update the selection mask.
 *
 *  Given the X and Y co-ordinates of the current mouse location, this
 *  function calls flood_fill() to select all tiles connected to the
 *  tile the mouse is positioned under, if any, so that a mouse click
 *  will remove the tiles (providing there are at least 2 connected).
 *
 *  @param core  Pointer to the application core structure.
 *  @param x     Absolute X co-ordinate of the mouse on the screen.
 *  @param y     Absolute Y co-ordinate of the mouse on the screen.
 */

void
update_selection (struct AppCore *core, int x, int y);

/** Perform a 4-directional flood fill to select connected tiles.
 *
 *  This flags the given tile, if said tile is of the given type, as
 *  TRUE (selected) in the selection mask, and recursively calls
 *  itself on its four adjacent neighbours.
 *
 *  This is used to select all tiles connected to the tile the mouse
 *  is under.
 *
 *  @param core  Pointer to the application core structure.
 *  @param tile  The tile reference to perform the flood fill from.
 *  @param type  The desired tile type.
 */

void
flood_fill (struct AppCore *core, int tile, int type);

/** Check whether the given tile has any same-type neighbours.
 *
 *  This checks in 4 directions whether there is a tile with the same
 *  tile type as the given tile, and returns TRUE if there is.
 *
 *  @param core  Pointer to the application core structure.
 *  @param tile  The tile reference to perform the flood fill from.
 *
 *  @return TRUE if there are neighbours, FALSE otherwise.
 */

int
has_neighbours (struct AppCore *core, int tile);

/** Reset the selection mask.
 *
 *  This fills the selection mask, core->gridmask, with FALSE.
 *  This should be done whenever the selection is to change.
 *
 *  @param core  Pointer to the application core structure.
 */

void
reset_mask (struct AppCore *core);

/** Remove ("kill") all flagged tiles. 
 *
 *  This replaces all tiles marked as TRUE (selected) in the grid mask
 *  (all tiles caught in a same-tiletype flood fill emanating from the
 *  currently moused tile) with null tiles, then calls do_gravity to
 *  ensure null gaps are filled.
 *
 *  @param core  Pointer to the application core structure.
 */

void
kill_tiles (struct AppCore *core);

/** Perform a gravity simulation.
 *
 *  This causes all tiles with null tiles (gaps) below them to "fall"
 *  so they lie on top of either another tile or the grid edge. 
 *
 *  This function also calls kill_column() to clean up any columns
 *  with no remaining tiles.
 *
 *  @param core  Pointer to the application core structure.
 */

void
do_gravity (struct AppCore *core);

/** Remove ("kill") an empty column. 
 *
 *  This decreases the number of columns marked for play and shifts
 *  all columns to the right of the killed column to the left, filling
 *  the gap.
 *
 *  @param core  Pointer to the application core structure.
 *  @param col   Column to kill.
 */

void
kill_column (struct AppCore *core, int col);

/** Check to see if the game is playable.
 *
 *  This checks, using has_neighbours(), to make sure that there is an
 *  instance of at least two connected tiles. If there is no such
 *  instance, the game cannot be played further.
 *
 *  @param core  Pointer to the application core structure.
 *
 *  @return TRUE if the game is playable, FALSE otherwise.
 */

int
is_playable (struct AppCore *core);

#endif /* __MAIN_H__ */
