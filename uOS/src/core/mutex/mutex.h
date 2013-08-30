/*============================================================================*/
/*
 * @brief:   mutex manager module source file
 *
 * @author:  bo.zeng
 *
 * @data:    2013.8.11
 *
 */
/*============================================================================*/
#ifndef MUTEX_H
#define MUTEX_H
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
 * @brief:      <initalize mutex module> 
 * @detail:     <initalize mutex module> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
EXTERN OS_Ret uOS_MutexInit(void);

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
EXTERN OS_Ret uOS_MutexPend(MutexIdType mutexId, TickType timeout);

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
EXTERN OS_Ret uOS_MutexPost(MutexIdType mutexId);

#ifdef CPLUSPLUS
}
#endif /* CPLUSPLUS */
#endif /* MUTEX_H */
