#include "Csm.h"

#include <string.h>

#define MAC_DATA_LEN ((uint8)100) // Maximum number of stored MACs
#define MAC_LEN      ((uint8)16)      // Length of MAC

static uint8 MacData[MAC_DATA_LEN][MAC_LEN]; // An array to store generated macs, uses jobId as an index




// A stub function to generate a MAC for the authentic-PDU
extern Std_ReturnType Csm_MacGenerate ( 
    uint32 jobId, 
    Crypto_OperationModeType mode,
    const uint8* dataPtr,
    uint32 dataLength,
    uint8* macPtr,
    uint32* macLengthPtr )
    {
        // Generate mac - To be done

        //******starts mac generation*********
        // Right now, I'm just using the data as the mac
        uint32 datalen = (dataLength > 16) ? 16 : dataLength;

        memcpy(MacData[jobId], dataPtr, datalen);
        //******end mac generation*********

        // the Authenticator should only be truncated down to the most significant (Shift the start)
        sint32 macDiff = MAC_LEN - (*macLengthPtr);
        uint8 macStart = (macDiff < 0) ? 0 : macDiff;

        // Update macLength
        (*macLengthPtr) = MAC_LEN - macStart;
        // Copy generated MAC to the required destination
        memcpy(macPtr, &MacData[jobId][macStart], *macLengthPtr);


    }

Std_ReturnType Csm_MacVerify(uint32 jobId, Crypto_OperationModeType mode, const uint8* dataPtr, uint32 dataLength,
const uint8* macPtr, const uint32 macLength, Crypto_VerifyResultType* verifyPtr)
{
    if (dataLength != macLength) 
    {
        return CRYPTO_E_KEY_SIZE_MISMATCH;
    } 
    else if (dataLength == macLength) 
    {
        if ((strcmp(dataPtr, macPtr)) == 0) 
        {
            return E_OK;
        }
            else 
        {
            return E_NOT_OK;
        }
    } 
    else 
    {
        return E_NOT_OK;
    }
}
