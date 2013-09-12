/*============================================================================*/
/*
 * @brief:   core of uOS
 *
 * @author:  bo.zeng
 *
 * @data:    2013.8.11
 *
 */
/*============================================================================*/

/*==============================[include]=====================================*/
#include "uOs.h"
#include "task.h"

/*=============================[static data]==================================*/

/*===========================[function define]================================*/
/******************************************************************************/
/*
 * @brief:      <start os> 
 * @detail:     <start os>
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @return:     <OS_Ret: os return>
 */
/******************************************************************************/
OS_Ret uOS_Start(void)
{
    if (OS_E_OK != uOS_TaskInit())
    {
        return OS_E_NOT_OK;
    }
    else if (OS_E_OK != uOS_CounterInit())
    {
        return OS_E_NOT_OK;
    }
    else if (OS_E_OK != uOS_EventInit())
    {
        return OS_E_NOT_OK;
    }
    else if (OS_E_OK != uOS_TimerInit())
    {
        return OS_E_NOT_OK;
    }
    else if (OS_E_OK != uOS_MutexInit())
    {
        return OS_E_NOT_OK;
    }
    else if (OS_E_OK != uOS_SemInit())
    {
        return OS_E_NOT_OK;
    }
    else if (OS_E_OK != uOS_FlagInit())
    {
        return OS_E_NOT_OK;
    }
    else if (OS_E_OK != uOS_CVInit())
    {
        return OS_E_NOT_OK;
    }
    else if (OS_E_OK != uOS_MsgQInit())
    {
        return OS_E_NOT_OK;
    }

    uOS_TaskSched();

    /* entry idle task */
    while (1)
    {
        /* do nothing */
    }

    return OS_E_OK;
}

