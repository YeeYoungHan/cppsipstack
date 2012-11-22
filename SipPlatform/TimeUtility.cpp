#include "SipPlatformDefine.h"

#ifdef WIN32
#include <sys/timeb.h>

int gettimeofday( struct timeval *tv, struct timezone *tz )
{
  struct _timeb timebuffer;

  _ftime( &timebuffer );
  tv->tv_sec = (long) timebuffer.time;
  tv->tv_usec = timebuffer.millitm * 1000;

  return 0;
}

#endif
