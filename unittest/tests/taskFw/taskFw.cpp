#include <CppUTest/TestHarness.h>

extern "C"{
    #include <taskFw.h>
}

TEST_GROUP(TestTaskFwGroup)
{
};

TEST(TestTaskFwGroup, createAndDelete)
{
    TaskFw* pThis = taskFw_create();
    taskFw_delete(pThis);
}
