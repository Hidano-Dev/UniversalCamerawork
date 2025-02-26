// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

#include "pch.h"
#include "ucapi_deserialization.h"
#include "kaitai/exceptions.h"

ucapi_serialization_t::ucapi_serialization_t(kaitai::kstream* p__io, kaitai::kstruct* p__parent, ucapi_serialization_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = this;
    m_header = 0;
    m_records = 0;

    try {
        _read();
    } catch(...) {
        _clean_up();
        throw;
    }
}

void ucapi_serialization_t::_read() {
    m_header = new header_t(m__io, this, m__root);
    m_records = new std::vector<record_t*>();
    const int l_records = header()->record_count();
    for (int i = 0; i < l_records; i++) {
        m_records->push_back(new record_t(m__io, this, m__root));
    }
}

ucapi_serialization_t::~ucapi_serialization_t() {
    _clean_up();
}

void ucapi_serialization_t::_clean_up() {
    if (m_header) {
        delete m_header; m_header = 0;
    }
    if (m_records) {
        for (std::vector<record_t*>::iterator it = m_records->begin(); it != m_records->end(); ++it) {
            delete *it;
        }
        delete m_records; m_records = 0;
    }
}

ucapi_serialization_t::header_t::header_t(kaitai::kstream* p__io, ucapi_serialization_t* p__parent, ucapi_serialization_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;
    m_reserved = 0;

    try {
        _read();
    } catch(...) {
        _clean_up();
        throw;
    }
}

void ucapi_serialization_t::header_t::_read() {
    m_signature = m__io->read_bytes(5);
    if (!(signature() == std::string("\x55\x43\x41\x50\x49", 5))) {
        throw kaitai::validation_not_equal_error<std::string>(std::string("\x55\x43\x41\x50\x49", 5), signature(), _io(), std::string("/types/header/seq/0"));
    }
    m_version_major = m__io->read_u1();
    m_version_minor = m__io->read_u1();
    m_reserved = new std::vector<uint8_t>();
    const int l_reserved = 17;
    for (int i = 0; i < l_reserved; i++) {
        m_reserved->push_back(m__io->read_u1());
    }
    m_record_count = m__io->read_u4le();
    m_checksum = m__io->read_u4le();
}

ucapi_serialization_t::header_t::~header_t() {
    _clean_up();
}

void ucapi_serialization_t::header_t::_clean_up() {
    if (m_reserved) {
        delete m_reserved; m_reserved = 0;
    }
}

ucapi_serialization_t::record_t::record_t(kaitai::kstream* p__io, ucapi_serialization_t* p__parent, ucapi_serialization_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;
    m_timecode = 0;
    m_reserved = 0;

    try {
        _read();
    } catch(...) {
        _clean_up();
        throw;
    }
}

void ucapi_serialization_t::record_t::_read() {
	m_camera_no = m__io->read_u4le();
    m_commands = m__io->read_u2le();
    m_timecode = new timecode_t(m__io, this, m__root);
    m_packet_no = m__io->read_u1();
    m_eye_position_right_m = m__io->read_f4le();
    m_eye_position_up_m = m__io->read_f4le();
    m_eye_position_forward_m = m__io->read_f4le();
    m_look_vector_right_m = m__io->read_f4le();
    m_look_vector_up_m = m__io->read_f4le();
    m_look_vector_forward_m = m__io->read_f4le();
    m_up_vector_right_m = m__io->read_f4le();
    m_up_vector_up_m = m__io->read_f4le();
    m_up_vector_forward_m = m__io->read_f4le();
    m_focal_length_mm = m__io->read_f4le();
    m_aspect_ratio = m__io->read_f4le();
    m_focus_distance_m = m__io->read_f4le();
    m_aperture = m__io->read_f4le();
    m_sensor_size_width_mm = m__io->read_f4le();
    m_sensor_size_height_mm = m__io->read_f4le();
    m_near_clip_m = m__io->read_f4le();
    m_far_clip_m = m__io->read_f4le();
    m_lens_shift_horizontal_ratio = m__io->read_f4le();
    m_lens_shift_vertical_ratio = m__io->read_f4le();
    m_lens_distortion_radial_coefficients_k1 = m__io->read_f4le();
    m_lens_distortion_radial_coefficients_k2 = m__io->read_f4le();
    m_lens_distortion_center_point_right_mm = m__io->read_f4le();
    m_lens_distortion_center_point_up_mm = m__io->read_f4le();
    m_reserved = new std::vector<uint8_t>();
    const int l_reserved = 25;
    for (int i = 0; i < l_reserved; i++) {
        m_reserved->push_back(m__io->read_u1());
    }
}

ucapi_serialization_t::record_t::~record_t() {
    _clean_up();
}

void ucapi_serialization_t::record_t::_clean_up() {
    if (m_timecode) {
        delete m_timecode; m_timecode = 0;
    }
    if (m_reserved) {
        delete m_reserved; m_reserved = 0;
    }
}

ucapi_serialization_t::timecode_t::timecode_t(kaitai::kstream* p__io, ucapi_serialization_t::record_t* p__parent, ucapi_serialization_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;

    try {
        _read();
    } catch(...) {
        _clean_up();
        throw;
    }
}

void ucapi_serialization_t::timecode_t::_read() {
    m_frame_number = m__io->read_bits_int_be(7);
    m_second_number = m__io->read_bits_int_be(6);
    m_minute_number = m__io->read_bits_int_be(6);
    m_hour_number = m__io->read_bits_int_be(5);
    m_reserved = m__io->read_bits_int_be(8);
}

ucapi_serialization_t::timecode_t::~timecode_t() {
    _clean_up();
}

void ucapi_serialization_t::timecode_t::_clean_up() {
}
