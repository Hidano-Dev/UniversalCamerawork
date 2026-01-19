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
            if (entry.function.size() >= functionBufSize) {
                return -2;  // Buffer too small
            }
            strncpy_s(outFunction, functionBufSize, entry.function.c_str(), _TRUNCATE);
        }
        if (outMessage && messageBufSize > 0) {
            if (entry.message.size() >= messageBufSize) {
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
        if (formatted.size() >= bufferSize) {
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
