/*============================================================================*/
/*
 * @brief:   task module source code
 *
 * @author:  bo.zeng
 *
 * @data:    2013.8.11
 *
 */
/*============================================================================*/

/*==============================[include]=====================================*/
#include "task.h"
#include "uhal.h"
#include "dlist.h"

/*=============================[type define]==================================*/
typedef enum 
{
    TASK_STATE_SUSPEND = 0,
    TASK_STATE_READY,
    TASK_STATE_WAITTING,
    TASK_STATE_RUNNING
}TaskStateType;

typedef struct
{
    TaskIdType        tid;
    TaskStateType     state;
    TaskPriType       pri;
    TaskPriType       basePri;
    Hal_TaskContext   context;
}TaskControlBlock;

/*=============================[extern data]==================================*/
extern CONST TaskMgrCfgType gTaskMgrCfg;

/*=============================[marco define]=================================*/
#if (CFG_VARIANT_TYPE == CFG_VARIANT_PC)
#define TASK_MGR_CFG    gTaskMgrCfg
#define TASK_NUM_CFG    (CFG_TASK_MAX_NUM)
#elif (CFG_VARIANT_TYPE == CFG_VARIANT_LT)
#define TASK_MGR_CFG    gTaskMgrCfg
#define TASK_NUM_CFG    (CFG_TASK_MAX_NUM)
#else
#define TASK_MGR_CFG    gTaskMgrCfg
#define TASK_NUM_CFG    (CFG_TASK_MAX_NUM)
#endif

#define TASK_ID_IDLE          (TASK_NUM_CFG-1)
#define TASK_IDLE_PRI         (CFG_PRI_MAX_NUM-1)
#define TASK_IDLE_STK_SIZE    (127)

#define ROW(list)   sBitmapIndex[list->bitmapRow]
#define COL(list)   sBitmapIndex[list->bitmap[ROW(list)]]
#define GetHighestPriority(list)   (ROW(list) << 3 | COL(list))
#define BitmapClear(list, pri)        \
    do{ \
        list->bitmap[(pri)>>3] &= ~(1 << ((pri)&0xFFU)); \
        list->bitmapRow &= ~(1<<((pri)>>3)); \
    }while(0)
#define BitmapSet(list, pri)   \
    do{      \
        list->bitmap[(pri)>>3] |= (1 << ((pri)&0xFFU)); \
        list->bitmapRow |= (1<<((pri)>>3)); \
    }while(0)

/*=============================[internal data]================================*/
STATIC TaskControlBlock sTcbTable[CFG_TASK_MAX_NUM];
STATIC TaskIdType       sRunningTaskId;

/*
 * Ready List
 */ 
STATIC TaskList sReadyList;

STATIC TaskNode sTaskNodeTable[CFG_TASK_MAX_NUM];
STATIC struct dlist     sFreeTaskNodeList;

/* Schedule level */
uint8                   gTaskSchedLockLevel = 0;

STATIC CONST uint8 sBitmapIndex[256] = 
{
    0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x00 to 0x0F */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x10 to 0x1F */
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x20 to 0x2F */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x30 to 0x3F */
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x40 to 0x4F */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x50 to 0x5F */
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x60 to 0x6F */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x70 to 0x7F */
    7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x80 to 0x8F */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x90 to 0x9F */
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xA0 to 0xAF */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xB0 to 0xBF */
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xC0 to 0xCF */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xD0 to 0xDF */
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xE0 to 0xEF */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0        /* 0xF0 to 0xFF */
};

/*=============================[internal function]============================*/
STATIC INLINE TaskNode *FreeNodeGet(void);
STATIC INLINE FreeNodeRecover(TaskNode *pNode);
STATIC void TaskListInit(TaskList *list);
STATIC void TaskListAdd(TaskList *list, TaskNode *pNode);
STATIC void TaskListDel(TaskList *list, TaskNode *pNode);
STATIC TaskNode *TaskListFindByTid(TaskList *list, TaskIdType tid);
STATIC TaskNode *TaskListFindByPri(TaskList *list, TaskPriType pri);
STATIC void TaskListRecalcPri(TaskList *list);

