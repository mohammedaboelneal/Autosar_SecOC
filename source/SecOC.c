/* "Copyright [2022/2023] <Tanta University>" */
#include "SecOC_Lcfg.h"
#include "SecOC_Cfg.h"
#include "SecOC_PBcfg.h"
#include "SecOC_Cbk.h"
#include "ComStack_Types.h"
#include "Rte_SecOC.h"
#include "SecOC.h"
#include "PduR_SecOC.h"
#include "Csm.h"
#include "Rte_SecOC_Type.h"
#include "PduR_Com.h"
#include "PduR_SecOC.h"


#include <string.h>


static PduInfoType SecOC_Buffer[SECOC_BUFFERLENGTH];

#define SECOC_CAN_DATAFRAME_MAX ((uint8)8)
#define SECOC_CAN_DATA_MAX      ((uint16)(SECOC_CAN_DATAFRAME_MAX - (SECOC_AUTH_INFO_TRUNC_LENGTH / 8)))

#define SECOC_SDATA_MAX         ((uint8)4)
#define SECOC_FRESHNESS_MAX     ((uint8)16)
#define SECOC_MACLEN_MAX        ((uint8)32)




extern SecOC_TxAuthenticPduLayerType SecOC_TxAuthenticPduLayer;
/****************************************************
 *          * Function Info *                           *
 *                                                      *
 * Function_Name        : authenticate                  *
 * Function_Index       : SecOC internal                *
 * Parameter in         : TxPduId                       *
 * Function_Descripton  : The lower layer communication * 
 * interface module confirms  the transmission of a PDU *
 *        or the failure to transmit a PDU              *
 ***************************************************/
static Std_ReturnType authenticate(const PduIdType TxPduId, const PduInfoType* AuthPdu, PduInfoType* SecPdu)
{
    Std_ReturnType result;
    Std_ReturnType ret;
    // 1. Prepare Secured I-PDU
    // 2. Construct Data for Authenticator
    uint8 DataToAuth[sizeof(TxPduId) + SECOC_SDATA_MAX + SECOC_FRESHNESS_MAX]; // CAN payload
    uint32 DataToAuthLen = 0;

    // DataToAuthenticator = Data Identifier | secured part of the Authentic I-PDU | Complete Freshness Value

    // Data Identifier
    (void)memcpy(&DataToAuth[DataToAuthLen], &TxPduId, sizeof(TxPduId));
    DataToAuthLen += sizeof(TxPduId);

    // secured part of the Authentic I-PDU
    (void)memcpy(&DataToAuth[DataToAuthLen], AuthPdu->SduDataPtr, AuthPdu->SduLength);
    DataToAuthLen += AuthPdu->SduLength;
    
    // Complete Freshness Value
    uint8 FreshnessVal[SECOC_FRESHNESS_MAX] ={0};
    uint32 FreshnesslenBits = SECOC_FRESHNESS_MAX * 8;

    result = SecOC_GetTxFreshness(TxPduId, FreshnessVal, &FreshnesslenBits);
    if(result != E_OK)
    {
        ret=result;
    }

    uint32 FreshnesslenBytes = BIT_TO_BYTES(FreshnesslenBits);

    (void)memcpy(&DataToAuth[DataToAuthLen], &FreshnessVal[SECOC_FRESHNESS_MAX - FreshnesslenBytes], FreshnesslenBytes);
    DataToAuthLen += FreshnesslenBytes;

    // MAC generation
    uint8  authenticatorPtr[SECOC_MACLEN_MAX];
    uint32  authenticatorLen = SECOC_AUTH_INFO_TRUNC_LENGTH / 8;
    result = Csm_MacGenerate(TxPduId, 0, DataToAuth, DataToAuthLen, authenticatorPtr, &authenticatorLen);

    if(result != E_OK)
    {
        ret=result;
    }
    // Create secured IPDU
    SecPdu->MetaDataPtr = AuthPdu->MetaDataPtr;
    SecPdu->SduLength = SECOC_CAN_DATAFRAME_MAX;

    (void)memcpy(SecPdu->SduDataPtr, AuthPdu->SduDataPtr, SECOC_CAN_DATA_MAX);
    (void)memcpy(&SecPdu->SduDataPtr[SECOC_CAN_DATA_MAX], authenticatorPtr, authenticatorLen);        


   return ret;
}


