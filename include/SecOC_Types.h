#ifndef INCLUDE_SECOC_TYPES_H_
#define INCLUDE_SECOC_TYPES_H_

#include "ComStack_Types.h"





/*
* Start of RxPduProcessing
*/

typedef enum
{
	BOTH = 0,
	FAILURE_ONLY,
	NONE
}SecOC_ClientServerVerificationStatusPropagationMode_Type;

typedef enum
{
	QUEUE = 0,
	REJECT,
	REPLACE
}SecOC_ReceptionOverflowStrategy_Type;

typedef enum
{
	BOTH = 0,
	FAILURE_ONLY,
	NONE
}SecOC_VerificationStatusPropagationMode_Type;

typedef enum
{
	SECOC_IFPDU = 0,
	SECOC_TPPDU
}SecOC_Pdu_Type;

typedef struct
{
	uint32 CsmJobId;
}Csm_JobType;



/*
* End of RxPduProcessing
*/


/*
* Start of TxPduProcessing
*/





typedef struct EcuC_PduType EcuC_PduType;

typedef enum
{
    SECOC_IFPDU,    /* SECOC_IFPDU Interface communication API */ 
    SECOC_TPPDU,    /* SECOC_TPPDU Transport Protocol communication API */ 
} SecOC_PduType_Type;


/*
* End of TxPduProcessing
*/

#endif

