#include "pch.h"
#include "ucapi_msgpack_serializer.h"
#include <msgpack.hpp>
#include <iostream>

namespace ucapi {

HRESULT MessagePackSerializer::Serialize(const CameraState& src, std::vector<uint8_t>& out) noexcept {
    try {
        ucapi_msgpack_record_t msgpack_record = ConvertToMsgPack(src);
        
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, msgpack_record);
        
        out.assign(sbuf.data(), sbuf.data() + sbuf.size());
        return S_OK;
    }
    catch (const std::exception& e) {
        std::cerr << "[MessagePackSerializer::Serialize] " << e.what() << std::endl;
        return E_FAIL;
    }
}

HRESULT MessagePackSerializer::Deserialize(const uint8_t* data, size_t size, CameraState& out) noexcept {
    if (data == nullptr || size == 0) {
        return E_INVALIDARG;
    }
    
    try {
        msgpack::object_handle oh = msgpack::unpack(reinterpret_cast<const char*>(data), size);
        msgpack::object obj = oh.get();
        
        ucapi_msgpack_record_t msgpack_record;
        obj.convert(msgpack_record);
        
        out = ConvertFromMsgPack(msgpack_record);
        return S_OK;
    }
    catch (const std::exception& e) {
        std::cerr << "[MessagePackSerializer::Deserialize] " << e.what() << std::endl;
        return E_FAIL;
    }
}

ucapi_msgpack_record_t MessagePackSerializer::ConvertToMsgPack(const CameraState& src) {
    ucapi_msgpack_record_t r;
    
    r.camera_no = src.m_camera_no;
    r.commands = src.m_commands;
    r.timecode = src.m_timecode;
    r.subframe = src.m_subframe;
    r.packet_no = src.m_packet_no;
    
    r.eye_position_right_m = src.m_eye_position_right_m;
    r.eye_position_up_m = src.m_eye_position_up_m;
    r.eye_position_forward_m = src.m_eye_position_forward_m;
    
    r.look_vector_right_m = src.m_look_vector_right_m;
    r.look_vector_up_m = src.m_look_vector_up_m;
    r.look_vector_forward_m = src.m_look_vector_forward_m;
    
    r.up_vector_right_m = src.m_up_vector_right_m;
    r.up_vector_up_m = src.m_up_vector_up_m;
    r.up_vector_forward_m = src.m_up_vector_forward_m;
    
    r.focal_length_mm = src.m_focal_length_mm;
    r.aspect_ratio = src.m_aspect_ratio;
    r.focus_distance_m = src.m_focus_distance_m;
    r.aperture = src.m_aperture;
    r.sensor_size_width_mm = src.m_sensor_size_width_mm;
    r.sensor_size_height_mm = src.m_sensor_size_height_mm;
    r.near_clip_m = src.m_near_clip_m;
    r.far_clip_m = src.m_far_clip_m;
    r.lens_shift_horizontal_ratio = src.m_lens_shift_horizontal_ratio;
    r.lens_shift_vertical_ratio = src.m_lens_shift_vertical_ratio;
    r.lens_distortion_radial_coefficients_k1 = src.m_lens_distortion_radial_coefficients_k1;
    r.lens_distortion_radial_coefficients_k2 = src.m_lens_distortion_radial_coefficients_k2;
    r.lens_distortion_center_point_right_mm = src.m_lens_distortion_center_point_right_mm;
    r.lens_distortion_center_point_up_mm = src.m_lens_distortion_center_point_up_mm;
    
    return r;
}

CameraState MessagePackSerializer::ConvertFromMsgPack(const ucapi_msgpack_record_t& msgpack_record) {
    CameraState crec(sizeof(CameraState));
    
    crec.m_camera_no = msgpack_record.camera_no;
    crec.m_commands = msgpack_record.commands;
    crec.m_timecode = msgpack_record.timecode;
    crec.m_subframe = msgpack_record.subframe;
    crec.m_packet_no = msgpack_record.packet_no;
    
    crec.m_eye_position_right_m = msgpack_record.eye_position_right_m;
    crec.m_eye_position_up_m = msgpack_record.eye_position_up_m;
    crec.m_eye_position_forward_m = msgpack_record.eye_position_forward_m;
    
    crec.m_look_vector_right_m = msgpack_record.look_vector_right_m;
    crec.m_look_vector_up_m = msgpack_record.look_vector_up_m;
    crec.m_look_vector_forward_m = msgpack_record.look_vector_forward_m;
    
    crec.m_up_vector_right_m = msgpack_record.up_vector_right_m;
    crec.m_up_vector_up_m = msgpack_record.up_vector_up_m;
    crec.m_up_vector_forward_m = msgpack_record.up_vector_forward_m;
    
    crec.m_focal_length_mm = msgpack_record.focal_length_mm;
    crec.m_aspect_ratio = msgpack_record.aspect_ratio;
    crec.m_focus_distance_m = msgpack_record.focus_distance_m;
    crec.m_aperture = msgpack_record.aperture;
    crec.m_sensor_size_width_mm = msgpack_record.sensor_size_width_mm;
    crec.m_sensor_size_height_mm = msgpack_record.sensor_size_height_mm;
    crec.m_near_clip_m = msgpack_record.near_clip_m;
    crec.m_far_clip_m = msgpack_record.far_clip_m;
    crec.m_lens_shift_horizontal_ratio = msgpack_record.lens_shift_horizontal_ratio;
    crec.m_lens_shift_vertical_ratio = msgpack_record.lens_shift_vertical_ratio;
    crec.m_lens_distortion_radial_coefficients_k1 = msgpack_record.lens_distortion_radial_coefficients_k1;
    crec.m_lens_distortion_radial_coefficients_k2 = msgpack_record.lens_distortion_radial_coefficients_k2;
    crec.m_lens_distortion_center_point_right_mm = msgpack_record.lens_distortion_center_point_right_mm;
    crec.m_lens_distortion_center_point_up_mm = msgpack_record.lens_distortion_center_point_up_mm;
    
    return crec;
}

}