Std_ReturnType SecOC_IfTransmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr) {
    Std_ReturnType result = E_OK;
    if (TxPduId < SECOC_BUFFERLENGTH) {
        SecOC_Buffer[TxPduId] = *PduInfoPtr;
    } else {
        result = E_NOT_OK;
    }

    return result;
}


void SecOC_TxConfirmation(PduIdType TxPduId, Std_ReturnType result) {
    if (result == E_OK) {
        // clear buffer
        SecOC_Buffer[TxPduId].MetaDataPtr = NULL;
        SecOC_Buffer[TxPduId].SduDataPtr = NULL;
        SecOC_Buffer[TxPduId].SduLength = 0;
    }
    PduR_SecOCIfTxConfirmation(TxPduId, result);
}



Std_ReturnType SecOC_GetTxFreshness(uint16 SecOCFreshnessValueID, uint8* SecOCFreshnessValue,
uint32* SecOCFreshnessValueLength) {
    SecOC_GetTxFreshnessCalloutType PTR = (SecOC_GetTxFreshnessCalloutType)FVM_GetTxFreshness;//why casting
Std_ReturnType result = PTR(SecOCFreshnessValueID, SecOCFreshnessValue, SecOCFreshnessValueLength);//why not &
    return result;
}





SecOC_StateType SecOc_State = SECOC_UNINIT;

const SecOC_GeneralType* tmpSecOCGeneral;
const SecOC_RxPduProcessingType* tmpSecOCRxPduProcessing;
const SecOC_TxPduProcessingType* tmpSecOCTxPduProcessing;

static Std_VersionInfoType _SecOC_VersionInfo ={(uint16)1,(uint16) 1,(uint8)1,(uint8)0,(uint8)0};

// #if (SECOC_ERROR_DETECT == STD_ON)
// #define VALIDATE_STATE_INIT()
// 	if(SECOC_INIT!=SecOc_State){
// 		Det_ReportError(); 
// 		return; 
// 	}

// #else
// #define VALIDATE_STATE_INIT(_api)
// #endif



uint8 success=1;
void SecOC_Init(const SecOC_ConfigType *config)
{
    tmpSecOCGeneral = config->general;
    tmpSecOCTxPduProcessing = config->secOCTxPduProcessings;
    tmpSecOCRxPduProcessing = config->secOCRxPduProcessings;
    if(!success)
    {
        SecOc_State = SECOC_E_UNINIT;        
    }                                   
    SecOc_State = SECOC_INIT;
}                   


// void SecOC_GetVersionInfo (Std_VersionInfoType* versioninfo)
// {

//     // VALIDATE_STATE_INIT();
//     (void)memcpy(versioninfo, &_SecOC_VersionInfo, sizeof(Std_VersionInfoType));//copy from source to distination with the length
// }


extern void SecOC_MainFunctionTx(void) {
    // check if initialized or not;
    if (SecOc_State == SECOC_UNINIT) {
        // cppcheck-suppress misra-c2012-15.5
        return;
    }
    PduIdType idx;
    PduInfoType transmitPduInfo;
    uint8 temp[8];
    transmitPduInfo.SduDataPtr = temp;
    

    
    
    for (idx = 0 ; idx < SECOC_BUFFERLENGTH ; idx++) {
        // check if there is data
        if ( SecOC_Buffer[idx].SduLength > 0 ) {
            authenticate(idx , &SecOC_Buffer[idx] , &transmitPduInfo);
            PduR_SecOCTransmit(idx , &transmitPduInfo);

        }
    }
}

void SecOC_TpTxConfirmation(PduIdType TxPduId,Std_ReturnType result)
{
    if (result == E_OK) {
        // clear buffer
        SecOC_Buffer[TxPduId].MetaDataPtr = NULL;
        SecOC_Buffer[TxPduId].SduDataPtr = NULL;
        SecOC_Buffer[TxPduId].SduLength = 0;
    }

    if (SecOC_TxAuthenticPduLayer.SecOCPduType == SECOC_TPPDU)
    {
        PduR_SecOCTpTxConfirmation(TxPduId, result);
    }
    else if (SecOC_TxAuthenticPduLayer.SecOCPduType == SECOC_IFPDU)
    {
        PduR_SecOCIfTxConfirmation(TxPduId, result);
    }
    else
    {
        // DET Report Error
    }

}

