/* ISC license. */

#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#include <skalibs/strerr2.h>

#define USAGE "s6-overlay-preinit /var/run"

static const char *VAR_RUN = "/var/run" ;
static const char *VAR_RUN_S6 = "/var/run/s6" ;

static const char *VAR_LOG = "/var/log" ;
static const char *VAR_LOG_S6 = "/var/log/s6" ;

int main (void)
{
  PROG = "s6-overlay-preinit";

  /*
   * performs initialization tasks that must run as root for s6-overlay,
   * then drops privileges and calls the rest of argv. Meant to run as
   * the very first step in s6-overlay's init script
   */

  /* requirement: /var/run must exist */
  if(mkdir(VAR_RUN, 0755) == -1)
  {
    if(errno != EEXIST)
    {
        /* /var/run does not exist and we were unable to create it */
        strerr_diefu2sys(111, "mkdir ", VAR_RUN) ;
    }
  }

  /* requirement: /var/log must exist (for catchall logger) */
  if(mkdir(VAR_LOG, 0755) == -1)
  {
    if(errno != EEXIST)
    {
        /* /var/run does not exist and we were unable to create it */
        strerr_diefu2sys(111, "mkdir ", VAR_LOG) ;
    }
  }

  /* requirement: /var/run/s6 must exist */
  if(mkdir(VAR_RUN_S6,0755) == -1)
  {
    if(errno != EEXIST)
    {
      strerr_diefu2sys(111, "mkdir ", VAR_RUN_S6) ;
    }
  }

  /* requirement: /var/log/s6 must exist */
  if(mkdir(VAR_LOG_S6,0755) == -1)
  {
    if(errno != EEXIST)
    {
      strerr_diefu2sys(111, "mkdir ", VAR_LOG_S6) ;
    }
  }

  /* requirement: /var/run/s6 must be owned by current user */
  if(chown(VAR_RUN_S6,getuid(),getgid()) == -1)
  {
    strerr_diefu2sys(111,"chown ", VAR_RUN_S6) ;
  }

  /* requirement: /var/log/s6 must be owned by current user */
  if(chown(VAR_LOG_S6,getuid(),getgid()) == -1)
  {
    strerr_diefu2sys(111,"chown ", VAR_LOG_S6) ;
  }

  return 0;

}

