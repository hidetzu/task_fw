#ifndef __TASKFW_H__
#define __TASKFW_H__

/**************************************************/
/* Definitions                                    */
/**************************************************/
typedef struct _t_taskFw t_taskFw;

typedef struct _t_taskFw_callMsg t_taskFw_callMsg;
typedef void (*t_taskFw_callMsgHandler)(t_taskFw_callMsg* pMsg);

typedef struct _t_taskFw_replayMsg t_taskFw_replayMsg;

struct _t_taskFw_callMsg {
    t_taskFw_replayMsg* pReplayMsg;
};


/**************************************************/
/* Prototype                                      */
/**************************************************/
t_taskFw* taskFw_create(void);
int taskFw_startTask(t_taskFw* pThis);
int taskFw_stopTask(t_taskFw* pThis);
void taskFw_delete(t_taskFw* pThis);

int taskFw_putCallMsg(t_taskFw* pThis,
        t_taskFw_callMsg* pCallMsg,
        t_taskFw_callMsgHandler callMsgHandler);

t_taskFw_replayMsg* taskFw_putCallMsgWithReplyMsg(t_taskFw* pThis,
        t_taskFw_callMsg* pCallMsg,
        t_taskFw_callMsgHandler callMsgHandler);

int taskFw_waitReplyMsg(t_taskFw* pThis,
        t_taskFw_replayMsg* pReplayMsg);




#endif  /*__TASKFW_H__*/
