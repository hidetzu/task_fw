
extern "C"{
    #include <taskFw.h>
    #include <stdio.h>
};

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

TEST_GROUP(TestTaskFwGroup)
{
    struct _taskFw {
        int dummy;
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

TEST(TestTaskFwGroup, createAndDelete)
{
    int test;
    mock().expectOneCall("unittest_malloc")
        .withParameter("size", (int)sizeof(struct _taskFw))
        .andReturnValue((void*)&test);

    TaskFw* pThis = taskFw_create();

    POINTERS_EQUAL(&test, pThis);
    mock().expectOneCall("unittest_free")
        .withParameter("ptr", (void*)pThis);

    taskFw_delete(pThis);
}

TEST(TestTaskFwGroup, createFailed)
{
    mock().expectOneCall("unittest_malloc")
        .withParameter("size", (int)sizeof(struct _taskFw))
        .andReturnValue((void*)NULL);

    TaskFw* pThis = taskFw_create();

    POINTERS_EQUAL(NULL, pThis);
    mock().expectOneCall("unittest_free")
        .withParameter("ptr", (void*)pThis);

    taskFw_delete(pThis);
}

