/*============================================================================*/
/*
 * @brief:   timer module source code
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
#include "timer.h"
#include "task.h"

#if (TRUE == CFG_TIMER_SUPPORT)
/*=============================[type define]==================================*/
typedef struct
{
    boolean        blRunning;
    TickType       curValue;
    TickType       period;
    uint8          repeat;
}TimerControlBlock;

/*=============================[extern data]==================================*/
EXTERN CONST TimerMgrCfgType gTimerMgrCfg;

/*=============================[marco define]=================================*/
#if (CFG_VARIANT_TYPE == CFG_VARIANT_PC)
#define TIMER_MGR_CFG    gTimerMgrCfg
#define TIMER_NUM_CFG    (CFG_TIMER_MAX_NUM)
#elif (CFG_VARIANT_TYPE == CFG_VARIANT_LT)
#define TIMER_MGR_CFG    gTimerMgrCfg
#define TIMER_NUM_CFG    (CFG_TIMER_MAX_NUM)
#else
#define TIMER_MGR_CFG    gTimerMgrCfg
#define TIMER_NUM_CFG    (CFG_TIMER_MAX_NUM)
#endif

/*=============================[internal data]================================*/
STATIC TimerControlBlock sTimerCBTable[CFG_TIMER_MAX_NUM];

/*=============================[internal function]============================*/
STATIC void uOS_TimerCheck(CounterIdType counterId);
STATIC OS_Ret uOS_TimerTrigger(TimerIdType timerId);

/*===========================[function define]================================*/
/******************************************************************************/
/*
 * @brief:      <initalize Timer module> 
 * @detail:     <initalize Timer module> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
OS_Ret uOS_TimerInit(void)
{
    TimerIdType timerId;

    for (timerId = 0; timerId < TIMER_NUM_CFG; timerId++)
    {
        sTimerCBTable[timerId].blRunning = FALSE;
        sTimerCBTable[timerId].curValue = 0;
        sTimerCBTable[timerId].period   = 0;
        sTimerCBTable[timerId].repeat   = 0;

        uOS_CounterSetCallback(TIMER_MGR_CFG.pTimerTableCfg[timerId].counterId, uOS_TimerCheck);
    }

    return OS_E_OK;
}

/******************************************************************************/
/*
 * @brief:      <get counter current tick> 
 * @detail:     <get counter current tick> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <timerId: timer id> 
 * @param[in]:  <period: period> 
 * @param[in]:  <repeat: repeat count of timer, 0xFF: always repeat> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
OS_Ret uOS_TimerSet(TimerIdType timerId, TickType period, uint8 repeat)
{
    RegsType regPsr;

#if (TRUE == CFG_DET_ERROR_DECTECT)      
    if (timerId >= TIMER_NUM_CFG)
    {
        return OS_E_NOT_OK;
    }
#endif

    Hal_EnterCritical();
    sTimerCBTable[timerId].period = period;
    sTimerCBTable[timerId].repeat = repeat;
    Hal_ExitCritical();

    return OS_E_OK;
}

/******************************************************************************/
/*
 * @brief:      <start timer> 
 * @detail:     <start timer> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <timerId: timer id> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
OS_Ret uOS_TimerStart(TimerIdType timerId)
{
    RegsType regPsr;

#if (TRUE == CFG_DET_ERROR_DECTECT)      
    if (timerId >= TIMER_NUM_CFG)
    {
        return OS_E_NOT_OK;
    }
#endif

    Hal_EnterCritical();
    sTimerCBTable[timerId].blRunning = TRUE;
    Hal_ExitCritical();


    return OS_E_OK;
}

/******************************************************************************/
/*
 * @brief:      <start timer> 
 * @detail:     <start timer> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <timerId: timer id> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
OS_Ret uOS_TimerStop(TimerIdType timerId)
{
    RegsType regPsr;

#if (TRUE == CFG_DET_ERROR_DECTECT)      
    if (timerId >= TIMER_NUM_CFG)
    {
        return OS_E_NOT_OK;
    }
#endif

    Hal_EnterCritical();
    sTimerCBTable[timerId].blRunning = FALSE;
    Hal_ExitCritical();

    return OS_E_OK;
}

/******************************************************************************/
/*
 * @brief:      <Timer trigger> 
 * @detail:     <Timer trigger> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <timerId: timer id> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
STATIC OS_Ret uOS_TimerTrigger(TimerIdType timerId)
{
    RegsType regPsr;

#if (TRUE == CFG_DET_ERROR_DECTECT)      
    if (timerId >= TIMER_NUM_CFG)
    {
        return OS_E_NOT_OK;
    }
#endif
    Hal_EnterCritical();
    if(TRUE == sTimerCBTable[timerId].blRunning)
    {
        if (sTimerCBTable[timerId].curValue >= sTimerCBTable[timerId].period)
        {
            sTimerCBTable[timerId].curValue = 0;

            if (0 == sTimerCBTable[timerId].repeat)
            {
                sTimerCBTable[timerId].blRunning = FALSE;
            }
            else if (0xFF != sTimerCBTable[timerId].repeat)
            {
                sTimerCBTable[timerId].repeat--;
            }
            else
            {
                /* do nothing */
            }
            
            if (NULL != TIMER_MGR_CFG.pTimerTableCfg[timerId].fpCallback)
            {
                TIMER_MGR_CFG.pTimerTableCfg[timerId].fpCallback(timerId);
            }
            else
            {
                uOS_TaskActive(TIMER_MGR_CFG.pTimerTableCfg[timerId].tid);                
            }
        }
        else
        {
            sTimerCBTable[timerId].curValue++;
        }
    }
    Hal_ExitCritical();

    return OS_E_OK;
}

/******************************************************************************/
/*
 * @brief:      <start timer> 
 * @detail:     <start timer> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <timerId: timer id> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
STATIC void uOS_TimerCheck(CounterIdType counterId)
{
    TimerIdType timerId;

    for (timerId = 0; timerId < TIMER_NUM_CFG; timerId++)
    {
        if (counterId == TIMER_MGR_CFG.pTimerTableCfg[timerId].counterId)
        {
            uOS_TimerTrigger(timerId);
        }
    }
}

#endif /* TRUE == CFG_COUNTER_SUPPORT */
