/** @file     base/dm-base.c
 *  @author   Matt Windsor (captainhayashi)
 *  @version  0.001
 *  @brief    Base interfacing code.
 */

#include "../dismal.h"
#include "dm-base.h"

/* Include the relevant base header. */

#ifdef DM_BASE_SDL
#include "dm-base-sdl.h"
#else /* !DM_BASE_SDL */

#ifdef DM_BASE_AMIGA68K
#include "dm-base-amiga68k.h"
#else /* !DM_BASE_AMIGA68K */

#ifdef DM_BASE_DOS
#include "dm-base-dos.h"
#else /* !DM_BASE_DOS */

#error No base selected!

#endif /* DM_BASE_DOS */
#endif /* DM_BASE_AMIGA68K */
#endif /* DM_BASE_SDL */


int dm_base_init(dm_Config *conf)
{
#ifdef DM_BASE_SDL
  return dm_base_sdl_init(conf);
#else /* !DM_BASE_SDL */

#ifdef DM_BASE_AMIGA68K
  return dm_base_amiga68k_init(conf);
#else /* !DM_BASE_AMIGA68K */

#ifdef DM_BASE_DOS
  return dm_base_dos_init(conf);
#else /* !DM_BASE_DOS */

#error No base selected!

#endif /* DM_BASE_DOS */
#endif /* DM_BASE_AMIGA68K */
#endif /* DM_BASE_SDL */
}

void dm_base_cleanup(void)
{
#ifdef DM_BASE_SDL
  dm_base_sdl_cleanup();
#else /* !DM_BASE_SDL */

#ifdef DM_BASE_AMIGA68K
  dm_base_amiga68k_cleanup();
#else /* !DM_BASE_AMIGA68K */

#ifdef DM_BASE_DOS
  dm_base_dos_cleanup();
#else /* !DM_BASE_DOS */

#error No base selected!

#endif /* DM_BASE_DOS */
#endif /* DM_BASE_AMIGA68K */
#endif /* DM_BASE_SDL */
}


int dm_get_base_id(void)
{
#ifdef DM_BASE_SDL
  return DM_SDL;
#else /* !DM_BASE_SDL */

#ifdef DM_BASE_AMIGA68K
  return DM_AMIGA68K;
#else /* !DM_BASE_AMIGA68K */

#ifdef DM_BASE_DOS
  return DM_DOS;
#else /* !DM_BASE_DOS */

#error No base selected!

#endif /* DM_BASE_DOS */
#endif /* DM_BASE_AMIGA68K */
#endif /* DM_BASE_SDL */
}
