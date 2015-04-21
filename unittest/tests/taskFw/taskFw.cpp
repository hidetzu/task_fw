
extern "C"{
    #include <taskFw.h>
    #include <taskFw_msgQueue.h>
    #include <stdio.h>
    #include <taskFw_api_private.h>
};

#include <mock_unittest_capi.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

TEST_GROUP(TestTaskFwGroup)
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

TEST(TestTaskFwGroup, createFailed)
{
    mock_unittest_malloc(sizeof(struct _t_taskFw), NULL);

    t_taskFw* pThis = taskFw_create();

    POINTERS_EQUAL(NULL, pThis);

    taskFw_delete(pThis);
}

TEST(TestTaskFwGroup, createFailed2)
{
    t_taskFw taskFwObj;

    mock_unittest_malloc(sizeof(t_taskFw), &taskFwObj);
    mock_unittest_malloc(sizeof(t_taskFw_msgQueue), NULL);
    mock_unittest_free(&taskFwObj);

    t_taskFw* pThis = taskFw_create();
    POINTERS_EQUAL(NULL, pThis);
    POINTERS_EQUAL(NULL, taskFwObj.pRequestQueue);
}

TEST(TestTaskFwGroup, startFailed)
{
    t_taskFw taskFwObj;
    taskFwObj.pRequestQueue = NULL;

    int ret = taskFw_startTask(NULL);
    CHECK_EQUAL(-1, ret);

    ret = taskFw_startTask(&taskFwObj);
    CHECK_EQUAL(-1, ret);
}

TEST(TestTaskFwGroup, startFailed2)
{
#if 0
    t_taskFw taskFwObj;
    t_taskFw_msgQueue msgQueue;

    mock_unittest_malloc(sizeof(t_taskFw), &taskFwObj);
    mock_unittest_malloc(sizeof(t_taskFw_msgQueue), &msgQueue);
    mock_unittest_pthread_mutex_init(&msgQueue.mutex, NULL, 0);
    mock_unittest_pthread_cond_init(&msgQueue.cond, NULL, 0);

    t_taskFw* pThis = taskFw_create();
    POINTERS_EQUAL(&taskFwObj, pThis);

    mock_unittest_pthread_create(&taskFwObj.tid,
        NULL,
        taskFw_start_routine,
        (void*)&taskFwObj, -1);


    int ret = taskFw_startTask(&taskFwObj);
    CHECK_EQUAL(-1, ret);


    mock_unittest_free(&taskFwObj);
    mock_unittest_free(&msgQueue);
    mock_unittest_pthread_mutex_destroy(&msgQueue.mutex);
    mock_unittest_pthread_cond_destroy(&msgQueue.cond);

    taskFw_delete(pThis);
#endif
}

TEST(TestTaskFwGroup, putMsgFailed)
{
    int ret = 0;
    t_taskFw_callMsg msg;
    t_taskFw task;
    ret = taskFw_putCallMsg(NULL,&msg, NULL);
    CHECK_EQUAL(-1, ret);

    ret = taskFw_putCallMsg(&task,NULL, NULL);
    CHECK_EQUAL(-1, ret);

}


TEST(TestTaskFwGroup, createAndDelete)
{
    t_taskFw taskFwObj;
    t_taskFw_msgQueue msgQueue;

    mock_unittest_malloc(sizeof(t_taskFw), &taskFwObj);
    mock_unittest_malloc(sizeof(t_taskFw_msgQueue), &msgQueue);
    mock_unittest_pthread_mutex_init(&msgQueue.mutex, NULL, 0);
    mock_unittest_pthread_cond_init(&msgQueue.cond, NULL, 0);

    mock_unittest_pthread_mutex_init(&taskFwObj.stateInfo.mutex, NULL, 0);
    mock_unittest_pthread_cond_init(&taskFwObj.stateInfo.cond, NULL, 0);

    t_taskFw* pThis = taskFw_create();
    POINTERS_EQUAL(&taskFwObj, pThis);
    POINTERS_EQUAL(&msgQueue, taskFwObj.pRequestQueue);

#if 0
    mock_unittest_pthread_create(&taskFwObj.tid,
        NULL,
        taskFw_start_routine,
        (void*)&taskFwObj, 0);

    t_taskFw_taskMsg taskMsg;
    mock_unittest_malloc(sizeof(t_taskFw_taskMsg), &taskMsg);
    mock_unittest_pthread_mutex_lock(&msgQueue.mutex);
    mock_unittest_pthread_cond_signal(&msgQueue.cond);
    mock_unittest_pthread_mutex_unlock(&msgQueue.mutex);

    mock_unittest_pthread_mutex_lock(&taskFwObj.stateInfo.mutex);
    mock_unittest_pthread_cond_wait(&taskFwObj.stateInfo.cond, &taskFwObj.stateInfo.mutex);
    mock_unittest_pthread_mutex_unlock(&taskFwObj.stateInfo.mutex);


    int ret = taskFw_startTask(&taskFwObj);
    CHECK_EQUAL(0, ret);


    mock_unittest_free(&taskFwObj);
    mock_unittest_free(&msgQueue);
    mock_unittest_pthread_mutex_destroy(&msgQueue.mutex);
    mock_unittest_pthread_cond_destroy(&msgQueue.cond);

    taskFw_delete(pThis);
#endif
}

