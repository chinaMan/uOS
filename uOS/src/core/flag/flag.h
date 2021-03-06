/*============================================================================*/
/*
 * @brief:   flag manager module header file
 *
 * @author:  bo.zeng
 *
 * @data:    2013.8.11
 *
 */
/*============================================================================*/
#ifndef FLAG_H
#define FLAG_H
#ifdef CPLUSPLUS
#define extern "C" {
#endif 

/*==============================[include]=====================================*/
#include "uos_types.h"
#include "uos_cfg.h"

/*==============================[marco]=======================================*/

/*==============================[typedef]=====================================*/

/*============================[extern data]===================================*/

/*===========================[extern function]================================*/
/******************************************************************************/
/*
 * @brief:      <initalize flag module> 
 * @detail:     <initalize flag module> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
EXTERN OS_Ret uOS_FlagInit(void);

/******************************************************************************/
/*
 * @brief:      <Flag Pend> 
 * @detail:     <Flag Pend> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <flagId: flag id> 
 * @param[in]:  <flag:  flag> 
 * @param[in]:  <waitType: FLAG_WAIT_TYPE_ANY, FLAG_WAIT_TYPE_OR> 
 * @param[in]:  <timeout: timeout wake and return> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
EXTERN OS_Ret uOS_FlagPend(
        FlagIdType flagId, 
        FlagType flag, 
        FlagWaitType waitType,
        TickType timeout);

/******************************************************************************/
/*
 * @brief:      <Event Post> 
 * @detail:     <Event Post> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <flagId: flag id> 
 * @param[in]:  <flag:  flag> 
 * @param[in]:  <flagAct: FLAG_ACT_TYPE_SET, FLAG_ACT_TYPE_CLEAR> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
EXTERN OS_Ret uOS_FlagPost(
        FlagIdType flagId, 
        FlagType flag, 
        FlagActType flagAct);

#ifdef CPLUSPLUS
}
#endif /* CPLUSPLUS */
#endif /* FLAG_H */
