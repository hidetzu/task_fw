#include <common_capi.h>

#include "CppUTestExt/MockSupport_c.h"

void* unittest_malloc(size_t size)
{
    mock_c()->actualCall("unittest_malloc")
        ->withIntParameters("size", (int)size);
    return mock_c()->returnValue().value.pointerValue;
}

void unittest_free(void *ptr)
{
    mock_c()->actualCall("unittest_free")
        ->withPointerParameters("ptr", ptr);
}

int unittest_pthread_mutex_init(pthread_mutex_t* mutex,
        const pthread_mutexattr_t* mutexattr)
{
    mock_c()->actualCall("unittest_pthread_mutex_init")
        ->withPointerParameters("mutex", (void *)mutex)
        ->withPointerParameters("mutexattr", (void *)mutexattr);

    return mock_c()->returnValue().value.intValue;
}

int unittest_pthread_mutex_destroy(pthread_mutex_t* mutex)
{
    mock_c()->actualCall("unittest_pthread_mutex_destroy")
        ->withPointerParameters("mutex", (void *)mutex);

    return mock_c()->returnValue().value.intValue;
}

int unittest_pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr)
{
    mock_c()->actualCall("unittest_pthread_cond_init")
        ->withPointerParameters("cond", (void *)cond)
        ->withPointerParameters("attr", (void *)attr);
    return mock_c()->returnValue().value.intValue;
}

int unittest_pthread_cond_destroy(pthread_cond_t *cond)
{
    mock_c()->actualCall("unittest_pthread_cond_destroy")
        ->withPointerParameters("cond", (void *)cond);

    return mock_c()->returnValue().value.intValue;
}

