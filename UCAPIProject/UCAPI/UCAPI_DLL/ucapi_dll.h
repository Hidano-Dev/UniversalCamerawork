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

    /// <summary>
    /// Serializes the given UCAPI structure into a newly allocated byte buffer.
    /// The caller must free the buffer using UCAPI_FreeBuffer.
    /// </summary>
    /// <param name="ucapi">Pointer to the UCAPI structure to serialize.</param>
    /// <param name="outBuffer">Pointer to the output byte buffer pointer.</param>
    /// <param name="outSize">Pointer to the output buffer size.</param>
    /// <returns>0 on success, non-zero on failure.</returns>
    UCAPI_API int UCAPI_Serialize(UCAPI_DllObject* ucapi, uint8_t** outBuffer, size_t* outSize);

    /// <summary>
    /// Frees a byte buffer allocated by UCAPI_Serialize.
    /// </summary>
    /// <param name="buffer">Pointer to the buffer to free.</param>
    UCAPI_API void UCAPI_FreeBuffer(uint8_t* buffer);

    /// <summary>
    /// Deserializes the given byte buffer into a UCAPI object.
    /// The returned handle must be freed using UCAPI_FreeObject.
    /// </summary>
    /// <param name="inBuffer">Pointer to the input byte buffer.</param>
    /// <param name="inSize">Size of the input buffer.</param>
    /// <returns>Pointer to a UCAPI_DllObject on success, or nullptr on failure.</returns>
    UCAPI_API UCAPI_DllObject* UCAPI_Deserialize(const uint8_t* inBuffer, size_t inSize);

    /// <summary>
    /// Frees the UCAPI object created by UCAPI_Deserialize or UCAPI_CreateDefault.
    /// </summary>
    /// <param name="obj">Pointer to the UCAPI_DllObject to free.</param>
    UCAPI_API void UCAPI_FreeObject(UCAPI_DllObject* obj);

    /// <summary>
    /// Creates a new default UCAPI object for serialization.
    /// </summary>
    /// <returns>Pointer to a new UCAPI_DllObject, or nullptr on failure.</returns>
    UCAPI_API UCAPI_DllObject* UCAPI_CreateDefault();

#ifdef __cplusplus
}
#endif

#endif // UCAPI_DLL_H
