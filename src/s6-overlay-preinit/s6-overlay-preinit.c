/* ISC license. */

#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

#include <skalibs/strerr2.h>
#include <skalibs/djbunix.h>
#include <skalibs/stralloc.h>

#define USAGE "s6-overlay-preinit /var/run"

int main (int argc, char const *const *argv)
{
  stralloc sa = STRALLOC_ZERO;
  PROG = "s6-overlay-preinit";
  if (argc < 2) strerr_dieusage(100, USAGE) ;

  /*
   * performs initialization tasks that must run as root for s6-overlay,
   * then drops privileges and calls the rest of argv. Meant to run as
   * the very first step in s6-overlay's init script
   */

  /* create string for /var/run/s6 */
  if(stralloc_copys(&sa,argv[1]) == 0)
    strerr_die2x(111,"s6-overlay","out of memory") ;

  /* remove trailing slashes, if any */
  while(sa.len && sa.s[sa.len-1] == '/')
  {
      sa.len--;
  }
  if(stralloc_append(&sa,'/') == 0)
    strerr_die2x(111,"s6-overlay","out of memory") ;
  if(stralloc_cats(&sa,"s6") == 0)
    strerr_die2x(111,"s6-overlay","out of memory") ;
  if(stralloc_0(&sa) == 0)
    strerr_die2x(111,"s6-overlay","out of memory") ;

  /* requirement: /var/run must exist */
  if(mkdir(argv[1], 0755) == -1)
  {
    if(errno != EEXIST)
    {
        /* /var/run does not exist and we were unable to create it */
        strerr_diefu2sys(111, "mkdir ", argv[1]) ;
    }
  }

  /* requirement: /var/run/s6 must exist */
  if(mkdir(sa.s,0755) == -1)
  {
    if(errno != EEXIST)
    {
      strerr_diefu2sys(111, "mkdir ", sa.s) ;
    }
  }

  /* requirement: /var/run/s6 must be owned by current user */
  if(chown(sa.s,getuid(),getgid()) == -1)
  {
    strerr_diefu2sys(111,"chown ", sa.s) ;
  }

  /* if running with suid bit set, uid = intended user, euid = root */
  if(seteuid(getuid()) == -1)
  {
      strerr_diefu1sys(111,"seteuid") ;
  }

  stralloc_free(&sa) ;
  return 0;

}

