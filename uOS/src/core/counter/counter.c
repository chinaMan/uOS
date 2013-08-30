/*============================================================================*/
/*
 * @brief:   counter module source code
 *
 * @author:  bo.zeng
 *
 * @data:    2013.8.11
 *
 */
/*============================================================================*/

/*==============================[include]=====================================*/
#include "uhal.h"
#include "counter.h"

#if (TRUE == CFG_COUNTER_SUPPORT)
/*=============================[type define]==================================*/
typedef struct
{
    TickType curTick;

    CounterCallBack fpCallback;
} CounterControlBlock;

/*=============================[extern data]==================================*/
extern CONST CounterMgrCfgType gCounterMgrCfg;

/*=============================[marco define]=================================*/
#if (CFG_VARIANT_TYPE == CFG_VARIANT_PC)
#define COUNTER_MGR_CFG    gCounterMgrCfg
#define COUNTER_NUM_CFG    (CFG_COUNTER_MAX_NUM)
#elif (CFG_VARIANT_TYPE == CFG_VARIANT_LT)
#define COUNTER_MGR_CFG    gCounterMgrCfg
#define COUNTER_NUM_CFG    (CFG_COUNTER_MAX_NUM)
#else
#define COUNTER_MGR_CFG    gCounterMgrCfg
#define COUNTER_NUM_CFG    (CFG_COUNTER_MAX_NUM)
#endif

/*=============================[internal data]================================*/
STATIC CounterControlBlock sCounterCBTable[CFG_COUNTER_MAX_NUM];

/*=============================[internal function]============================*/

/*===========================[function define]================================*/
/******************************************************************************/
/*
 * @brief:      <initalize counter module> 
 * @detail:     <initalize counter module> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
OS_Ret uOS_CounterInit(void)
{
    CounterIdType cid;

    for (cid = 0; cid < COUNTER_NUM_CFG; cid++)
    {
        sCounterCBTable[cid].curTick = 0;
    }

    return OS_E_OK;
}

/******************************************************************************/
/*
 * @brief:      <counter trigger> 
 * @detail:     <counter trigger> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
OS_Ret uOS_CounterTrigger(CounterIdType counterId)
{
    RegsType regPsr;

#if (TRUE == CFG_DET_ERROR_DECTECT)      
    if (counterId >= COUNTER_NUM_CFG)
    {
        return OS_E_NOT_OK;
    }
#endif

    Hal_EnterCritical();
    if (sCounterCBTable[counterId].curTick >= COUNTER_MGR_CFG.pCounterTableCfg[counterId].maxValue)
    {
        sCounterCBTable[counterId].curTick = 0;
    }
    else
    {
        sCounterCBTable[counterId].curTick = 0;
    }

    if (NULL != sCounterCBTable[counterId].fpCallback)
    {
        Hal_ExitCritical();
        sCounterCBTable[counterId].fpCallback(counterId);
    }
    else
    {
        Hal_ExitCritical();
    }

    return OS_E_OK;
}

/******************************************************************************/
/*
 * @brief:      <get counter tick> 
 * @detail:     <get counter tick> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
TickType uOS_CounterCurTick(CounterIdType counterId)
{
    RegsType regPsr;
    TickType curTick;

#if (TRUE == CFG_DET_ERROR_DECTECT)      
    if (counterId >= COUNTER_NUM_CFG)
    {
        return 0;
    }
#endif

    Hal_EnterCritical();
    curTick = sCounterCBTable[counterId].curTick;
    Hal_ExitCritical();

    return curTick;
}

/******************************************************************************/
/*
 * @brief:      <set CallBack function for counter trigger> 
 * @detail:     <set CallBack function for counter trigger> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
OS_Ret uOS_CounterSetCallback(CounterIdType counterId, 
                                     CounterCallBack fpCallback)
{
    RegsType regPsr;

#if (TRUE == CFG_DET_ERROR_DECTECT)      
    if (counterId >= COUNTER_NUM_CFG)
    {
        return OS_E_NOT_OK;
    }
#endif

    Hal_EnterCritical();
    sCounterCBTable[counterId].fpCallback = fpCallback;
    Hal_ExitCritical();

    return OS_E_OK;
}

#endif /* TRUE == CFG_COUNTER_SUPPORT */
