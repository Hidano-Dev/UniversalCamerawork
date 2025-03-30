#include "pch.h"
#include "ucapi_serializer.h"
#include <cstring>
#include "ucapi.h"

void write_ucapi(const ucapi_t* obj, std::ostream& os) {
	os.write(std::to_string(obj->magic()).c_str(), 2);
	os.write(std::to_string(obj->version()).c_str(), 2);
	os.write(std::to_string(obj->num_payload()).c_str(), 2);
	os.write(std::to_string(obj->payload_length()).c_str(), 2);
	os.write(std::to_string(obj->crc16()).c_str(), 2);

	auto itr = obj->payload()->begin();
	for (int i = 0; i < obj->num_payload(); i++) {
		auto rec = *itr;
		os.write(std::to_string(rec->camera_no()).c_str(), 4);
		os.write(std::to_string(rec->commands()).c_str(), 2);

		auto timecode = rec->timecode();
		uint8_t timecode_data[3] = { 0 };
		timecode_data[0] = (timecode->frame_number() & 0x7F);
		timecode_data[1] = ((timecode->second_number() & 0x3F) << 2) | ((timecode->minute_number() & 0x30) >> 4);
		timecode_data[2] = ((timecode->minute_number() & 0x0F) << 4) | (timecode->hour_number() & 0x1F);
		os.write(reinterpret_cast<char*>(timecode_data), 3);
		os.write(std::to_string(rec->packet_no()).c_str(), sizeof(rec->packet_no()));
		os.write(std::to_string(rec->eye_position_right_m()).c_str(), sizeof(rec->eye_position_right_m()));
		os.write(std::to_string(rec->eye_position_up_m()).c_str(), sizeof(rec->eye_position_up_m()));
		os.write(std::to_string(rec->eye_position_forward_m()).c_str(), sizeof(rec->eye_position_forward_m()));
		os.write(std::to_string(rec->look_vector_right_m()).c_str(), sizeof(rec->look_vector_right_m()));
		os.write(std::to_string(rec->look_vector_up_m()).c_str(), sizeof(rec->look_vector_up_m()));
		os.write(std::to_string(rec->look_vector_forward_m()).c_str(), sizeof(rec->look_vector_forward_m()));
		os.write(std::to_string(rec->up_vector_right_m()).c_str(), sizeof(rec->up_vector_right_m()));
		os.write(std::to_string(rec->up_vector_up_m()).c_str(), sizeof(rec->up_vector_up_m()));
		os.write(std::to_string(rec->up_vector_forward_m()).c_str(), sizeof(rec->up_vector_forward_m()));
		os.write(std::to_string(rec->focal_length_mm()).c_str(), sizeof(rec->focal_length_mm()));
		os.write(std::to_string(rec->aspect_ratio()).c_str(), sizeof(rec->aspect_ratio()));
		os.write(std::to_string(rec->focus_distance_m()).c_str(), sizeof(rec->focus_distance_m()));
		os.write(std::to_string(rec->aperture()).c_str(), sizeof(rec->aperture()));
		os.write(std::to_string(rec->sensor_size_width_mm()).c_str(), sizeof(rec->sensor_size_width_mm()));
		os.write(std::to_string(rec->sensor_size_height_mm()).c_str(), sizeof(rec->sensor_size_height_mm()));
		os.write(std::to_string(rec->near_clip_m()).c_str(), sizeof(rec->near_clip_m()));
		os.write(std::to_string(rec->far_clip_m()).c_str(), sizeof(rec->far_clip_m()));
		os.write(std::to_string(rec->lens_shift_horizontal_ratio()).c_str(), sizeof(rec->lens_shift_horizontal_ratio()));
		os.write(std::to_string(rec->lens_shift_vertical_ratio()).c_str(), sizeof(rec->lens_shift_vertical_ratio()));
		os.write(std::to_string(rec->lens_distortion_radial_coefficients_k1()).c_str(), sizeof(rec->lens_distortion_radial_coefficients_k1()));
		os.write(std::to_string(rec->lens_distortion_radial_coefficients_k2()).c_str(), sizeof(rec->lens_distortion_radial_coefficients_k2()));
		os.write(std::to_string(rec->lens_distortion_center_point_right_mm()).c_str(), sizeof(rec->lens_distortion_center_point_right_mm()));
		os.write(std::to_string(rec->lens_distortion_center_point_up_mm()).c_str(), sizeof(rec->lens_distortion_center_point_up_mm()));
		os.write(reinterpret_cast<const char*>(rec->reserved()->data()), 25);
		itr++;
	}
}
