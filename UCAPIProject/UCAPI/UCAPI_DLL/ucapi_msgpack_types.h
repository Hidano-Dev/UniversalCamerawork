#pragma once
#include <msgpack.hpp>
#include <vector>
#include <cstdint>
#include <array>

// SMPTE LTC data structure for MessagePack serialization
struct smpte_ltc_msgpack_t {
    std::array<uint8_t, 10> data;  // 80 bits = 10 bytes
    
    MSGPACK_DEFINE(data);
    
    // Convenience methods
    void clear() {
        data.fill(0);
        set_sync_word();
    }
    
    void set_time(uint8_t hour, uint8_t minute, uint8_t second, uint8_t frame);
    void set_drop_frame(bool drop_frame);
    void set_color_frame(bool color_frame);
    void set_sync_word();
    bool is_valid_sync_word() const;
    
    // Individual field access
    uint8_t get_frame() const;
    uint8_t get_second() const;
    uint8_t get_minute() const;
    uint8_t get_hour() const;
    bool get_drop_frame() const;
    bool get_color_frame() const;
    
    void set_frame(uint8_t frame);
    void set_second(uint8_t second);
    void set_minute(uint8_t minute);
    void set_hour(uint8_t hour);
    
    // Convenience properties for backward compatibility
    uint8_t frame, second, minute, hour;
    bool drop_frame, color_frame;
};

struct ucapi_msgpack_record_t {
    uint32_t camera_no;
    uint16_t commands;
    smpte_ltc_msgpack_t timecode;  // Changed from uint32_t to smpte_ltc_msgpack_t
    float subframe;                // Changed from uint32_t to float
    uint8_t packet_no;

    float eye_position_right_m;
    float eye_position_up_m;
    float eye_position_forward_m;
    float look_vector_right_m;
    float look_vector_up_m;
    float look_vector_forward_m;
    float up_vector_right_m;
    float up_vector_up_m;
    float up_vector_forward_m;

    float focal_length_mm;
    float aspect_ratio;
    float focus_distance_m;
    float aperture;
    float sensor_size_width_mm;
    float sensor_size_height_mm;
    float near_clip_m;
    float far_clip_m;
    float lens_shift_horizontal_ratio;
    float lens_shift_vertical_ratio;
    float lens_distortion_radial_coefficients_k1;
    float lens_distortion_radial_coefficients_k2;
    float lens_distortion_center_point_right_mm;
    float lens_distortion_center_point_up_mm;

    MSGPACK_DEFINE(
        camera_no, commands, timecode, subframe, packet_no,
        eye_position_right_m, eye_position_up_m, eye_position_forward_m,
        look_vector_right_m, look_vector_up_m, look_vector_forward_m,
        up_vector_right_m, up_vector_up_m, up_vector_forward_m,
        focal_length_mm, aspect_ratio, focus_distance_m, aperture,
        sensor_size_width_mm, sensor_size_height_mm,
        near_clip_m, far_clip_m,
        lens_shift_horizontal_ratio, lens_shift_vertical_ratio,
        lens_distortion_radial_coefficients_k1, lens_distortion_radial_coefficients_k2,
        lens_distortion_center_point_right_mm, lens_distortion_center_point_up_mm
    );
};

struct ucapi_msgpack_t {
    uint16_t magic;
    uint16_t version;
    uint16_t num_payload;
    uint16_t crc16;

    std::vector<ucapi_msgpack_record_t> payload;

    MSGPACK_DEFINE(magic, version, num_payload, crc16, payload);
};
