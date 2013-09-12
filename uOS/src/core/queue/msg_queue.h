/*============================================================================*/
/*
 * @brief:   message queue manager module header file
 *
 * @author:  bo.zeng
 *
 * @data:    2013.9.12
 *
 */
/*============================================================================*/
#ifndef MSG_QUEUE_H
#define MSG_QUEUE_H
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
 * @brief:      <initalize message queue module> 
 * @detail:     <initalize message queue module> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
EXTERN OS_Ret uOS_MsgQInit(void);

/******************************************************************************/
/*
 * @brief:      <message queue Pend> 
 * @detail:     <message queue Pend> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
EXTERN void *uOS_MsgQPend(MsgQIdType msgQId, TickType timeout);

/******************************************************************************/
/*
 * @brief:      <message queue Post> 
 * @detail:     <message queue Post> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
EXTERN OS_Ret uOS_MsgQPost(MsgQIdType msgQId, void *msg);

/******************************************************************************/
/*
 * @brief:      <message queue Post front> 
 * @detail:     <message queue Post front> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
EXTERN OS_Ret uOS_MsgQPostFront(MsgQIdType msgQId, void *msg);

#ifdef CPLUSPLUS
}
#endif /* CPLUSPLUS */
#endif /* MSG_QUEUE_H */
