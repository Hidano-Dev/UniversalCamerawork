#include "UCPFrame.h"
#include <cstring>
#include <iostream>

// CRC-32の計算（簡易実装）
static uint32_t CalculateCRC32(const uint8_t* data, size_t length) {
    uint32_t crc = 0xFFFFFFFF;
    for (size_t i = 0; i < length; ++i) {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; ++j) {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xEDB88320;
            else
                crc = crc >> 1;
        }
    }
    return crc ^ 0xFFFFFFFF;
}

void SerializeUCPFrame(const UCPFrame& frame, uint8_t* buffer, size_t bufferSize) {
    if (bufferSize < UCPFrameSize) {
        std::cerr << "Buffer size is too small." << std::endl;
        return;
    }
    std::memcpy(buffer, &frame, UCPFrameSize);
}

bool DeserializeUCPFrame(const uint8_t* buffer, size_t bufferSize, UCPFrame& frame) {
    if (bufferSize < UCPFrameSize) {
        std::cerr << "Buffer size is too small." << std::endl;
        return false;
    }
    std::memcpy(&frame, buffer, UCPFrameSize);

    // チェックサムはchecksumフィールド以外のデータで計算する
    uint32_t computedChecksum = CalculateCRC32(buffer, UCPFrameSize - sizeof(uint32_t));
    if (computedChecksum != frame.checksum) {
        std::cerr << "Checksum verification failed." << std::endl;
        return false;
    }
    return true;
}
