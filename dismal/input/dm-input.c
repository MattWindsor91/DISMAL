/** @file     input/dm-input.c
 *  @author   Matt Windsor (captainhayashi)
 *  @version  0.001
 *  @brief    Input system generic and interfacing code.
 */

/* Include the relevant headers. */

#include "../dismal.h"
#include "dm-input.h"

#ifdef DM_BASE_SDL
#include "../base/dm-base-sdl.h"
#include "dm-input-sdl.h"
#else /* !DM_BASE_SDL */

#ifdef DM_BASE_AMIGA68K
#include "../base/dm-base-amiga68k.h"
#include "dm-input-amiga68k.h"
#else /* !DM_BASE_AMIGA68K */

#ifdef DM_BASE_DOS
#include "../base/dm-base-dos.h"
#include "dm-input-dos.h"
#else /* !DM_BASE_DOS */

#error No base selected!

#endif /* DM_BASE_DOS */
#endif /* DM_BASE_AMIGA68K */
#endif /* DM_BASE_SDL */

static struct dm_InputBase *_ib;

int dm_input_init(dm_Config *conf)
{
  _ib = malloc(sizeof(struct dm_InputBase));

  if (_ib) {
    _ib->callbacks = NULL;

#ifdef DM_BASE_SDL
    return dm_input_sdl_init(conf);
#else /* !DM_BASE_SDL */

#ifdef DM_BASE_AMIGA68K
    return dm_input_amiga68k_init(conf);
#else /* !DM_BASE_AMIGA68K */

#ifdef DM_BASE_DOS
    return dm_input_dos_init(conf);
#else /* !DM_BASE_DOS */

#error No base selected!

#endif /* DM_BASE_DOS */
#endif /* DM_BASE_AMIGA68K */
#endif /* DM_BASE_SDL */
  } else {
    dm_fatal("INPUT: Could not allocate input base!");
  }

  return DM_FAILURE;
}

void dm_input_cleanup(void)
{
#ifdef DM_BASE_SDL
  dm_input_sdl_cleanup();
#else /* !DM_BASE_SDL */

#ifdef DM_BASE_AMIGA68K
  dm_input_amiga68k_cleanup();
#else /* !DM_BASE_AMIGA68K */

#ifdef DM_BASE_DOS
  dm_input_dos_cleanup();
#else /* !DM_BASE_DOS */

#error No base selected!

#endif /* DM_BASE_DOS */
#endif /* DM_BASE_AMIGA68K */
#endif /* DM_BASE_SDL */

  if (_ib) {
    free(_ib);
    _ib = NULL;
  }
}

void dm_input_process(void)
{
#ifdef DM_BASE_SDL
  dm_input_sdl_process();
#else /* !DM_BASE_SDL */

#ifdef DM_BASE_AMIGA68K
  dm_input_amiga68k_process();
#else /* !DM_BASE_AMIGA68K */

#ifdef DM_BASE_DOS
  dm_input_dos_process();
#else /* !DM_BASE_DOS */

#error No base selected!

#endif /* DM_BASE_DOS */
#endif /* DM_BASE_AMIGA68K */
#endif /* DM_BASE_SDL */
}

struct dm_InputCallback *dm_input_install_callback(void (*cb)(dm_InputEvent *event), 
                                                   int types)
{   
  struct dm_InputCallback *pnew, *p;
  
  pnew = malloc(sizeof(struct dm_InputCallback));

  if (pnew) {
    pnew->cb = cb;
    pnew->types = types;
    pnew->next = NULL;

    /* Link to list */
    if (_ib->callbacks == NULL) {
      _ib->callbacks = pnew;
    } else {
      for (p = _ib->callbacks; p->next != NULL; p = p->next) {
        ; /* Do nothing until p->next is NULL. */
      }
      p->next = pnew;
    }
  }
  return pnew;
}

int dm_input_unload_callback(struct dm_InputCallback *ptr)
{
  struct dm_InputCallback *p;

  if (ptr) {
    /* Corner-case for if ptr is the first callback. */
    if (_ib->callbacks == ptr) {
      _ib->callbacks = ptr->next;
      free(ptr);
      return DM_SUCCESS;
    }

    for (p = _ib->callbacks; p->next != NULL; p = p->next) {
      if (p->next == ptr) {
        /* Now we've found the list node before this one, replace its 
           next pointer with the to-be-unloaded node's next pointer. */
        p->next = ptr->next;

        /* Now delete the callback. */
        free(ptr);
        return DM_SUCCESS;
      }
    }
  }

  return DM_FAILURE;
}

void dm_input_event_release(dm_InputEvent *event)
{
  struct dm_InputCallback *p;

  for (p = _ib->callbacks; p != NULL; p = p->next) {
    /* Trigger all callbacks with the relevant type. */
    if (p->types & event->type) {
      p->cb(event);
    }
  }
}
