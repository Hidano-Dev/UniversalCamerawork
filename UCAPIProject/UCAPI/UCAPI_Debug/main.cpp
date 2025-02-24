#ifdef _DEBUG
#include <iostream>
#include "ucapi_dll.h"

/// <summary>
/// Demonstrates creating default UCAPI data, serialization, and deserialization
/// using unified memory management.
/// </summary>
int main() {
    // Create a default UCAPI object for serialization.
    UCAPI_DllObject* defaultHandle = UCAPI_CreateDefault();
    if (!defaultHandle) {
        std::cerr << "Failed to create default UCAPI object." << std::endl;
        return -1;
    }
    std::cout << "Default UCAPI object created successfully." << std::endl;

    // Serialize the default UCAPI object.
    uint8_t* serializedBuffer = nullptr;
    size_t serializedSize = 0;
    if (UCAPI_Serialize(defaultHandle, &serializedBuffer, &serializedSize) != 0) {
        std::cerr << "Serialization failed." << std::endl;
        UCAPI_FreeObject(defaultHandle);
        return -1;
    }
    std::cout << "Serialization succeeded. Serialized size: " << serializedSize << " bytes." << std::endl;

    // Deserialize the serialized data.
    UCAPI_DllObject* deserializedHandle = UCAPI_Deserialize(serializedBuffer, serializedSize);
    if (!deserializedHandle) {
        std::cerr << "Deserialization failed." << std::endl;
        UCAPI_FreeBuffer(serializedBuffer);
        UCAPI_FreeObject(defaultHandle);
        return -1;
    }
    std::cout << "Deserialization succeeded." << std::endl;

    // Free the allocated resources.
    UCAPI_FreeBuffer(serializedBuffer);
    UCAPI_FreeObject(deserializedHandle);
    UCAPI_FreeObject(defaultHandle);

    return 0;
}
#endif // _DEBUG
