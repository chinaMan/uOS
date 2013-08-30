/*============================================================================*/
/*
 * @brief:   event manager module source file
 *
 * @author:  bo.zeng
 *
 * @data:    2013.8.11
 *
 */
/*============================================================================*/
#ifndef EVENT_H
#define EVENT_H
#ifdef CPLUSPLUS
#define extern "C" {
#endif 

/*==============================[include]=====================================*/
#include "uos_types.h"
#include "uos_cfg.h"

/*==============================[marco]=======================================*/

/*==============================[typedef]=====================================*/
typedef enum
{
    EVENT_TYPE_SEM,
    EVENT_TYPE_MUTEX,
    EVENT_TYPE_MBOX,
    EVENT_TYPE_QEUEU,
    EVENT_TYPE_FLAG
} EventType;

/*============================[extern data]===================================*/

/*===========================[extern function]================================*/
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
EXTERN OS_Ret uOS_EventInit(void);

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
EXTERN OS_Ret uOS_EventPend(EventIdType eventId, TickType timeout);

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
EXTERN OS_Ret uOS_EventPost(EventIdType eventId);

#ifdef CPLUSPLUS
}
#endif /* CPLUSPLUS */
#endif /* EVENT_H */
