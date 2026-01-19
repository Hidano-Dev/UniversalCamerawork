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

    UCAPI_API uint16_t UCAPI_CalcCRC16(const void* recordPtr, size_t length, uint16_t poly, uint16_t initValue);

    // Create and destroy API object
    UCAPI_API ucapi_t* UCAPI_Create();
    UCAPI_API void UCAPI_Destroy(ucapi_t* obj);

    // Add an empty record to the payload
    UCAPI_API int UCAPI_AddRecord(ucapi_t* obj);

    // === Logger API ===

    /// Get the number of log entries currently stored
    /// @return Number of log entries (0 to 256)
    UCAPI_API size_t UCAPI_GetLogCount();

    /// Get a log entry by index
    /// @param index Log entry index (0 = oldest)
    /// @param outLevel Output: log level (0=None, 1=Error, 2=Warning, 3=Info, 4=Debug)
    /// @param outTimestamp Output: timestamp in milliseconds
    /// @param outFunction Output: function name buffer (caller allocated)
    /// @param functionBufSize Size of outFunction buffer
    /// @param outMessage Output: message buffer (caller allocated)
    /// @param messageBufSize Size of outMessage buffer
    /// @return 0 on success, -1 if index out of range, -2 if buffer too small
    UCAPI_API int UCAPI_GetLog(
        size_t index,
        uint8_t* outLevel,
        uint64_t* outTimestamp,
        char* outFunction,
        size_t functionBufSize,
        char* outMessage,
        size_t messageBufSize
    );

    /// Get a log entry message only (simplified API)
    /// @param index Log entry index (0 = oldest)
    /// @param outBuffer Output: message buffer including level prefix "[ERROR] ..."
    /// @param bufferSize Size of outBuffer
    /// @return 0 on success, -1 if index out of range, -2 if buffer too small
    UCAPI_API int UCAPI_GetLogMessage(
        size_t index,
        char* outBuffer,
        size_t bufferSize
    );

    /// Clear all log entries
    UCAPI_API void UCAPI_ClearLogs();

    /// Set the minimum log level to record
    /// @param level Log level (0=None, 1=Error, 2=Warning, 3=Info, 4=Debug)
    UCAPI_API void UCAPI_SetLogLevel(uint8_t level);

    /// Get the current log level
    /// @return Current log level
    UCAPI_API uint8_t UCAPI_GetLogLevel();

#ifdef __cplusplus
}
#endif

#endif // UCAPI_DLL_H
