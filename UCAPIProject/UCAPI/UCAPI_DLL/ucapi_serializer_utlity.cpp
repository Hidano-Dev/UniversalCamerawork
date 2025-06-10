#include "pch.h"
#include "ucapi_serializer_utility.h"
#include <cstring>
#include "ucapi.h"

void write_ucapi(const ucapi_t* obj, std::ostream& os) {
	os.write(reinterpret_cast<const char*>(&obj->m_magic), sizeof(obj->m_magic));
	os.write(reinterpret_cast<const char*>(&obj->m_version), sizeof(obj->m_version));
	os.write(reinterpret_cast<const char*>(&obj->m_num_payload), sizeof(obj->m_num_payload));
	os.write(reinterpret_cast<const char*>(&obj->m_crc16), sizeof(obj->m_crc16));

	for (int i = 0; i < obj->m_num_payload; i++) {
		auto rec = obj->m_payload[i];
		os.write(reinterpret_cast<const char*>(&rec.m_camera_no), sizeof(rec.m_camera_no));
		os.write(reinterpret_cast<const char*>(&rec.m_commands), sizeof(rec.m_commands));

		os.write(reinterpret_cast<const char*>(rec.m_timecode.data), 10);
		os.write(reinterpret_cast<const char*>(&rec.m_subframe), sizeof(rec.m_subframe));
		os.write(reinterpret_cast<const char*>(&rec.m_packet_no), sizeof(rec.m_packet_no));
		os.write(reinterpret_cast<const char*>(&rec.m_eye_position_right_m), sizeof(rec.m_eye_position_right_m));
		os.write(reinterpret_cast<const char*>(&rec.m_eye_position_up_m), sizeof(rec.m_eye_position_up_m));
		os.write(reinterpret_cast<const char*>(&rec.m_eye_position_forward_m), sizeof(rec.m_eye_position_forward_m));
		os.write(reinterpret_cast<const char*>(&rec.m_look_vector_right_m), sizeof(rec.m_look_vector_right_m));
		os.write(reinterpret_cast<const char*>(&rec.m_look_vector_up_m), sizeof(rec.m_look_vector_up_m));
		os.write(reinterpret_cast<const char*>(&rec.m_look_vector_forward_m), sizeof(rec.m_look_vector_forward_m));
		os.write(reinterpret_cast<const char*>(&rec.m_up_vector_right_m), sizeof(rec.m_up_vector_right_m));
		os.write(reinterpret_cast<const char*>(&rec.m_up_vector_up_m), sizeof(rec.m_up_vector_up_m));
		os.write(reinterpret_cast<const char*>(&rec.m_up_vector_forward_m), sizeof(rec.m_up_vector_forward_m));
		os.write(reinterpret_cast<const char*>(&rec.m_focal_length_mm), sizeof(rec.m_focal_length_mm));
		os.write(reinterpret_cast<const char*>(&rec.m_aspect_ratio), sizeof(rec.m_aspect_ratio));
		os.write(reinterpret_cast<const char*>(&rec.m_focus_distance_m), sizeof(rec.m_focus_distance_m));
		os.write(reinterpret_cast<const char*>(&rec.m_aperture), sizeof(rec.m_aperture));
		os.write(reinterpret_cast<const char*>(&rec.m_sensor_size_width_mm), sizeof(rec.m_sensor_size_width_mm));
		os.write(reinterpret_cast<const char*>(&rec.m_sensor_size_height_mm), sizeof(rec.m_sensor_size_height_mm));
		os.write(reinterpret_cast<const char*>(&rec.m_near_clip_m), sizeof(rec.m_near_clip_m));
		os.write(reinterpret_cast<const char*>(&rec.m_far_clip_m), sizeof(rec.m_far_clip_m));
		os.write(reinterpret_cast<const char*>(&rec.m_lens_shift_horizontal_ratio), sizeof(rec.m_lens_shift_horizontal_ratio));
		os.write(reinterpret_cast<const char*>(&rec.m_lens_shift_vertical_ratio), sizeof(rec.m_lens_shift_vertical_ratio));
		os.write(reinterpret_cast<const char*>(&rec.m_lens_distortion_radial_coefficients_k1), sizeof(rec.m_lens_distortion_radial_coefficients_k1));
		os.write(reinterpret_cast<const char*>(&rec.m_lens_distortion_radial_coefficients_k2), sizeof(rec.m_lens_distortion_radial_coefficients_k2));
		os.write(reinterpret_cast<const char*>(&rec.m_lens_distortion_center_point_right_mm), sizeof(rec.m_lens_distortion_center_point_right_mm));
		os.write(reinterpret_cast<const char*>(&rec.m_lens_distortion_center_point_up_mm), sizeof(rec.m_lens_distortion_center_point_up_mm));
		// m_reserved‚Ì•ª25ƒoƒCƒg‚ð0‚Å–„‚ß‚é
		os.write(reinterpret_cast<const char*>(new uint8_t[25]()), 25);
	}
}
