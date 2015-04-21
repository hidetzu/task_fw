#include <stdio.h>
#include <stdlib.h>
#include <taskFw.h>
#include <unistd.h>



static void taskMessageHandler(
    t_taskFw_callMsg* pMsg)
{
    printf("test test start\n");
    sleep(10);
    printf("test test end\n");
}

int main(int argc, char const* argv[])
{
    t_taskFw* pTask = taskFw_create();

    taskFw_startTask(pTask);

    int i = 0;
#if 0
    for(i = 0; i < 10; i++) {
        t_taskFw_callMsg* callMsg = malloc(sizeof(callMsg));
        taskFw_putCallMsg(pTask,callMsg,
            taskMessageHandler);
        sleep(1);
        printf("fasfdsfdsa\n");
    }
#endif

    for(i = 0; i < 10; i++) {
        t_taskFw_callMsg* callMsg = malloc(sizeof(callMsg));
        t_taskFw_replayMsg* pReplayMsg =
            taskFw_putCallMsgWithReplyMsg(pTask, callMsg,
            taskMessageHandler);
        taskFw_waitReplyMsg(pTask, pReplayMsg);
        printf("fasfdsfdsa\n");
        sleep(1);
    }


    taskFw_stopTask(pTask);
    taskFw_delete(pTask);


    return 0;
}
