/*============================================================================*/
/*
 * @brief:   sempaphore manager module source file
 *
 * @author:  bo.zeng
 *
 * @data:    2013.8.11
 *
 */
/*============================================================================*/
#ifndef SEM_H
#define SEM_H
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
 * @brief:      <initalize sempaphore module> 
 * @detail:     <initalize sempaphore module> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
EXTERN OS_Ret uOS_SemInit(void);

/******************************************************************************/
/*
 * @brief:      <Sempaphore Pend> 
 * @detail:     <Sempaphore Pend> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
EXTERN OS_Ret uOS_SemPend(SemIdType semId, TickType timeout);

/******************************************************************************/
/*
 * @brief:      <Sempaphore Post> 
 * @detail:     <Sempaphore Post> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
EXTERN OS_Ret uOS_SemPost(SemIdType semId);

#ifdef CPLUSPLUS
}
#endif /* CPLUSPLUS */
#endif /* SEM_H */
