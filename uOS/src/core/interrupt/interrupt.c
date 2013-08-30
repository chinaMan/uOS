/*============================================================================*/
/*
 * @brief:   interrupt manager module source code
 *
 * @author:  bo.zeng
 *
 * @data:    2013.8.11
 *
 */
/*============================================================================*/

/*==============================[include]=====================================*/
#include "interrupt.h"
#include "uhal.h"

/*=============================[type define]==================================*/

/*=============================[extern data]==================================*/

/*=============================[marco define]=================================*/

/*=============================[internal data]================================*/
STATIC uint8 sIntLockLevel = 0;
/*=============================[internal function]============================*/

/*===========================[function define]================================*/
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
STATIC OS_Ret uOS_IntInit(void)
{}

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
STATIC void uOS_IntEntry(void)
{
    if (sIntLockLevel < 255U)
    {
        sIntLockLevel++;
    }
}

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
STATIC void uOS_IntExit(void)
{
    RegsType regPsr;

    Hal_EnterCritical();
    if (sIntLockLevel > 0U)
    {
        sIntLockLevel--;
    }

    if (0U == sIntLockLevel)
    {
        Hal_ExitCritical();
        uOS_TaskSched();
    }
    Hal_ExitCritical();
}

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
STATIC boolean uOS_InIntHandler(void)
{
    return (0U < sIntLockLevel) ? TRUE : FALSE;
}
