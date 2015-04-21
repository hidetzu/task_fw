
extern "C"{
    #include <signal.h>
    #include <unistd.h>
    #include <taskFw_msgQueue.h>
    #include <taskFw_api_private.h>
};

#include <mock_unittest_capi.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

static t_taskFw_msgQueue* pWaitMsgQueue;
static t_taskFw_taskMsg waitMsg;

static void test_signal_handler(int sig) {
    int ret = taskFw_msgQueue_putMsg(pWaitMsgQueue, &waitMsg);
    CHECK_EQUAL(0, ret);
}

TEST_GROUP(TestTaskFw_msgQueue)
{
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
    mock_unittest_malloc(sizeof(t_taskFw_msgQueue), NULL);

    t_taskFw_msgQueue* pThis= taskFw_msgQueue_create();
    POINTERS_EQUAL(NULL, pThis);
}

TEST(TestTaskFw_msgQueue, createMutextError)
{
    t_taskFw_msgQueue msgQueue;

    mock_unittest_malloc(sizeof(t_taskFw_msgQueue), &msgQueue);
    mock_unittest_pthread_mutex_init(&msgQueue.mutex,
            NULL, -1);
    mock_unittest_free(&msgQueue);

    t_taskFw_msgQueue* pThis= taskFw_msgQueue_create();
    POINTERS_EQUAL(NULL, pThis);
}

TEST(TestTaskFw_msgQueue, createCondError)
{
    t_taskFw_msgQueue msgQueue;

    mock_unittest_malloc(sizeof(t_taskFw_msgQueue), &msgQueue);
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
    t_taskFw_msgQueue msgQueue;

    mock_unittest_malloc(sizeof(t_taskFw_msgQueue), &msgQueue);
    mock_unittest_pthread_mutex_init(&msgQueue.mutex, NULL, 0);
    mock_unittest_pthread_cond_init(&msgQueue.cond, NULL, 0);

    t_taskFw_msgQueue* pThis= taskFw_msgQueue_create();

    POINTERS_EQUAL(&msgQueue, pThis);
    POINTERS_EQUAL(NULL, msgQueue.pTop);
    POINTERS_EQUAL(NULL, msgQueue.pTail);

    mock_unittest_free(&msgQueue);
    mock_unittest_pthread_mutex_destroy(&msgQueue.mutex);
    mock_unittest_pthread_cond_destroy(&msgQueue.cond);

    taskFw_msgQueue_delete(pThis);
}

TEST(TestTaskFw_msgQueue, putMsgFailed1)
{
    int ret = 0;
    t_taskFw_msgQueue msgQueue;

    mock_unittest_malloc(sizeof(t_taskFw_msgQueue), &msgQueue);
    mock_unittest_pthread_mutex_init(&msgQueue.mutex, NULL, 0);
    mock_unittest_pthread_cond_init(&msgQueue.cond, NULL, 0);

    t_taskFw_msgQueue* pThis= taskFw_msgQueue_create();

    ret = taskFw_msgQueue_putMsg(pThis, NULL);
    CHECK_EQUAL(-1, ret );

}

TEST(TestTaskFw_msgQueue, putMsgFailed2)
{
    t_taskFw_taskMsg msg;
    int ret = taskFw_msgQueue_putMsg(NULL, &msg);
    CHECK_EQUAL(-1, ret );
}


TEST(TestTaskFw_msgQueue, QueueAddMsgs)
{
    t_taskFw_msgQueue msgQueue;

    mock_unittest_malloc(sizeof(t_taskFw_msgQueue), &msgQueue);
    mock_unittest_pthread_mutex_init(&msgQueue.mutex, NULL, 0);
    mock_unittest_pthread_cond_init(&msgQueue.cond, NULL, 0);

    t_taskFw_msgQueue* pThis= taskFw_msgQueue_create();
    POINTERS_EQUAL(&msgQueue, pThis);

    int ret = 0;
    t_taskFw_taskMsg msg;

    mock_unittest_pthread_mutex_lock(&msgQueue.mutex);
    mock_unittest_pthread_cond_signal(&msgQueue.cond);
    mock_unittest_pthread_mutex_unlock(&msgQueue.mutex);

    ret = taskFw_msgQueue_putMsg(pThis, &msg);
    CHECK_EQUAL(0, ret );

    t_taskFw_taskMsg msg1;

    mock_unittest_pthread_mutex_lock(&msgQueue.mutex);
    mock_unittest_pthread_cond_signal(&msgQueue.cond);
    mock_unittest_pthread_mutex_unlock(&msgQueue.mutex);

    ret = taskFw_msgQueue_putMsg(pThis, &msg1);
    CHECK_EQUAL(0, ret );


    mock_unittest_pthread_mutex_lock(&msgQueue.mutex);
    mock_unittest_pthread_mutex_unlock(&msgQueue.mutex);

    t_taskFw_taskMsg* pResult = taskFw_msgQueue_getMsg(pThis);
    CHECK_EQUAL(&msg, pResult);

    mock_unittest_pthread_mutex_lock(&msgQueue.mutex);
    mock_unittest_pthread_mutex_unlock(&msgQueue.mutex);

    t_taskFw_taskMsg* pResult1 = taskFw_msgQueue_getMsg(pThis);
    CHECK_EQUAL(&msg1, pResult1);

    mock_unittest_free(&msgQueue);
    mock_unittest_pthread_mutex_destroy(&msgQueue.mutex);
    mock_unittest_pthread_cond_destroy(&msgQueue.cond);

    taskFw_msgQueue_delete(pThis);
}

TEST(TestTaskFw_msgQueue, QueueAddMsg2)
{
    t_taskFw_msgQueue msgQueue;

    mock_unittest_malloc(sizeof(t_taskFw_msgQueue), &msgQueue);
    mock_unittest_pthread_mutex_init(&msgQueue.mutex, NULL, 0);
    mock_unittest_pthread_cond_init(&msgQueue.cond, NULL, 0);

    t_taskFw_msgQueue* pThis= taskFw_msgQueue_create();
    POINTERS_EQUAL(&msgQueue, pThis);


    t_taskFw_taskMsg* pResult = NULL;
    pWaitMsgQueue = pThis;

    signal(SIGALRM, test_signal_handler);
    alarm(5);


    mock_unittest_pthread_mutex_lock(&msgQueue.mutex);
    mock_unittest_pthread_cond_wait(&msgQueue.cond, &msgQueue.mutex);
    mock_unittest_pthread_mutex_unlock(&msgQueue.mutex);

    mock_unittest_pthread_mutex_lock(&msgQueue.mutex);
    mock_unittest_pthread_cond_signal(&msgQueue.cond);
    mock_unittest_pthread_mutex_unlock(&msgQueue.mutex);

    pResult = taskFw_msgQueue_getMsg(pThis);
    CHECK_EQUAL(&waitMsg, pResult);


    mock_unittest_free(&msgQueue);
    mock_unittest_pthread_mutex_destroy(&msgQueue.mutex);
    mock_unittest_pthread_cond_destroy(&msgQueue.cond);

    taskFw_msgQueue_delete(pThis);
}
