#ifndef UCAPI_H_
#define UCAPI_H_

// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

#include "kaitai/kaitaistruct.h"
#include <stdint.h>
#include <vector>

#if KAITAI_STRUCT_VERSION < 9000L
#error "Incompatible Kaitai Struct C++/STL API: version 0.9 or later is required"
#endif

class ucapi_serialization_t : public kaitai::kstruct {

public:
    class header_t;
    class record_t;
    class timecode_t;

    ucapi_serialization_t(kaitai::kstream* p__io, kaitai::kstruct* p__parent = 0, ucapi_serialization_t* p__root = 0);

private:
    void _read();
    void _clean_up();

public:
    ~ucapi_serialization_t();

    class header_t : public kaitai::kstruct {

    public:

        header_t(kaitai::kstream* p__io, ucapi_serialization_t* p__parent = 0, ucapi_serialization_t* p__root = 0);

    private:
        void _read();
        void _clean_up();

    public:
        ~header_t();

    private:
        std::string m_signature;
        uint8_t m_version_major;
        uint8_t m_version_minor;
        std::vector<uint8_t>* m_reserved;
        uint32_t m_record_count;
        uint32_t m_checksum;
        ucapi_serialization_t* m__root;
        ucapi_serialization_t* m__parent;

    public:
        std::string signature() const { return m_signature; }
        uint8_t version_major() const { return m_version_major; }
        uint8_t version_minor() const { return m_version_minor; }
        std::vector<uint8_t>* reserved() const { return m_reserved; }
        uint32_t record_count() const { return m_record_count; }
        uint32_t checksum() const { return m_checksum; }
        ucapi_serialization_t* _root() const { return m__root; }
        ucapi_serialization_t* _parent() const { return m__parent; }
    };

    class record_t : public kaitai::kstruct {

    public:

        record_t(kaitai::kstream* p__io, ucapi_serialization_t* p__parent = 0, ucapi_serialization_t* p__root = 0);

    private:
        void _read();
        void _clean_up();

    public:
        ~record_t();

    private:
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
        ucapi_serialization_t* m__root;
        ucapi_serialization_t* m__parent;

    public:
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
        ucapi_serialization_t* _root() const { return m__root; }
        ucapi_serialization_t* _parent() const { return m__parent; }
    };

    class timecode_t : public kaitai::kstruct {

    public:

        timecode_t(kaitai::kstream* p__io, ucapi_serialization_t::record_t* p__parent = 0, ucapi_serialization_t* p__root = 0);

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
        ucapi_serialization_t* m__root;
        ucapi_serialization_t::record_t* m__parent;

    public:
        uint64_t frame_number() const { return m_frame_number; }
        uint64_t second_number() const { return m_second_number; }
        uint64_t minute_number() const { return m_minute_number; }
        uint64_t hour_number() const { return m_hour_number; }
        uint64_t reserved() const { return m_reserved; }
        ucapi_serialization_t* _root() const { return m__root; }
        ucapi_serialization_t::record_t* _parent() const { return m__parent; }
    };

private:
    header_t* m_header;
    std::vector<record_t*>* m_records;
    ucapi_serialization_t* m__root;
    kaitai::kstruct* m__parent;

public:
    header_t* header() const { return m_header; }
    std::vector<record_t*>* records() const { return m_records; }
    ucapi_serialization_t* _root() const { return m__root; }
    kaitai::kstruct* _parent() const { return m__parent; }
};

#endif  // UCAPI_H_
