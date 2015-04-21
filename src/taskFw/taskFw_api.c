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
static void taskFw_msgHandler(t_taskFw_taskMsg* pTaskMsg);
static t_taskFw_taskMsg* taskFw_createTaskMsg(e_taskCtrl_msgType msgType,
        t_taskMessageHandler msgHandler);

static t_taskFw_replayMsg* taskFw_createReplayMsg(void);
static void taskFw_deleteReplayMsg(t_taskFw_replayMsg* pReplayMsg);

static void taskFw_setReplayMsg(t_taskFw_replayMsg* pReplayMsg);
static void taskFw_waitReplayMsg(t_taskFw_replayMsg* pReplayMsg);
static void taskFw_changeState(t_taskFw* pThis, e_taskFw_taskState newState);
static void taskFw_waitState(t_taskFw* pThis, e_taskFw_taskState newState);
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
        goto MSGQUEUE_CREATE_ERROR;

    pInst->stateInfo.state = TASKFW_TASKSTATE_INIT;
    int ret = common_pthread_mutex_init(&pInst->stateInfo.mutex,
                NULL);
    if( 0 != ret)
        goto MUTEX_INIT_ERROR;

    ret = common_pthread_cond_init(&pInst->stateInfo.cond,
                NULL);
    if( 0 != ret) {
        goto COND_INIT_ERROR;
    }


    return pInst;

COND_INIT_ERROR:
    common_pthread_mutex_destroy(&pInst->stateInfo.mutex);

MUTEX_INIT_ERROR:
    taskFw_msgQueue_delete(pInst->pRequestQueue);

MSGQUEUE_CREATE_ERROR:
    common_free(pInst);
    return NULL;
}

int
taskFw_startTask(
    t_taskFw* pThis)
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

    t_taskFw_taskMsg* taskMsg =
        taskFw_createTaskMsg(TASKCTRL_MSGTYPE_INIT, NULL);

    taskFw_msgQueue_putMsg(pThis->pRequestQueue,taskMsg);

    taskFw_waitState(pThis, TASKFW_TASKSTATE_INIT);

    printf("start task end\n");
    return 0;
}

int taskFw_putCallMsg(t_taskFw* pThis,
        t_taskFw_callMsg* pCallMsg,
        t_taskFw_callMsgHandler callMsgHandler)
{
    if( NULL == pThis )
        return -1;
    if( NULL == pCallMsg )
        return -1;
    if( NULL == callMsgHandler )
        return -1;

    t_taskFw_taskMsg* taskMsg =
        taskFw_createTaskMsg(TASKCTRL_MSGTYPE_EXECUTE, taskFw_msgHandler);

    taskMsg->pCallMsg = pCallMsg;
    taskMsg->callMsgHandler = callMsgHandler;
    pCallMsg->pReplayMsg = NULL;

    return taskFw_msgQueue_putMsg(pThis->pRequestQueue,taskMsg);
}

t_taskFw_replayMsg* taskFw_putCallMsgWithReplyMsg(t_taskFw* pThis,
        t_taskFw_callMsg* pCallMsg,
        t_taskFw_callMsgHandler callMsgHandler)
{
    t_taskFw_taskMsg* taskMsg =
        taskFw_createTaskMsg(TASKCTRL_MSGTYPE_EXECUTE, taskFw_msgHandler);
    taskMsg->msgHandler  = taskFw_msgHandler;
    taskMsg->pCallMsg = pCallMsg;
    taskMsg->callMsgHandler = callMsgHandler;

    t_taskFw_replayMsg* replayMsg = taskFw_createReplayMsg();
    pCallMsg->pReplayMsg = replayMsg;

    taskFw_msgQueue_putMsg(pThis->pRequestQueue,taskMsg);

    return replayMsg;
}

int taskFw_waitReplyMsg(t_taskFw* pThis,
        t_taskFw_replayMsg* pReplayMsg)
{
    taskFw_waitReplayMsg(pReplayMsg);
    taskFw_deleteReplayMsg(pReplayMsg);
    return 0;
}


