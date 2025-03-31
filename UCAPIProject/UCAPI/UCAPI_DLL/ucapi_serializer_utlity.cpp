#include "pch.h"
#include "ucapi_serializer_utility.h"
#include <cstring>
#include "ucapi.h"

void write_ucapi(const ucapi_t* obj, std::ostream& os) {
	os.write(reinterpret_cast<const char*>(&obj->m_magic), sizeof(obj->m_magic));
	os.write(reinterpret_cast<const char*>(&obj->m_version), sizeof(obj->m_version));
	os.write(reinterpret_cast<const char*>(&obj->m_num_payload), sizeof(obj->m_num_payload));
	os.write(reinterpret_cast<const char*>(&obj->m_payload_length), sizeof(obj->m_payload_length));
	os.write(reinterpret_cast<const char*>(&obj->m_crc16), sizeof(obj->m_crc16));

	auto itr = obj->m_payload->begin();
	for (int i = 0; i < obj->m_num_payload; i++) {
		auto rec = *itr;
		os.write(std::to_string(rec->m_camera_no).c_str(), 4);
		os.write(std::to_string(rec->m_commands).c_str(), 2);

		auto timecode = rec->m_timecode;
		uint8_t timecode_data[4] = { 0 };
		timecode_data[0] = (timecode->m_frame_number & 0x7F);
		timecode_data[1] = ((timecode->m_second_number & 0x3F) << 2) | ((timecode->m_minute_number & 0x30) >> 4);
		timecode_data[2] = ((timecode->m_minute_number & 0x0F) << 4) | (timecode->m_hour_number & 0x1F);
		timecode_data[3] = timecode->m_reserved;
		os.write(reinterpret_cast<char*>(timecode_data), 4);
		os.write(reinterpret_cast<const char*>(&rec->m_packet_no), sizeof(rec->m_packet_no));
		os.write(reinterpret_cast<const char*>(&rec->m_eye_position_right_m), sizeof(rec->m_eye_position_right_m));
		os.write(reinterpret_cast<const char*>(&rec->m_eye_position_up_m), sizeof(rec->m_eye_position_up_m));
		os.write(reinterpret_cast<const char*>(&rec->m_eye_position_forward_m), sizeof(rec->m_eye_position_forward_m));
		os.write(reinterpret_cast<const char*>(&rec->m_look_vector_right_m), sizeof(rec->m_look_vector_right_m));
		os.write(reinterpret_cast<const char*>(&rec->m_look_vector_up_m), sizeof(rec->m_look_vector_up_m));
		os.write(reinterpret_cast<const char*>(&rec->m_look_vector_forward_m), sizeof(rec->m_look_vector_forward_m));
		os.write(reinterpret_cast<const char*>(&rec->m_up_vector_right_m), sizeof(rec->m_up_vector_right_m));
		os.write(reinterpret_cast<const char*>(&rec->m_up_vector_up_m), sizeof(rec->m_up_vector_up_m));
		os.write(reinterpret_cast<const char*>(&rec->m_up_vector_forward_m), sizeof(rec->m_up_vector_forward_m));
		os.write(reinterpret_cast<const char*>(&rec->m_focal_length_mm), sizeof(rec->m_focal_length_mm));
		os.write(reinterpret_cast<const char*>(&rec->m_aspect_ratio), sizeof(rec->m_aspect_ratio));
		os.write(reinterpret_cast<const char*>(&rec->m_focus_distance_m), sizeof(rec->m_focus_distance_m));
		os.write(reinterpret_cast<const char*>(&rec->m_aperture), sizeof(rec->m_aperture));
		os.write(reinterpret_cast<const char*>(&rec->m_sensor_size_width_mm), sizeof(rec->m_sensor_size_width_mm));
		os.write(reinterpret_cast<const char*>(&rec->m_sensor_size_height_mm), sizeof(rec->m_sensor_size_height_mm));
		os.write(reinterpret_cast<const char*>(&rec->m_near_clip_m), sizeof(rec->m_near_clip_m));
		os.write(reinterpret_cast<const char*>(&rec->m_far_clip_m), sizeof(rec->m_far_clip_m));
		os.write(reinterpret_cast<const char*>(&rec->m_lens_shift_horizontal_ratio), sizeof(rec->m_lens_shift_horizontal_ratio));
		os.write(reinterpret_cast<const char*>(&rec->m_lens_shift_vertical_ratio), sizeof(rec->m_lens_shift_vertical_ratio));
		os.write(reinterpret_cast<const char*>(&rec->m_lens_distortion_radial_coefficients_k1), sizeof(rec->m_lens_distortion_radial_coefficients_k1));
		os.write(reinterpret_cast<const char*>(&rec->m_lens_distortion_radial_coefficients_k2), sizeof(rec->m_lens_distortion_radial_coefficients_k2));
		os.write(reinterpret_cast<const char*>(&rec->m_lens_distortion_center_point_right_mm), sizeof(rec->m_lens_distortion_center_point_right_mm));
		os.write(reinterpret_cast<const char*>(&rec->m_lens_distortion_center_point_up_mm), sizeof(rec->m_lens_distortion_center_point_up_mm));
		// m_reserved‚Ì•ª‚Í0‚Å–„‚ß‚é
		os.write(std::string(rec->m_reserved->size(), 0).c_str(), rec->m_reserved->size());
		itr++;
	}
}
