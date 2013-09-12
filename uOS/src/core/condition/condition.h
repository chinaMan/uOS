/*============================================================================*/
/*
 * @brief:   condition manager module source file
 *
 * @author:  bo.zeng
 *
 * @data:    2013.8.11
 *
 */
/*============================================================================*/
#ifndef CONDITION_H
#define CONDITION_H
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
 * @brief:      <initalize condition variable module> 
 * @detail:     <initalize condition variable module> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
EXTERN OS_Ret uOS_CVInit(void);

/******************************************************************************/
/*
 * @brief:      <condition variable Pend> 
 * @detail:     <condition variable Pend> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
EXTERN OS_Ret uOS_CVPend(CVIdType cvId, TickType timeout);

/******************************************************************************/
/*
 * @brief:      <condition variable Post> 
 * @detail:     <condition variable Post> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
EXTERN OS_Ret uOS_CVPost(CVIdType cvId);

#ifdef CPLUSPLUS
}
#endif /* CPLUSPLUS */
#endif /* CONDITION_H */
