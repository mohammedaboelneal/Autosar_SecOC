#include "CanIF.h"
// #include "OSconfig.h"

#ifdef LINUX
#include "ethernet.h"
#endif
#include "PduR_CanIf.h"

/****************************************************
 *          * Function Info *                       *
 *                                                  *
 * Function_Name        : CanIf_Transmit            *
 * Function_Index       : 8.3.6 [SWS_CANIF_00005]   *
 * Function_File        : SWS of CAN Interface      *
 * Function_Descripton  : Requests transmission     *
 *              of a PDU                            *
 ***************************************************/
PduInfoType* CanIF_Buffer[CANIF_BUFFERLENGTH];
Std_ReturnType CanIf_Transmit(PduIdType TxPduId,const PduInfoType* PduInfoPtr)
{
    // CanIF_Buffer[TxPduId] = PduInfoPtr; //copy_buffer
    Std_ReturnType result = E_OK;

    
    #ifdef LINUX
    result = ethernet_send(TxPduId, PduInfoPtr->SduDataPtr , PduInfoPtr->SduLength);
    #endif

    PduR_CanIfTxConfirmation(TxPduId , result);
    return result;
}