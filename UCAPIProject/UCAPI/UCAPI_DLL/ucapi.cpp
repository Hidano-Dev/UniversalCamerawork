// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

#include "pch.h"
#include "ucapi.h"
#include <iostream>

ucapi_t::ucapi_t(kaitai::kstream* p__io, kaitai::kstruct* p__parent, ucapi_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = this;
    m_payload = 0;
    m__raw_payload = 0;
    m__io__raw_payload = 0;

    try {
        _read();
    } catch(...) {
        _clean_up();
        throw;
    }
}

void ucapi_t::_read() {
    try {
        m_magic = m__io->read_u2le();
        m_version = m__io->read_u2le();
        m_num_payload = m__io->read_u2le();
        m_payload_length = m__io->read_u2le();
        m_crc16 = m__io->read_u2le();
        m__raw_payload = new std::vector<std::string>();
        m__io__raw_payload = new std::vector<kaitai::kstream*>();
        m_payload = new std::vector<record_t*>();
        const int l_payload = num_payload();
        for (int i = 0; i < l_payload; i++) {
            m__raw_payload->push_back(m__io->read_bytes((m_num_payload * m_payload_length)));
            kaitai::kstream* io__raw_payload = new kaitai::kstream(m__raw_payload->at(m__raw_payload->size() - 1));
            m__io__raw_payload->push_back(io__raw_payload);
            m_payload->push_back(new record_t(io__raw_payload, this, m__root));
		}
	}
	catch (std::exception& e) {
		_clean_up();
		// Print the exception message to the console.
		std::cerr << e.what() << std::endl;
		throw;
    }
}

ucapi_t::~ucapi_t() {
    _clean_up();
}

void ucapi_t::_clean_up() {
    if (m__raw_payload) {
        delete m__raw_payload; m__raw_payload = 0;
    }
    if (m__io__raw_payload) {
        for (std::vector<kaitai::kstream*>::iterator it = m__io__raw_payload->begin(); it != m__io__raw_payload->end(); ++it) {
            delete *it;
        }
        delete m__io__raw_payload; m__io__raw_payload = 0;
    }
    if (m_payload) {
        for (std::vector<record_t*>::iterator it = m_payload->begin(); it != m_payload->end(); ++it) {
            delete *it;
        }
        delete m_payload; m_payload = 0;
    }
}

ucapi_t::timecode_t::timecode_t(kaitai::kstream* p__io, ucapi_t::record_t* p__parent, ucapi_t* p__root) : kaitai::kstruct(p__io) {
    m__parent = p__parent;
    m__root = p__root;

    try {
        _read();
    } catch(...) {
        _clean_up();
        throw;
    }
}

void ucapi_t::timecode_t::_read() {
    m_frame_number = m__io->read_bits_int_be(7);
    m_second_number = m__io->read_bits_int_be(6);
    m_minute_number = m__io->read_bits_int_be(6);
    m_hour_number = m__io->read_bits_int_be(5);
    m_reserved = m__io->read_bits_int_be(8);
}

ucapi_t::timecode_t::~timecode_t() {
    _clean_up();
}

void ucapi_t::timecode_t::_clean_up() {
}

ucapi_t::record_t::record_t(kaitai::kstream* p__io, ucapi_t* p__parent, ucapi_t* p__root) : kaitai::kstruct(p__io) {
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

void ucapi_t::record_t::_read() {
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

ucapi_t::record_t::~record_t() {
    _clean_up();
}

void ucapi_t::record_t::_clean_up() {
    if (m_timecode) {
        delete m_timecode; m_timecode = 0;
    }
    if (m_reserved) {
        delete m_reserved; m_reserved = 0;
    }
}
