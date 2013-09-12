/*============================================================================*/
/*
 * @brief:   FLAG module source code
 *
 * @author:  bo.zeng
 *
 * @data:    2013.8.11
 *
 */
/*============================================================================*/

/*==============================[include]=====================================*/
#include "flag.h"
#include "task.h"
#include "event.h"
#include "uhal.h"
#include "dlist.h"

#if (TRUE == CFG_FLAG_SUPPORT)
/*=============================[type define]==================================*/
typedef struct
{
    struct dlist_node   dNode;

    FlagType            flag;
    FlagWaitType        waitType;
    TaskControlBlock    *pTCB;
} FlagNode;

typedef struct
{
    FlagType     flags;
    struct dlist waitList;
} FlagGrpControlBlock;

/*=============================[extern data]==================================*/
EXTERN CONST FlagMgrCfgType gFlagMgrCfg;

/*=============================[marco define]=================================*/
#if (CFG_VARIANT_TYPE == CFG_VARIANT_PC)
#define FLAG_MGR_CFG    gFlagMgrCfg
#define FLAG_NUM_CFG    (CFG_FLAG_MAX_NUM)
#elif (CFG_VARIANT_TYPE == CFG_VARIANT_LT)
#define FLAG_MGR_CFG    gFlagMgrCfg
#define FLAG_NUM_CFG    (CFG_FLAG_MAX_NUM)
#else
#define FLAG_MGR_CFG    gFlagMgrCfg
#define FLAG_NUM_CFG    (CFG_FLAG_MAX_NUM)
#endif

/*=============================[internal data]================================*/
STATIC FlagGrpControlBlock sFlagGrpCBTable[CFG_FLAG_MAX_NUM];
STATIC FlagNode sFlagNodes[CFG_FLAG_WAITTASK_MAX_NUM];
STATIC struct dlist sFreeFlagList;

/*=============================[internal function]============================*/
STATIC INLINE boolean Flag_IsFlagGen(FlagIdType flagId, FlagType flag, FlagWaitType waitType);

/*===========================[function define]================================*/
/******************************************************************************/
/*
 * @brief:      <initalize Flag module> 
 * @detail:     <initalize Flag module> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
OS_Ret uOS_FlagInit(void)
{
    FlagIdType flagGrpId = 0;
    uint8 i = 0;

    /* flags group control block initalize */
    for (flagGrpId = 0; flagGrpId < FLAG_NUM_CFG; flagGrpId++)
    {
        sFlagGrpCBTable[flagGrpId].flags
            = FLAG_MGR_CFG.pFlagTableCfg[flagGrpId].initFlag;
    }

    /* free flag node initalize */
    dlist_init(&sFreeFlagList);
    for (i = 0; i < CFG_FLAG_WAITTASK_MAX_NUM; i++)
    {
        dlist_append(&sFreeFlagList, &sFlagNodes[i].dNode);    
    }

    return OS_E_OK;
}

/******************************************************************************/
/*
 * @brief:      <Flag Pend> 
 * @detail:     <Flag Pend> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
OS_Ret uOS_FlagPend(
        FlagIdType flagId, 
        FlagType flag, 
        FlagWaitType waitType,
        TickType timeout)
{
    RegsType regPsr;
    FlagNode *pFlagNode = NULL;
    boolean blConsume = FALSE;

#if (TRUE == CFG_DET_ERROR_DECTECT)    
    if (flagId >= FLAG_NUM_CFG)
    {
        return OS_E_NOT_OK;
    }
    else if (waitType >= FLAG_WAIT_TYPE_MAX)
    {
        return OS_E_NOT_OK;
    }
    else
    {
        /* do nothing */
    }
#endif

    if (FLAG_CONSUME == (waitType & FLAG_CONSUME))
    {
        waitType &= ~FLAG_CONSUME;
        blConsume = TRUE;
    }

    Hal_EnterCritical();
    if (TRUE == Flag_IsFlagGen(flagId, flag, waitType))
    {
        if (TRUE == blConsume)
        {
            sFlagGrpCBTable[flagId].flags &= ~flag;
        }
        Hal_ExitCritical();
        return OS_E_OK;
    }

    /* block task */
    pFlagNode = (FlagNode *)dlist_first(&sFreeFlagList);
    dlist_del(&sFreeFlagList, &pFlagNode->dNode);

    pFlagNode->flag = flag;
    pFlagNode->waitType = waitType;
    pFlagNode->pTCB = uOS_TaskReadyListDel(uOS_CurTaskId());
    pFlagNode->pTCB->state = TASK_STATE_WAITTING;
    dlist_append(&sFlagGrpCBTable[flagId].waitList, &pFlagNode->dNode);

    Hal_ExitCritical();

    uOS_TaskSched();

    if (blConsume)
    {
        Hal_EnterCritical();
        sFlagGrpCBTable[flagId].flags &= ~flag;
        Hal_ExitCritical();
    }

    return OS_E_OK;
}

