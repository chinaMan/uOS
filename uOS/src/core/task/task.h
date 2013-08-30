/*============================================================================*/
/*
 * @brief:   task manager module source file
 *
 * @author:  bo.zeng
 *
 * @data:    2013.8.11
 *
 */
/*============================================================================*/
#ifndef TASK_H
#define TASK_H
#ifdef CPLUSPLUS
#define extern "C" {
#endif 

/*==============================[include]=====================================*/
#include "uos_types.h"
#include "uos_cfg.h"
#include "dlist.h"

/*==============================[marco]=======================================*/

/*==============================[typedef]=====================================*/
typedef struct 
{
    struct dlist_node dNode;    

    TaskIdType        tid;
}TaskNode;

typedef struct
{
    TaskIdType    highestPriTid;

    struct dlist  dList[CFG_PRI_MAX_NUM];
}TaskList;

typedef TaskList TaskWaitList;

/*============================[extern data]===================================*/
EXTERN uint8 gTaskSchedLockLevel;

/*===========================[extern function]================================*/
/******************************************************************************/
/*
 * @brief:      <initalize task module> 
 * @detail:     <initalize task module>
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <tid: task id> 
 * @return:     <OS_Ret: os return>
 */
/******************************************************************************/
EXTERN OS_Ret uOS_TaskInit(void);

/******************************************************************************/
/*
 * @brief:      <active task> 
 * @detail:     <active task from OS_SUSPEND to OS_READY>
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <tid: task id> 
 * @return:     <OS_Ret: os return>
 */
/******************************************************************************/
EXTERN OS_Ret uOS_TaskActive(TaskIdType tid);

/******************************************************************************/
/*
 * @brief:      <terminate task> 
 * @detail:     <terminate task to OS_SUSPEND> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <tid: task id> 
 * @return:     <OS_Ret: os return>
 */
/******************************************************************************/
EXTERN OS_Ret uOS_TaskTerminate(TaskIdType tid);

/******************************************************************************/
/*
 * @brief:      <schedule task> 
 * @detail:     <schedule task to OS_SUSPEND> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <tid: task id> 
 * @return:     <OS_Ret: os return>
 */
/******************************************************************************/
EXTERN void uOS_TaskSched(void);

/******************************************************************************/
/*
 * @brief:      <task wait to TASK_STATE_WAITTING> 
 * @detail:     <task wait to TASK_STATE_WAITTING, 
 *               and add tcb to sepcified wait list> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[in]:  <None> 
 * @return:     <OS_Ret: os return>
 */
/******************************************************************************/
EXTERN void uOS_TaskWaitListInit(TaskWaitList *list);

#if (TRUE == CFG_UOS_TASKWAIT_API)
/******************************************************************************/
/*
 * @brief:      <task wait to TASK_STATE_WAITTING> 
 * @detail:     <task wait to TASK_STATE_WAITTING, 
 *               and add tcb to sepcified wait list> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[in]:  <None> 
 * @return:     <OS_Ret: os return>
 */
/******************************************************************************/
EXTERN OS_Ret uOS_TaskWait(TaskWaitList *list, TickType timeout);
#else
#define uOS_TaskWait(list, timeout)
#endif /* TRUE == CFG_UOS_TASKWAIT_API */

#if (TRUE == CFG_UOS_TASKWAKE_API)
/******************************************************************************/
/*
 * @brief:      <wake task from TASK_STATE_WAITTING> 
 * @detail:     <wake task from TASK_STATE_WAITTING> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <tid: task id> 
 * @param[in]:  <None> 
 * @return:     <OS_Ret: os return>
 */
/******************************************************************************/
EXTERN OS_Ret uOS_TaskWake(TaskWaitList *list);
#else
#define uOS_TaskWake(list)
#endif /* TRUE == CFG_UOS_TASKWAKE_API */

#if (TRUE == CFG_UOS_CUR_TASK_ID_API)
/******************************************************************************/
/*
 * @brief:      <get current task id> 
 * @detail:     <get current task id> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[out]: <None> 
 * @return:     <task id>
 */
/******************************************************************************/
EXTERN TaskIdType uOS_CurTaskId(void);
#else
#define uOS_CurTaskId()
#endif /* TRUE == CFG_UOS_CUR_TASK_ID_API */

#if (TRUE == CFG_UOS_TASK_CHANGE_PRI_API)
/******************************************************************************/
/*
 * @brief:      <change task priority> 
 * @detail:     <change task priority> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <tid: task id> 
 * @param[out]: <pState: task state> 
 * @return:     <OS_Ret: os return>
 */
/******************************************************************************/
EXTERN OS_Ret uOS_TaskChangePri(TaskIdType tid, TaskPriType pri);
#else
#define uOS_TaskChangePri(tid, pri)
#endif /* TRUE == CFG_UOS_TASK_CHANGE_PRI_API */

/******************************************************************************/
/*
 * @brief:      <lock scheduler> 
 * @detail:     <lock scheduler> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <None>
 */
/******************************************************************************/
#define uOS_TaskSchedLock()             \
    do{                                 \
        if (gTaskSchedLockLevel < 255U) \
        {                               \
            gTaskSchedLockLevel++       \
        }                               \
    }while(0)

/******************************************************************************/
/*
 * @brief:      <unlock scheduler> 
 * @detail:     <unlock scheduler> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <None>
 */
/******************************************************************************/
#define uOS_TaskSchedUnlock()         \
    do{                               \
        if (gTaskSchedLockLevel > 0U) \
        {                             \
            gTaskSchedLockLevel--     \
        }                             \
    }while(0)

/******************************************************************************/
/*
 * @brief:      <task priority inherit> 
 * @detail:     <task wait to TASK_STATE_WAITTING, 
 *               and add tcb to sepcified wait list> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[in]:  <None> 
 * @return:     <OS_Ret: os return>
 */
/******************************************************************************/
EXTERN OS_Ret uOS_TaskPriInherit(TaskIdType tid, TaskPriType pri);

/******************************************************************************/
/*
 * @brief:      <task priority inherit> 
 * @detail:     <task wait to TASK_STATE_WAITTING, 
 *               and add tcb to sepcified wait list> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[in]:  <None> 
 * @return:     <OS_Ret: os return>
 */
/******************************************************************************/
EXTERN OS_Ret uOS_TaskPriRecover(TaskIdType tid);

#ifdef CPLUSPLUS
}
#endif /* CPLUSPLUS */
#endif /* TASK_H */
