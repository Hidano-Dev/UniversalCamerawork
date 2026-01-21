#include "pch.h"
#include "ucapi_dll.h"
#include <memory>
#include <cstring>
#include <msgpack.hpp>
#include "ucapi.h"
#include "ucapi_config.h"
#include "ucapi_logger.h"

UCAPI_API ucapi_t* UCAPI_Deserialize(const uint8_t* buffer, size_t payloadCount) {
    // Guard against null or empty buffer
    if (buffer == nullptr || payloadCount == 0) {
        return nullptr;
    }
    try {
        auto serializer = ucapi::Config::GetSerializerFactory().CreateSerializer("MsgPack");
        if (!serializer) {
            UCAPI_LOG_ERROR("Failed to create MsgPack serializer");
            return nullptr;
        }
        
        ucapi::CameraState cameraState(sizeof(ucapi::CameraState));
        HRESULT hr = serializer->Deserialize(buffer, payloadCount, cameraState);
        if (FAILED(hr)) {
            UCAPI_LOG_ERROR("Deserialization failed");
            return nullptr;
        }

        auto native = std::make_unique<ucapi_t>();
        native->m_magic = UCAPI_MAGIC;
        native->m_version = 1;
        native->m_num_payload = 1;
        native->m_crc16 = 0;
        native->m_payload.emplace_back(sizeof(ucapi_t::record_t));
        native->m_payload[0] = cameraState;

        return native.release();
    }
    catch (const std::exception& e) {
        UCAPI_LOG_ERROR(e.what());
        return nullptr;
    }
}

UCAPI_API int UCAPI_Serialize(ucapi_t* obj, uint8_t** outBuffer, size_t* outSize) {
    // Guard against null inputs
    if (obj == nullptr || outBuffer == nullptr || outSize == nullptr) {
        return -1;
    }
    try {
        auto serializer = ucapi::Config::GetSerializerFactory().CreateSerializer("MsgPack");
        if (!serializer) {
            UCAPI_LOG_ERROR("Failed to create MsgPack serializer");
            return -1;
        }

        std::vector<uint8_t> buffer;
        if (obj->m_num_payload > 0) {
            HRESULT hr = serializer->Serialize(obj->m_payload[0], buffer);
            if (FAILED(hr)) {
                UCAPI_LOG_ERROR("Serialization failed");
                return -1;
            }
        } else {
            ucapi::CameraState emptyState(sizeof(ucapi::CameraState));
            HRESULT hr = serializer->Serialize(emptyState, buffer);
            if (FAILED(hr)) {
                UCAPI_LOG_ERROR("Serialization failed");
                return -1;
            }
        }
        
        *outSize = buffer.size();
        *outBuffer = new uint8_t[*outSize];
        std::memcpy(*outBuffer, buffer.data(), *outSize);
        return 0;
    }
    catch (const std::exception& e) {
        UCAPI_LOG_ERROR(e.what());
        return -1;
    }
}

/// <summary>
/// Frees a byte buffer allocated by UCAPI_Serialize.
/// </summary>
/// <param name="buffer">Pointer to the buffer to free.</param>
UCAPI_API void UCAPI_FreeBuffer(uint8_t* buffer) {
    delete[] buffer;
}

UCAPI_API uint16_t UCAPI_CalcCRC16(const void* recordPtr, size_t length, uint16_t poly, uint16_t initValue) {
    return ucapi_t::computeCRC16(reinterpret_cast<ucapi_t::record_t*>(const_cast<void*>(recordPtr)), length, poly, initValue);
}

// C API for object creation and destruction
UCAPI_API ucapi_t* UCAPI_Create() {
    return new ucapi_t(nullptr);
}

UCAPI_API void UCAPI_Destroy(ucapi_t* obj) {
    delete obj;
}

// =============================================================================
// C-compatible Serialize/Deserialize API implementation
// =============================================================================

