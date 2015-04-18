
extern "C"{
    #include <taskFw_msgQueue.h>
};

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

static void mock_unittest_malloc(int size, void* pReturn) {
    mock().expectOneCall("unittest_malloc")
        .withParameter("size", (int)size)
        .andReturnValue((void*)pReturn);
}

static void mock_unittest_free(void* pObject) {
    mock().expectOneCall("unittest_free")
        .withParameter("ptr", pObject);
}

static void mock_unittest_pthread_mutex_init(pthread_mutex_t* mutex,
        pthread_mutexattr_t* attr,
        int returnValue) {
    mock().expectOneCall("unittest_pthread_mutex_init")
        .withParameter("mutex", mutex)
        .withParameter("mutexattr", attr)
        .andReturnValue(returnValue);
}

static void mock_unittest_pthread_mutex_destroy(pthread_mutex_t* mutex) {
    mock().expectOneCall("unittest_pthread_mutex_destroy")
        .withParameter("mutex", mutex);
}

static void mock_unittest_pthread_cond_init(pthread_cond_t* cond,
        pthread_condattr_t* attr,
        int returnValue) {
    mock().expectOneCall("unittest_pthread_cond_init")
        .withParameter("cond", cond)
        .withParameter("attr", (void*)NULL)
        .andReturnValue(returnValue);
}

static void mock_unittest_pthread_cond_destroy(pthread_cond_t* cond) {
    mock().expectOneCall("unittest_pthread_cond_destroy")
        .withParameter("cond", cond);
}

TEST_GROUP(TestTaskFw_msgQueue)
{
    struct _t_taskFw_msgQueue {
        pthread_mutex_t mutex;
        pthread_cond_t cond;
    };

    void setup()
    {
    }

    void teardown()
    {
        mock().checkExpectations();
        mock().clear();
    }
};

TEST(TestTaskFw_msgQueue, createFailed)
{
    mock_unittest_malloc(sizeof(struct _t_taskFw_msgQueue), NULL);

    t_taskFw_msgQueue* pThis= taskFw_msgQueue_create();
    POINTERS_EQUAL(NULL, pThis);
}

TEST(TestTaskFw_msgQueue, createMutextError)
{
    struct _t_taskFw_msgQueue msgQueue;

    mock_unittest_malloc(sizeof(struct _t_taskFw_msgQueue), &msgQueue);
    mock_unittest_pthread_mutex_init(&msgQueue.mutex,
            NULL, -1);
    mock_unittest_free(&msgQueue);

    t_taskFw_msgQueue* pThis= taskFw_msgQueue_create();
    POINTERS_EQUAL(NULL, pThis);
}

TEST(TestTaskFw_msgQueue, createCondError)
{
    struct _t_taskFw_msgQueue msgQueue;

    mock_unittest_malloc(sizeof(struct _t_taskFw_msgQueue), &msgQueue);
    mock_unittest_pthread_mutex_init(&msgQueue.mutex, NULL, 0);
    mock_unittest_pthread_cond_init(&msgQueue.cond, NULL, -1);
    mock_unittest_pthread_mutex_destroy(&msgQueue.mutex);

    mock_unittest_free(&msgQueue);

    t_taskFw_msgQueue* pThis= taskFw_msgQueue_create();
    POINTERS_EQUAL(NULL, pThis);
}

TEST(TestTaskFw_msgQueue, deleteNullObject)
{
    taskFw_msgQueue_delete(NULL);
}

TEST(TestTaskFw_msgQueue, createAndDelete)
{
    struct _t_taskFw_msgQueue msgQueue;

    mock_unittest_malloc(sizeof(struct _t_taskFw_msgQueue), &msgQueue);
    mock_unittest_pthread_mutex_init(&msgQueue.mutex, NULL, 0);
    mock_unittest_pthread_cond_init(&msgQueue.cond, NULL, 0);

    t_taskFw_msgQueue* pThis= taskFw_msgQueue_create();

    POINTERS_EQUAL(&msgQueue, pThis);

    mock_unittest_free(&msgQueue);
    mock_unittest_pthread_mutex_destroy(&msgQueue.mutex);
    mock_unittest_pthread_cond_destroy(&msgQueue.cond);

    taskFw_msgQueue_delete(pThis);
}

TEST(TestTaskFw_msgQueue, QueueAddOneMsg)
{
    struct _t_taskFw_msgQueue msgQueue;

    mock_unittest_malloc(sizeof(struct _t_taskFw_msgQueue), &msgQueue);
    mock_unittest_pthread_mutex_init(&msgQueue.mutex, NULL, 0);
    mock_unittest_pthread_cond_init(&msgQueue.cond, NULL, 0);

    t_taskFw_msgQueue* pThis= taskFw_msgQueue_create();
    POINTERS_EQUAL(&msgQueue, pThis);

    int ret = 0;

    t_taskFw_taskMsg msg;
    msg.type = 1;
    ret = taskFw_msgQueue_putMsg(pThis, &msg);
    CHECK_EQUAL(0, ret );

    ret = taskFw_msgQueue_putMsg(pThis, NULL);
    CHECK_EQUAL(-1, ret );

    ret = taskFw_msgQueue_putMsg(NULL, &msg);
    CHECK_EQUAL(-1, ret );


    POINTERS_EQUAL( NULL, taskFw_msgQueue_getMsg(NULL));
    POINTERS_EQUAL( NULL, taskFw_msgQueue_getMsg(pThis));

    mock_unittest_free(&msgQueue);
    mock_unittest_pthread_mutex_destroy(&msgQueue.mutex);
    mock_unittest_pthread_cond_destroy(&msgQueue.cond);

    taskFw_msgQueue_delete(pThis);
}
