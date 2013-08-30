/*============================================================================*/
/*
 * @brief:   mutex module source code
 *
 * @author:  bo.zeng
 *
 * @data:    2013.8.11
 *
 */
/*============================================================================*/

/*==============================[include]=====================================*/
#include "mutex.h"
#include "task.h"
#include "event.h"
#include "uhal.h"

#if (TRUE == CFG_MUTEX_SUPPORT)
/*=============================[type define]==================================*/
typedef struct
{
    sint8 value;
} MutexControlBlock;

/*=============================[extern data]==================================*/
EXTERN CONST MutexMgrCfgType gMutexMgrCfg;

/*=============================[marco define]=================================*/
#if (CFG_VARIANT_TYPE == CFG_VARIANT_PC)
#define MUTEX_MGR_CFG    gMutexMgrCfg
#define MUTEX_NUM_CFG    (CFG_MUTEX_MAX_NUM)
#elif (CFG_VARIANT_TYPE == CFG_VARIANT_LT)
#define MUTEX_MGR_CFG    gMutexMgrCfg
#define MUTEX_NUM_CFG    (CFG_MUTEX_MAX_NUM)
#else
#define MUTEX_MGR_CFG    gMutexMgrCfg
#define MUTEX_NUM_CFG    (CFG_MUTEX_MAX_NUM)
#endif

/*=============================[internal data]================================*/
STATIC MutexControlBlock sMutexCBTable[CFG_MUTEX_MAX_NUM];

/*=============================[internal function]============================*/

/*===========================[function define]================================*/
/******************************************************************************/
/*
 * @brief:      <initalize mutex module> 
 * @detail:     <initalize mutex module> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
OS_Ret uOS_MutexInit(void)
{
    MutexIdType mutexId;

    for (mutexId = 0; mutexId < MUTEX_NUM_CFG; mutexId++)
    {
        sMutexCBTable[mutexId].value 
            = MUTEX_MGR_CFG.pMutexTableCfg[mutexId].initValue;
    }

    return OS_E_OK;
}

/******************************************************************************/
/*
 * @brief:      <Mutex Pend> 
 * @detail:     <Mutex Pend> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
OS_Ret uOS_MutexPend(MutexIdType mutexId, TickType timeout)
{
    RegsType regPsr;

#if (TRUE == CFG_DET_ERROR_DECTECT)    
    if (mutexId >= MUTEX_NUM_CFG)
    {
        return OS_E_NOT_OK;
    }
#endif

    Hal_EnterCritical();
    if (1 == sMutexCBTable[mutexId].value)
    {
        sMutexCBTable[mutexId].value = 0;
        uOS_TaskPriInherit(uOS_CurTaskId(), MUTEX_MGR_CFG.pMutexTableCfg[mutexId].pri);
        Hal_ExitCritical();
        return OS_E_OK;
    }
    Hal_ExitCritical();

    uOS_EventPend(MUTEX_MGR_CFG.pMutexTableCfg[mutexId].eventId, timeout);
    uOS_TaskSched();

    Hal_EnterCritical();
    if (1 == sMutexCBTable[mutexId].value)
    {
        sMutexCBTable[mutexId].value = 0;
        uOS_TaskPriInherit(uOS_CurTaskId(), MUTEX_MGR_CFG.pMutexTableCfg[mutexId].pri);
        Hal_ExitCritical();
        return OS_E_OK;
    }
    else
    {
        Hal_ExitCritical();
        return OS_E_NOT_OK;
    }

    return OS_E_OK;
}

/******************************************************************************/
/*
 * @brief:      <Event Post> 
 * @detail:     <Event Post> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
OS_Ret uOS_MutexPost(MutexIdType mutexId)
{
    RegsType regPsr;

#if (TRUE == CFG_DET_ERROR_DECTECT)    
    if (mutexId >= MUTEX_NUM_CFG)
    {
        return OS_E_NOT_OK;
    }
#endif

    Hal_EnterCritical();
    if (1 == sMutexCBTable[mutexId].value)
    {
        Hal_ExitCritical();
        return OS_E_NOT_OK;    
    }

    sMutexCBTable[mutexId].value = 1;
    Hal_ExitCritical();

    uOS_EventPost(MUTEX_MGR_CFG.pMutexTableCfg[mutexId].eventId);
    uOS_TaskPriRecover(uOS_CurTaskId());
    uOS_TaskSched();

    return OS_E_OK;
}

#endif /* TRUE == CFG_MUTEX_SUPPORT */
