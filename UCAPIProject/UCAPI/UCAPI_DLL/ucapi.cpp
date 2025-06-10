// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

#include "pch.h"
#include <iostream>
#include "ucapi.h"

ucapi_t::ucapi_t(const void* dataPtr){
    m_magic = 0x55AA;
	m_version = 0;
	m_num_payload = 1;
	m_payload.clear();
	m_payload.reserve(m_num_payload);

	if (dataPtr == nullptr) {
		m_crc16 = 0;
		return;
	}

    try {
        _read(dataPtr);
		m_crc16 = computeCRC16(reinterpret_cast<record_t*>(m_payload.data()), sizeof(record_t) * m_num_payload);

    } catch(...) {
        _clean_up();
        throw;
    }
}

void ucapi_t::_read(const void* dataPtr) {
    try {
		const uint8_t* data = reinterpret_cast<const uint8_t*>(dataPtr);
		m_magic = (data[1] << 8) | data[0];
		m_version = (data[3] << 8) | data[2];
		m_num_payload = (data[5] << 8) | data[4];
		m_crc16 = (data[7] << 8) | data[6];

		auto payloadLength = sizeof(record_t);

		size_t payloadSize = m_num_payload * payloadLength;

		if (payloadSize == 0) {
			return;
		}

		if (payloadSize > 0x10000) {
			throw std::runtime_error("Invalid payload size");
		}

		m_payload.clear();
		m_payload.reserve(m_num_payload);
		auto payloadArray = new std::vector<record_t>();
		for (int i = 0; i < m_num_payload; i++) {
			m_payload.emplace_back(payloadLength, &data[10 + i * payloadLength]);
		}
	}
	catch (std::exception& e) {
		_clean_up();
		std::cerr << e.what() << std::endl;
		throw;
    }
}

ucapi_t::~ucapi_t() {
    _clean_up();
}

void ucapi_t::_clean_up() {
	m_payload.clear();
}

uint16_t ucapi_t::computeCRC16(record_t* record, size_t length, uint16_t poly, uint16_t initValue) {
	const uint8_t* data = reinterpret_cast<const uint8_t*>(record);
	uint16_t crc = initValue;
	for (size_t i = 0; i < length; ++i) {
		crc ^= static_cast<uint16_t>(data[i]) << 8;
		for (int j = 0; j < 8; ++j) {
			if (crc & 0x8000) {
				crc = (crc << 1) ^ poly;
			}
			else {
				crc <<= 1;
			}
		}
	}
	return crc;
}

ucapi_t::record_t::record_t(size_t payload_length, const void* dataPtr) {
	m_camera_no = 0;
	m_commands = 0;
	memset(m_timecode, 0, sizeof(m_timecode));
	m_subframe = 0;
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
	uint8_t* reserved = new uint8_t[25];

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

	m_camera_no = *reinterpret_cast<const uint32_t*>(&data[0]);
	m_commands = *reinterpret_cast<const uint16_t*>(&data[4]);
	memcpy(m_timecode, &data[6], 10);
	m_subframe = *reinterpret_cast<const uint32_t*>(&data[16]);
	m_packet_no = *reinterpret_cast<const uint8_t*>(&data[20]);
	m_eye_position_right_m = *reinterpret_cast<const float*>(&data[21]);
	m_eye_position_up_m = *reinterpret_cast<const float*>(&data[25]);
	m_eye_position_forward_m = *reinterpret_cast<const float*>(&data[29]);
	m_look_vector_right_m = *reinterpret_cast<const float*>(&data[33]);
	m_look_vector_up_m = *reinterpret_cast<const float*>(&data[37]);
	m_look_vector_forward_m = *reinterpret_cast<const float*>(&data[41]);
	m_up_vector_right_m = *reinterpret_cast<const float*>(&data[45]);
	m_up_vector_up_m = *reinterpret_cast<const float*>(&data[49]);
	m_up_vector_forward_m = *reinterpret_cast<const float*>(&data[53]);
	m_focal_length_mm = *reinterpret_cast<const float*>(&data[57]);
	m_aspect_ratio = *reinterpret_cast<const float*>(&data[61]);
	m_focus_distance_m = *reinterpret_cast<const float*>(&data[65]);
	m_aperture = *reinterpret_cast<const float*>(&data[69]);
	m_sensor_size_width_mm = *reinterpret_cast<const float*>(&data[73]);
	m_sensor_size_height_mm = *reinterpret_cast<const float*>(&data[77]);
	m_near_clip_m = *reinterpret_cast<const float*>(&data[81]);
	m_far_clip_m = *reinterpret_cast<const float*>(&data[85]);
	m_lens_shift_horizontal_ratio = *reinterpret_cast<const float*>(&data[89]);
	m_lens_shift_vertical_ratio = *reinterpret_cast<const float*>(&data[93]);
	m_lens_distortion_radial_coefficients_k1 = *reinterpret_cast<const float*>(&data[97]);
	m_lens_distortion_radial_coefficients_k2 = *reinterpret_cast<const float*>(&data[101]);
	m_lens_distortion_center_point_right_mm = *reinterpret_cast<const float*>(&data[105]);
	m_lens_distortion_center_point_up_mm = *reinterpret_cast<const float*>(&data[109]);
}

ucapi_t::record_t::~record_t() {
    _clean_up();
}

void ucapi_t::record_t::_clean_up() {
	
}
