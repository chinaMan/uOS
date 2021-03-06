/*============================================================================*/
/*
 * @brief:   data type define about os module
 *
 * @author:  bo.zeng
 *
 * @data:    2013.8.11
 *
 */
/*============================================================================*/
#ifndef UOS_TYPES_H
#define UOS_TYPES_H
#ifdef CPLUSPLUS
#define extern "C" {
#endif 

/*==============================[include]=====================================*/
#include "compiler.h"
#include "uos_cfg.h"

/*==============================[marco]=======================================*/
/* OS Return Value */
#define OS_E_OK        0
#define OS_E_NOT_OK    1

/*==============================[typedef]=====================================*/
typedef uint8 OS_Ret;

/* 
 * task 
 */
/* task entry function */
typedef void (*TaskEntry)(void);

typedef struct
{
    TaskEntry           fpEntry;    /* entry function of task */
    boolean             autoStart;  /* auto active when os start */
    TaskStkSizeType     stkSize;    /* stack size */
    uint32              stkBottom;  
}TaskTableCfgType;

typedef struct
{
    CONST TaskPriType         *pPriTableCfg;    
}TaskSchedCfgType;

typedef struct
{
    CONST TaskTableCfgType    *pTaskTableCfg;
    CONST TaskSchedCfgType    taskSchedCfg;
} TaskMgrCfgType;

/*
 * Counter
 */
typedef uint16 TickType;
typedef void (*CounterCallBack)(CounterIdType counterId);
typedef struct
{
    TickType maxValue;    
} CounterCfgType;

typedef struct
{
    CONST CounterCfgType  *pCounterTableCfg;
} CounterMgrCfgType;

/*
 * Timer
 */
typedef void (*TimerCallBack)(TimerIdType timerId);
typedef struct
{
    CounterIdType counterId;

    TimerCallBack fpCallback;

    TaskIdType    tid;
} TimerCfgType;

typedef struct
{
    CONST TimerCfgType  *pTimerTableCfg;
} TimerMgrCfgType;

/*
 * Mutex
 */
typedef struct
{
    sint8        initValue;    /* 0, 1 */
    TaskPriType  pri;
    EventIdType  eventId;
} MutexCfgType;

typedef struct
{
    CONST MutexCfgType  *pMutexTableCfg;
} MutexMgrCfgType;

/*
 * Sempaphore
 */
typedef struct
{
    uint16       initValue;
    uint16       cnt;
    TaskPriType  pri;
    EventIdType  eventId;
} SemCfgType;

typedef struct
{
    CONST SemCfgType  *pSemTableCfg;
} SemMgrCfgType;

/*
 * Flag
 */
typedef uint16  FlagType;
#define FLAG_CONSUME  0x80
typedef enum
{
    FLAG_WAIT_TYPE_ANY,
    FLAG_WAIT_TYPE_OR,
    FLAG_WAIT_TYPE_MAX
} FlagWaitType;
typedef enum
{
    FLAG_ACT_TYPE_SET,
    FLAG_ACT_TYPE_CLEAR,
    FLAG_ACT_TYPE_MAX
} FlagActType;

typedef struct
{
    FlagType     initFlag;
} FlagCfgType;

typedef struct
{
    CONST FlagCfgType  *pFlagTableCfg;
} FlagMgrCfgType;

/*
 * Condition variable 
 */
typedef struct
{
    EventIdType  eventId;
} CVCfgType;

typedef struct
{
    CONST CVCfgType  *pCVTableCfg;
} CVMgrCfgType;

/*
 * Message Queue
 */
typedef struct
{
    uint16       size;
    void         **msgStorage;
    EventIdType  eventId;
} MsgQCfgType;

typedef struct
{
    CONST MsgQCfgType  *pMsgQTableCfg;
} MsgQMgrCfgType;

#ifdef CPLUSPLUS
}
#endif /* CPLUSPLUS */
#endif /* UOS_TYPES_H */
