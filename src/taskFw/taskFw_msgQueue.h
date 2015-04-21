#ifndef __TASKFW_MSGQUEUE_H__
#define __TASKFW_MSGQUEUE_H__

#include <taskFw.h>

/**************************************************/
/* Definitions                                    */
/**************************************************/
typedef struct _t_taskFw_msgQueue t_taskFw_msgQueue;
typedef struct _t_taskFw_taskMsg t_taskFw_taskMsg;

typedef void (*t_taskMessageHandler)(t_taskFw_taskMsg* pMsg);

typedef enum _e_taskCtrl_msgType {
    TASKCTRL_MSGTYPE_INIT=0,
    TASKCTRL_MSGTYPE_EXECUTE,
    TASKCTRL_MSGTYPE_SHUTDOWN,
}e_taskCtrl_msgType;

struct _t_taskFw_taskMsg {
    e_taskCtrl_msgType msgType;
    t_taskFw_callMsg* pCallMsg;
    t_taskFw_callMsgHandler callMsgHandler;

    struct _t_taskFw_taskMsg* pNext;
    t_taskMessageHandler msgHandler;
};

struct _t_taskFw_msgQueue {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    t_taskFw_taskMsg* pTop;
    t_taskFw_taskMsg* pTail;
};



/**************************************************/
/* Prototype                                      */
/**************************************************/
t_taskFw_msgQueue* taskFw_msgQueue_create(void);
void taskFw_msgQueue_delete(t_taskFw_msgQueue* pThis);

int taskFw_msgQueue_putMsg(t_taskFw_msgQueue* pThis,
        t_taskFw_taskMsg* pMsg);

t_taskFw_taskMsg* taskFw_msgQueue_getMsg(t_taskFw_msgQueue* pThis);



#endif  /*__TASKFW_MSGQUEUE_H__*/
