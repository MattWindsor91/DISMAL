/** @file     input/dm-input.h
 *  @author   Matt Windsor (captainhayashi)
 *  @version  0.001
 *  @brief    Header for input generic/interfacing code.
 *
 *  The DISMAL input system provides a wrapper around the input
 *  facilities of DISMAL's target.
 *
 *  Input drivers are mapped 1:1 to bases, so the input system
 *  selected will always match the base selected.
 */

#ifndef __DM_INPUT_H__
#define __DM_INPUT_H__

#include "../dismal.h"

enum {
  DM_QUIT_EVENT              = (1<<0), /**< Identifier for low-level
                                          quit event. */
  DM_MOUSE_MOTION_EVENT      = (1<<1), /**< Identifier for mouse motion
                                         event. */
  DM_MOUSE_BUTTON_DOWN_EVENT = (1<<2), /**< Identifier for mouse
                                         button down event. */
  DM_MOUSE_BUTTON_UP_EVENT   = (1<<3), /**< Identifier for mouse
                                         button up event. */
  DM_ASCII_KEY_DOWN_EVENT    = (1<<4), /**< Identifier for ASCII
                                          keydown event. */
  DM_ASCII_KEY_UP_EVENT      = (1<<5), /**< Identifier for ASCII
                                          keyup event. */
  DM_SPECIAL_KEY_DOWN_EVENT  = (1<<6), /**< Identifier for ASCII
                                          keydown event. */
  DM_SPECIAL_KEY_UP_EVENT    = (1<<7), /**< Identifier for ASCII
                                          keyup event. */

  DM_CB_FAIL = -1,           /**< Callback ID returned for install failure. */

  DM_LMB = (1<<0),           /**< Left mouse button. */
  DM_MMB = (1<<1),           /**< Middle mouse button. */
  DM_RMB = (1<<2),           /**< Right mouse button. */

  DM_SK_ESCAPE = 0           /**< Identifier for Escape special key. */
};

typedef union dm_InputEvent dm_InputEvent;        /**< Input event type. */
typedef struct dm_InputCallback dm_InputCallback; /**< Input callback type. */

/** The input system base structure. */
struct dm_InputBase {
  struct dm_InputCallback *callbacks; /**< Linked list of callbacks. */
};

/** A mouse motion input event. */
struct dm_MouseMotionEvent {
  unsigned char type;   /**< The type identifier of the input event. */
  unsigned short x; /**< New X co-ordinate of the mouse on the
                       screen. */
  unsigned short y; /**< New Y co-ordinate of the mouse on the
                       screen. */
  unsigned short xraw; /**< Mouse X co-ordinate without
                          transformations. */
  unsigned short yraw; /**< Mouse X co-ordinate without
                          transformations. */
  short deltax; /**< Change in X co-ordinate from previous position. */
  short deltay; /**< Change in Y co-ordinate from previous position. */
};


/** A mouse button input event. */
struct dm_MouseButtonEvent {
  unsigned char type;   /**< The type identifier of the input event. */ 
  unsigned char button; /**< Which button was pressed. */
};


/** An ASCII keyboard event. */
struct dm_ASCIIKeyEvent {
  unsigned char type; /**< Whether the key was pressed or released. */
  char code; /**< The ASCII code pressed. */
};


/** A special (non-ASCII) keyboard event. */
struct dm_SpecialKeyEvent {
  unsigned char type; /**< Whether the key was pressed or released. */
  char code; /**< The special code pressed (see enum). */
};


/** An input event package. */
union dm_InputEvent {
  unsigned char type;   /**< The type identifier of the input event. */
  struct dm_MouseMotionEvent motion; /**< A mouse motion event. */
  struct dm_MouseButtonEvent button; /**< A mouse button event. */
  struct dm_ASCIIKeyEvent ascii;     /**< An ASCII keyboard event. */
};


/** A callback node. */
struct dm_InputCallback {
  void (*cb)(dm_InputEvent *event); /**< The callback function
                                       pointer. */
  int types;                        /**< Types of event that will
                                       trigger the callback. */

  struct dm_InputCallback *next;   /**< Next callback in the linked
                                      list. */
};


/** Initialise the compiled input module.
 *
 *  This will initialise the internal 
 *
 *  @param conf  A pointer to a dm_Config structure filled with
 *  initial configuration values.
 *
 *  @return non-zero (DM_SUCCESS) for success, zero (DM_FAILURE) for
 *  failure.
 */
int dm_input_init(struct dm_Config *conf);


/** De-initialise the compiled input module. */
void dm_input_cleanup(void);


/** Process one frame of input.
 *
 *  This function calls the base-specific input routines to handle any
 *  pending input events, and trigger any relevant callbacks.
 *
 */
void dm_input_process(void);


/** Install a callback.
 *
 *  On the event types specified in types, the callback function will
 *  be triggered with a parameter of a struct dm_InputEvent. This will
 *  include the information necessary to process the event.
 *
 *  @param cb     A pointer to the callback function to install.
 *  @param types  A bitwise OR'd flag list of events that should
 *                trigger the callback.
 *
 *  @return a pointer to the installed dm_InputCallback, used for
 *  identifying the callback later (eg for deletion).
 */

struct dm_InputCallback *dm_input_install_callback(void (*cb)(dm_InputEvent *event),
                                                   int types);

/** Unload a callback.
 *
 *  @see dm_input_install_callback()
 *
 *  @param ptr  A pointer to the callback to unload.
 *
 *  @return DM_SUCCESS if the callback was found and deleted,
 *  DM_FAILURE otherwise.
 */

int dm_input_unload_callback(struct dm_InputCallback *ptr);

/** Release an event package to all relevant callbacks.
 *
 *  @param event  The event to release to callbacks.
 */

void dm_input_event_release(dm_InputEvent *event);

#endif /* __DM_INPUT_H__ */