namespace {
    // Helper: Convert flat record to CameraState
    ucapi::CameraState FlatToCameraState(const ucapi_record_flat_t* flat) {
        ucapi::CameraState state(sizeof(ucapi::CameraState));
        state.m_camera_no = flat->camera_no;
        state.m_commands = flat->commands;
        state.m_timecode = flat->timecode;
        state.m_subframe = flat->subframe;
        state.m_packet_no = flat->packet_no;
        state.m_eye_position_right_m = flat->eye_position_right_m;
        state.m_eye_position_up_m = flat->eye_position_up_m;
        state.m_eye_position_forward_m = flat->eye_position_forward_m;
        state.m_look_vector_right_m = flat->look_vector_right_m;
        state.m_look_vector_up_m = flat->look_vector_up_m;
        state.m_look_vector_forward_m = flat->look_vector_forward_m;
        state.m_up_vector_right_m = flat->up_vector_right_m;
        state.m_up_vector_up_m = flat->up_vector_up_m;
        state.m_up_vector_forward_m = flat->up_vector_forward_m;
        state.m_focal_length_mm = flat->focal_length_mm;
        state.m_aspect_ratio = flat->aspect_ratio;
        state.m_focus_distance_m = flat->focus_distance_m;
        state.m_aperture = flat->aperture;
        state.m_sensor_size_width_mm = flat->sensor_size_width_mm;
        state.m_sensor_size_height_mm = flat->sensor_size_height_mm;
        state.m_near_clip_m = flat->near_clip_m;
        state.m_far_clip_m = flat->far_clip_m;
        state.m_lens_shift_horizontal_ratio = flat->lens_shift_horizontal_ratio;
        state.m_lens_shift_vertical_ratio = flat->lens_shift_vertical_ratio;
        state.m_lens_distortion_radial_coefficients_k1 = flat->lens_distortion_radial_coefficients_k1;
        state.m_lens_distortion_radial_coefficients_k2 = flat->lens_distortion_radial_coefficients_k2;
        state.m_lens_distortion_center_point_right_mm = flat->lens_distortion_center_point_right_mm;
        state.m_lens_distortion_center_point_up_mm = flat->lens_distortion_center_point_up_mm;
        return state;
    }

    // Helper: Convert CameraState to flat record
    void CameraStateToFlat(const ucapi::CameraState& state, ucapi_record_flat_t* flat) {
        flat->camera_no = state.m_camera_no;
        flat->commands = state.m_commands;
        flat->timecode = state.m_timecode;
        flat->subframe = state.m_subframe;
        flat->packet_no = state.m_packet_no;
        flat->eye_position_right_m = state.m_eye_position_right_m;
        flat->eye_position_up_m = state.m_eye_position_up_m;
        flat->eye_position_forward_m = state.m_eye_position_forward_m;
        flat->look_vector_right_m = state.m_look_vector_right_m;
        flat->look_vector_up_m = state.m_look_vector_up_m;
        flat->look_vector_forward_m = state.m_look_vector_forward_m;
        flat->up_vector_right_m = state.m_up_vector_right_m;
        flat->up_vector_up_m = state.m_up_vector_up_m;
        flat->up_vector_forward_m = state.m_up_vector_forward_m;
        flat->focal_length_mm = state.m_focal_length_mm;
        flat->aspect_ratio = state.m_aspect_ratio;
        flat->focus_distance_m = state.m_focus_distance_m;
        flat->aperture = state.m_aperture;
        flat->sensor_size_width_mm = state.m_sensor_size_width_mm;
        flat->sensor_size_height_mm = state.m_sensor_size_height_mm;
        flat->near_clip_m = state.m_near_clip_m;
        flat->far_clip_m = state.m_far_clip_m;
        flat->lens_shift_horizontal_ratio = state.m_lens_shift_horizontal_ratio;
        flat->lens_shift_vertical_ratio = state.m_lens_shift_vertical_ratio;
        flat->lens_distortion_radial_coefficients_k1 = state.m_lens_distortion_radial_coefficients_k1;
        flat->lens_distortion_radial_coefficients_k2 = state.m_lens_distortion_radial_coefficients_k2;
        flat->lens_distortion_center_point_right_mm = state.m_lens_distortion_center_point_right_mm;
        flat->lens_distortion_center_point_up_mm = state.m_lens_distortion_center_point_up_mm;
    }
}

UCAPI_API int UCAPI_SerializeRecord(
    const ucapi_record_flat_t* record,
    uint8_t** outBuffer,
    size_t* outSize
) {
    if (record == nullptr || outBuffer == nullptr || outSize == nullptr) {
        return -1;
    }
    try {
        auto serializer = ucapi::Config::GetSerializerFactory().CreateSerializer("MsgPack");
        if (!serializer) {
            UCAPI_LOG_ERROR("Failed to create MsgPack serializer");
            return -1;
        }

        ucapi::CameraState state = FlatToCameraState(record);
        std::vector<uint8_t> buffer;
        HRESULT hr = serializer->Serialize(state, buffer);
        if (FAILED(hr)) {
            UCAPI_LOG_ERROR("Serialization failed");
            return -1;
        }

        *outSize = buffer.size();
        *outBuffer = new uint8_t[*outSize];
        std::memcpy(*outBuffer, buffer.data(), *outSize);
        return 0;
    }
    catch (const std::exception& e) {
        UCAPI_LOG_ERROR(e.what());
        return -1;
    }
}

