#ifdef OS_WINCE
# if _WIN32_WCE > 0x600
#  include <stdlib.h>
# endif
# include <winsock2.h>
#endif

#ifndef SRC_GLOBAL_FIRST_H
#define SRC_GLOBAL_FIRST_H

#ifdef OS_WINCE
# define PRINTMSG RETAILMSG
//# define PRINTMSG 
#else
# define PRINTMSG
#endif
#if defined(OS_WIN32) || defined(OS_WINCE)
#else
# define __FUNCTIONW__ __FUNCTION__
#endif
#endif // SRC_GLOBAL_FIRST_H