/*===========================[function define]================================*/
/******************************************************************************/
/*
 * @brief:      <initalize task module> 
 * @detail:     <initalize task module, it called by uOS_Start>
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <tid: task id> 
 * @return:     <OS_Ret: os return>
 */
/******************************************************************************/
OS_Ret uOS_TaskInit(void)
{
    TaskIdType tid = 0;
    TaskNode *pReadyNode;

    /* 1. initalize free taskNode list, ready list */
    dlist_init(&sFreeTaskNodeList);
    for (tid = 0; tid < TASK_NUM_CFG; tid++)
    {
        dlist_append(&sFreeTaskNodeList, &sTaskNodeTable[tid].dNode);
    }
    TaskListInit(&sReadyList);


    /* 2. idle task initalize */
    sTcbTable[TASK_ID_IDLE].tid = TASK_ID_IDLE;
    sTcbTable[TASK_ID_IDLE].pri = TASK_IDLE_PRI;
    sTcbTable[TASK_ID_IDLE].basePri = TASK_IDLE_PRI;
    pReadyNode = FreeNodeGet();
    pReadyNode->tid = TASK_ID_IDLE;
    TaskListAdd(&sReadyList, pReadyNode);
    sTcbTable[TASK_ID_IDLE].state = TASK_STATE_READY;

    /* 3. initalize runtime data */
    for (tid = 0; tid < TASK_NUM_CFG - 1; tid++)
    {
        sTcbTable[tid].tid = tid;
        Hal_TaskContextInit(&(sTcbTable[tid].context), 
                            (RegsType)(TASK_MGR_CFG.pTaskTableCfg[tid].stkBottom
                            + TASK_MGR_CFG.pTaskTableCfg[tid].stkSize-1),
                            (RegsType)TASK_MGR_CFG.pTaskTableCfg[tid].fpEntry);
        sTcbTable[tid].pri = TASK_MGR_CFG.taskSchedCfg.pPriTableCfg[tid];
        sTcbTable[tid].basePri = TASK_MGR_CFG.taskSchedCfg.pPriTableCfg[tid];
        if (TRUE == TASK_MGR_CFG.pTaskTableCfg[tid].autoStart)
        {
            /* auto start */
            pReadyNode = FreeNodeGet();
            pReadyNode->tid = tid;
            TaskListAdd(&sReadyList, pReadyNode);
            sTcbTable[tid].state = TASK_STATE_READY;
        }
        else
        {
            sTcbTable[tid].state = TASK_STATE_SUSPEND;
        }
    }

    sRunningTaskId = TASK_ID_IDLE;     /* set idle task as running task */

    return OS_E_OK;
}

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
OS_Ret uOS_TaskActive(TaskIdType tid)
{
    RegsType regPsr;
    TaskNode *pReadyNode;

#if (TRUE == CFG_DET_ERROR_DECTECT)
    if (tid >= TASK_NUM_CFG)
    {
        return OS_E_NOT_OK;
    }
    else if (TASK_STATE_SUSPEND != sTcbTable[tid].state)
    {
        return OS_E_NOT_OK;
    }
#endif /* TRUE == CFG_DET_ERROR_DECTECT*/

    Hal_EnterCritical();

    /* get free ready node */
    pReadyNode = FreeNodeGet();
    pReadyNode->tid = tid;

    sTcbTable[tid].state = TASK_STATE_READY;
    TaskListAdd(&sReadyList, pReadyNode);
    Hal_ExitCritical();

    uOS_TaskSched();

    return OS_E_OK;
}

/******************************************************************************/
/*
 * @brief:      <terminate task> 
 * @detail:     <terminate task to OS_SUSPEND> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <tid: task id> 
 * @return:     <None>
 */
