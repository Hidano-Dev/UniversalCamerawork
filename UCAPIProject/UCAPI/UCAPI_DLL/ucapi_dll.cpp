#include "pch.h"
#include "ucapi.h"
#include "ucapi_dll.h"
#include "ucapi_serializer.h"
#include <kaitai/kaitaistruct.h>
#include <sstream>
#include <vector>
#include <cstring>
#include <exception>
#include <iostream>

// Define the wrapper structure to hold both the deserialized UCAPI object and its stream.
struct UCAPI_DllObject {
    ucapi_t* obj;
    kaitai::kstream* ks;
};

/// <summary>
/// Serializes the given UCAPI structure into a newly allocated byte buffer.
/// </summary>
/// <param name="ucapi">Pointer to the UCAPI structure to serialize.</param>
/// <param name="outBuffer">Pointer to the output byte buffer pointer.</param>
/// <param name="outSize">Pointer to the output buffer size.</param>
/// <returns>0 on success, non-zero on failure.</returns>
extern "C" UCAPI_API int UCAPI_Serialize(UCAPI_DllObject* ucapi, uint8_t** outBuffer, size_t* outSize) {
    try {
		const ucapi_t* ucapiObj = ucapi->obj;
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
        ucapi_t* obj = new ucapi_t(ks);
        // Allocate and initialize the wrapper.
        UCAPI_DllObject* wrapper = new UCAPI_DllObject;
        wrapper->obj = obj;
        wrapper->ks = ks;
        return wrapper;
    }
    catch (const std::exception& exp) {
		// Print the exception message to the console.
		std::cerr << exp.what() << std::endl;
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

/// <summary>
/// Creates a new default UCAPI object for serialization wrapped in a UCAPI_DllObject.
/// </summary>
/// <returns>Pointer to a new UCAPI_DllObject, or nullptr on failure.</returns>
extern "C" UCAPI_API UCAPI_DllObject* UCAPI_CreateDefault() {
    try {
		std::string defaultData;
		// Append magic number (0x55AA, 2 bytes little-endian)
		defaultData.push_back(0xAA);
		defaultData.push_back(0x55);
		// Append version number (0x0002, 2 bytes little-endian)
		defaultData.push_back(0x02);
		defaultData.push_back(0x00);
		// Append number of records (1, 2 bytes little-endian)
		defaultData.push_back(0x01);
		defaultData.push_back(0x00);
		// Append payload length (128, 2 bytes little-endian)
		defaultData.push_back(0x80);
		defaultData.push_back(0x00);
		// Append CRC-16 checksum (0, 2 bytes little-endian)
		defaultData.push_back(0x00);
		defaultData.push_back(0x00);
		// Append one default record (128 bytes, zero-filled)
		defaultData.append(128, '\0');

        // Create a new kaitai::kstream from the binary string.
        kaitai::kstream* ks = new kaitai::kstream(defaultData);
        // Use the deserialization constructor to create a new UCAPI object.
        ucapi_t* obj = new ucapi_t(ks);
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
