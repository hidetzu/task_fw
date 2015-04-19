#include <common_capi.h>

#include <taskFw.h>

#include "taskFw_msgQueue.h"
#include "taskFw_api_private.h"

#include <stdio.h>
#include <unistd.h>

/**************************************************/
/* Definitions                                    */
/**************************************************/

/**************************************************/
/* Prototype                                      */
/**************************************************/
void taskFw_msgHandler(t_taskFw_taskMsg* pTaskMsg);
/**************************************************/
/* Global var                                     */
/**************************************************/

/**************************************************/
/* Public Functions                               */
/**************************************************/
t_taskFw* taskFw_create(void)
{
    t_taskFw* pInst =(t_taskFw*)common_malloc(sizeof(t_taskFw));
    if( NULL == pInst )
        return NULL;
    
    pInst->pRequestQueue = taskFw_msgQueue_create();
    if( NULL == pInst->pRequestQueue ) 
        goto ERROR;

    return pInst;
ERROR:
    common_free(pInst);
    return NULL;
}

int taskFw_startTask(t_taskFw* pThis)
{
    if( NULL == pThis )
        return -1;

    if( NULL == pThis->pRequestQueue )
        return -1;

    int ret =common_pthread_create(&pThis->tid,
            NULL,
            taskFw_start_routine,
            (void*)pThis);
    if( 0 != ret )
        return -1;

    return 0;
}

int taskFw_putMsg(t_taskFw* pThis,
        t_taskFw_callMsg* pCallMsg,
        t_taskFw_callMsgHandler callMsgHandler)
{
    if( NULL == pThis )
        return -1;
    if( NULL == pCallMsg )
        return -1;
    if( NULL == callMsgHandler )
        return -1;

    t_taskFw_taskMsg* taskMsg = (t_taskFw_taskMsg*)
        common_malloc(sizeof(t_taskFw_taskMsg));
    taskMsg->msgHandler  = taskFw_msgHandler;
    taskMsg->pCallMsg = pCallMsg;
    taskMsg->callMsgHandler = callMsgHandler;

    return taskFw_msgQueue_putMsg(pThis->pRequestQueue,taskMsg);
}

void taskFw_delete(t_taskFw* pThis)
{
    if( pThis ) {
        taskFw_msgQueue_delete(pThis->pRequestQueue);
        common_free(pThis);
    }
}

/**************************************************/
/* Internal Functions                             */
/**************************************************/
void* taskFw_start_routine(void* arg)
{
    t_taskFw* pThis = (t_taskFw*)arg;
    printf("pThis=%p\n", pThis);
    while(1) {
        t_taskFw_taskMsg* pRequestMsg = taskFw_msgQueue_getMsg(pThis->pRequestQueue);
        pRequestMsg->msgHandler(pRequestMsg);
        common_free(pRequestMsg);
        sleep(1);
    }

    return NULL;
}

void taskFw_msgHandler(t_taskFw_taskMsg* pTaskMsg)
{
    if(pTaskMsg->callMsgHandler)
     pTaskMsg->callMsgHandler(pTaskMsg->pCallMsg);
}
