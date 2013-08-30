/*============================================================================*/
/*
 * @brief:   header file of uOs, define the interface all OS
 *
 * @author:  bo.zeng
 *
 * @data:    2013.8.11
 *
 */
/*============================================================================*/
#ifndef UOS_H
#define UOS_H
#ifdef CPLUSPLUS
#define extern "C" {
#endif 

/*==============================[include]=====================================*/
#include "uOs_types.h"
#include "uOs_cfg.h"
#include "task.h"
#include "timer.h"
#include "mutex.h"
#include "sem.h"

/*==============================[marco]=======================================*/

/*==============================[typedef]=====================================*/

/******************************************************************************/
/*
 * @brief:      <start os> 
 * @detail:     <start os module> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <OS_Ret: os return>
 */
/******************************************************************************/
EXTERN OS_Ret uOS_Start(void);

#ifdef CPLUSPLUS
}
#endif /* CPLUSPLUS */
#endif /* UOS_H */
