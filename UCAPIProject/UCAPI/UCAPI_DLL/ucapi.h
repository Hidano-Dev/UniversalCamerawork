#ifndef UCAPI_H_
#define UCAPI_H_

// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

#include <kaitai/kaitaistruct.h>
#include <kaitai/kaitaistream.h>
#include <stdint.h>
#include <vector>

#if KAITAI_STRUCT_VERSION < 11000L
#error "Incompatible Kaitai Struct C++/STL API: version 0.11 or later is required"
#endif

class ucapi_t : public kaitai::kstruct {

public:
    class timecode_t;
    class record_t;

    ucapi_t(kaitai::kstream* p__io, kaitai::kstruct* p__parent = 0, ucapi_t* p__root = 0);

private:
    void _read();
    void _clean_up();

public:
    ~ucapi_t();

    class timecode_t : public kaitai::kstruct {

    public:

        timecode_t(kaitai::kstream* p__io, ucapi_t::record_t* p__parent = 0, ucapi_t* p__root = 0);

    private:
        void _read();
        void _clean_up();

    public:
        ~timecode_t();

    private:
        uint64_t m_frame_number;
        uint64_t m_second_number;
        uint64_t m_minute_number;
        uint64_t m_hour_number;
        uint64_t m_reserved;
        ucapi_t* m__root;
        ucapi_t::record_t* m__parent;

    public:
        uint64_t frame_number() const { return m_frame_number; }
        uint64_t second_number() const { return m_second_number; }
        uint64_t minute_number() const { return m_minute_number; }
        uint64_t hour_number() const { return m_hour_number; }
        uint64_t reserved() const { return m_reserved; }
        ucapi_t* _root() const { return m__root; }
        ucapi_t::record_t* _parent() const { return m__parent; }
    };

    class record_t : public kaitai::kstruct {

    public:

        record_t(kaitai::kstream* p__io, ucapi_t* p__parent = 0, ucapi_t* p__root = 0);

    private:
        void _read();
        void _clean_up();

    public:
        ~record_t();

    private:
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
        ucapi_t* m__root;
        ucapi_t* m__parent;

    public:
        uint32_t camera_no() const { return m_camera_no; }
        uint16_t commands() const { return m_commands; }
        timecode_t* timecode() const { return m_timecode; }
        uint8_t packet_no() const { return m_packet_no; }
        float eye_position_right_m() const { return m_eye_position_right_m; }
        float eye_position_up_m() const { return m_eye_position_up_m; }
        float eye_position_forward_m() const { return m_eye_position_forward_m; }
        float look_vector_right_m() const { return m_look_vector_right_m; }
        float look_vector_up_m() const { return m_look_vector_up_m; }
        float look_vector_forward_m() const { return m_look_vector_forward_m; }
        float up_vector_right_m() const { return m_up_vector_right_m; }
        float up_vector_up_m() const { return m_up_vector_up_m; }
        float up_vector_forward_m() const { return m_up_vector_forward_m; }
        float focal_length_mm() const { return m_focal_length_mm; }
        float aspect_ratio() const { return m_aspect_ratio; }
        float focus_distance_m() const { return m_focus_distance_m; }
        float aperture() const { return m_aperture; }
        float sensor_size_width_mm() const { return m_sensor_size_width_mm; }
        float sensor_size_height_mm() const { return m_sensor_size_height_mm; }
        float near_clip_m() const { return m_near_clip_m; }
        float far_clip_m() const { return m_far_clip_m; }
        float lens_shift_horizontal_ratio() const { return m_lens_shift_horizontal_ratio; }
        float lens_shift_vertical_ratio() const { return m_lens_shift_vertical_ratio; }
        float lens_distortion_radial_coefficients_k1() const { return m_lens_distortion_radial_coefficients_k1; }
        float lens_distortion_radial_coefficients_k2() const { return m_lens_distortion_radial_coefficients_k2; }
        float lens_distortion_center_point_right_mm() const { return m_lens_distortion_center_point_right_mm; }
        float lens_distortion_center_point_up_mm() const { return m_lens_distortion_center_point_up_mm; }
        std::vector<uint8_t>* reserved() const { return m_reserved; }
        ucapi_t* _root() const { return m__root; }
        ucapi_t* _parent() const { return m__parent; }
    };

private:
    uint16_t m_magic;
    uint16_t m_version;
    uint16_t m_num_payload;
    uint16_t m_payload_length;
    uint16_t m_crc16;
    std::vector<record_t*>* m_payload;
    ucapi_t* m__root;
    kaitai::kstruct* m__parent;
    std::vector<std::string>* m__raw_payload;
    std::vector<kaitai::kstream*>* m__io__raw_payload;

public:

    /**
     * Magic number (e.g. 0x55AA)
     */
    uint16_t magic() const { return m_magic; }

    /**
     * Packet format version (e.g. 0x0002)
     */
    uint16_t version() const { return m_version; }

    /**
     * Number of records in this packet
     */
    uint16_t num_payload() const { return m_num_payload; }

    /**
     * Total length of payload in bytes (num_payload × 128)
     */
    uint16_t payload_length() const { return m_payload_length; }

    /**
     * CRC-16 checksum of payload[]
     */
    uint16_t crc16() const { return m_crc16; }

    /**
     * Array of UCAPI records
     */
    std::vector<record_t*>* payload() const { return m_payload; }
    ucapi_t* _root() const { return m__root; }
    kaitai::kstruct* _parent() const { return m__parent; }
    std::vector<std::string>* _raw_payload() const { return m__raw_payload; }
    std::vector<kaitai::kstream*>* _io__raw_payload() const { return m__io__raw_payload; }
};

#endif  // UCAPI_H_
