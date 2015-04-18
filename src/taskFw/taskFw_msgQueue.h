#ifndef __TASKFW_MSGQUEUE_H__
#define __TASKFW_MSGQUEUE_H__

#include <taskFw.h>

/**************************************************/
/* Definitions                                    */
/**************************************************/
typedef struct _t_taskFw_msgQueue t_taskFw_msgQueue;
typedef struct _t_taskFw_taskMsg t_taskFw_taskMsg;

typedef void (*t_taskMessageHandler)(t_taskFw_taskMsg* pMsg);

struct _t_taskFw_taskMsg {
    t_taskFw_callMsg* pCallMsg;
    t_taskFw_callMsgHandler callMsgHandler;

    struct _t_taskFw_taskMsg* pNext;
    t_taskMessageHandler msgHandler;
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
