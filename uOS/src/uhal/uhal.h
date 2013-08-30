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
#ifndef UHAL_H
#define UHAL_H
#ifdef CPLUSPLUS
#define extern "C" {
#endif 

/*==============================[include]=====================================*/
#include "compiler.h"
#include "uos_cfg.h"

/*==============================[typedef]=====================================*/
typedef uint32 RegsType;

typedef struct
{
    RegsType  R[11];    /* R2-R12 */
    RegsType  SP;       /* R13 */
    RegsType  LR;       /* R14 */
    RegsType  CPSR;     
} Hal_TaskContext;

typedef void (*Hal_ISRHandler)(void);

/******************************************************************************/
/*
 * @brief:      <initalize task context> 
 * @detail:     <initalize task context> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <None>
 */
/******************************************************************************/
#define Hal_TaskContextInit(pTaskContext, sp, pc) \
    do{                                           \
        (pTaskContext)->SP    = sp;               \
        (pTaskContext)->LR    = pc;               \
        (pTaskContext)->CPSR  = 0x1F;             \
    }while(0)

/******************************************************************************/
/*
 * @brief:      <enter accessing critical resource> 
 * @detail:     <enter accessing critical resource> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <None>
 */
/******************************************************************************/
#define Hal_EnterCritical()      \
    do{                          \
        int temp;                \
        __asm{                   \
            mrs temp, cpsr       \
        }                        \
        regPsr = temp;           \
        temp = temp | 0xc0;      \
        __asm{                   \
            msr cpsr_c, temp;    \
        }                        \
    }while(0)

/******************************************************************************/
/*
 * @brief:      <exit accessing critical resource> 
 * @detail:     <exit accessing critical resource> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <None>
 */
/******************************************************************************/
#define Hal_ExitCritical()      \
    do{                         \
        int temp = regPsr;      \
        __asm{                  \
            msr cpsr_c, temp    \
        }                       \
    }while(0)

/******************************************************************************/
/*
 * @brief:      <switch task context> 
 * @detail:     <switch task context> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <pDestContext: destination task context> 
 * @param[out]: <pSrcContext: source task context> 
 * @return:     <None>
 */
/******************************************************************************/
EXTERN void Hal_ContextSwitch(Hal_TaskContext *pSrcContext, Hal_TaskContext *pDestContext);

/******************************************************************************/
/*
 * @brief:      <int context save> 
 * @detail:     <int context save> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <None>
 */
/******************************************************************************/
EXTERN void Hal_IntContextSave(void);

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
EXTERN void Hal_IntContextResume(void);

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
EXTERN void Hal_SWIContextSave(void);

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
EXTERN void Hal_SWIContextResume(void);

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
EXTERN uint8 Hal_GetVectNum(void);

#ifdef CPLUSPLUS
}
#endif /* CPLUSPLUS */
#endif /* UHAL_H */