int taskFw_stopTask(t_taskFw* pThis)
{
    t_taskFw_taskMsg* taskMsg =
        taskFw_createTaskMsg(TASKFW_TASKSTATE_SHUTDWON, NULL);

    taskFw_msgQueue_putMsg(pThis->pRequestQueue,taskMsg);
    taskFw_waitState(pThis, TASKFW_TASKSTATE_SHUTDWON);

    common_pthread_join(pThis->tid, NULL);

    return 0;
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

    while(pThis->stateInfo.state != TASKFW_TASKSTATE_SHUTDWON ) {
        t_taskFw_taskMsg* pRequestMsg = taskFw_msgQueue_getMsg(pThis->pRequestQueue);
        e_taskCtrl_msgType msgType = pRequestMsg->msgType;

        switch(msgType) {
        case TASKCTRL_MSGTYPE_INIT:
            taskFw_changeState(pThis, TASKFW_TASKSTATE_RUN);
            break;
        case TASKCTRL_MSGTYPE_EXECUTE:
            pRequestMsg->msgHandler(pRequestMsg);
            common_free(pRequestMsg->pCallMsg);
            break;
        case TASKCTRL_MSGTYPE_SHUTDOWN:
            taskFw_changeState(pThis, TASKFW_TASKSTATE_SHUTDWON);
            break;
        default:
            break;
        }
        common_free(pRequestMsg);
    }

    return NULL;
}

static void taskFw_setReplayMsg(t_taskFw_replayMsg* pReplayMsg)
{
    common_pthread_mutex_lock(&pReplayMsg->mutex);
    pReplayMsg->isReplay = 1;
    common_pthread_cond_signal(&pReplayMsg->cond);
    common_pthread_mutex_unlock(&pReplayMsg->mutex);
}

static void taskFw_waitReplayMsg(t_taskFw_replayMsg* pReplayMsg)
{
    common_pthread_mutex_lock(&pReplayMsg->mutex); 
    while(!pReplayMsg->isReplay)
        common_pthread_cond_wait(&pReplayMsg->cond, &pReplayMsg->mutex);
    common_pthread_mutex_unlock(&pReplayMsg->mutex); 
}

static void taskFw_changeState(t_taskFw* pThis, e_taskFw_taskState newState)
{
    common_pthread_mutex_lock(&pThis->stateInfo.mutex);
    pThis->stateInfo.state = newState;
    common_pthread_cond_signal(&pThis->stateInfo.cond);
    common_pthread_mutex_unlock(&pThis->stateInfo.mutex);
}

static void
taskFw_waitState(t_taskFw* pThis, e_taskFw_taskState newState)
{
    common_pthread_mutex_lock(&pThis->stateInfo.mutex);
    while( pThis->stateInfo.state != newState )
        common_pthread_cond_wait(&pThis->stateInfo.cond, &pThis->stateInfo.mutex);
    common_pthread_mutex_unlock(&pThis->stateInfo.mutex);
}

static t_taskFw_replayMsg* taskFw_createReplayMsg(void)
{
    t_taskFw_replayMsg* replayMsg = (t_taskFw_replayMsg*)
        common_malloc(sizeof(t_taskFw_replayMsg));
    replayMsg->isReplay = 0;
    common_pthread_mutex_init(&replayMsg->mutex,NULL);
    common_pthread_cond_init(&replayMsg->cond, NULL);

    return replayMsg;
}

static void taskFw_deleteReplayMsg(t_taskFw_replayMsg* pReplayMsg)
{
    common_free(pReplayMsg);
}

static t_taskFw_taskMsg*
taskFw_createTaskMsg(e_taskCtrl_msgType msgType,
        t_taskMessageHandler msgHandler)
{
    t_taskFw_taskMsg* taskMsg = (t_taskFw_taskMsg*)
        common_malloc(sizeof(t_taskFw_taskMsg));
    taskMsg->msgType = msgType;
    taskMsg->msgHandler  = msgHandler;
    taskMsg->pCallMsg = NULL;
    taskMsg->callMsgHandler = NULL;

    return taskMsg;
}

static void
taskFw_msgHandler(
    t_taskFw_taskMsg* pTaskMsg)
{
    if(!pTaskMsg->callMsgHandler)
        return;

    pTaskMsg->callMsgHandler(pTaskMsg->pCallMsg);
    if(!pTaskMsg->pCallMsg->pReplayMsg)
        return;

    t_taskFw_replayMsg* pReplayMsg = pTaskMsg->pCallMsg->pReplayMsg;
    taskFw_setReplayMsg(pReplayMsg);
}