/******************************************************************************/
/*
 * @brief:      <Event Post> 
 * @detail:     <Event Post> 
 * @sync/async: <sync>
 * @reetrancy:  <reetrancy>
 * @param[in]:  <None> 
 * @param[out]: <None> 
 * @return:     <os return>
 */
/******************************************************************************/
OS_Ret uOS_FlagPost(FlagIdType flagId, FlagType flag, FlagActType flagAct)
{
    RegsType regPsr;
    FlagNode *pFlagNode = NULL;

#if (TRUE == CFG_DET_ERROR_DECTECT)    
    if (flagId >= FLAG_NUM_CFG)
    {
        return OS_E_NOT_OK;
    }
    else if (FLAG_ACT_TYPE_MAX <= flagAct)
    {
        return OS_E_NOT_OK;
    }
    else
    {
        /* do nothing */
    }
#endif

    Hal_EnterCritical();
    if (FLAG_ACT_TYPE_SET == flagAct)
    {
        sFlagGrpCBTable[flagId].flags |= flag;
    }
    else if (FLAG_ACT_TYPE_CLEAR == flagAct)
    {
        sFlagGrpCBTable[flagId].flags &= ~flag;
    }
    else
    {
        /* do nothing */
    }

    /* check waitting task */
    pFlagNode = (FlagNode *)dlist_first(&sFlagGrpCBTable[flagId].waitList);        
    while (&pFlagNode->dNode != dlist_last(&sFlagGrpCBTable[flagId].waitList))
    {
        if (TRUE == Flag_IsFlagGen(flagId, pFlagNode->flag, pFlagNode->waitType))
        {
            /* add task readylist */
            FlagNode *pTmpFlagCB = NULL;
            pTmpFlagCB = (FlagNode *)dlist_next(&sFlagGrpCBTable[flagId].waitList, &pFlagNode->dNode);
            dlist_del(&sFlagGrpCBTable[flagId].waitList, &pFlagNode->dNode);
            dlist_append(&sFreeFlagList, &pFlagNode->dNode);
            pFlagNode->pTCB->state = TASK_STATE_READY;
            uOS_TaskReadyListAdd(pFlagNode->pTCB);
        }
        else
        {
            pFlagNode = (FlagNode *)dlist_next(&sFlagGrpCBTable[flagId].waitList, &pFlagNode->dNode);        
        }

    }

    if (&pFlagNode->dNode == dlist_last(&sFlagGrpCBTable[flagId].waitList))
    {
        if (TRUE == Flag_IsFlagGen(flagId, pFlagNode->flag, pFlagNode->waitType))
        {
            /* add task readylist */
            dlist_del(&sFlagGrpCBTable[flagId].waitList, &pFlagNode->dNode);
            dlist_append(&sFreeFlagList, &pFlagNode->dNode);
            pFlagNode->pTCB->state = TASK_STATE_READY;
            uOS_TaskReadyListAdd(pFlagNode->pTCB);
        }
    }
    Hal_ExitCritical();

    uOS_TaskSched();

    return OS_E_OK;
}

STATIC INLINE boolean Flag_IsFlagGen(FlagIdType flagId, FlagType flag, FlagWaitType waitType)
{
    boolean blFlagGen = FALSE;

    switch (waitType)
    {
        case FLAG_WAIT_TYPE_ANY:
            if (flag == (sFlagGrpCBTable[flagId].flags & flag))
            {
                blFlagGen = TRUE;
            }
            break;
        case FLAG_WAIT_TYPE_OR:
            if (0 != (sFlagGrpCBTable[flagId].flags & flag))
            {
                blFlagGen = TRUE;
            }
            break;
        defulat:
            break;
    }

    return blFlagGen;
}

#endif /* TRUE == CFG_FLAG_SUPPORT */
