#ifndef __MOCK_UNITTEST_CAPI_H__
#define __MOCK_UNITTEST_CAPI_H__

void mock_unittest_malloc(int size, void* pReturn);
void mock_unittest_free(void* pObject);
void mock_unittest_pthread_mutex_init(pthread_mutex_t* mutex, pthread_mutexattr_t* attr, int returnValue);
void mock_unittest_pthread_mutex_destroy(pthread_mutex_t* mutex);
void mock_unittest_pthread_cond_init(pthread_cond_t* cond, pthread_condattr_t* attr, int returnValue);
void mock_unittest_pthread_cond_destroy(pthread_cond_t* cond);
void mock_unittest_pthread_mutex_lock(pthread_mutex_t* mutex);
void mock_unittest_pthread_mutex_unlock(pthread_mutex_t* mutex);
void mock_unittest_pthread_cond_signal(pthread_cond_t* cond);
void mock_unittest_pthread_cond_wait(pthread_cond_t* cond, pthread_mutex_t* mutex);
void mock_unittest_pthread_create(pthread_t *thread,
        const pthread_attr_t *attr, 
        void *(*start_routine)(void *), 
        void *arg, int returnValue);
void mock_unittest_pthread_join(pthread_t *thread,
        void **value_ptr,
        int returnValue);


#endif  /*__MOCK_UNITTEST_CAPI_H__*/
