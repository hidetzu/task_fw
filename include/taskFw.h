#ifndef __TASKFW_H__
#define __TASKFW_H__

/**************************************************/
/* Definitions                                    */
/**************************************************/
typedef struct _taskFw TaskFw;
typedef struct _t_taskFw_taskMsg t_taskFw_taskMsg;

typedef void (*t_taskMessageHandler)(t_taskFw_taskMsg* pMsg);

struct _t_taskFw_taskMsg {
    struct _t_taskFw_taskMsg* pNext;
    t_taskMessageHandler msgHandler;
};

/**************************************************/
/* Prototype                                      */
/**************************************************/
TaskFw* taskFw_create(void);
void taskFw_delete(TaskFw* pThis);

#endif  /*__TASKFW_H__*/
