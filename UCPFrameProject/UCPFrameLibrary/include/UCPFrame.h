#ifndef UCPFRAME_H
#define UCPFRAME_H

#include <cstdint>
#include <cstddef>

#ifdef _WIN32
    #ifdef UCPFRAME_EXPORTS
        #define UCPFRAME_API __declspec(dllexport)
    #else
        #define UCPFRAME_API __declspec(dllimport)
    #endif
#else
    #define UCPFRAME_API
#endif

// 余計なパディングを防ぐためにパック指定
#pragma pack(push, 1)
struct UCPFrame {
    uint8_t header[8];    // 0-2byte: ID ("UCP" => 0x55, 0x43, 0x50), 3byte: MajorVersion, 4byte: MinorVersion, 5-7byte: ReservedArea
    uint16_t commands;    // 例: DOF_ENABLE = 0x04, LENS_DISTORTION_ENABLE = 0x08
    uint32_t timecode;    // 0-6bit: FrameNumber, 7-12bit: SecondNumber, 13-18bit: MinuteNumber, 19-23bit: HourNumber
    uint8_t packetNo;
    float eyePosition_right_m;
    float eyePosition_up_m;
    float eyePosition_forward_m;
    float lookVector_right_m;
    float lookVector_up_m;
    float lookVector_forward_m;
    float upVector_right_m;
    float upVector_up_m;
    float upVector_forward_m;
    float focalLength_mm;
    float aspectRatio;
    float focusDistance_m;
    float aperture;
    float sensorSize_width_mm;
    float sensorSize_height_mm;
    float nearClip_m;
    float farClip_m;
    float lensShift_horizontal_ratio;
    float lensShift_vertical_ratio;
    float lensDistortion_RadialCoefficients_k1;
    float lensDistortion_RadialCoefficients_k2;
    float lensDistortion_CenterPoint_right_mm;
    float lensDistortion_CenterPoint_up_mm;
    uint32_t checksum;    // CRC-32 (Polynomial: 0xEDB88320, InitialValue: 0xFFFFFFFF, FinalXORValue: 0xFFFFFFFF)
};
#pragma pack(pop)

// 固定サイズの定義
constexpr size_t UCPFrameSize = sizeof(UCPFrame);

extern "C" {

    // シリアライズ：UCPFrame構造体をバイナリバッファにコピー
    UCPFRAME_API void SerializeUCPFrame(const UCPFrame& frame, uint8_t* buffer, size_t bufferSize);

    // デシリアライズ：バイナリバッファからUCPFrame構造体に復元し、チェックサム検証を行う
    UCPFRAME_API bool DeserializeUCPFrame(const uint8_t* buffer, size_t bufferSize, UCPFrame& frame);

}

#endif // UCPFRAME_H
