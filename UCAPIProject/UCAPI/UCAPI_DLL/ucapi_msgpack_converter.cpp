#include "pch.h"
#include "ucapi_msgpack_converter.h"
#include "ucapi.h"
#include "ucapi_msgpack_types.h"

ucapi_t* convert_to_ucapi(const ucapi_msgpack_t& msgpack_obj) {
    ucapi_t* ucapi = new ucapi_t();

    ucapi->m_magic = msgpack_obj.magic;
    ucapi->m_version = msgpack_obj.version;
    ucapi->m_num_payload = msgpack_obj.num_payload;
    ucapi->m_crc16 = msgpack_obj.crc16;
	ucapi->m_payload.reserve(msgpack_obj.num_payload);

	auto payload_length = sizeof(ucapi_t::record_t);

    for (const auto& rec : msgpack_obj.payload) {
		ucapi->m_payload.emplace_back(payload_length);
        ucapi_t::record_t& crec = ucapi->m_payload.back();
		memset(&crec, 0, sizeof(ucapi_t::record_t));

        crec.m_camera_no = rec.camera_no;
        crec.m_commands = rec.commands;
        std::copy(rec.timecode.begin(), rec.timecode.end(), crec.m_timecode.begin());
        crec.m_packet_no = rec.packet_no;

        crec.m_eye_position_right_m = rec.eye_position_right_m;
        crec.m_eye_position_up_m = rec.eye_position_up_m;
        crec.m_eye_position_forward_m = rec.eye_position_forward_m;

        crec.m_look_vector_right_m = rec.look_vector_right_m;
        crec.m_look_vector_up_m = rec.look_vector_up_m;
        crec.m_look_vector_forward_m = rec.look_vector_forward_m;

        crec.m_up_vector_right_m = rec.up_vector_right_m;
        crec.m_up_vector_up_m = rec.up_vector_up_m;
        crec.m_up_vector_forward_m = rec.up_vector_forward_m;

        crec.m_focal_length_mm = rec.focal_length_mm;
        crec.m_aspect_ratio = rec.aspect_ratio;
        crec.m_focus_distance_m = rec.focus_distance_m;
        crec.m_aperture = rec.aperture;
        crec.m_sensor_size_width_mm = rec.sensor_size_width_mm;
        crec.m_sensor_size_height_mm = rec.sensor_size_height_mm;
        crec.m_near_clip_m = rec.near_clip_m;
        crec.m_far_clip_m = rec.far_clip_m;
        crec.m_lens_shift_horizontal_ratio = rec.lens_shift_horizontal_ratio;
        crec.m_lens_shift_vertical_ratio = rec.lens_shift_vertical_ratio;
        crec.m_lens_distortion_radial_coefficients_k1 = rec.lens_distortion_radial_coefficients_k1;
        crec.m_lens_distortion_radial_coefficients_k2 = rec.lens_distortion_radial_coefficients_k2;
        crec.m_lens_distortion_center_point_right_mm = rec.lens_distortion_center_point_right_mm;
        crec.m_lens_distortion_center_point_up_mm = rec.lens_distortion_center_point_up_mm;
    }

    return ucapi;
}

ucapi_msgpack_t convert_to_msgpack(const ucapi_t* ucapi_obj) {
    ucapi_msgpack_t out;
    out.magic = ucapi_obj->m_magic;
    out.version = ucapi_obj->m_version;
    out.num_payload = ucapi_obj->m_num_payload;
    out.crc16 = ucapi_obj->m_crc16;

    for (int i = 0; i < ucapi_obj->m_num_payload; ++i) {
        const ucapi_t::record_t& rec = ucapi_obj->m_payload[i];
        ucapi_msgpack_record_t r;

        r.camera_no = rec.m_camera_no;
        r.commands = rec.m_commands;

		r.timecode = rec.m_timecode;
        r.packet_no = rec.m_packet_no;

        r.eye_position_right_m = rec.m_eye_position_right_m;
        r.eye_position_up_m = rec.m_eye_position_up_m;
        r.eye_position_forward_m = rec.m_eye_position_forward_m;

        r.look_vector_right_m = rec.m_look_vector_right_m;
        r.look_vector_up_m = rec.m_look_vector_up_m;
        r.look_vector_forward_m = rec.m_look_vector_forward_m;

        r.up_vector_right_m = rec.m_up_vector_right_m;
        r.up_vector_up_m = rec.m_up_vector_up_m;
        r.up_vector_forward_m = rec.m_up_vector_forward_m;

        r.focal_length_mm = rec.m_focal_length_mm;
        r.aspect_ratio = rec.m_aspect_ratio;
        r.focus_distance_m = rec.m_focus_distance_m;
        r.aperture = rec.m_aperture;
        r.sensor_size_width_mm = rec.m_sensor_size_width_mm;
        r.sensor_size_height_mm = rec.m_sensor_size_height_mm;
        r.near_clip_m = rec.m_near_clip_m;
        r.far_clip_m = rec.m_far_clip_m;
        r.lens_shift_horizontal_ratio = rec.m_lens_shift_horizontal_ratio;
        r.lens_shift_vertical_ratio = rec.m_lens_shift_vertical_ratio;
        r.lens_distortion_radial_coefficients_k1 = rec.m_lens_distortion_radial_coefficients_k1;
        r.lens_distortion_radial_coefficients_k2 = rec.m_lens_distortion_radial_coefficients_k2;
        r.lens_distortion_center_point_right_mm = rec.m_lens_distortion_center_point_right_mm;
        r.lens_distortion_center_point_up_mm = rec.m_lens_distortion_center_point_up_mm;

        out.payload.push_back(r);
    }

    return out;
}
