#include "pch.h"
#include "ucapi_dll.h"
#include <iostream>
#include <msgpack.hpp>
#include "ucapi.h"
#include "ucapi_msgpack_converter.h"
#include "ucapi_serializer_utility.h"

struct UCAPI_DllObject {
    ucapi_t* obj;
};

UCAPI_API ucapi_t* UCAPI_Deserialize(const uint8_t* buffer, size_t size) {
    try {
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

UCAPI_API uint8_t* UCAPI_EncodeToBinary(UCAPI_DllObject* obj, size_t* outSize) {
    try {
		std::ostringstream oss(std::ios::binary);
		write_ucapi(obj->obj, oss);

		std::string bin = oss.str();
		*outSize = bin.size();

		uint8_t* out = new uint8_t[*outSize];
		std::memcpy(out, bin.data(), *outSize);

		return out;
    }
    catch (const std::exception& e) {
        std::cerr << "[UCAPI_EncodeToBinary] " << e.what() << std::endl;
        *outSize = 0;
        return nullptr;
    }
}

UCAPI_API UCAPI_DllObject* UCAPI_DecodeFromBinary(const uint8_t* data, size_t size) {
    try {
        if (!data || size == 0) return nullptr;

        UCAPI_DllObject* wrapper = new UCAPI_DllObject;
        wrapper->obj = new ucapi_t(static_cast<const void*>(data));  // _read ‚ª‘–‚é
        return wrapper;
    }
    catch (const std::exception& e) {
        std::cerr << "[UCAPI_DecodeFromBinary] " << e.what() << std::endl;
        return nullptr;
    }
}

/// <summary>
/// Frees a byte buffer allocated by UCAPI_Serialize.
/// </summary>
/// <param name="buffer">Pointer to the buffer to free.</param>
UCAPI_API void UCAPI_FreeBuffer(uint8_t* buffer) {
    delete[] buffer;
}

/// <summary>
/// Frees the UCAPI object created by UCAPI_Deserialize.
/// </summary>
/// <param name="obj">Pointer to the UCAPI_DllObject to free.</param>
UCAPI_API void UCAPI_FreeObject(UCAPI_DllObject* obj) {
    if (obj) {
        if (obj->obj) {
            delete obj->obj;
        }
        delete obj;
    }
}
