#include <CppUTest/TestHarness.h>

extern "C"{
    #include <taskFw.h>
}

TEST_GROUP(TestTaskFwGroup)
{
};

TEST(TestTaskFwGroup, FirstTest)
{
    TaskFw* pInst = taskFw_create();
    POINTERS_EQUAL(NULL, pInst);
}
