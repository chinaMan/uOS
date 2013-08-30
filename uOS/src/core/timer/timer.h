/*============================================================================*/
/*
 * @brief:   timer manager module source file
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

#if (TRUE == CFG_TIMER_SUPPORT)
/*==============================[marco]=======================================*/

/*==============================[typedef]=====================================*/

/*============================[extern data]===================================*/

/*===========================[extern function]================================*/
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
EXTERN OS_Ret uOS_TimerInit(void);

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
EXTERN OS_Ret uOS_TimerSet(TimerIdType timerId, TickType period, uint8 repeat);

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
EXTERN OS_Ret uOS_TimerStart(TimerIdType timerId);

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
EXTERN OS_Ret uOS_TimerStop(TimerIdType timerId);

#endif /* TRUE == CFG_TIMER_SUPPORT */
#ifdef CPLUSPLUS
}
#endif /* CPLUSPLUS */
#endif /* COUNTER_H */