/******************************************************************************/
OS_Ret uOS_TaskTerminate(TaskIdType tid)
{
    RegsType regPsr;
    TaskNode *pTaskNode;

#if (TRUE == CFG_DET_ERROR_DECTECT)
    if (tid >= TASK_NUM_CFG)
    {
        return OS_E_NOT_OK;
    }
    else if (TASK_STATE_READY != sTcbTable[tid].state)
    {
        return OS_E_NOT_OK;
    }
#endif /* TRUE == CFG_DET_ERROR_DECTECT */

    Hal_EnterCritical();

    pTaskNode = TaskListFindByTid(&sReadyList, tid);
    TaskListDel(&sReadyList, pTaskNode);

    FreeNodeRecover(pTaskNode);

    sTcbTable[sRunningTaskId].state = TASK_STATE_SUSPEND;
    Hal_ExitCritical();

    uOS_TaskSched();

    return OS_E_OK;
}

/******************************************************************************/
/*
 * @brief:      <schedule task> 
 * @detail:     <schedule highest priority task to run> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <tid: task id> 
 * @return:     <OS_Ret: os return>
 */
/******************************************************************************/
void uOS_TaskSched(void)
{
    RegsType    regPsr;
    TaskIdType  oldTaskId;

    Hal_EnterCritical();
    if (gTaskSchedLockLevel > 0)
    {
        Hal_ExitCritical();
        return;
    }

    if (sReadyList.highestPriTid == sRunningTaskId)
    {
        Hal_ExitCritical();
        return;
    }

    oldTaskId = sRunningTaskId;
    sRunningTaskId = sReadyList.highestPriTid;
    Hal_ContextSwitch(&sTcbTable[oldTaskId].context, &sTcbTable[sRunningTaskId].context);

    Hal_ExitCritical();
}

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
void uOS_TaskWaitListInit(TaskWaitList *list)
{
    TaskListInit(list);
}

#if (TRUE == CFG_UOS_TASKWAIT_API)
/******************************************************************************/
/*
 * @brief:      <task wait to TASK_STATE_WAITTING> 
 * @detail:     <task wait to TASK_STATE_WAITTING> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @return:     <OS_Ret: os return>
 */
/******************************************************************************/
OS_Ret uOS_TaskWait(TaskWaitList *list, TickType timeout)
{
    RegsType regPsr;
    TaskNode *pTaskNode;

#if (TRUE == CFG_DET_ERROR_DECTECT)
    if (sRunningTaskId >= TASK_NUM_CFG)
    {
        return OS_E_NOT_OK;
    }
#endif /* TRUE == CFG_DET_ERROR_DECTECT*/

    Hal_EnterCritical();
    pTaskNode = TaskListFindByTid(&sReadyList, sRunningTaskId);
    TaskListDel(&sReadyList, pTaskNode);
    TaskListAdd(list, pTaskNode);
    sTcbTable[sRunningTaskId].state = TASK_STATE_WAITTING;
    Hal_ExitCritical();

    return OS_E_OK;
}
#endif /* TRUE == CFG_UOS_TASKWAIT_API */

#if (TRUE == CFG_UOS_TASKWAKE_API)
/******************************************************************************/
/*
 * @brief:      <wake task from TASK_STATE_WAITTING> 
 * @detail:     <wake task from TASK_STATE_WAITTING> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <tid: task id> 
 * @return:     <OS_Ret: os return>
 */
/******************************************************************************/
OS_Ret uOS_TaskWake(TaskWaitList *list)
{
    RegsType regPsr;
    TaskNode *pTaskNode;

#if (TRUE == CFG_DET_ERROR_DECTECT)
    if (NULL == list)
    {
        return OS_E_NOT_OK;
    }
#endif /* TRUE == CFG_DET_ERROR_DECTECT*/

    Hal_EnterCritical();
    pTaskNode = TaskListFindByTid(list, list->highestPriTid);
    TaskListDel(list, pTaskNode);
    TaskListAdd(&sReadyList, pTaskNode);

    sTcbTable[pTaskNode->tid].state = TASK_STATE_READY;
    Hal_ExitCritical();

    return OS_E_OK;
}
#endif /* TRUE == CFG_UOS_TASKWAKE_API */


#if (TRUE == CFG_UOS_CUR_TASK_ID_API)
/******************************************************************************/
/*
 * @brief:      <get current task id> 
 * @detail:     <get current task id> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[out]: <pTid: pointer to task id> 
 * @return:     <OS_Ret: os return>
 */
