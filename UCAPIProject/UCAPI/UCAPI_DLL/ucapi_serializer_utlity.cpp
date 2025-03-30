#include "pch.h"
#include "ucapi_serializer_utility.h"
#include <cstring>
#include "ucapi.h"

void write_ucapi(const ucapi_t* obj, std::ostream& os) {
	os.write(std::to_string(obj->m_magic).c_str(), 2);
	os.write(std::to_string(obj->m_version).c_str(), 2);
	os.write(std::to_string(obj->m_num_payload).c_str(), 2);
	os.write(std::to_string(obj->m_payload_length).c_str(), 2);
	os.write(std::to_string(obj->m_crc16).c_str(), 2);

	auto itr = obj->m_payload->begin();
	for (int i = 0; i < obj->m_num_payload; i++) {
		auto rec = *itr;
		os.write(std::to_string(rec->m_camera_no).c_str(), 4);
		os.write(std::to_string(rec->m_commands).c_str(), 2);

		auto timecode = rec->m_timecode;
		uint8_t timecode_data[3] = { 0 };
		timecode_data[0] = (timecode->m_frame_number & 0x7F);
		timecode_data[1] = ((timecode->m_second_number & 0x3F) << 2) | ((timecode->m_minute_number & 0x30) >> 4);
		timecode_data[2] = ((timecode->m_minute_number & 0x0F) << 4) | (timecode->m_hour_number & 0x1F);
		os.write(reinterpret_cast<char*>(timecode_data), 3);
		os.write(std::to_string(rec->m_packet_no).c_str(), sizeof(rec->m_packet_no));
		os.write(std::to_string(rec->m_eye_position_right_m).c_str(), sizeof(rec->m_eye_position_right_m));
		os.write(std::to_string(rec->m_eye_position_up_m).c_str(), sizeof(rec->m_eye_position_up_m));
		os.write(std::to_string(rec->m_eye_position_forward_m).c_str(), sizeof(rec->m_eye_position_forward_m));
		os.write(std::to_string(rec->m_look_vector_right_m).c_str(), sizeof(rec->m_look_vector_right_m));
		os.write(std::to_string(rec->m_look_vector_up_m).c_str(), sizeof(rec->m_look_vector_up_m));
		os.write(std::to_string(rec->m_look_vector_forward_m).c_str(), sizeof(rec->m_look_vector_forward_m));
		os.write(std::to_string(rec->m_up_vector_right_m).c_str(), sizeof(rec->m_up_vector_right_m));
		os.write(std::to_string(rec->m_up_vector_up_m).c_str(), sizeof(rec->m_up_vector_up_m));
		os.write(std::to_string(rec->m_up_vector_forward_m).c_str(), sizeof(rec->m_up_vector_forward_m));
		os.write(std::to_string(rec->m_focal_length_mm).c_str(), sizeof(rec->m_focal_length_mm));
		os.write(std::to_string(rec->m_aspect_ratio).c_str(), sizeof(rec->m_aspect_ratio));
		os.write(std::to_string(rec->m_focus_distance_m).c_str(), sizeof(rec->m_focus_distance_m));
		os.write(std::to_string(rec->m_aperture).c_str(), sizeof(rec->m_aperture));
		os.write(std::to_string(rec->m_sensor_size_width_mm).c_str(), sizeof(rec->m_sensor_size_width_mm));
		os.write(std::to_string(rec->m_sensor_size_height_mm).c_str(), sizeof(rec->m_sensor_size_height_mm));
		os.write(std::to_string(rec->m_near_clip_m).c_str(), sizeof(rec->m_near_clip_m));
		os.write(std::to_string(rec->m_far_clip_m).c_str(), sizeof(rec->m_far_clip_m));
		os.write(std::to_string(rec->m_lens_shift_horizontal_ratio).c_str(), sizeof(rec->m_lens_shift_horizontal_ratio));
		os.write(std::to_string(rec->m_lens_shift_vertical_ratio).c_str(), sizeof(rec->m_lens_shift_vertical_ratio));
		os.write(std::to_string(rec->m_lens_distortion_radial_coefficients_k1).c_str(), sizeof(rec->m_lens_distortion_radial_coefficients_k1));
		os.write(std::to_string(rec->m_lens_distortion_radial_coefficients_k2).c_str(), sizeof(rec->m_lens_distortion_radial_coefficients_k2));
		os.write(std::to_string(rec->m_lens_distortion_center_point_right_mm).c_str(), sizeof(rec->m_lens_distortion_center_point_right_mm));
		os.write(std::to_string(rec->m_lens_distortion_center_point_up_mm).c_str(), sizeof(rec->m_lens_distortion_center_point_up_mm));
		os.write(reinterpret_cast<const char*>(rec->m_reserved->data()), 25);
		itr++;
	}
}
