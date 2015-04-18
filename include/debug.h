#ifndef __DEBUG_H__
#define __DEBUG_H__

#define DEBUG

#ifdef DEBUG
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

extern pid_t gettid(void);
#define debug_print     fprintf
#define debug_gettid    gettid
#else

#define debug_print 1? (void)0 : fprintf
#define debug_gettid 1? (void)0 : gettid
#endif

#define DEBUG_PRINT(fmt, ...)  \
    debug_print(stdout, "tid=%p,""%s(%d):" fmt, (void*)pthread_self(), __func__, __LINE__, ##__VA_ARGS__ )

#define DEBUG_ERROR(fmt, ...)  \
    debug_print(stderr, "tid=%p,""%s(%d):" fmt, (void*)pthread_self(), __func__, __LINE__, ##__VA_ARGS__ )


#endif  /*__DEBUG_H__*/
