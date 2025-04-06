#include "pch.h"
#include "ucapi_dll.h"
#include <iostream>
#include <msgpack.hpp>
#include "ucapi.h"
#include "ucapi_msgpack_converter.h"

UCAPI_API ucapi_t* UCAPI_Deserialize(const uint8_t* buffer, size_t payloadCount) {
    try {
		size_t size = payloadCount * sizeof(ucapi_msgpack_record_t);
        msgpack::object_handle oh = msgpack::unpack(reinterpret_cast<const char*>(buffer), size);
        msgpack::object obj = oh.get();

        ucapi_msgpack_t msgpack_obj;
        obj.convert(msgpack_obj);

        ucapi_t* native = convert_to_ucapi(msgpack_obj);
        return native;
    }
    catch (const std::exception& e) {
        std::cerr << "[UCAPI_Deserialize] " << e.what() << std::endl;
        return nullptr;
    }
}

UCAPI_API int UCAPI_Serialize(ucapi_t* obj, uint8_t** outBuffer, size_t* outSize) {
    try {
        ucapi_msgpack_t packed = convert_to_msgpack(obj);
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, packed);

        *outSize = sbuf.size();
        *outBuffer = new uint8_t[*outSize];
        std::memcpy(*outBuffer, sbuf.data(), *outSize);
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
