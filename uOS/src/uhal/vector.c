/*============================================================================*/
/*
 * @brief:   uOs hardware abstract layer
 *
 * @compiler: keil4.0
 *
 * @mcu: S3C2440
 *
 * @author:  bo.zeng
 *
 * @data:    2013.8.11
 *
 */
/*============================================================================*/

/*==============================[include]=====================================*/
#include "uhal.h"

/*==============================[typedef]=====================================*/
/******************************************************************************/
/*
 * @brief:      <Reset Handler> 
 * @detail:     <Reset Handler> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <None>
 */
/******************************************************************************/
__asm void Hal_IsrReset(void)
{
}

/******************************************************************************/
/*
 * @brief:      <Reset Handler> 
 * @detail:     <Reset Handler> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <None>
 */
/******************************************************************************/
void Hal_IsrUnInts(void)
{
    while(1);
}

/******************************************************************************/
/*
 * @brief:      <SWI Handler> 
 * @detail:     <SWI Handler> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <None>
 */
/******************************************************************************/
void Hal_IsrSwi(void)
{
    Hal_SWIContextSave();
    Hal_SWIContextResume();
}

/******************************************************************************/
/*
 * @brief:      <Prefect About Handler> 
 * @detail:     <Prefect About Handler> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <None>
 */
/******************************************************************************/
void Hal_IsrPrefAbt(void)
{
    while(1);
}

/******************************************************************************/
/*
 * @brief:      <Data About Handler> 
 * @detail:     <Data About Handler> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <None>
 */
/******************************************************************************/
void Hal_IsrDataAbt(void)
{
    while(1);
}

/******************************************************************************/
/*
 * @brief:      <Data About Handler> 
 * @detail:     <Data About Handler> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <None>
 */
/******************************************************************************/
void Hal_IsrIrq(void)
{
    Hal_IntContextSave();
    //Hal_IrqHandler();
    Hal_IntContextResume();
}

#if 0
void Hal_IrqHandler(void)
{
    uint8 irq;
    RegsType regPsr;
    RegsType regMask;

    uOS_IntEntry();
    irq = Hal_GetIrqNum();

    regMask = Hal_SuspendLowIrq(irq);
    Hal_EnterCritical();
    /* sIrqHandlerTable[irq](); */
    Hal_ExitCritical();
    uOS_IntExit();
    Hal_ResumeLowIrq(regMask);
}
#endif

/******************************************************************************/
/*
 * @brief:      <Data About Handler> 
 * @detail:     <Data About Handler> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <None>
 */
/******************************************************************************/
void Hal_IsrFiq(void)
{
    while(1);
}
