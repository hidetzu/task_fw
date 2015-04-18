#include <unistd.h>

#include <common_capi.h>
#include "CppUTestExt/MockSupport_c.h"

int wait_signale = 0;
int pthread_create_start = 0;

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

int unittest_pthread_mutex_lock(pthread_mutex_t *mutex)
{
    mock_c()->actualCall("unittest_pthread_mutex_lock")
        ->withPointerParameters("mutex", (void *)mutex);
    return mock_c()->returnValue().value.intValue;
}

int unittest_pthread_mutex_unlock(pthread_mutex_t *mutex)
{
    mock_c()->actualCall("unittest_pthread_mutex_unlock")
        ->withPointerParameters("mutex", (void *)mutex);

    return mock_c()->returnValue().value.intValue;
}
int unittest_pthread_cond_signal(pthread_cond_t *cond)
{
    mock_c()->actualCall("unittest_pthread_cond_signal")
    ->withPointerParameters("cond", (void *)cond);
    if (wait_signale == 0) {
        wait_signale = 1;
    }

    return mock_c()->returnValue().value.intValue;
}

int unittest_pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
    mock_c()->actualCall("unittest_pthread_cond_wait")
    ->withPointerParameters("cond", (void *)cond)
    ->withPointerParameters("mutex", (void *)mutex);

    while (wait_signale == 0) {
        sleep(1);
    }
    wait_signale = 0;

    return mock_c()->returnValue().value.intValue;
}

int unittest_pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg)
{
    mock_c()->actualCall("unittest_pthread_create")
    ->withPointerParameters("thread", (void *)thread)
    ->withPointerParameters("attr", (void *)attr)
    ->withPointerParameters("start_routine", (void*)start_routine)
    ->withPointerParameters("arg", arg);


    return mock_c()->returnValue().value.intValue;
}

int unittest_pthread_join(pthread_t thread, void **value_ptr)
{
    mock_c()->actualCall("unittest_pthread_join")
    ->withPointerParameters("thread", (void *)thread)
    ->withPointerParameters("value_ptr", value_ptr);

    return mock_c()->returnValue().value.intValue;
}
