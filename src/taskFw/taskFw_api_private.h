#ifndef __TASKFW_API_PRIVATE_H__
#define __TASKFW_API_PRIVATE_H__

/**************************************************/
/* Definitions                                    */
/**************************************************/
typedef enum _e_taskFw_taskState {
    TASKFW_TASKSTATE_INIT=0,
    TASKFW_TASKSTATE_RUN,
    TASKFW_TASKSTATE_SHUTDWON,
}e_taskFw_taskState;

typedef struct _t_taskFw_stateInfo {
    e_taskFw_taskState state;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}t_taskFw_stateInfo;


struct _t_taskFw {
    pthread_t tid;
    t_taskFw_msgQueue* pRequestQueue;
    t_taskFw_stateInfo stateInfo;
};

struct _t_taskFw_replayMsg {
    int isReplay;
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
};


/**************************************************/
/* Prototype                                      */
/**************************************************/
void* taskFw_start_routine(void* arg);


#endif  /*__TASKFW_API_PRIVATE_H__*/