// #if (SECOC_USE_TX_CONFIRMATION == 1)
//     void SecOc_SPduTxConfirmation(uint16 SecOCFreshnessValueID) {
//          Specific User's Code need to be written here
//     }
// #endif

/*
#define MAX_COUNTER_FRESHNESS_IDS   10

Std_ReturnType SecOC_GetTxFreshnessTruncData (uint16 SecOCFreshnessValueID,uint8* SecOCFreshnessValue,
uint32* SecOCFreshnessValueLength,uint8* SecOCTruncatedFreshnessValue,uint32* SecOCTruncatedFreshnessValueLength) 
{
    Std_ReturnType result = E_OK; 
    if (SecOCFreshnessValueID > (MAX_COUNTER_FRESHNESS_IDS-1)) 
    {
        result = E_NOT_OK;
    }
    else if (SecOCTruncatedFreshnessValueLength > SECOC_MAX_FRESHNESS_SIZE) 
    {
        result = E_NOT_OK;
    }
    SecOC_FreshnessArrayType counter[MAX_COUNTER_FRESHNESS_IDS] = {0};
    uint32 Datalength = SECOC_MAX_FRESHNESS_SIZE - (*SecOCTruncatedFreshnessValueLength);
    
    int DataIndex = (SECOC_MAX_FRESHNESS_SIZE - 1); 
    uint8 big_End_index =0;
    for ( ; ((DataIndex >= Datalength) && (big_End_index < (*SecOCTruncatedFreshnessValueLength))); DataIndex-- , big_End_index++) 
    {
        SecOCTruncatedFreshnessValue[big_End_index] = counter[SecOCFreshnessValueID][DataIndex];
    }
    return result;
}
*/

Std_ReturnType SecOC_GetRxFreshness(uint16 SecOCFreshnessValueID,const uint8* SecOCTruncatedFreshnessValue,uint32 SecOCTruncatedFreshnessValueLength,
    uint16 SecOCAuthVerifyAttempts,uint8* SecOCFreshnessValue,uint32* SecOCFreshnessValueLength)
{
    FVM_GetRxFreshness(SecOCFreshnessValueID,SecOCTruncatedFreshnessValue,SecOCTruncatedFreshnessValueLength,
    SecOCAuthVerifyAttempts,SecOCFreshnessValue,SecOCFreshnessValueLength);
}

void SecOC_test()
{
    uint16 SecOCFreshnessValueID=10;
    uint8 SecOCTruncatedFreshnessValue[1]={3};
    uint32 SecOCTruncatedFreshnessValueLength=1;
    uint16 SecOCAuthVerifyAttempts=0;
    uint8 SecOCFreshnessValue[8]={0};
    uint32 SecOCFreshnessValueLength=8;
    FVM_IncreaseCounter(SecOCFreshnessValueID, &SecOCFreshnessValueLength);
    FVM_IncreaseCounter(SecOCFreshnessValueID, &SecOCFreshnessValueLength);
    FVM_IncreaseCounter(SecOCFreshnessValueID, &SecOCFreshnessValueLength);
    FVM_IncreaseCounter(SecOCFreshnessValueID, &SecOCFreshnessValueLength);
    FVM_IncreaseCounter(SecOCFreshnessValueID, &SecOCFreshnessValueLength);
    FVM_IncreaseCounter(SecOCFreshnessValueID, &SecOCFreshnessValueLength);
    FVM_IncreaseCounter(SecOCFreshnessValueID, &SecOCFreshnessValueLength);
    FVM_IncreaseCounter(SecOCFreshnessValueID, &SecOCFreshnessValueLength);
    // FVM_IncreaseCounter(SecOCFreshnessValueID, &SecOCFreshnessValueLength);
    printf("%d \n",SecOC_GetRxFreshness(SecOCFreshnessValueID,SecOCTruncatedFreshnessValue,SecOCTruncatedFreshnessValueLength,
    SecOCAuthVerifyAttempts,SecOCFreshnessValue,&SecOCFreshnessValueLength));
    for(uint8 i=0;i<SecOCFreshnessValueLength;i++)
    {
        printf("%d \t",SecOCFreshnessValue[i]);
    }
    printf("\n%d \t",SecOCFreshnessValueLength);
}