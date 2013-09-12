/*============================================================================*/
/*
 * @brief:   uos pre-compiler config data
 *
 * @author:  bo.zeng
 *
 * @data:    2013.8.11
 *
 */
/*============================================================================*/

/*==============================[include]=====================================*/
#include "uOs.h"

/*=============================[export data/function]=========================*/
EXTERN void TaskKeyPad(void);
EXTERN void TaskMmi(void);

/*=============================[task configure]===============================*/
STATIC byte TaskKeyPadStk[1024];
STATIC byte TaskMmiStk[1024];
STATIC CONST TaskTableCfgType sTaskTableCfgData[CFG_TASK_MAX_NUM-1] = 
{
    /* fpEntry     autoStart  stkSize   stkBottom */
    {TaskKeyPad,   TRUE,      1024,     (CONST uint32)TaskKeyPadStk},        
    {TaskMmi,      TRUE,      1024,     (CONST uint32)TaskMmiStk},        
};

STATIC CONST TaskPriType sTaskPriCfgData[CFG_TASK_MAX_NUM-1] = 
{
    0, 1
};

CONST TaskMgrCfgType gTaskMgrCfg = 
{
    sTaskTableCfgData,
    {
        sTaskPriCfgData
    },
};

#if (TRUE == CFG_COUNTER_SUPPORT)
STATIC CONST CounterCfgType sCounterTableCfgData[CFG_COUNTER_MAX_NUM] =
{
    0xFFFF,
};

CONST CounterMgrCfgType gCounterMgrCfg = {sCounterTableCfgData};
#endif


#if (TRUE == CFG_TIMER_SUPPORT)
STATIC CONST TimerCfgType sTimerTableCfgData[CFG_TIMER_MAX_NUM] =
{
    {COUNTER_ID_SYSTEM, NULL, TASK_ID_KEYPAD},
};

CONST TimerMgrCfgType gTimerMgrCfg = {sTimerTableCfgData};
#endif

#if (TRUE == CFG_MUTEX_SUPPORT)
STATIC CONST MutexCfgType sMutexTableCfgData[CFG_MUTEX_MAX_NUM] =
{
    {1, 0, 0},
};

CONST MutexMgrCfgType gMutexMgrCfg = {sMutexTableCfgData};
#endif

#if (TRUE == CFG_SEM_SUPPORT)
STATIC CONST SemCfgType sSemTableCfgData[CFG_MUTEX_MAX_NUM] =
{
    {2, 2, 0, 1},
};

CONST SemMgrCfgType gSemMgrCfg = {sSemTableCfgData};
#endif

#if (TRUE == CFG_FLAG_SUPPORT)
STATIC CONST FlagCfgType sFlagTableCfgData[CFG_FLAG_MAX_NUM] =
{
    {0},
};

CONST FlagMgrCfgType gFlagMgrCfg = {sFlagTableCfgData};
#endif

#if (TRUE == CFG_CV_SUPPORT)
STATIC CONST CVCfgType sCVTableCfgData[CFG_FLAG_MAX_NUM] =
{
    {3},
};

CONST CVMgrCfgType gCVMgrCfg = {sCVTableCfgData};
#endif

#if (TRUE == CFG_MSGQ_SUPPORT)
void *gMsgQ[3];
STATIC CONST MsgQCfgType sMsgQTableCfgData[CFG_FLAG_MAX_NUM] =
{
    {3, (void **)gMsgQ, 4},
};

CONST MsgQMgrCfgType gMsgQMgrCfg = {sMsgQTableCfgData};
#endif
