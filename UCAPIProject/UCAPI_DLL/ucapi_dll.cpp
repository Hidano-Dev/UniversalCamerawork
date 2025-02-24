#include "pch.h"
#include "ucapi_dll.h"
#include "ucapi_serialization.h"
#include "ucapi_deserialization.h"
#include "kaitai/kaitaistruct.h"
#include <sstream>
#include <vector>
#include <cstring>
#include <exception>

// Define the wrapper structure to hold both the deserialized UCAPI object and its stream.
struct UCAPI_DllObject {
    ucapi_serialization_t* obj;
    kaitai::kstream* ks;
};

/// <summary>
/// Serializes the given UCAPI structure into a newly allocated byte buffer.
/// </summary>
/// <param name="ucapi">Pointer to the UCAPI structure to serialize.</param>
/// <param name="outBuffer">Pointer to the output byte buffer pointer.</param>
/// <param name="outSize">Pointer to the output buffer size.</param>
/// <returns>0 on success, non-zero on failure.</returns>
extern "C" UCAPI_API int UCAPI_Serialize(const void* ucapi, uint8_t** outBuffer, size_t* outSize) {
    try {
        // Cast the input pointer to the UCAPI object type.
        const ucapi_serialization_t* ucapiObj = reinterpret_cast<const ucapi_serialization_t*>(ucapi);
        // Create an output stream in binary mode.
        std::ostringstream oss(std::ios::binary);
        // Serialize the UCAPI object into the stream.
        write_ucapi(ucapiObj, oss);
        // Obtain the binary data.
        std::string data = oss.str();
        *outSize = data.size();
        // Allocate a buffer for the output data.
        *outBuffer = new uint8_t[*outSize];
        std::memcpy(*outBuffer, data.data(), *outSize);
        return 0;
    }
    catch (const std::exception&) {
        return -1;
    }
}

/// <summary>
/// Frees a byte buffer allocated by UCAPI_Serialize.
/// </summary>
/// <param name="buffer">Pointer to the buffer to free.</param>
extern "C" UCAPI_API void UCAPI_FreeBuffer(uint8_t* buffer) {
    delete[] buffer;
}

/// <summary>
/// Deserializes the given byte buffer into a UCAPI object.
/// </summary>
/// <param name="inBuffer">Pointer to the input byte buffer.</param>
/// <param name="inSize">Size of the input buffer.</param>
/// <returns>Pointer to a UCAPI_DllObject on success, or nullptr on failure.</returns>
extern "C" UCAPI_API UCAPI_DllObject* UCAPI_Deserialize(const uint8_t* inBuffer, size_t inSize) {
    try {
        // Create a std::string from the input buffer.
        std::string data(reinterpret_cast<const char*>(inBuffer), inSize);
        // Create a new kaitai::kstream from the binary data.
        kaitai::kstream* ks = new kaitai::kstream(data);
        // Deserialize the UCAPI object using the kstream.
        ucapi_serialization_t* obj = new ucapi_serialization_t(ks);
        // Allocate and initialize the wrapper.
        UCAPI_DllObject* wrapper = new UCAPI_DllObject;
        wrapper->obj = obj;
        wrapper->ks = ks;
        return wrapper;
    }
    catch (const std::exception&) {
        return nullptr;
    }
}

/// <summary>
/// Frees the UCAPI object created by UCAPI_Deserialize.
/// </summary>
/// <param name="obj">Pointer to the UCAPI_DllObject to free.</param>
extern "C" UCAPI_API void UCAPI_FreeObject(UCAPI_DllObject* obj) {
    if (obj) {
        if (obj->obj) {
            delete obj->obj;
        }
        if (obj->ks) {
            delete obj->ks;
        }
        delete obj;
    }
}