/******************************************************************************/
TaskIdType uOS_CurTaskId(void)
{
    return sRunningTaskId;
}
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
OS_Ret uOS_TaskChangePri(TaskIdType tid, TaskPriType pri)
{
    RegsType regPsr;
    TaskNode *pTaskNode;

#if (TRUE == CFG_DET_ERROR_DECTECT)
    if (tid >= TASK_NUM_CFG)
    {
        return OS_E_NOT_OK;
    }
    else if (pri >= CFG_PRI_MAX_NUM)
    {
        return OS_E_NOT_OK;
    }
    else if (TASK_STATE_READY != sTcbTable[tid].state)
    {
        return OS_E_NOT_OK;
    }
#endif /* TRUE == CFG_DET_ERROR_DECTECT*/

    Hal_EnterCritical();
    if (sTcbTable[tid].pri != pri)
    {
        /* change position in ready list */
        pTaskNode = TaskListFindByTid(&sReadyList, tid);
        TaskListDel(&sReadyList, pTaskNode);
        sTcbTable[tid].pri = pri;
        sTcbTable[tid].basePri = pri;
        TaskListAdd(&sReadyList, pTaskNode);
    }
    Hal_ExitCritical();

    return OS_E_OK;
}
#endif /* TRUE == CFG_UOS_TASK_CHANGE_PRI_API */

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
OS_Ret uOS_TaskPriInherit(TaskIdType tid, TaskPriType pri)
{
    RegsType regPsr;
    TaskNode *pTaskNode;

#if (TRUE == CFG_DET_ERROR_DECTECT)
    if (tid >= TASK_NUM_CFG)
    {
        return OS_E_NOT_OK;
    }
    else if (pri >= CFG_PRI_MAX_NUM)
    {
        return OS_E_NOT_OK;
    }
    else if (TASK_STATE_READY == sTcbTable[tid].state)
    {
        return OS_E_NOT_OK;
    }
#endif /* TRUE == CFG_DET_ERROR_DECTECT*/

    Hal_EnterCritical();
    if (sTcbTable[tid].pri != pri)
    {
        /* change position in ready list */
        pTaskNode = TaskListFindByTid(&sReadyList, tid);
        TaskListDel(&sReadyList, pTaskNode);
        sTcbTable[tid].pri = pri;
        TaskListAdd(&sReadyList, pTaskNode);
    }
    Hal_ExitCritical();

    return OS_E_OK;

}

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
OS_Ret uOS_TaskPriRecover(TaskIdType tid)
{
    RegsType regPsr;
    TaskNode *pTaskNode;

#if (TRUE == CFG_DET_ERROR_DECTECT)
    if (tid >= TASK_NUM_CFG)
    {
        return OS_E_NOT_OK;
    }
    else if (pri >= CFG_PRI_MAX_NUM)
    {
        return OS_E_NOT_OK;
    }
    else if (TASK_STATE_READY == sTcbTable[tid].state)
    {
        return OS_E_NOT_OK;
    }
#endif /* TRUE == CFG_DET_ERROR_DECTECT*/

    Hal_EnterCritical();
    if (sTcbTable[tid].pri != sTcbTable[tid].basePri)
    {
        /* change position in ready list */
        pTaskNode = TaskListFindByTid(&sReadyList, tid);
        TaskListDel(&sReadyList, pTaskNode);
        sTcbTable[tid].pri = sTcbTable[tid].basePri;
        TaskListAdd(&sReadyList, pTaskNode);
    }
    Hal_ExitCritical();

    return OS_E_OK;
}

/******************************************************************************/
/*
 * @brief:      <task list init> 
 * @detail:     <task list init> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <tid: task id> 
 * @param[out]: <None> 
 * @return:     <ready node>
 */
/******************************************************************************/
STATIC void TaskListInit(TaskList *list)
{
    TaskPriType pri;

    if (NULL != list)
    {
        for (pri = 0; pri < CFG_PRI_MAX_NUM; pri++)
        {
            dlist_init(&list->dList[pri]);    
        }
        list->highestPriTid = TASK_ID_IDLE;
    }
}

