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

		enum {
			FRAME_RATE_23_976 = 0,
			FRAME_RATE_24 = 1,
			FRAME_RATE_25 = 2,
			FRAME_RATE_29_97 = 3,
			FRAME_RATE_30 = 4,
			FRAME_RATE_50 = 5,
			FRAME_RATE_59_94 = 6,
			FRAME_RATE_60 = 7,
			FRAME_RATE_119_88 = 8,
			FRAME_RATE_120 = 9,
			FRAME_RATE_144 = 10,
			FRAME_RATE_239_76 = 11,
			FRAME_RATE_240 = 12,
		};

    public:
        ~timecode_t();

        uint8_t m_frame_number;
        uint8_t m_second_number;
        uint8_t m_minute_number;
        uint8_t m_hour_number;
		uint8_t m_frame_rate;
		uint8_t m_drop_frame;
        uint8_t m_reserved;
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
        timecode_t m_timecode;
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
    };

    uint16_t m_magic;
    uint16_t m_version;
    uint16_t m_num_payload;
    uint16_t m_payload_length;
    uint16_t m_crc16;
    std::vector<record_t> m_payload;
};

#endif  // UCAPI_H_
