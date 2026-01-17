// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

#include "pch.h"
#include <iostream>
#include "ucapi.h"

ucapi_t::ucapi_t(const void* dataPtr, size_t bufferSize){
    // magic is initialized with UCAPI_MAGIC (0x55AA)
    m_magic = UCAPI_MAGIC;
	m_version = 0;
	// Default to no payload when created without data
	m_num_payload = 0;
	m_payload.clear();
	m_payload.reserve(m_num_payload);

	if (dataPtr == nullptr || bufferSize == 0) {
		m_crc16 = 0;
		return;
	}

    try {
        _read(dataPtr, bufferSize);
		m_crc16 = computeCRC16(reinterpret_cast<record_t*>(m_payload.data()), sizeof(record_t) * m_num_payload);

    } catch(...) {
        _clean_up();
        throw;
    }
}

void ucapi_t::_read(const void* dataPtr, size_t bufferSize) {
    try {
		// Validate minimum buffer size for header
		if (bufferSize < UCAPI_HEADER_SIZE) {
			throw std::runtime_error("Buffer too small for header");
		}

		const uint8_t* data = reinterpret_cast<const uint8_t*>(dataPtr);
		m_magic = (data[1] << 8) | data[0];
		m_version = (data[3] << 8) | data[2];
		m_num_payload = (data[5] << 8) | data[4];
		m_crc16 = (data[7] << 8) | data[6];

		auto payloadLength = sizeof(record_t);

		// Calculate payload byte size
		size_t payloadSize = m_num_payload * payloadLength;

		// Return early if no payload
		if (payloadSize == 0) {
			return;
		}

		// Reject invalid payload size
		if (payloadSize > UCAPI_MAX_PAYLOAD_SIZE) {
			throw std::runtime_error("Invalid payload size");
		}

		// Validate buffer size for all payloads
		size_t requiredSize = UCAPI_HEADER_SIZE + payloadSize;
		if (bufferSize < requiredSize) {
			throw std::runtime_error("Buffer too small for payloads");
		}

		m_payload.clear();
		m_payload.reserve(m_num_payload);
		for (int i = 0; i < m_num_payload; i++) {
			m_payload.emplace_back(payloadLength, &data[UCAPI_HEADER_SIZE + i * payloadLength]);
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
	m_payload.clear();
}

uint16_t ucapi_t::computeCRC16(record_t* record, size_t length, uint16_t poly, uint16_t initValue) {
    const uint8_t* data = reinterpret_cast<const uint8_t*>(record);
    uint16_t crc = initValue;
    for (size_t i = 0; i < length; ++i) {
        crc ^= static_cast<uint16_t>(data[i]) << UCAPI_BITS_PER_BYTE;
        for (int j = 0; j < UCAPI_BITS_PER_BYTE; ++j) {
            if (crc & UCAPI_CRC16_MSB_MASK) {
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
	m_timecode = 0;
	m_subframe = 0.0f;
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

void ucapi_t::record_t::_read(const void* dataPtr, size_t payload_length) {
	if (payload_length < UCAPI_MIN_RECORD_SIZE) {
		throw std::runtime_error("Payload buffer too small for record");
	}

	const uint8_t* data = reinterpret_cast<const uint8_t*>(dataPtr);

	m_camera_no = *reinterpret_cast<const uint32_t*>(&data[0]);
	m_commands = *reinterpret_cast<const uint16_t*>(&data[4]);
	m_timecode = *reinterpret_cast<const uint32_t*>(&data[6]);
	m_subframe = *reinterpret_cast<const float*>(&data[10]);
	m_packet_no = *reinterpret_cast<const uint8_t*>(&data[14]);
	m_eye_position_right_m = *reinterpret_cast<const float*>(&data[15]);
	m_eye_position_up_m = *reinterpret_cast<const float*>(&data[19]);
	m_eye_position_forward_m = *reinterpret_cast<const float*>(&data[23]);
	m_look_vector_right_m = *reinterpret_cast<const float*>(&data[27]);
	m_look_vector_up_m = *reinterpret_cast<const float*>(&data[31]);
	m_look_vector_forward_m = *reinterpret_cast<const float*>(&data[35]);
	m_up_vector_right_m = *reinterpret_cast<const float*>(&data[39]);
	m_up_vector_up_m = *reinterpret_cast<const float*>(&data[43]);
	m_up_vector_forward_m = *reinterpret_cast<const float*>(&data[47]);
	m_focal_length_mm = *reinterpret_cast<const float*>(&data[51]);
	m_aspect_ratio = *reinterpret_cast<const float*>(&data[55]);
	m_focus_distance_m = *reinterpret_cast<const float*>(&data[59]);
	m_aperture = *reinterpret_cast<const float*>(&data[63]);
	m_sensor_size_width_mm = *reinterpret_cast<const float*>(&data[67]);
	m_sensor_size_height_mm = *reinterpret_cast<const float*>(&data[71]);
	m_near_clip_m = *reinterpret_cast<const float*>(&data[75]);
	m_far_clip_m = *reinterpret_cast<const float*>(&data[79]);
	m_lens_shift_horizontal_ratio = *reinterpret_cast<const float*>(&data[83]);
	m_lens_shift_vertical_ratio = *reinterpret_cast<const float*>(&data[87]);
	m_lens_distortion_radial_coefficients_k1 = *reinterpret_cast<const float*>(&data[91]);
	m_lens_distortion_radial_coefficients_k2 = *reinterpret_cast<const float*>(&data[95]);
	m_lens_distortion_center_point_right_mm = *reinterpret_cast<const float*>(&data[99]);
	m_lens_distortion_center_point_up_mm = *reinterpret_cast<const float*>(&data[103]);
}

ucapi_t::record_t::~record_t() {
    _clean_up();
}

void ucapi_t::record_t::_clean_up() {
	// Currently no dynamically allocated resources to clean up.
	// All members are POD types (uint32_t, uint16_t, uint8_t, float).
	// If dynamic resources are added in the future, release them here.
}
