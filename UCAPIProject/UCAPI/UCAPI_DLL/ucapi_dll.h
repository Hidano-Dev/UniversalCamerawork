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

#ifdef __cplusplus
extern "C" {
#endif

    /// <summary>
    /// Opaque handle for a UCAPI object including its deserialized data and associated stream.
    /// </summary>
    typedef struct UCAPI_DllObject UCAPI_DllObject;

    UCAPI_API UCAPI_DllObject* UCAPI_DeserializeMessagePack(const uint8_t* buffer, size_t size);

    UCAPI_API int UCAPI_SerializeMessagePack(UCAPI_DllObject* obj, uint8_t** outBuffer, size_t* outSize);
    
    UCAPI_API uint8_t* UCAPI_EncodeToBinary(UCAPI_DllObject* obj, size_t* outSize);
    
    UCAPI_API UCAPI_DllObject* UCAPI_DecodeFromBinary(const uint8_t* data, size_t size);

    /// <summary>
    /// Frees a byte buffer allocated by UCAPI_Serialize.
    /// </summary>
    /// <param name="buffer">Pointer to the buffer to free.</param>
    UCAPI_API void UCAPI_FreeBuffer(uint8_t* buffer);

    /// <summary>
    /// Frees the UCAPI object created by UCAPI_Deserialize or UCAPI_CreateDefault.
    /// </summary>
    /// <param name="obj">Pointer to the UCAPI_DllObject to free.</param>
    UCAPI_API void UCAPI_FreeObject(UCAPI_DllObject* obj);

#ifdef __cplusplus
}
#endif

#endif // UCAPI_DLL_H
