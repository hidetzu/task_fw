#include <common_capi.h>
#include <stdio.h>
#include <CppUTestExt/MockSupport.h>

void mock_unittest_malloc(int size, void* pReturn)
{
    mock().expectOneCall("unittest_malloc")
        .withParameter("size", (int)size)
        .andReturnValue((void*)pReturn);
}

void mock_unittest_free(void* pObject)
{
    mock().expectOneCall("unittest_free")
        .withParameter("ptr", pObject);
}

void mock_unittest_pthread_mutex_init(pthread_mutex_t* mutex,
        pthread_mutexattr_t* attr,
        int returnValue)
{
    mock().expectOneCall("unittest_pthread_mutex_init")
        .withParameter("mutex", mutex)
        .withParameter("mutexattr", attr)
        .andReturnValue(returnValue);
}

void mock_unittest_pthread_mutex_destroy(pthread_mutex_t* mutex)
{
    mock().expectOneCall("unittest_pthread_mutex_destroy")
        .withParameter("mutex", mutex);
}

void mock_unittest_pthread_cond_init(pthread_cond_t* cond,
        pthread_condattr_t* attr,
        int returnValue)
{
    mock().expectOneCall("unittest_pthread_cond_init")
        .withParameter("cond", cond)
        .withParameter("attr", (void*)NULL)
        .andReturnValue(returnValue);
}

void mock_unittest_pthread_cond_destroy(pthread_cond_t* cond)
{
    mock().expectOneCall("unittest_pthread_cond_destroy")
        .withParameter("cond", cond);
}

void mock_unittest_pthread_mutex_lock(pthread_mutex_t* mutex)
{
    mock().expectOneCall("unittest_pthread_mutex_lock")
        .withParameter("mutex", mutex);
}

void mock_unittest_pthread_mutex_unlock(pthread_mutex_t* mutex)
{
    mock().expectOneCall("unittest_pthread_mutex_unlock")
        .withParameter("mutex", mutex);
}

void mock_unittest_pthread_cond_signal(pthread_cond_t* cond)
{
    mock().expectOneCall("unittest_pthread_cond_signal")
        .withParameter("cond", cond);
}

void mock_unittest_pthread_cond_wait(pthread_cond_t* cond,
    pthread_mutex_t* mutex)
{
    mock().expectOneCall("unittest_pthread_cond_wait")
        .withParameter("cond", cond)
        .withParameter("mutex", mutex);
}

void mock_unittest_pthread_create(
        pthread_t *thread,
        const pthread_attr_t *attr, 
        void *(*start_routine)(void *), 
        void *arg,
        int returnValue)
{
    mock().expectOneCall("unittest_pthread_create")
        .withParameter("thread", thread)
        .withParameter("attr", (void*)attr)
        .withParameter("start_routine", (void*)start_routine)
        .withParameter("arg", arg)
        .andReturnValue(returnValue);

}

void mock_unittest_pthread_join(pthread_t *thread,
        void **value_ptr,
        int returnValue)
{
    mock().expectOneCall("unittest_pthread_join")
        .withParameter("thread", thread)
        .withParameter("value_ptr", value_ptr)
        .andReturnValue(returnValue);
}
