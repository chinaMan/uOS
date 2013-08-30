/*============================================================================*/
/*
 * @brief:   interrupt manager module header file
 *
 * @author:  bo.zeng
 *
 * @data:    2013.8.11
 *
 */
/*============================================================================*/
#ifndef INTERRUPT_H
#define INTERRUPT_H
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
 * @brief:      <initalize interrupt module> 
 * @detail:     <initalize interrupt module>
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @return:     <OS_Ret: os return>
 */
/******************************************************************************/
EXTERN OS_Ret uOS_IntInit(void);

/******************************************************************************/
/*
 * @brief:      <entry interrupt> 
 * @detail:     <entry interrupt>
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @return:     <OS_Ret: os return>
 */
/******************************************************************************/
EXTERN void uOS_IntEntry(void);

/******************************************************************************/
/*
 * @brief:      <exit interrupt> 
 * @detail:     <exit interrupt>
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @return:     <OS_Ret: os return>
 */
/******************************************************************************/
EXTERN void uOS_IntExit(void);

/******************************************************************************/
/*
 * @brief:      <in interrupt> 
 * @detail:     <in interrupt>
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @return:     <OS_Ret: os return>
 */
/******************************************************************************/
EXTERN boolean uOS_InIntHandler(void);

#ifdef CPLUSPLUS
}
#endif /* CPLUSPLUS */
#endif /* INTERRUPT_H */
