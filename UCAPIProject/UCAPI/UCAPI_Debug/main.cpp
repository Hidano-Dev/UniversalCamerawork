#ifdef _DEBUG
#include "pch.h"
#include <iostream>
#include <iomanip>
#include <string>
#include "ucapi.h"
#include "ucapi_dll.h"
#include "ucapi_msgpack_types.h"
#include "ucapi_timecode.h"

void Dump(ucapi_t* handle);

// �ŏ��\���� MessagePack �I�u�W�F�N�g���\�z���ăe�X�g
int main() {
    // MessagePack�\���̂̏�����
    ucapi_msgpack_record_t record;
    record.camera_no = 1;
    record.commands = 0x0A;
    // timecodeを4バイト値で初期化
    record.timecode = 0x12345678;
    record.subframe = 0;
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
	record.aspect_ratio = 1.78f;
	record.focus_distance_m = 1.0f;
	record.aperture = 2.8f;
	record.sensor_size_width_mm = 36.0f;
	record.sensor_size_height_mm = 24.0f;
	record.near_clip_m = 0.1f;
	record.far_clip_m = 100.0f;
	record.lens_shift_horizontal_ratio = 0.0f;
	record.lens_shift_vertical_ratio = 0.0f;
	record.lens_distortion_radial_coefficients_k1 = 0.0f;
	record.lens_distortion_radial_coefficients_k2 = 0.0f;
	record.lens_distortion_center_point_right_mm = 0.0f;
	record.lens_distortion_center_point_up_mm = 0.0f;

    ucapi_msgpack_t data;
    data.magic = 0x55AA;
    data.version = 1;
    data.num_payload = 1;
    data.payload.push_back(record);

    size_t recordSize = sizeof(ucapi_msgpack_record_t);
    uint16_t crc = UCAPI_CalcCRC16(&record, recordSize, 0x1021, 0xFFFF);
    data.crc16 = crc;

    // MessagePack�o�C�i���ɃG���R�[�h
    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, data);

    // Deserialize via DLL
    auto handle = UCAPI_Deserialize(reinterpret_cast<const uint8_t*>(sbuf.data()), 1);
    if (!handle) {
        std::cerr << "Deserialization failed." << std::endl;
        return -1;
    }

    std::cout << "Deserialization succeeded!" << std::endl;
    Dump(handle);

    // Serialize back via DLL
    uint8_t* outBuf = nullptr;
    size_t outSize = 0;
    if (UCAPI_Serialize(handle, &outBuf, &outSize) != 0) {
        std::cerr << "Serialization failed." << std::endl;
        return -1;
    }

    std::cout << "Serialization succeeded! Size: " << outSize << " bytes" << std::endl;

    // ��n��
    UCAPI_FreeBuffer(outBuf);
    return 0;
}

void Dump(ucapi_t* obj) {
    std::cout << "Magic: " << obj->m_magic << std::endl;
    std::cout << "Version: " << obj->m_version << std::endl;
    std::cout << "Num Payload: " << obj->m_num_payload << std::endl;
    std::cout << "CRC16: 0x" << std::hex << std::setw(4) << std::setfill('0') << obj->m_crc16 << std::dec << std::endl;

    for (size_t i = 0; i < obj->m_num_payload; ++i) {
        const auto& m_payload = obj->m_payload[i];
        std::cout << "--- Payload " << i << " ---" << std::endl;
        std::cout << "CameraNo " << m_payload.m_camera_no << std::endl;
        std::cout << "Commands " << m_payload.m_commands << std::endl;
        // timecodeを16進ダンプで出力
        std::cout << "Timecode: 0x" << std::hex << std::setw(8) << std::setfill('0') << m_payload.m_timecode << std::dec << std::endl;
        std::cout << "Subframe " << m_payload.m_subframe << std::endl;
		std::cout << "Packet No " << m_payload.m_packet_no << std::endl;
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
		std::cout << "Lens Shift Horizontal Ratio " << m_payload.m_lens_shift_horizontal_ratio << std::endl;
		std::cout << "Lens Shift Vertical Ratio " << m_payload.m_lens_shift_vertical_ratio << std::endl;
		std::cout << "Lens Distortion Radial Coefficients K1 " << m_payload.m_lens_distortion_radial_coefficients_k1 << std::endl;
		std::cout << "Lens Distortion Radial Coefficients K2 " << m_payload.m_lens_distortion_radial_coefficients_k2 << std::endl;
		std::cout << "Lens Distortion Center Point Right " << m_payload.m_lens_distortion_center_point_right_mm << std::endl;
		std::cout << "Lens Distortion Center Point Up " << m_payload.m_lens_distortion_center_point_up_mm << std::endl;
    }
}

#endif // _DEBUG
