#include "Csm.h"

#include <string.h>

#define MAC_DATA_LEN 100 // Maximum number of stored MACs
#define MAC_LEN 16      // Length of MAC

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
        uint32 datalen = (dataLength <= 16) ? 16 : dataLength;

        // cppcheck-suppress misra-c2012-17.7
        memcpy(MacData[jobId], dataPtr, datalen);
        //******end mac generation*********

        // the Authenticator should only be truncated down to the most significant (Shift the start)
        uint32 macDiff = MAC_LEN - (*macLengthPtr);
        uint8 macStart = (macDiff < 0) ? 0 : macDiff;

        // Update macLength
        (*macLengthPtr) = MAC_LEN - macStart;
        
        // Copy generated MAC to the required destination
        // cppcheck-suppress misra-c2012-17.7
        memcpy(macPtr, MacData[jobId][macStart], macLengthPtr);


    }