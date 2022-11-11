/******************************************************************************
 * File Name   : SecOC_Lcfg                                                    *
 * Description : Header file of SecOC configuration                           *
 * Created on  : November 9, 2022                                                *
 ******************************************************************************/
#include "SecOC_Cfg.h"
#include "SecOC_Lcfg.h"

SecOC_TxAuthenticPduLayerType SecOC_TxAuthenticPduLayer=
{
    SECOC_PDU_TYPE,
    SECOC_TX_AUTHENTIC_LAYER_PDUID,
    SECOC_TX_AUTHENTIC_LAYER_PDUREF,
};

SecOC_TxSecuredPduType SecOC_TxSecuredPdu=
{
    SECOC_AUTH_PDUHEADER_LENGTH,
    SECOC_TX_SECURED_LAYER_PDUID,
    &EcuC_Pdu,
};

















































































// SecOC_TxAuthenticPduLayerType SecOC_TxAuthenticPduLayer = 
// {
//     SECOC_PDU_TYPE,
//     SECOC_TX_AUTHENTIC_LAYER_PDU_ID,
// }

SecOC_TxPduProcessingType SecOC_TxPduProcessing = {
    SECOC_AUTHENTICATION_BUILD_ATTEMPTS,
    SECOC_AUTH_INFO_TRUNC_LENGTH,
    SECOC_DATA_ID,
    SECOC_FRESHNESS_VALUE_ID,
    SECOC_FRESHNESS_VALUE_LENGTH,
    SECOC_FRESHNESS_VALUE_TRUNC_LENGTH,
    SECOC_PROVIDE_TX_TRUNCATED_FRESHNESS_VALUE,
    SECOC_RE_AUTHENTICATE_AFTER_TRIGGER_TRANSMIT,
    SECOC_TX_PDU_UNUSED_AREAS_DEFAULT,
    SECOC_USE_TX_CONFIRMATION,
    //                                                  SecOCSameBufferPduRef;
    //                                                  SecOCTxAuthServiceConfigRef
    //                                                  SecOCTxPduMainFunctionRef;

    SECOC_TX_SECURED_LAYER_PDUREF,
};

SecOC_UseMessageLinkType SecOC_UseMessageLink=
{
    SECOC_MESSAGE_LINKLEN,
    SECOC_MESSAGE_LINKPOS,
};

SecOC_TxCryptographicPduType SecOC_TxCryptographicPdu=
{
    SECOC_TX_CRYPTOGRAPHIC_PDUID,
    SECOC_TX_CRYPTOGRAPHIC_PDUREF,
};

SecOC_TxAuthenticPduType SecOC_TxAuthenticPdu=
{
    SECOC_AUTH_PDUHEADER_LENGTH,
    SECOC_TX_AUTHENTIC_PDUID,
    SECOC_TX_AUTHENTIC_PDUREF,
};

SecOC_TxSecuredPduCollectionType SecOC_TxSecuredPduCollection=
{
    &SecOC_TxAuthenticPdu,
    &SecOC_TxCryptographicPdu,
    &SecOC_UseMessageLink,
};

SecOC_TxPduSecuredAreaType SecOC_TxPduSecuredArea=
{
    SECOC_SECURED_TX_PDULENGTH,
    SECOC_SECURED_TX_PDUOFFSET,
};


