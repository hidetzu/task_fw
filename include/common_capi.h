#ifndef __COMMON_CAPI_H__
#define __COMMON_CAPI_H__

#include <stdlib.h>
#include <pthread.h>

#ifndef UNITTEST_ENABLE
#define common_malloc malloc
#define common_free   free
#define common_pthread_mutex_init pthread_mutex_init 
#define common_pthread_mutex_destroy pthread_mutex_destroy 
#define common_pthread_cond_init pthread_cond_init
#define common_pthread_cond_destroy pthread_cond_destroy

#else
void* unittest_malloc(size_t size);
void unittest_free(void* ptr);
int unittest_pthread_mutex_init(pthread_mutex_t* mutex,
        const pthread_mutexattr_t* mutexattr);
int unittest_pthread_mutex_destroy(pthread_mutex_t* mutex);
int unittest_pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr);
int unittest_pthread_cond_destroy(pthread_cond_t *cond);

#define common_malloc unittest_malloc
#define common_free   unittest_free
#define common_pthread_mutex_init unittest_pthread_mutex_init 
#define common_pthread_mutex_destroy unittest_pthread_mutex_destroy 
#define common_pthread_cond_init unittest_pthread_cond_init
#define common_pthread_cond_destroy unittest_pthread_cond_destroy

#endif

#endif  /*__COMMON_CAPI_H__*/
