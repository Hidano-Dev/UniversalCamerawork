#include "pch.h"
#include <iostream>
#include <iomanip>
#include "../UCAPI_DLL/ucapi.h"
#include "../UCAPI_DLL/ucapi_msgpack_converter.h"

int main()
{
    std::cout << "UCAPI Debug Test - SMPTE LTC Version" << std::endl;

    smpte_ltc_msgpack_t smpte_tc;
    smpte_tc.clear();
    smpte_tc.frame = 12;
    smpte_tc.second = 34;
    smpte_tc.minute = 56;
    smpte_tc.hour = 14;
    smpte_tc.drop_frame = false;
    smpte_tc.color_frame = false;
    smpte_tc.set_sync_word();

    ucapi_msgpack_record_t record;
    record.camera_no = 1;
    record.commands = 0x0A;
    record.timecode = smpte_tc;  // SMPTE LTC timecode
    record.subframe = 0.0f;      // float type for subframe
    record.packet_no = 5;
    record.eye_position_right_m = 1.0f;
    record.eye_position_up_m = 2.0f;
    record.eye_position_forward_m = 3.0f;
    record.look_vector_right_m = 4.0f;
    record.look_vector_up_m = 5.0f;
    record.look_vector_forward_m = 6.0f;
    record.up_vector_right_m = 7.0f;
    record.up_vector_up_m = 8.0f;
    record.up_vector_forward_m = 9.0f;
    record.focal_length_mm = 50.0f;
    record.aspect_ratio = 1.777f;
    record.focus_distance_m = 10.0f;
    record.aperture = 2.8f;
    record.sensor_size_width_mm = 36.0f;
    record.sensor_size_height_mm = 24.0f;
    record.near_clip_m = 0.1f;
    record.far_clip_m = 1000.0f;
    record.lens_shift_horizontal_ratio = 0.0f;
    record.lens_shift_vertical_ratio = 0.0f;
    record.lens_distortion_radial_coefficients_k1 = 0.0f;
    record.lens_distortion_radial_coefficients_k2 = 0.0f;
    record.lens_distortion_center_point_right_mm = 0.0f;
    record.lens_distortion_center_point_up_mm = 0.0f;

    std::cout << std::endl << "=== Serializing to MessagePack ===" << std::endl;
    
    ucapi_msgpack_t msgpack_data;
    msgpack_data.m_payload.push_back(record);

    try {
        auto packed_data = ucapi_msgpack_converter::pack(msgpack_data);
        std::cout << "Serialization successful. Size: " << packed_data.size() << " bytes" << std::endl;

        std::cout << std::endl << "=== Deserializing from MessagePack ===" << std::endl;
        
        auto unpacked_data = ucapi_msgpack_converter::unpack(packed_data);
        
        std::cout << "Deserialization successful. Records: " << unpacked_data.m_payload.size() << std::endl;

        for (size_t i = 0; i < unpacked_data.m_payload.size(); ++i) {
            const auto& m_payload = unpacked_data.m_payload[i];
            
            std::cout << std::endl << "--- Payload " << i << " ---" << std::endl;
            std::cout << "CameraNo " << m_payload.m_camera_no << std::endl;
            std::cout << "Commands " << m_payload.m_commands << std::endl;
            
            std::cout << "Timecode SMPTE " << static_cast<int>(m_payload.m_timecode.hour) << ":" 
                << static_cast<int>(m_payload.m_timecode.minute) << ":" 
                << static_cast<int>(m_payload.m_timecode.second) << ":" 
                << static_cast<int>(m_payload.m_timecode.frame);
            if (m_payload.m_timecode.drop_frame) std::cout << " (DF)";
            if (m_payload.m_timecode.color_frame) std::cout << " (CF)";
            std::cout << std::endl;
            
            std::cout << "Subframe " << m_payload.m_subframe << std::endl;
            std::cout << "Packet No " << static_cast<int>(m_payload.m_packet_no) << std::endl;
            std::cout << "Eye Position Right " << m_payload.m_eye_position_right_m << std::endl;
            std::cout << "Eye Position Up " << m_payload.m_eye_position_up_m << std::endl;
            std::cout << "Eye Position Forward " << m_payload.m_eye_position_forward_m << std::endl;
            std::cout << "Look Vector Right " << m_payload.m_look_vector_right_m << std::endl;
            std::cout << "Look Vector Up " << m_payload.m_look_vector_up_m << std::endl;
            std::cout << "Look Vector Forward " << m_payload.m_look_vector_forward_m << std::endl;
            std::cout << "Up Vector Right " << m_payload.m_up_vector_right_m << std::endl;
            std::cout << "Up Vector Up " << m_payload.m_up_vector_up_m << std::endl;
            std::cout << "Up Vector Forward " << m_payload.m_up_vector_forward_m << std::endl;
            std::cout << "Focal Length " << m_payload.m_focal_length_mm << std::endl;
            std::cout << "Aspect Ratio " << m_payload.m_aspect_ratio << std::endl;
            std::cout << "Focus Distance " << m_payload.m_focus_distance_m << std::endl;
            std::cout << "Aperture " << m_payload.m_aperture << std::endl;
            std::cout << "Sensor Size Width " << m_payload.m_sensor_size_width_mm << std::endl;
            std::cout << "Sensor Size Height " << m_payload.m_sensor_size_height_mm << std::endl;
            std::cout << "Near Clip " << m_payload.m_near_clip_m << std::endl;
            std::cout << "Far Clip " << m_payload.m_far_clip_m << std::endl;
            std::cout << "Lens Shift Horizontal " << m_payload.m_lens_shift_horizontal_ratio << std::endl;
            std::cout << "Lens Shift Vertical " << m_payload.m_lens_shift_vertical_ratio << std::endl;
            std::cout << "Lens Distortion K1 " << m_payload.m_lens_distortion_radial_coefficients_k1 << std::endl;
            std::cout << "Lens Distortion K2 " << m_payload.m_lens_distortion_radial_coefficients_k2 << std::endl;
            std::cout << "Lens Distortion Center Right " << m_payload.m_lens_distortion_center_point_right_mm << std::endl;
            std::cout << "Lens Distortion Center Up " << m_payload.m_lens_distortion_center_point_up_mm << std::endl;
        }

        std::cout << std::endl << "=== SMPTE Timecode Validation ===" << std::endl;
        smpte_ltc_t test_tc;
        test_tc.clear();
        test_tc.set_time(14, 56, 34, 12);
        test_tc.set_drop_frame(false);
        test_tc.set_color_frame(false);
        test_tc.set_sync_word();
        
        if (test_tc.is_valid_sync_word()) {
            std::cout << "SMPTE sync word validation: PASSED" << std::endl;
        } else {
            std::cout << "SMPTE sync word validation: FAILED" << std::endl;
        }
        
        std::cout << "Test timecode: " << static_cast<int>(test_tc.get_hour()) << ":" 
            << static_cast<int>(test_tc.get_minute()) << ":" 
            << static_cast<int>(test_tc.get_second()) << ":" 
            << static_cast<int>(test_tc.get_frame()) << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    std::cout << std::endl << "UCAPI Debug Test completed successfully!" << std::endl;
    return 0;
}
