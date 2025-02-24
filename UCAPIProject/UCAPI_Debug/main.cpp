#ifdef _DEBUG
#include <iostream>
#include "ucapi_dll.h"
#include "ucapi_deserialization.h"

/// <summary>
/// Demonstrates creating default UCAPI data, serialization, and deserialization.
/// </summary>
int main() {
    // Create a default UCAPI object for serialization.
    void* defaultObjVoid = UCAPI_CreateDefault();
    if (!defaultObjVoid) {
        std::cerr << "Failed to create default UCAPI object." << std::endl;
        return -1;
    }
    auto defaultObj = reinterpret_cast<ucapi_serialization_t*>(defaultObjVoid);

    // Serialize the default UCAPI object.
    uint8_t* serializedBuffer = nullptr;
    size_t serializedSize = 0;
    if (UCAPI_Serialize(defaultObj, &serializedBuffer, &serializedSize) != 0) {
        std::cerr << "Serialization failed." << std::endl;
        delete defaultObj;
        return -1;
    }
    std::cout << "Serialization succeeded. Serialized size: " << serializedSize << " bytes." << std::endl;

    // Deserialize the serialized data.
    UCAPI_DllObject* deserializedObj = UCAPI_Deserialize(serializedBuffer, serializedSize);
    if (!deserializedObj) {
        std::cerr << "Deserialization failed." << std::endl;
        UCAPI_FreeBuffer(serializedBuffer);
        delete defaultObj;
        return -1;
    }
    std::cout << "Deserialization succeeded." << std::endl;

    // Free the allocated resources.
    UCAPI_FreeBuffer(serializedBuffer);
    UCAPI_FreeObject(deserializedObj);
    delete defaultObj;

    return 0;
}
#endif // _DEBUG
