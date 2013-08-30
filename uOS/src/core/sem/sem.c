/*============================================================================*/
/*
 * @brief:   sempaphore module source code
 *
 * @author:  bo.zeng
 *
 * @data:    2013.8.11
 *
 */
/*============================================================================*/

/*==============================[include]=====================================*/
#include "sem.h"
#include "task.h"
#include "event.h"
#include "uhal.h"

#if (TRUE == CFG_SEM_SUPPORT)
/*=============================[type define]==================================*/
typedef struct
{
    uint16 value;
} SemControlBlock;

/*=============================[extern data]==================================*/
EXTERN CONST SemMgrCfgType gSemMgrCfg;

/*=============================[marco define]=================================*/
#if (CFG_VARIANT_TYPE == CFG_VARIANT_PC)
#define SEM_MGR_CFG    gSemMgrCfg
#define SEM_NUM_CFG    (CFG_SEM_MAX_NUM)
#elif (CFG_VARIANT_TYPE == CFG_VARIANT_LT)
#define SEM_MGR_CFG    gSemMgrCfg
#define SEM_NUM_CFG    (CFG_SEM_MAX_NUM)
#else
#define SEM_MGR_CFG    gSemMgrCfg
#define SEM_NUM_CFG    (CFG_SEM_MAX_NUM)
#endif

/*=============================[internal data]================================*/
STATIC SemControlBlock sSemCBTable[CFG_SEM_MAX_NUM];

/*=============================[internal function]============================*/

/*===========================[function define]================================*/
/******************************************************************************/
/*
 * @brief:      <initalize sem module> 
 * @detail:     <initalize sem module> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
OS_Ret uOS_SemInit(void)
{
    SemIdType semId;

    for (semId = 0; semId < SEM_NUM_CFG; semId++)
    {
        sSemCBTable[semId].value 
            = SEM_MGR_CFG.pSemTableCfg[semId].initValue;
    }

    return OS_E_OK;
}

/******************************************************************************/
/*
 * @brief:      <Sem Pend> 
 * @detail:     <Sem Pend> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
OS_Ret uOS_SemPend(SemIdType semId, TickType timeout)
{
    RegsType regPsr;

#if (TRUE == CFG_DET_ERROR_DECTECT)    
    if (semId >= SEM_NUM_CFG)
    {
        return OS_E_NOT_OK;
    }
#endif

    Hal_EnterCritical();
    if (sSemCBTable[semId].value > 0)
    {
        sSemCBTable[semId].value--;
        Hal_ExitCritical();
        uOS_TaskPriInherit(uOS_CurTaskId(), SEM_MGR_CFG.pSemTableCfg[semId].pri);
        return OS_E_OK;
    }
    Hal_ExitCritical();

    uOS_EventPend(SEM_MGR_CFG.pSemTableCfg[semId].eventId, timeout);
    uOS_TaskSched();

    Hal_EnterCritical();
    if (sSemCBTable[semId].value > 0)
    {
        sSemCBTable[semId].value--;
        Hal_ExitCritical();
        uOS_TaskPriInherit(uOS_CurTaskId(), SEM_MGR_CFG.pSemTableCfg[semId].pri);
        return OS_E_OK;
    }
    else
    {
        Hal_ExitCritical();
        return OS_E_NOT_OK;
    }

    return OS_E_OK;
}

/******************************************************************************/
/*
 * @brief:      <Sem Post> 
 * @detail:     <Sem Post> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
OS_Ret uOS_SemPost(SemIdType semId)
{
    RegsType regPsr;
    OS_Ret ret = OS_E_NOT_OK;

#if (TRUE == CFG_DET_ERROR_DECTECT)    
    if (semId >= SEM_NUM_CFG)
    {
        return OS_E_NOT_OK;
    }
#endif

    Hal_EnterCritical();
    if (sSemCBTable[semId].value >= SEM_MGR_CFG.pSemTableCfg[semId].cnt)
    {
        Hal_ExitCritical();
        return OS_E_NOT_OK;
    }

    sSemCBTable[semId].value++;
    Hal_ExitCritical();

    uOS_EventPost(SEM_MGR_CFG.pSemTableCfg[semId].eventId);
    uOS_TaskPriRecover(uOS_CurTaskId());
    uOS_TaskSched();

    return OS_E_OK;
}

#endif /* TRUE == CFG_SEM_SUPPORT */
