/*============================================================================*/
/*
 * @brief:   counter manager module source file
 *
 * @author:  bo.zeng
 *
 * @data:    2013.8.11
 *
 */
/*============================================================================*/
#ifndef COUNTER_H
#define COUNTER_H
#ifdef CPLUSPLUS
#define extern "C" {
#endif 

/*==============================[include]=====================================*/
#include "uos_types.h"
#include "uos_cfg.h"

#if (TRUE == CFG_COUNTER_SUPPORT)
/*==============================[marco]=======================================*/

/*==============================[typedef]=====================================*/

/*============================[extern data]===================================*/

/*===========================[extern function]================================*/
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
EXTERN OS_Ret uOS_CounterInit(void);

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
EXTERN OS_Ret uOS_CounterTrigger(CounterIdType counterId);

/******************************************************************************/
/*
 * @brief:      <get counter current tick> 
 * @detail:     <get counter current tick> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
EXTERN TickType uOS_CounterCurTick(CounterIdType counterId);

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
EXTERN OS_Ret uOS_CounterSetCallback(CounterIdType counterId, 
                                     CounterCallBack fpCallback);

#endif /* TRUE == CFG_COUNTER_SUPPORT */
#ifdef CPLUSPLUS
}
#endif /* CPLUSPLUS */
#endif /* COUNTER_H */
