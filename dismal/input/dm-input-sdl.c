/** @file     input/dm-input-sdl.c
 *  @author   Matt Windsor (captainhayashi)
 *  @version  0.001
 *  @brief    SDL input driver.
 */

#include "SDL/SDL.h"

#include "dm-input-sdl.h"
#include "../dismal.h"

int dm_input_sdl_init(struct dm_Config *conf)
{
  SDL_EnableUNICODE(1);
  return DM_SUCCESS;
}


void dm_input_sdl_cleanup(void)
{
}


void dm_input_sdl_process(void)
{
  SDL_Event sdlevent;
  union dm_InputEvent event;

  while (SDL_PollEvent(&sdlevent)) {
    /* Null out the event. */
    event.type = 0;

    switch(sdlevent.type) {
    case SDL_QUIT:
      /* Quit event (eg window close attempted). */
      event.type = DM_QUIT_EVENT;
      break;
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
      /* Mouse button events. */
      if (sdlevent.button.button == SDL_BUTTON_LEFT) {
        event.button.button = DM_LMB;
      } else if (sdlevent.button.button == SDL_BUTTON_MIDDLE) {
        event.button.button = DM_MMB;
      } else {
        /* This of course assumes SDL mice will only ever have 3 buttons. */
        event.button.button = DM_RMB;
      } 

      if (sdlevent.type == SDL_MOUSEBUTTONDOWN) {
        event.type = DM_MOUSE_BUTTON_DOWN_EVENT;
      } else {
        event.type = DM_MOUSE_BUTTON_UP_EVENT;
      }

      break;
    case SDL_MOUSEMOTION:
      /* Mouse motion events. */
      dm_sdl_mouse_motion(&event, &sdlevent);
      break;
    case SDL_KEYDOWN:
    case SDL_KEYUP:
      /* Keyboard events. */
      
      /* Use SDL's unicode support to check for an ASCII key. (It works!) */

      if (sdlevent.key.keysym.unicode < 0x80 && sdlevent.key.keysym.unicode > 0) {
        /* ASCII key */

        if (sdlevent.key.type == SDL_KEYDOWN) { 
          event.ascii.type = DM_ASCII_KEY_DOWN_EVENT;
        } else {
          event.ascii.type = DM_ASCII_KEY_UP_EVENT;
        }

        dm_debug("eh, steve");

        event.ascii.code = (char) sdlevent.key.keysym.unicode;
      }
        break;
    default:
      break;
    }

    /* If there was a proper event, release it to callbacks. */
    if (event.type != 0) {
      dm_input_event_release(&event);
    }
  }
}

void dm_sdl_mouse_motion(dm_InputEvent *event, SDL_Event *sdlevent)
{
  /* We need to check to see if the raw X and Y are within the
     translated screen boundaries. */
  unsigned short left, top, right, bottom;

  left = top = 0;
  right = DM_LOWRES_WIDTH;
  bottom = DM_LOWRES_HEIGHT;

  dm_coord_translate(&left, &top, DM_TRUE);
  dm_coord_translate(&right, &bottom, DM_TRUE);

  if (sdlevent->motion.x >= left && 
      sdlevent->motion.x < right &&
      sdlevent->motion.y >= top &&
      sdlevent->motion.y < bottom) {

    event->motion.type = DM_MOUSE_MOTION_EVENT;
    event->motion.xraw = event->motion.x = sdlevent->motion.x;
    event->motion.yraw = event->motion.y = sdlevent->motion.y;
    event->motion.deltax = sdlevent->motion.xrel;
    event->motion.deltay = sdlevent->motion.yrel;

    /* We now need to detranslate to normalise the X and Y 
       to the logical screen coordinates. */

    dm_coord_detranslate(&(event->motion.x), &(event->motion.y), 
                         DM_TRUE);
    dm_debug("%u %u", event->motion.x, event->motion.y);
  }
}