/******************************************************************************/
/*
 * @brief:      <task list add> 
 * @detail:     <task list add> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <tid: task id> 
 * @param[out]: <None> 
 * @return:     <ready node>
 */
/******************************************************************************/
STATIC void TaskListAdd(TaskList *list, TaskNode *pNode)
{
    TaskPriType   pri;

    pri = sTcbTable[pNode->tid].pri;

    /* insert list */
    dlist_add(&list->dList[pri], &pNode->dNode, &list->dList[pri].head);

    /* set bitmap */
    BitmapSet(list, pri);

    /* re-calc highest priority*/
    TaskListRecalcPri(list);
}

/******************************************************************************/
/*
 * @brief:      <task list delete> 
 * @detail:     <task list delete> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <tid: task id> 
 * @param[out]: <None> 
 * @return:     <ready node>
 */
/******************************************************************************/
STATIC void TaskListDel(TaskList *list, TaskNode *pNode)
{
    dlist_del(&list->dList[sTcbTable[pNode->tid].pri], &pNode->dNode);

    /* set bitmap */
    if (1 == dlist_empty(&list->dList[sTcbTable[pNode->tid].pri]))
    {
        BitmapClear(list, sTcbTable[pNode->tid].pri);
    }

    TaskListRecalcPri(list);
}

/******************************************************************************/
/*
 * @brief:      <task list find> 
 * @detail:     <task list find> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <tid: task id> 
 * @param[out]: <None> 
 * @return:     <ready node>
 */
/******************************************************************************/
STATIC TaskNode *TaskListFindByTid(TaskList *list, TaskIdType tid)
{
    TaskPriType pri = 0;
    TaskNode *pNode = NULL;
    struct dlist_node *node = NULL;

    pri = sTcbTable[tid].pri;

    for (node = dlist_first(&list->dList[pri]); NULL != node; 
            node = dlist_next(&list->dList[pri], node))
    {
        pNode = contained_of(node, TaskNode, dNode);
        if (tid == pNode->tid)
        {
            break;
        }
    }

    return pNode;
}

/******************************************************************************/
/*
 * @brief:      <task list find> 
 * @detail:     <task list find> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <tid: task id> 
 * @param[out]: <None> 
 * @return:     <ready node>
 */
/******************************************************************************/
STATIC TaskNode *TaskListFindByPri(TaskList *list, TaskPriType pri)
{
    return contained_of(dlist_first(&list->dList[pri]), TaskNode, dNode);
}

/******************************************************************************/
/*
 * @brief:      <re-calc highest pri task> 
 * @detail:     <re-calc highest pri task> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <pri: task id> 
 * @param[out]: <None> 
 * @return:     <None>
 */
/******************************************************************************/
STATIC void TaskListRecalcPri(TaskList *list)
{
    TaskPriType pri = 0;
    TaskNode *pReadyNode;

    /* find highest priority */
    pri = GetHighestPriority(list);

    pReadyNode = contained_of(dlist_first(&list->dList[pri]), 
        TaskNode, dNode);

    list->highestPriTid = pReadyNode->tid;
}

/******************************************************************************/
/*
 * @brief:      <task list find> 
 * @detail:     <task list find> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <tid: task id> 
 * @param[out]: <None> 
 * @return:     <ready node>
 */
/******************************************************************************/
STATIC INLINE TaskNode *FreeNodeGet(void)
{
    TaskNode *pTaskNode;

    pTaskNode = contained_of(dlist_last(&sFreeTaskNodeList), TaskNode, dNode);
    dlist_del(&sFreeTaskNodeList, &pTaskNode->dNode);

    return pTaskNode;
}

/******************************************************************************/
/*
 * @brief:      <task list find> 
 * @detail:     <task list find> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <tid: task id> 
 * @param[out]: <None> 
 * @return:     <ready node>
 */
/******************************************************************************/
STATIC INLINE FreeNodeRecover(TaskNode *pNode)
{
    dlist_append(&sFreeTaskNodeList, &pNode->dNode);
}
