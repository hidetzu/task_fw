
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

    return 0;
}

t_taskFw_taskMsg* taskFw_msgQueue_getMsg(t_taskFw_msgQueue* pThis)
{
    return NULL;
}

/**************************************************/
/* Internal Functions                             */
/**************************************************/