UCAPI_API int UCAPI_DeserializeRecord(
    const uint8_t* buffer,
    size_t bufferSize,
    ucapi_record_flat_t* outRecord
) {
    if (buffer == nullptr || bufferSize == 0 || outRecord == nullptr) {
        return -1;
    }
    try {
        auto serializer = ucapi::Config::GetSerializerFactory().CreateSerializer("MsgPack");
        if (!serializer) {
            UCAPI_LOG_ERROR("Failed to create MsgPack serializer");
            return -1;
        }

        ucapi::CameraState state(sizeof(ucapi::CameraState));
        HRESULT hr = serializer->Deserialize(buffer, bufferSize, state);
        if (FAILED(hr)) {
            UCAPI_LOG_ERROR("Deserialization failed");
            return -1;
        }

        CameraStateToFlat(state, outRecord);
        return 0;
    }
    catch (const std::exception& e) {
        UCAPI_LOG_ERROR(e.what());
        return -1;
    }
}

// Add an empty record to the payload
UCAPI_API int UCAPI_AddRecord(ucapi_t* obj) {
    if (obj == nullptr) {
        return -1;
    }
    try {
        obj->m_payload.emplace_back(0, nullptr);
        obj->m_num_payload = static_cast<uint16_t>(obj->m_payload.size());
        return 0;
    }
    catch (const std::exception& e) {
        UCAPI_LOG_ERROR(e.what());
        return -1;
    }
}

// === Logger API Implementation ===

UCAPI_API size_t UCAPI_GetLogCount() {
    try {
        return ucapi::Logger::GetInstance().GetLogCount();
    }
    catch (...) {
        return 0;
    }
}

UCAPI_API int UCAPI_GetLog(
    size_t index,
    uint8_t* outLevel,
    uint64_t* outTimestamp,
    char* outFunction,
    size_t functionBufSize,
    char* outMessage,
    size_t messageBufSize
) {
    try {
        ucapi::LogEntry entry;
        if (!ucapi::Logger::GetInstance().GetLogEntry(index, entry)) {
            return -1;  // Index out of range
        }

        if (outLevel) {
            *outLevel = static_cast<uint8_t>(entry.level);
        }
        if (outTimestamp) {
            *outTimestamp = entry.timestamp;
        }
        if (outFunction && functionBufSize > 0) {
            if (entry.function.size() + 1 > functionBufSize) {
                return -2;  // Buffer too small
            }
            strncpy_s(outFunction, functionBufSize, entry.function.c_str(), _TRUNCATE);
        }
        if (outMessage && messageBufSize > 0) {
            if (entry.message.size() + 1 > messageBufSize) {
                return -2;  // Buffer too small
            }
            strncpy_s(outMessage, messageBufSize, entry.message.c_str(), _TRUNCATE);
        }
        return 0;
    }
    catch (...) {
        return -1;
    }
}

UCAPI_API int UCAPI_GetLogMessage(size_t index, char* outBuffer, size_t bufferSize) {
    if (outBuffer == nullptr || bufferSize == 0) {
        return -1;
    }

    try {
        ucapi::LogEntry entry;
        if (!ucapi::Logger::GetInstance().GetLogEntry(index, entry)) {
            return -1;
        }

        const char* levelStr = "";
        switch (entry.level) {
            case ucapi::LogLevel::Error:   levelStr = "[ERROR] "; break;
            case ucapi::LogLevel::Warning: levelStr = "[WARNING] "; break;
            case ucapi::LogLevel::Info:    levelStr = "[INFO] "; break;
            case ucapi::LogLevel::Debug:   levelStr = "[DEBUG] "; break;
            default: break;
        }

        std::string formatted = std::string(levelStr) + "[" + entry.function + "] " + entry.message;
        if (formatted.size() + 1 > bufferSize) {
            return -2;
        }
        strncpy_s(outBuffer, bufferSize, formatted.c_str(), _TRUNCATE);
        return 0;
    }
    catch (...) {
        return -1;
    }
}

UCAPI_API void UCAPI_ClearLogs() {
    try {
        ucapi::Logger::GetInstance().ClearLogs();
    }
    catch (...) {
        // Silently ignore if logger not initialized
    }
}

UCAPI_API void UCAPI_SetLogLevel(uint8_t level) {
    try {
        if (level <= static_cast<uint8_t>(ucapi::LogLevel::Debug)) {
            ucapi::Logger::GetInstance().SetLogLevel(static_cast<ucapi::LogLevel>(level));
        }
    }
    catch (...) {
        // Silently ignore
    }
}

UCAPI_API uint8_t UCAPI_GetLogLevel() {
    try {
        return static_cast<uint8_t>(ucapi::Logger::GetInstance().GetLogLevel());
    }
    catch (...) {
        return 1;  // Default to Error level
    }
}
