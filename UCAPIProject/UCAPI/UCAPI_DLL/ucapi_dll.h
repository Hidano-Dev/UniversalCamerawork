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

// =============================================================================
// C-compatible flat record structure (POD type for interop)
// =============================================================================
#pragma pack(push, 1)
typedef struct ucapi_record_flat_t {
    uint32_t camera_no;
    uint16_t commands;
    uint32_t timecode;
    float subframe;
    uint8_t packet_no;
    float eye_position_right_m;
    float eye_position_up_m;
    float eye_position_forward_m;
    float look_vector_right_m;
    float look_vector_up_m;
    float look_vector_forward_m;
    float up_vector_right_m;
    float up_vector_up_m;
    float up_vector_forward_m;
    float focal_length_mm;
    float aspect_ratio;
    float focus_distance_m;
    float aperture;
    float sensor_size_width_mm;
    float sensor_size_height_mm;
    float near_clip_m;
    float far_clip_m;
    float lens_shift_horizontal_ratio;
    float lens_shift_vertical_ratio;
    float lens_distortion_radial_coefficients_k1;
    float lens_distortion_radial_coefficients_k2;
    float lens_distortion_center_point_right_mm;
    float lens_distortion_center_point_up_mm;
} ucapi_record_flat_t;
#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif

    // =============================================================================
    // C-compatible Serialize/Deserialize API (recommended for interop)
    // =============================================================================

    /// Serialize a flat record to MessagePack format
    /// @param record Pointer to flat record structure
    /// @param outBuffer Output: pointer to allocated buffer (caller must free with UCAPI_FreeBuffer)
    /// @param outSize Output: size of the allocated buffer
    /// @return 0 on success, -1 on failure
    UCAPI_API int UCAPI_SerializeRecord(
        const ucapi_record_flat_t* record,
        uint8_t** outBuffer,
        size_t* outSize
    );

    /// Deserialize MessagePack data to a flat record
    /// @param buffer Input buffer containing MessagePack data
    /// @param bufferSize Size of the input buffer
    /// @param outRecord Output: pointer to flat record structure (caller allocated)
    /// @return 0 on success, -1 on failure
    UCAPI_API int UCAPI_DeserializeRecord(
        const uint8_t* buffer,
        size_t bufferSize,
        ucapi_record_flat_t* outRecord
    );

    // =============================================================================
    // Legacy API (uses C++ class internally - not recommended for interop)
    // =============================================================================

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
