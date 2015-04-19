#ifndef __TASKFW_API_PRIVATE_H__
#define __TASKFW_API_PRIVATE_H__

/**************************************************/
/* Definitions                                    */
/**************************************************/
struct _t_taskFw {
    pthread_t tid;
    t_taskFw_msgQueue* pRequestQueue;
    int dummy;
};


/**************************************************/
/* Prototype                                      */
/**************************************************/
void* taskFw_start_routine(void* arg);


#endif  /*__TASKFW_API_PRIVATE_H__*/
