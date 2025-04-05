#include "pch.h"
#include "ucapi_dll.h"
#include <iostream>
#include <msgpack.hpp>
#include "ucapi_msgpack_converter.h"

struct UCAPI_DllObject {
    ucapi_t* obj;
};

UCAPI_API UCAPI_DllObject* UCAPI_DeserializeMessagePack(const uint8_t* buffer, size_t size) {
    try {
        msgpack::object_handle oh = msgpack::unpack(reinterpret_cast<const char*>(buffer), size);
        msgpack::object obj = oh.get();

        ucapi_msgpack_t msgpack_obj;
        obj.convert(msgpack_obj);

        ucapi_t* native = convert_to_ucapi(msgpack_obj);

        UCAPI_DllObject* wrapper = new UCAPI_DllObject;
        wrapper->obj = native;
        return wrapper;
    }
    catch (const std::exception& e) {
        std::cerr << "[UCAPI_DeserializeMessagePack] " << e.what() << std::endl;
        return nullptr;
    }
}

UCAPI_API int UCAPI_SerializeMessagePack(UCAPI_DllObject* obj, uint8_t** outBuffer, size_t* outSize) {
    try {
        ucapi_msgpack_t packed = convert_to_msgpack(obj->obj);
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, packed);

        *outSize = sbuf.size();
        *outBuffer = new uint8_t[*outSize];
        std::memcpy(*outBuffer, sbuf.data(), *outSize);
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "[UCAPI_SerializeMessagePack] " << e.what() << std::endl;
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
