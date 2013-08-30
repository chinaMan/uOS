/*============================================================================*/
/*
 * @brief:   event module source code
 *
 * @author:  bo.zeng
 *
 * @data:    2013.8.11
 *
 */
/*============================================================================*/

/*==============================[include]=====================================*/
#include "event.h"
#include "task.h"
#include "uhal.h"

#if (TRUE == CFG_EVENT_SUPPORT)
/*=============================[type define]==================================*/
typedef struct
{
    TaskIdType   owner;
    TaskWaitList waitList;
} EventControlBlock;

/*=============================[extern data]==================================*/

/*=============================[marco define]=================================*/

/*=============================[internal data]================================*/
STATIC EventControlBlock sEventCBTable[CFG_EVENT_MAX_NUM];

/*=============================[internal function]============================*/

/*===========================[function define]================================*/
/******************************************************************************/
/*
 * @brief:      <initalize Event module> 
 * @detail:     <initalize Event module> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
OS_Ret uOS_EventInit(void)
{
    EventIdType eventId;

    for (eventId = 0; eventId < CFG_EVENT_MAX_NUM; eventId++)
    {
        uOS_TaskWaitListInit(&sEventCBTable[eventId].waitList);
    }

    return OS_E_OK;
}

/******************************************************************************/
/*
 * @brief:      <Event Pend> 
 * @detail:     <Event Pend> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
OS_Ret uOS_EventPend(EventIdType eventId, TickType timeout)
{
#if (TRUE == CFG_DET_ERROR_DECTECT)      
    if (eventId >= EVENT_NUM_CFG)
    {
        return OS_E_NOT_OK;
    }
#endif

    uOS_TaskWait(&sEventCBTable[eventId].waitList, timeout);

    return OS_E_NOT_OK;
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
OS_Ret uOS_EventPost(EventIdType eventId)
{
#if (TRUE == CFG_DET_ERROR_DECTECT)      
    if (eventId >= EVENT_NUM_CFG)
    {
        return OS_E_NOT_OK;
    }
#endif

    uOS_TaskWake(&sEventCBTable[eventId].waitList);

    return OS_E_NOT_OK;
}

#endif /* TRUE == CFG_EVENT_SUPPORT */
