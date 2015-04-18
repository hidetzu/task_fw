
#include <pthread.h>
#include <common_capi.h>

#include <taskFw_msgQueue.h>

/**************************************************/
/* Definitions                                    */
/**************************************************/

/**************************************************/
/* Prototype                                      */
/**************************************************/
struct _t_taskFw_msgQueue {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    t_taskFw_taskMsg* pTop;
    t_taskFw_taskMsg* pTail;
};

/**************************************************/
/* Global var                                     */
/**************************************************/

/**************************************************/
/* Public Functions                               */
/**************************************************/
t_taskFw_msgQueue* taskFw_msgQueue_create(void)
{
    t_taskFw_msgQueue* inst = (t_taskFw_msgQueue*)
        common_malloc(sizeof(t_taskFw_msgQueue));
    if( NULL == inst )
        return NULL;

    int ret = common_pthread_mutex_init(&inst->mutex,
                NULL);
    if( 0 != ret)
        goto ERROR;


    ret = common_pthread_cond_init(&inst->cond,
                NULL);
    if( 0 != ret) {
        common_pthread_mutex_destroy(&inst->mutex);
        goto ERROR;
    }

    inst->pTop = NULL;
    inst->pTail = NULL;

    return inst;

ERROR:
    common_free(inst);
    return NULL;
}

void taskFw_msgQueue_delete(t_taskFw_msgQueue* pThis)
{
    if( pThis ) {
        common_pthread_mutex_destroy(&pThis->mutex);
        common_pthread_cond_destroy(&pThis->cond);
        common_free(pThis);
    }
}

int taskFw_msgQueue_putMsg(t_taskFw_msgQueue* pThis,
        t_taskFw_taskMsg* pMsg)
{
    if( NULL == pThis )
        return -1;

    if( NULL == pMsg )
        return -1;

    common_pthread_mutex_lock(&pThis->mutex);

    if( NULL == pThis->pTop ) {
        pThis->pTop = pMsg;
        pThis->pTail = pThis->pTop;
        pMsg->pNext = NULL;
    } else {
        pThis->pTail->pNext = pMsg;
        pThis->pTail = pMsg;
        pMsg->pNext = NULL;
    }

    common_pthread_cond_signal(&pThis->cond);
    common_pthread_mutex_unlock(&pThis->mutex);

    return 0;
}

t_taskFw_taskMsg* taskFw_msgQueue_getMsg(t_taskFw_msgQueue* pThis)
{
    if( NULL == pThis)
        return NULL;

    t_taskFw_taskMsg* result = NULL;

    common_pthread_mutex_lock(&pThis->mutex);
    while( NULL == pThis->pTop )
        common_pthread_cond_wait(&pThis->cond,&pThis->mutex);

    result = pThis->pTop;
    pThis->pTop = pThis->pTop->pNext;

    common_pthread_mutex_unlock(&pThis->mutex);

    return result;
}

/**************************************************/
/* Internal Functions                             */
/**************************************************/

