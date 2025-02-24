#include "pch.h"
#include "ucapi_serialization.h"
#include <cstring>  // for memcpy

namespace {

    /// <summary>
    /// Writes a single byte to the output stream.
    /// </summary>
    void write_u1(std::ostream& os, uint8_t val) {
        os.write(reinterpret_cast<const char*>(&val), sizeof(val));
    }

    /// <summary>
    /// Writes a 16-bit unsigned integer in little-endian format to the output stream.
    /// </summary>
    void write_u2le(std::ostream& os, uint16_t val) {
        uint8_t b[2];
        b[0] = val & 0xFF;
        b[1] = (val >> 8) & 0xFF;
        os.write(reinterpret_cast<const char*>(b), sizeof(b));
    }

    /// <summary>
    /// Writes a 32-bit unsigned integer in little-endian format to the output stream.
    /// </summary>
    void write_u4le(std::ostream& os, uint32_t val) {
        uint8_t b[4];
        b[0] = val & 0xFF;
        b[1] = (val >> 8) & 0xFF;
        b[2] = (val >> 16) & 0xFF;
        b[3] = (val >> 24) & 0xFF;
        os.write(reinterpret_cast<const char*>(b), sizeof(b));
    }

    /// <summary>
    /// Writes a 32-bit float in little-endian format to the output stream.
    /// </summary>
    void write_f4le(std::ostream& os, float val) {
        uint32_t as_int;
        static_assert(sizeof(float) == sizeof(uint32_t), "float must be 4 bytes");
        std::memcpy(&as_int, &val, sizeof(float));
        write_u4le(os, as_int);
    }

    /// <summary>
    /// Writes a sequence of bytes to the output stream.
    /// </summary>
    void write_bytes(std::ostream& os, const char* data, std::size_t size) {
        os.write(data, size);
    }

}  // namespace

void write_ucapi_header(const ucapi_serialization_t::header_t* header, std::ostream& os) {
    // Write the 5-byte signature (e.g. "UCAPI")
    write_bytes(os, header->signature().data(), 5);
    // Write version information (1 byte each)
    write_u1(os, header->version_major());
    write_u1(os, header->version_minor());
    // Write reserved 17 bytes
    const std::vector<uint8_t>* reserved = header->reserved();
    for (size_t i = 0; i < reserved->size(); i++) {
        write_u1(os, reserved->at(i));
    }
    // Write record count (4 bytes, little-endian)
    write_u4le(os, header->record_count());
    // Write checksum (4 bytes, little-endian)
    write_u4le(os, header->checksum());
}

void write_ucapi_timecode(const ucapi_serialization_t::timecode_t* timecode, std::ostream& os) {
    // Pack the timecode fields into a 32-bit integer.
    // Field sizes: frame_number (7 bits), second_number (6 bits),
    // minute_number (6 bits), hour_number (5 bits), reserved (8 bits)
    uint32_t packed = 0;
    packed |= ((timecode->frame_number() & 0x7F) << (6 + 6 + 5 + 8)); // shift by 25 bits
    packed |= ((timecode->second_number() & 0x3F) << (6 + 5 + 8));      // shift by 19 bits
    packed |= ((timecode->minute_number() & 0x3F) << (5 + 8));            // shift by 13 bits
    packed |= ((timecode->hour_number() & 0x1F) << 8);                    // shift by 8 bits
    packed |= (timecode->reserved() & 0xFF);                              // 8 bits
    // Write the packed 32-bit integer in big-endian order
    uint8_t b[4];
    b[0] = (packed >> 24) & 0xFF;
    b[1] = (packed >> 16) & 0xFF;
    b[2] = (packed >> 8) & 0xFF;
    b[3] = packed & 0xFF;
    os.write(reinterpret_cast<const char*>(b), sizeof(b));
}

void write_ucapi_record(const ucapi_serialization_t::record_t* record, std::ostream& os) {
    // Write commands (2 bytes, little-endian)
    write_u2le(os, record->commands());
    // Write nested timecode structure
    write_ucapi_timecode(record->timecode(), os);
    // Write packet number (1 byte)
    write_u1(os, record->packet_no());
    // Write camera position/orientation and parameters (each as 4-byte float, little-endian)
    write_f4le(os, record->eye_position_right_m());
    write_f4le(os, record->eye_position_up_m());
    write_f4le(os, record->eye_position_forward_m());
    write_f4le(os, record->look_vector_right_m());
    write_f4le(os, record->look_vector_up_m());
    write_f4le(os, record->look_vector_forward_m());
    write_f4le(os, record->up_vector_right_m());
    write_f4le(os, record->up_vector_up_m());
    write_f4le(os, record->up_vector_forward_m());
    write_f4le(os, record->focal_length_mm());
    write_f4le(os, record->aspect_ratio());
    write_f4le(os, record->focus_distance_m());
    write_f4le(os, record->aperture());
    write_f4le(os, record->sensor_size_width_mm());
    write_f4le(os, record->sensor_size_height_mm());
    write_f4le(os, record->near_clip_m());
    write_f4le(os, record->far_clip_m());
    write_f4le(os, record->lens_shift_horizontal_ratio());
    write_f4le(os, record->lens_shift_vertical_ratio());
    write_f4le(os, record->lens_distortion_radial_coefficients_k1());
    write_f4le(os, record->lens_distortion_radial_coefficients_k2());
    write_f4le(os, record->lens_distortion_center_point_right_mm());
    write_f4le(os, record->lens_distortion_center_point_up_mm());
    // Write reserved 29 bytes
    const std::vector<uint8_t>* reserved = record->reserved();
    for (size_t i = 0; i < reserved->size(); i++) {
        write_u1(os, reserved->at(i));
    }
}

void write_ucapi(const ucapi_serialization_t* ucapi, std::ostream& os) {
    // Write header first
    write_ucapi_header(ucapi->header(), os);
    // Write each record (the number of records is given by header->record_count())
    const std::vector<ucapi_serialization_t::record_t*>* records = ucapi->records();
    for (size_t i = 0; i < records->size(); i++) {
        write_ucapi_record(records->at(i), os);
    }
}
