#ifndef UCAPI_DLL_H
#define UCAPI_DLL_H

#ifdef _WIN32
#ifdef UCAPI_DLL_EXPORTS
#define UCAPI_API __declspec(dllexport)
#else
#define UCAPI_API __declspec(dllimport)
#endif
#else
#define UCAPI_API
#endif

#include <cstddef>
#include <cstdint>
#include "ucapi.h"

#ifdef __cplusplus
extern "C" {
#endif

    UCAPI_API ucapi_t* UCAPI_Deserialize(const uint8_t* buffer, size_t payloadCount);

    UCAPI_API int UCAPI_Serialize(ucapi_t* obj, uint8_t** outBuffer, size_t* outSize);

    /// <summary>
    /// Frees a byte buffer allocated by UCAPI_Serialize.
    /// </summary>
    /// <param name="buffer">Pointer to the buffer to free.</param>
    UCAPI_API void UCAPI_FreeBuffer(uint8_t* buffer);

#ifdef __cplusplus
}
#endif

#endif // UCAPI_DLL_H
