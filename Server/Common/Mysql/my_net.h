/*
   Copyright (c) 2000, 2011, Oracle and/or its affiliates. All rights reserved.










   The lines above are intentionally left blank
*/

/*
  This file is also used to make handling of sockets and ioctl()
  portable accross systems.

*/

#ifndef _my_net_h
#define _my_net_h

#include "my_global.h"                  /* C_MODE_START, C_MODE_END */

C_MODE_START

#include <errno.h>

#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif

#ifdef HAVE_POLL
#include <sys/poll.h>
#endif

#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif

#if !defined(__WIN__) && !defined(HAVE_BROKEN_NETINET_INCLUDES)
#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
# if !defined(alpha_linux_port)
#   include <netinet/tcp.h>
# endif
#endif

#if defined(__WIN__)
#define O_NONBLOCK 1    /* For emulation of fcntl() */

/*
  SHUT_RDWR is called SD_BOTH in windows and
  is defined to 2 in winsock2.h
  #define SD_BOTH 0x02
*/
#define SHUT_RDWR 0x02
#else
#include <netdb.h>     /* getaddrinfo() & co */
#endif

/*
  On OSes which don't have the in_addr_t, we guess that using uint32 is the best
  possible choice. We guess this from the fact that on HP-UX64bit & FreeBSD64bit
  & Solaris64bit, in_addr_t is equivalent to uint32. And on Linux32bit too.
*/
#ifndef HAVE_IN_ADDR_T
#define in_addr_t uint32
#endif


C_MODE_END
#endif
