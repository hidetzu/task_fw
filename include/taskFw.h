#ifndef __TASKFW_H__
#define __TASKFW_H__

/**************************************************/
/* Definitions                                    */
/**************************************************/
typedef struct _taskFw TaskFw;

/**************************************************/
/* Prototype                                      */
/**************************************************/
TaskFw* taskFw_create(void);
void taskFw_delete(TaskFw* pThis);

#endif  /*__TASKFW_H__*/
