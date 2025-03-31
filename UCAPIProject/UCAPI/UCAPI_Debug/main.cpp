#ifdef _DEBUG
#include <iostream>
#include <iomanip>
#include <ucapi.h>
#include <ucapi_dll.h>

struct UCAPI_DllObject { // Forward declaration
    ucapi_t* obj;
};

void Dump(UCAPI_DllObject* handle);
void RawDump(UCAPI_DllObject* handle);

/// <summary>
/// Demonstrates creating default UCAPI data, serialization, and deserialization
/// using unified memory management.
/// </summary>
int main() {
   // Create a default UCAPI object for serialization.
   UCAPI_DllObject* defaultHandle = UCAPI_CreateDefault();
   if (!defaultHandle) {
       std::cerr << "Failed to create default UCAPI object." << std::endl;
       return -1;
   }
   std::cout << "Default UCAPI object created successfully." << std::endl;

   // 一度ダンプしてみる
   RawDump(defaultHandle);

   // Serialize the default UCAPI object.
   uint8_t* serializedBuffer = nullptr;
   size_t serializedSize = 0;
   if (UCAPI_Serialize(defaultHandle, &serializedBuffer, &serializedSize) != 0) {
       std::cerr << "Serialization failed." << std::endl;
       UCAPI_FreeObject(defaultHandle);
       return -1;
   }
   std::cout << "Serialization succeeded. Serialized size: " << serializedSize << " bytes." << std::endl;

   // Deserialize the serialized data.
   UCAPI_DllObject* deserializedHandle = UCAPI_Deserialize(serializedBuffer, serializedSize);
   if (!deserializedHandle) {
       std::cerr << "Deserialization failed." << std::endl;
       UCAPI_FreeBuffer(serializedBuffer);
       UCAPI_FreeObject(defaultHandle);
       return -1;
   }
   std::cout << "Deserialization succeeded." << std::endl;

   // オブジェクトに変換して各フィールドを表示
   Dump(deserializedHandle);

   // Free the allocated resources.
   UCAPI_FreeBuffer(serializedBuffer);
   UCAPI_FreeObject(deserializedHandle);
   UCAPI_FreeObject(defaultHandle);

   return 0;
}

void Dump(UCAPI_DllObject* handle) {
    ucapi_t* obj = handle->obj;
    std::cout << "Magic: " << obj->m_magic << std::endl;
    std::cout << "Version: " << obj->m_version << std::endl;
    std::cout << "Num Payload: " << obj->m_num_payload << std::endl;
    std::cout << "Payload Length: " << obj->m_payload_length << std::endl;
    std::cout << "CRC16: " << obj->m_crc16 << std::endl;


    // Recordのフィールドを表示
    ucapi_t::record_t* m_payload = obj->m_payload;
    std::cout << "CameraNo " << m_payload->m_camera_no << std::endl;
    std::cout << "Commands " << m_payload->m_commands << std::endl;
    std::cout << "Timecode Frame Number " << m_payload->m_timecode.m_frame_number << std::endl;
    std::cout << "Timecode Second Number " << m_payload->m_timecode.m_second_number << std::endl;
    std::cout << "Timecode Minute Number " << m_payload->m_timecode.m_minute_number << std::endl;
    std::cout << "Timecode Hour Number " << m_payload->m_timecode.m_hour_number << std::endl;
    std::cout << "Timecode Reserved " << m_payload->m_timecode.m_reserved << std::endl;
    std::cout << "PacketNo " << std::to_string(m_payload->m_packet_no) << std::endl;
    std::cout << "Eye Position Right M " << m_payload->m_eye_position_right_m << std::endl;
    std::cout << "Eye Position Up M " << m_payload->m_eye_position_up_m << std::endl;
    std::cout << "Eye Position Forward M " << m_payload->m_eye_position_forward_m << std::endl;
    std::cout << "Look Vector Right M " << m_payload->m_look_vector_right_m << std::endl;
    std::cout << "Look Vector Up M " << m_payload->m_look_vector_up_m << std::endl;
    std::cout << "Look Vector Forward M " << m_payload->m_look_vector_forward_m << std::endl;
    std::cout << "Up Vector Right M " << m_payload->m_up_vector_right_m << std::endl;
    std::cout << "Up Vector Up M " << m_payload->m_up_vector_up_m << std::endl;
    std::cout << "Up Vector Forward M " << m_payload->m_up_vector_forward_m << std::endl;
    std::cout << "Focal Length Mm " << m_payload->m_focal_length_mm << std::endl;
    std::cout << "Aspect Ratio " << m_payload->m_aspect_ratio << std::endl;
    std::cout << "Focus Distance M " << m_payload->m_focus_distance_m << std::endl;
    std::cout << "Aperture " << m_payload->m_aperture << std::endl;
    std::cout << "Sensor Size Width Mm " << m_payload->m_sensor_size_width_mm << std::endl;
    std::cout << "Sensor Size Height Mm " << m_payload->m_sensor_size_height_mm << std::endl;
    std::cout << "Near Clip M " << m_payload->m_near_clip_m << std::endl;
    std::cout << "Far Clip M " << m_payload->m_far_clip_m << std::endl;
    std::cout << "Lens Shift Horizontal Ratio " << m_payload->m_lens_shift_horizontal_ratio << std::endl;
    std::cout << "Lens Shift Vertical Ratio " << m_payload->m_lens_shift_vertical_ratio << std::endl;
    std::cout << "Lens Distortion Radial Coefficients K1 " << m_payload->m_lens_distortion_radial_coefficients_k1 << std::endl;
    std::cout << "Lens Distortion Radial Coefficients K2 " << m_payload->m_lens_distortion_radial_coefficients_k2 << std::endl;
    std::cout << "Lens Distortion Center Point Right Mm " << m_payload->m_lens_distortion_center_point_right_mm << std::endl;
    std::cout << "Lens Distortion Center Point Up Mm " << m_payload->m_lens_distortion_center_point_up_mm << std::endl;
}

void RawDump(UCAPI_DllObject* handle) {  
   ucapi_t* obj = handle->obj;  
   // 全てをバイト列として表示
   std::cout << "Raw Data: ";
   for (size_t i = 0; i < sizeof(ucapi_t); i++) {
	   // 1バイトずつ16進数で表示
	   // 2桁表示で、ハイフンで区切る
	   std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)((uint8_t*)obj)[i] << "-";
   }
   std::cout << std::endl;
}

#endif // _DEBUG
