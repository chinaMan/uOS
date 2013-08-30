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

typedef struct
{
    RegsType  TCFG0;
    RegsType  TCFG1;
    RegsType  TCON;
    RegsType  TCNTB0;
    RegsType  TCMPB0;
    RegsType  TCNTO0;
    RegsType  TCNTB1;
    RegsType  TCMPB1;
    RegsType  TCNTO1;
    RegsType  TCNTB2;
    RegsType  TCMPB2;
    RegsType  TCNTO2;
    RegsType  TCNTB3;
    RegsType  TCMPB3;
    RegsType  TCNTO3;
    RegsType  TCNTB4;
    RegsType  TCNTO4;
}Hal_TimerRegs;
#define REGS_TIMER ((VOLIATE Hal_TimerRegs *)0x51000000)

typedef struct
{
    RegsType  SRCPND;
    RegsType  INTMOD;
    RegsType  INTMSK;
    RegsType  PRIORITY;
    RegsType  INTPND;
    RegsType  INTOFFSET;
    RegsType  SUBSRCPND;
    RegsType  INTSUBMSK;
}Hal_IntRegs;
#define REGS_INT  ((VOLIATE Hal_IntRegs *)0x4A000000)

/******************************************************************************/
/*
 * @brief:      <save task context> 
 * @detail:     <save task context> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <pTaskContext: task context> 
 * @return:     <0: save ok, 1: switch back>
 */
/******************************************************************************/
__asm void Hal_ContextSwitch(Hal_TaskContext *pSrcContext, Hal_TaskContext *pDestContext)
{
    /* save source context */
    STMIA R0, {R2-R12, SP, LR}
    MRS R2,  CPSR
    STR R2,  [R0, #13*4]

    /* resume destination context */
    LDMIA R1, {R2-R12, SP, LR}
    LDR R2,  [R1, #13*4]
    MSR CPSR_cxsf, R2 
    LDR R2,  [R1]

    BX  LR
}

/******************************************************************************/
/*
 * @brief:      <int context save> 
 * @detail:     <IRQ Mode  -->  SYS Mode> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <None>
 */
/******************************************************************************/
__asm void Hal_IntContextSave(void)
{
    MSR CPSR_c, #0xDF        /* entry system mode */
    STMFD SP!, {R0-R12, LR}  /* save R0-R12, LR */

    MSR CPSR_c, #0xD2        /* entry irq mode */
    SUBS R0, LR, #4
    MRS  R1, SPSR

    MSR CPSR_c, #0xDF        /* entry system mode */
    STMFD SP!, {R0-R1}       /* save PC, CPSR */
}

/******************************************************************************/
/*
 * @brief:      <int context resume> 
 * @detail:     <int context resume> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <pDestContext: destination task context> 
 * @param[out]: <pSrcContext: source task context> 
 * @return:     <None>
 */
/******************************************************************************/
__asm void Hal_IntContextResume(void)
{
    LDMFD SP!, {R0}
    MSR CPSR_cxsf, R0

    LDMFD SP!, {R0-R12, LR, PC}^
}

/******************************************************************************/
/*
 * @brief:      <swi context save> 
 * @detail:     <SVC Mode  -->  SYS Mode  -->  SVC Mode> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <None>
 */
/******************************************************************************/
__asm void Hal_SWIContextSave(void)
{
    STMFD SP!, {R0-R12}
}

/******************************************************************************/
/*
 * @brief:      <int context resume> 
 * @detail:     <int context resume> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <pDestContext: destination task context> 
 * @param[out]: <pSrcContext: source task context> 
 * @return:     <None>
 */
/******************************************************************************/
__asm void Hal_SWIContextResume(void)
{
    //LDMFD SP!, {R0-R12}^
}

/******************************************************************************/
/*
 * @brief:      <get vector num> 
 * @detail:     <get vector num> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <None>
 */
/******************************************************************************/
uint8 Hal_GetVectNum(void)
{
    return (REGS_INT->INTOFFSET);
}
