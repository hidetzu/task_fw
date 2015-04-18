#ifndef __TASKFW_MSGQUEUE_PRIVATE_H__
#define __TASKFW_MSGQUEUE_PRIVATE_H__

/**************************************************/
/* Definitions                                    */
/**************************************************/

struct _t_taskFw_msgQueue {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    t_taskFw_taskMsg* pTop;
    t_taskFw_taskMsg* pTail;
};

/**************************************************/
/* Prototype                                      */
/**************************************************/

#endif  /*__TASKFW_MSGQUEUE_PRIVATE_H__*/
