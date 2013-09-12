/*============================================================================*/
/*
 * @brief:   uos config header file
 *
 * @author:  bo.zeng
 *
 * @data:    2013.8.11
 *
 */
/*============================================================================*/
#ifndef UOS_CFG_H
#define UOS_CFG_H
#ifdef CPLUSPLUS
#define extern "C" {
#endif 
/*
 *  variant type
 */
#define  CFG_VARIANT_PC    0
#define  CFG_VARIANT_LT    0
#define  CFG_VARIANT_PB    0
#define  CFG_VARIANT_TYPE  CFG_VARIANT_PC


#define  CFG_DET_ERROR_DECTECT   FALSE
#define  CFG_DEM_ERROR_DECTECT   TRUE

/* debug */
#define  DGL_ERROR               0
#define  DGL_WARN                1
#define  DGL_INFO                2
#define  DGL_ALL                 3
#define  CFG_DEBUG_LEVEL         DEBUG_LEVEL_INFO
#define  printk

/* Module support switch */
#define  CFG_COUNTER_SUPPORT          TRUE
#define  CFG_TIMER_SUPPORT            TRUE
#define  CFG_EVENT_SUPPORT            TRUE
#define  CFG_FLAG_SUPPORT             TRUE
#define  CFG_CV_SUPPORT               TRUE
#define  CFG_SEM_SUPPORT              TRUE
#define  CFG_MUTEX_SUPPORT            TRUE
#define  CFG_MSGQ_SUPPORT             TRUE

/*===============================[Task Cofigure]==============================*/
/*
 *  Task Manager General
 */
/* api support */
#define  CFG_UOS_TASKWAIT_API           TRUE
#define  CFG_UOS_TASKWAKE_API           TRUE
#define  CFG_UOS_CUR_TASK_ID_API        TRUE
#define  CFG_UOS_TASK_CHANGE_PRI_API    TRUE

#define  CFG_TASK_MAX_NUM        3       /* max number of task */
#define  CFG_PRI_MAX_NUM         8       /* max number of task */

typedef uint8  TaskIdType;               /* depend on CFG_TASK_MAX_NUM */
typedef uint8  TaskPriType;
typedef uint16 TaskStkSizeType;

/*
 * Task configure
 */
#define  TASK_ID_KEYPAD         0
#define  TASK_ID_MMI            1

/*
 * Counter Manager General
 */
#define  CFG_COUNTER_MAX_NUM    1
typedef uint8  CounterIdType;           /* depend on CFG_COUNTER_MAX_NUM */

/*
 * Counter configure
 */
#define  COUNTER_ID_SYSTEM      0

/*
 * Timer General 
 */
#define  CFG_TIMER_MAX_NUM      1
typedef uint8  TimerIdType;

/*
 * Event General 
 */
#define  CFG_EVENT_MAX_NUM      (CFG_MUTEX_MAX_NUM+CFG_CV_MAX_NUM+CFG_SEM_MAX_NUM+CFG_MSGQ_MAX_NUM)
typedef uint8  EventIdType;

/*
 * Mutex General 
 */
#define  CFG_MUTEX_MAX_NUM      1
typedef uint8  MutexIdType;

/*
 * Sempaphore General 
 */
#define  CFG_SEM_MAX_NUM        1
typedef uint8  SemIdType;

/*
 * Flag General 
 */
#define  CFG_FLAG_MAX_NUM                1
#define  CFG_FLAG_WAITTASK_MAX_NUM       32
typedef uint8  FlagIdType;

/*
 * condition variable General 
 */
#define  CFG_CV_MAX_NUM          1
typedef uint8  CVIdType;

/*
 * message queue General
 */
#define  CFG_MSGQ_MAX_NUM       1
typedef uint8  MsgQIdType;

/*===========================[Interrupt Cofigure]============================*/

#ifdef CPLUSPLUS
}
#endif /* CPLUSPLUS */
#endif /* UOS_CFG_H */
