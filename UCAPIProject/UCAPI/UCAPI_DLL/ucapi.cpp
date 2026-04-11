#include "pch.h"
#include "ucapi.h"

ucapi_t::ucapi_t() {
    m_magic = UCAPI_MAGIC;
	m_version = 0;
	m_num_payload = 0;
	m_crc16 = 0;
	m_payload.clear();
}

ucapi_t::~ucapi_t() {
    _clean_up();
}

void ucapi_t::_clean_up() {
	m_payload.clear();
}

uint16_t ucapi_t::computeCRC16(record_t* record, size_t length, uint16_t poly, uint16_t initValue) {
    // Early return for empty input to avoid potential ASan warnings on null pointer cast
    if (length == 0) {
        return initValue;
    }
    if (record == nullptr) {
        return initValue;
    }
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

ucapi_t::record_t::record_t() {
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
}

ucapi_t::record_t::~record_t() {
    _clean_up();
}

void ucapi_t::record_t::_clean_up() {
	// Currently no dynamically allocated resources to clean up.
	// All members are POD types (uint32_t, uint16_t, uint8_t, float).
	// If dynamic resources are added in the future, release them here.
}
