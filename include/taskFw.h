#ifndef __TASKFW_H__
#define __TASKFW_H__

/**************************************************/
/* Definitions                                    */
/**************************************************/
typedef struct _t_taskFw t_taskFw;

typedef struct _t_taskFw_callMsg t_taskFw_callMsg;
typedef void (*t_taskFw_callMsgHandler)(t_taskFw_callMsg* pMsg);

struct _t_taskFw_callMsg {
    int dummy;
};


/**************************************************/
/* Prototype                                      */
/**************************************************/
t_taskFw* taskFw_create(void);
int taskFw_startTask(t_taskFw* pThis);

int taskFw_putMsg(t_taskFw* pThis,
        t_taskFw_callMsg* pCallMsg,
        t_taskFw_callMsgHandler callMsgHandler);


void taskFw_delete(t_taskFw* pThis);


#endif  /*__TASKFW_H__*/
