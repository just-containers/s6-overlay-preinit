/* ISC license. */

#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#include <skalibs/strerr2.h>

static const char *VAR        = "/var" ;
static const char *VAR_RUN    = "/var/run" ;
static const char *VAR_RUN_S6 = "/var/run/s6" ;

int main (void)
{
  PROG = "s6-overlay-preinit";

  /*
   * performs initialization tasks that must run as root for s6-overlay,
   * then drops privileges and calls the rest of argv. Meant to run as
   * the very first step in s6-overlay's init script
   */

  /* requirement: /var must exist */
  if(mkdir(VAR, 0755) == -1)
  {
    if(errno != EEXIST)
    {
        /* /var does not exist and we were unable to create it */
        strerr_diefu2sys(111, "mkdir ", VAR) ;
    }
  }

  /* requirement: /var/run must exist */
  if(mkdir(VAR_RUN, 0755) == -1)
  {
    if(errno != EEXIST)
    {
        /* /var/run does not exist and we were unable to create it */
        strerr_diefu2sys(111, "mkdir ", VAR_RUN) ;
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

  /* requirement: /var/run/s6 must be owned by current user,
   * if (and only if) current user != root */
  uid_t uid  = getuid() ,
        gid  = getgid() ,
        euid = geteuid() ;

  struct stat s6stat ;
  if(stat(VAR_RUN_S6, &s6stat) == -1)
  {
    strerr_diefu2sys(111, "stat ", VAR_RUN_S6) ;
  }

  /* only call chown if uid/gid are not from current user */
  if(s6stat.st_uid != uid || s6stat.st_gid != gid) 
  {
    if (euid == 0) /* chown will fail if we're not root */
    {
      if (chown(VAR_RUN_S6, uid, gid) == -1)
      {
        strerr_diefu2sys(111, "chown ", VAR_RUN_S6) ;
      }
    }
  }

  return 0;

}

