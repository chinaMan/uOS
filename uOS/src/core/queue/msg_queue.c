/*============================================================================*/
/*
 * @brief:   message queue module source code
 *
 * @author:  bo.zeng
 *
 * @data:    2013.9.12
 *
 */
/*============================================================================*/

/*==============================[include]=====================================*/
#include "msg_queue.h"
#include "queue.h"
#include "task.h"
#include "event.h"
#include "uhal.h"

#if (TRUE == CFG_MSGQ_SUPPORT)
/*=============================[type define]==================================*/
typedef struct
{
    struct queue sQueue;
} MsgQControlBlock;

/*=============================[extern data]==================================*/
EXTERN CONST MsgQMgrCfgType gMsgQMgrCfg;

/*=============================[marco define]=================================*/
#if (CFG_VARIANT_TYPE == CFG_VARIANT_PC)
#define MSGQ_MGR_CFG    gMsgQMgrCfg
#define MSGQ_NUM_CFG    (CFG_MSGQ_MAX_NUM)
#elif (CFG_VARIANT_TYPE == CFG_VARIANT_LT)
#define MSGQ_MGR_CFG    gMsgQMgrCfg
#define MSGQ_NUM_CFG    (CFG_MSGQ_MAX_NUM)
#else
#define MSGQ_MGR_CFG    gMsgQMgrCfg
#define MSGQ_NUM_CFG    (CFG_MSGQ_MAX_NUM)
#endif

/*=============================[internal data]================================*/
STATIC MsgQControlBlock sMsgQCBTable[CFG_MSGQ_MAX_NUM];

/*=============================[internal function]============================*/

/*===========================[function define]================================*/
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
OS_Ret uOS_MsgQInit(void)
{
    MsgQIdType msgQId;

    for (msgQId = 0; msgQId < MSGQ_NUM_CFG; msgQId++)
    {
        queue_init(&sMsgQCBTable[msgQId].sQueue, 
                MSGQ_MGR_CFG.pMsgQTableCfg[msgQId].msgStorage,
                MSGQ_MGR_CFG.pMsgQTableCfg[msgQId].size);
    }

    return OS_E_OK;
}

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
void *uOS_MsgQPend(MsgQIdType msgQId, TickType timeout)
{
    RegsType regPsr;
    void *msg = NULL;

#if (TRUE == CFG_DET_ERROR_DECTECT)    
    if (msgQId >= MSGQ_NUM_CFG)
    {
        return OS_E_NOT_OK;
    }
#endif

    Hal_EnterCritical();
    if (1 != queue_empty(&sMsgQCBTable[msgQId].sQueue))
    {
        msg = queue_de(&sMsgQCBTable[msgQId].sQueue);
        Hal_ExitCritical();
        return msg;
    }
    Hal_ExitCritical();

    uOS_EventPend(MSGQ_MGR_CFG.pMsgQTableCfg[msgQId].eventId, timeout);
    uOS_TaskSched();

    Hal_EnterCritical();
    if (1 != queue_empty(&sMsgQCBTable[msgQId].sQueue))
    {
        msg = queue_de(&sMsgQCBTable[msgQId].sQueue);
    }
    Hal_ExitCritical();

    return msg;
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
OS_Ret uOS_MsgQPost(MsgQIdType msgQId, void *msg)
{
    RegsType regPsr;

#if (TRUE == CFG_DET_ERROR_DECTECT)    
    if (msgQId >= MSGQ_NUM_CFG)
    {
        return OS_E_NOT_OK;
    }
    else if (NULL == msg)
    {
        return OS_E_NOT_OK;
    }
#endif

    Hal_EnterCritical();
    if (1 == queue_full(&sMsgQCBTable[msgQId].sQueue))
    {
        Hal_ExitCritical();
        return OS_E_NOT_OK;    
    }

    queue_en(&sMsgQCBTable[msgQId].sQueue, msg);
    Hal_ExitCritical();

    uOS_EventPost(MSGQ_MGR_CFG.pMsgQTableCfg[msgQId].eventId);

    uOS_TaskSched();

    return OS_E_OK;
}

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
OS_Ret uOS_MsgQPostFront(MsgQIdType msgQId, void *msg)
{
    RegsType regPsr;

#if (TRUE == CFG_DET_ERROR_DECTECT)    
    if (msgQId >= MSGQ_NUM_CFG)
    {
        return OS_E_NOT_OK;
    }
    else if (NULL == msg)
    {
        return OS_E_NOT_OK;
    }
#endif

    Hal_EnterCritical();
    if (1 == queue_full(&sMsgQCBTable[msgQId].sQueue))
    {
        Hal_ExitCritical();
        return OS_E_NOT_OK;    
    }

    queue_en_front(&sMsgQCBTable[msgQId].sQueue, msg);
    Hal_ExitCritical();

    uOS_EventPost(MSGQ_MGR_CFG.pMsgQTableCfg[msgQId].eventId);

    uOS_TaskSched();

    return OS_E_OK;
}

#endif /* TRUE == CFG_MSGQ_SUPPORT */
