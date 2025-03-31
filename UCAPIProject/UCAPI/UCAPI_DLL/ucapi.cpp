// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

#include "pch.h"
#include "ucapi.h"
#include <iostream>

ucapi_t::ucapi_t(const void* dataPtr){
	// magicは0x5543で初期化
	m_magic = 0x5543;
	m_version = 0;
	m_num_payload = 1;
	m_payload_length = 128;
	m_crc16 = 0;
	m_payload = new std::vector<record_t*>();
	m__raw_payload = new std::vector<std::string>();

	if (dataPtr == nullptr) {
		m_payload->push_back(new record_t(m_payload_length));
		return;
	}

    try {
        _read(dataPtr);
    } catch(...) {
        _clean_up();
        throw;
    }
}

void ucapi_t::_read(const void* dataPtr) {
    try {
		// 全体のバイト数を把握するため、最初の10バイトを読み取る
		const uint8_t* data = reinterpret_cast<const uint8_t*>(dataPtr);
		m_magic = (data[1] << 8) | data[0];
		m_version = (data[3] << 8) | data[2];
		m_num_payload = (data[5] << 8) | data[4];
		m_payload_length = (data[7] << 8) | data[6];
		m_crc16 = (data[9] << 8) | data[8];

		// ペイロードのバイト数を計算する
		size_t payloadSize = m_num_payload * m_payload_length;

		// ペイロードのバイト数が0の場合は読み取らない
		if (payloadSize == 0) {
			return;
		}

		// ペイロードのバイト数が不正な場合は例外をスローする
		if (payloadSize > 0x10000) {
			throw std::runtime_error("Invalid payload size");
		}

		// ペイロードのバイト数を読み取る
		m__raw_payload = new std::vector<std::string>();
        m__raw_payload->push_back(std::string(reinterpret_cast<const char*>(&data[10]), payloadSize));

		m_payload = new std::vector<record_t*>();
		for (int i = 0; i < m_num_payload; i++) {
			m_payload->push_back(new record_t((size_t)m_payload_length, &data[10 + i * m_payload_length]));
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
    if (m_payload) {
        for (std::vector<record_t*>::iterator it = m_payload->begin(); it != m_payload->end(); ++it) {
            delete *it;
        }
        delete m_payload; m_payload = 0;
    }
}

ucapi_t::timecode_t::timecode_t(const void* dataPtr) {
	m_frame_number = 0;
	m_second_number = 0;
	m_minute_number = 0;
	m_hour_number = 0;
	m_reserved = 0;

	if (dataPtr == nullptr) {
		return;
	}

    try {
        _read(dataPtr);
    } catch(...) {
        _clean_up();
        throw;
    }
}

void ucapi_t::timecode_t::_read(const void* dataPtr) {
	const uint8_t* data = reinterpret_cast<const uint8_t*>(dataPtr);

	// 4バイトのデータを読み取る
	m_frame_number = (data[0] & 0x7F);
	m_second_number = ((data[0] & 0x80) >> 7) | ((data[1] & 0x3F) << 1);
	m_minute_number = ((data[1] & 0xC0) >> 6) | ((data[2] & 0x0F) << 2);
	m_hour_number = (data[2] & 0xF0) >> 4;
	m_reserved = data[3];
}

ucapi_t::timecode_t::~timecode_t() {
    _clean_up();
}

void ucapi_t::timecode_t::_clean_up() {
}

ucapi_t::record_t::record_t(size_t payload_length, const void* dataPtr) {
	m_camera_no = 0;
	m_commands = 0;
	m_timecode = new timecode_t();
	m_packet_no = 0;
	m_eye_position_right_m = 0;
	m_eye_position_up_m = 0;
	m_eye_position_forward_m = 0;
	m_look_vector_right_m = 0;
	m_look_vector_up_m = 0;
	m_look_vector_forward_m = 0;
	m_up_vector_right_m = 0;
	m_up_vector_up_m = 0;
	m_up_vector_forward_m = 0;
	m_focal_length_mm = 0;
	m_aspect_ratio = 0;
	m_focus_distance_m = 0;
	m_aperture = 0;
	m_sensor_size_width_mm = 0;
	m_sensor_size_height_mm = 0;
	m_near_clip_m = 0;
	m_far_clip_m = 0;
	m_lens_shift_horizontal_ratio = 0;
	m_lens_shift_vertical_ratio = 0;
	m_lens_distortion_radial_coefficients_k1 = 0;
	m_lens_distortion_radial_coefficients_k2 = 0;
	m_lens_distortion_center_point_right_mm = 0;
	m_lens_distortion_center_point_up_mm = 0;
	m_reserved = new std::vector<uint8_t>();

	for (int i = 0; i < 25; i++) {
		m_reserved->push_back(0);
	}

	if (dataPtr == nullptr) {
		return;
	}

    try {
        _read(dataPtr, payload_length);
    } catch(...) {
        _clean_up();
        throw;
    }
}

void ucapi_t::record_t::_read(const void* dataPtr, size_t payload_length = 0) {
	const uint8_t* data = reinterpret_cast<const uint8_t*>(dataPtr);

	// ペイロードのバイト数分のデータを読み取る
	m_camera_no = (data[3] << 24) | (data[2] << 16) | (data[1] << 8) | data[0];
	m_commands = (data[5] << 8) | data[4];
	m_timecode = new timecode_t(&data[6]);
	m_packet_no = data[10];
	m_eye_position_right_m = *reinterpret_cast<const float*>(&data[11]);
	m_eye_position_up_m = *reinterpret_cast<const float*>(&data[15]);
	m_eye_position_forward_m = *reinterpret_cast<const float*>(&data[19]);
	m_look_vector_right_m = *reinterpret_cast<const float*>(&data[23]);
	m_look_vector_up_m = *reinterpret_cast<const float*>(&data[27]);
	m_look_vector_forward_m = *reinterpret_cast<const float*>(&data[31]);
	m_up_vector_right_m = *reinterpret_cast<const float*>(&data[35]);
	m_up_vector_up_m = *reinterpret_cast<const float*>(&data[39]);
	m_up_vector_forward_m = *reinterpret_cast<const float*>(&data[43]);
	m_focal_length_mm = *reinterpret_cast<const float*>(&data[47]);
	m_aspect_ratio = *reinterpret_cast<const float*>(&data[51]);
	m_focus_distance_m = *reinterpret_cast<const float*>(&data[55]);
	m_aperture = *reinterpret_cast<const float*>(&data[59]);
	m_sensor_size_width_mm = *reinterpret_cast<const float*>(&data[63]);
	m_sensor_size_height_mm = *reinterpret_cast<const float*>(&data[67]);
	m_near_clip_m = *reinterpret_cast<const float*>(&data[71]);
	m_far_clip_m = *reinterpret_cast<const float*>(&data[75]);
	m_lens_shift_horizontal_ratio = *reinterpret_cast<const float*>(&data[79]);
	m_lens_shift_vertical_ratio = *reinterpret_cast<const float*>(&data[83]);
	m_lens_distortion_radial_coefficients_k1 = *reinterpret_cast<const float*>(&data[87]);
	m_lens_distortion_radial_coefficients_k2 = *reinterpret_cast<const float*>(&data[91]);
	m_lens_distortion_center_point_right_mm = *reinterpret_cast<const float*>(&data[95]);
	m_lens_distortion_center_point_up_mm = *reinterpret_cast<const float*>(&data[99]);
	m_reserved = new std::vector<uint8_t>();
	for (int i = 0; i < 25; i++) {
		m_reserved->push_back(data[103 + i]);
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
