#include <stdio.h>
#include <taskFw.h>
#include <unistd.h>

static void taskMessageHandler(
    t_taskFw_callMsg* pMsg)
{
    printf("test test\n");
}

int main(int argc, char const* argv[])
{
    t_taskFw* pTask = taskFw_create();

    taskFw_startTask(pTask);
    int i = 0;
    for(i = 0; i < 10; i++) {
        t_taskFw_callMsg callMsg;
        taskFw_putMsg(pTask,&callMsg,
            taskMessageHandler);
        sleep(1);
    }

    taskFw_delete(pTask);


    return 0;
}
