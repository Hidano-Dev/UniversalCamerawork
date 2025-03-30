#ifndef UCAPI_H_
#define UCAPI_H_

#include <stdint.h>
#include <string>
#include <vector>

class ucapi_t{

public:
    class timecode_t;
    class record_t;

    ucapi_t(const void* dataPtr = nullptr);

private:
    void _read(const void* dataPtr);
    void _clean_up();

public:
    ~ucapi_t();

    class timecode_t{

    public:

        timecode_t(const void* dataPtr = nullptr);

    private:
        void _read(const void* dataPtr);
        void _clean_up();

    public:
        ~timecode_t();

        uint64_t m_frame_number;
        uint64_t m_second_number;
        uint64_t m_minute_number;
        uint64_t m_hour_number;
        uint64_t m_reserved;
    };

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
        timecode_t* m_timecode;
        uint8_t m_packet_no;
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
        std::vector<uint8_t>* m_reserved;
    };

    uint16_t m_magic;
    uint16_t m_version;
    uint16_t m_num_payload;
    uint16_t m_payload_length;
    uint16_t m_crc16;
    std::vector<record_t*>* m_payload;
	std::vector<std::string>* m__raw_payload;
};

#endif  // UCAPI_H_
