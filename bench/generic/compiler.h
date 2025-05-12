#ifndef COMPILER_H
#define COMPILER_H

#ifdef __MINGW32__
    #include "float.h"
    #include "limits.h"
    #define SPRINTF ::sprintf
#else
#define SPRINTF ::sprintf_s
#endif

/* ring Internal is for acccess from code only*/
#define  RING_INTERNAL 0xff001234u
/* ring external is for access from script and user interface*/
#define  RING_EXTERNAL 0xff004321u


#endif // COMPILER_H
