#ifndef UCAPI_H_
#define UCAPI_H_

#include <stdint.h>
#include <string>
#include <vector>
#include "ucapi_timecode.h"

// =============================================================================
// UCAPI Constants
// =============================================================================

/// Magic number identifying valid UCAPI data (0x55AA in little-endian)
constexpr uint16_t UCAPI_MAGIC = 0x55AA;

/// Header size in bytes: magic(2) + version(2) + num_payload(2) + crc16(2) + reserved(2)
constexpr size_t UCAPI_HEADER_SIZE = 10;

/// Maximum payload size in bytes (64KB limit for safety)
constexpr size_t UCAPI_MAX_PAYLOAD_SIZE = 0x10000;

/// Minimum record size in bytes (last field offset 103 + 4 bytes for float)
constexpr size_t UCAPI_MIN_RECORD_SIZE = 107;

// =============================================================================
// CRC16-CCITT Constants
// =============================================================================

/// CRC16-CCITT polynomial (x^16 + x^12 + x^5 + 1)
constexpr uint16_t UCAPI_CRC16_POLYNOMIAL = 0x1021;

/// CRC16-CCITT initial value
constexpr uint16_t UCAPI_CRC16_INIT_VALUE = 0xFFFF;

/// CRC16 most significant bit mask (for bit-by-bit calculation)
constexpr uint16_t UCAPI_CRC16_MSB_MASK = 0x8000;

/// Number of bits per byte (for CRC calculation loop)
constexpr int UCAPI_BITS_PER_BYTE = 8;

// =============================================================================

class ucapi_t{

public:
    class record_t;

    ucapi_t(const void* dataPtr = nullptr, size_t bufferSize = 0);

    static uint16_t computeCRC16(record_t* record, size_t length, uint16_t poly = UCAPI_CRC16_POLYNOMIAL, uint16_t initValue = UCAPI_CRC16_INIT_VALUE);

private:
    void _read(const void* dataPtr, size_t bufferSize);
    void _clean_up();

public:
    ~ucapi_t();

    class record_t{

    public:

        record_t(size_t payload_length, const void* dataPtr = nullptr);

    private:
        void _read(const void* dataPtr, size_t payload_length);
        void _clean_up();

    public:
        ~record_t();

        uint32_t m_camera_no;
        uint16_t m_commands;
        uint32_t m_timecode;
        float m_subframe;
        uint8_t m_packet_no;

        // Timecode accessors for convenient field access
        timecode_t get_timecode() const { return timecode_t::unpack(m_timecode); }
        void set_timecode(const timecode_t& tc) { m_timecode = timecode_t::pack(tc); }
        float m_eye_position_right_m;
        float m_eye_position_up_m;
        float m_eye_position_forward_m;
        float m_look_vector_right_m;
        float m_look_vector_up_m;
        float m_look_vector_forward_m;
        float m_up_vector_right_m;
        float m_up_vector_up_m;
        float m_up_vector_forward_m;
        float m_focal_length_mm;
        float m_aspect_ratio;
        float m_focus_distance_m;
        float m_aperture;
        float m_sensor_size_width_mm;
        float m_sensor_size_height_mm;
        float m_near_clip_m;
        float m_far_clip_m;
        float m_lens_shift_horizontal_ratio;
        float m_lens_shift_vertical_ratio;
        float m_lens_distortion_radial_coefficients_k1;
        float m_lens_distortion_radial_coefficients_k2;
        float m_lens_distortion_center_point_right_mm;
        float m_lens_distortion_center_point_up_mm;
    };

    uint16_t m_magic;
    uint16_t m_version;
    uint16_t m_num_payload;
    uint16_t m_crc16;
    std::vector<record_t> m_payload;
};

#endif  // UCAPI_H_
