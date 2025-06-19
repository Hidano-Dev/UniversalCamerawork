#include "pch.h"
#include "ucapi_dll.h"
#include <iostream>
#include <msgpack.hpp>
#include "ucapi.h"
#include "ucapi_msgpack_converter.h"
#include "ucapi_config.h"

UCAPI_API ucapi_t* UCAPI_Deserialize(const uint8_t* buffer, size_t payloadCount) {
    // Guard against null or empty buffer
    if (buffer == nullptr || payloadCount == 0) {
        return nullptr;
    }
    try {
        auto serializer = ucapi::Config::GetSerializerFactory().CreateSerializer("MsgPack");
        if (!serializer) {
            std::cerr << "[UCAPI_Deserialize] Failed to create MsgPack serializer" << std::endl;
            return nullptr;
        }
        
        ucapi::CameraState cameraState(sizeof(ucapi::CameraState));
        HRESULT hr = serializer->Deserialize(buffer, payloadCount, cameraState);
        if (FAILED(hr)) {
            std::cerr << "[UCAPI_Deserialize] Deserialization failed" << std::endl;
            return nullptr;
        }
        
        ucapi_t* native = new ucapi_t();
        native->m_magic = 0x55AA;
        native->m_version = 1;
        native->m_num_payload = 1;
        native->m_crc16 = 0;
        native->m_payload.emplace_back(sizeof(ucapi_t::record_t));
        native->m_payload[0] = cameraState;
        
        return native;
    }
    catch (const std::exception& e) {
        std::cerr << "[UCAPI_Deserialize] " << e.what() << std::endl;
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
            std::cerr << "[UCAPI_Serialize] Failed to create MsgPack serializer" << std::endl;
            return -1;
        }
        
        std::vector<uint8_t> buffer;
        if (obj->m_num_payload > 0) {
            HRESULT hr = serializer->Serialize(obj->m_payload[0], buffer);
            if (FAILED(hr)) {
                std::cerr << "[UCAPI_Serialize] Serialization failed" << std::endl;
                return -1;
            }
        } else {
            ucapi::CameraState emptyState(sizeof(ucapi::CameraState));
            HRESULT hr = serializer->Serialize(emptyState, buffer);
            if (FAILED(hr)) {
                std::cerr << "[UCAPI_Serialize] Serialization failed" << std::endl;
                return -1;
            }
        }
        
        *outSize = buffer.size();
        *outBuffer = new uint8_t[*outSize];
        std::memcpy(*outBuffer, buffer.data(), *outSize);
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "[UCAPI_Serialize] " << e.what() << std::endl;
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
