/*============================================================================*/
/*
 * @brief:   condition variable module source code
 *
 * @author:  bo.zeng
 *
 * @data:    2013.8.11
 *
 */
/*============================================================================*/

/*==============================[include]=====================================*/
#include "condition.h"
#include "dlist.h"
#include "task.h"
#include "event.h"
#include "uhal.h"

#if (TRUE == CFG_CV_SUPPORT)
/*=============================[type define]==================================*/
typedef struct
{
    boolean condVarPass;
    struct dlist   waitList;
} CVControlBlock;

/*=============================[extern data]==================================*/
EXTERN CONST CVMgrCfgType gCVMgrCfg;

/*=============================[marco define]=================================*/
#if (CFG_VARIANT_TYPE == CFG_VARIANT_PC)
#define CV_MGR_CFG    gCVMgrCfg
#define CV_NUM_CFG    (CFG_CV_MAX_NUM)
#elif (CFG_VARIANT_TYPE == CFG_VARIANT_LT)
#define CV_MGR_CFG    gCVMgrCfg
#define CV_NUM_CFG    (CFG_CV_MAX_NUM)
#else
#define CV_MGR_CFG    gCVMgrCfg
#define CV_NUM_CFG    (CFG_CV_MAX_NUM)
#endif

/*=============================[internal data]================================*/
STATIC CVControlBlock sCVCBTable[CFG_CV_MAX_NUM];

/*=============================[internal function]============================*/

/*===========================[function define]================================*/
/******************************************************************************/
/*
 * @brief:      <initalize condition variable module> 
 * @detail:     <initalize condition variable module> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
OS_Ret uOS_CVInit(void)
{
    CVIdType cvId;

    for (cvId = 0; cvId < CV_NUM_CFG; cvId++)
    {
        sCVCBTable[cvId].condVarPass = FALSE;
    }

    return OS_E_OK;
}

/******************************************************************************/
/*
 * @brief:      <condition variable Pend> 
 * @detail:     <condition variable Pend> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
OS_Ret uOS_CVPend(CVIdType cvId, TickType timeout)
{
    RegsType regPsr;
    OS_Ret   ret = OS_E_NOT_OK;
    TaskControlBlock *pTCB = NULL;

#if (TRUE == CFG_DET_ERROR_DECTECT)    
    if (cvId >= CV_NUM_CFG)
    {
        return OS_E_NOT_OK;
    }
#endif

    Hal_EnterCritical();
    if (TRUE == sCVCBTable[cvId].condVarPass)
    {
        Hal_ExitCritical();
        return OS_E_OK;
    }
    Hal_ExitCritical();

    pTCB = uOS_TaskReadyListDel(uOS_CurTaskId());
    dlist_append(&sCVCBTable[cvId].waitList, &pTCB->dNode);
    uOS_TaskSched();

    Hal_EnterCritical();
    if (TRUE == sCVCBTable[cvId].condVarPass)
    {
        ret = OS_E_OK;
    }
    else
    {
        ret = OS_E_NOT_OK;
    }
    Hal_ExitCritical();

    return ret;
}

/******************************************************************************/
/*
 * @brief:      <condition variable Post> 
 * @detail:     <condition variable Post> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
OS_Ret uOS_CVPost(CVIdType cvId)
{
    RegsType regPsr;
    TaskControlBlock *pTCB = NULL;

#if (TRUE == CFG_DET_ERROR_DECTECT)    
    if (cvId >= CV_NUM_CFG)
    {
        return OS_E_NOT_OK;
    }
#endif

    Hal_EnterCritical();
    sCVCBTable[cvId].condVarPass = TRUE;
    Hal_ExitCritical();

    while (1 != dlist_empty(&sCVCBTable[cvId].waitList))
    {
        pTCB = (TaskControlBlock *)dlist_first(&sCVCBTable[cvId].waitList);        
        dlist_del(&sCVCBTable[cvId].waitList, &pTCB->dNode);

        uOS_TaskReadyListAdd(pTCB);
    }
    uOS_TaskSched();

    return OS_E_OK;
}

#endif /* TRUE == CFG_CV_SUPPORT */
