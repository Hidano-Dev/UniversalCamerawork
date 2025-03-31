#ifdef _DEBUG
#include <iostream>
#include <ucapi.h>
#include <ucapi_dll.h>

struct UCAPI_DllObject { // Forward declaration
    ucapi_t* obj;
};

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
   ucapi_t* obj = deserializedHandle->obj;
   std::cout << "Magic: " << obj->magic() << std::endl;
   std::cout << "Version: " << obj->version() << std::endl;
   std::cout << "Num Payload: " << obj->num_payload() << std::endl;
   std::cout << "Payload Length: " << obj->payload_length() << std::endl;
   std::cout << "CRC16: " << obj->crc16() << std::endl;


   // Recordのフィールドを表示
   std::vector<ucapi_t::record_t*>* m_payload = obj->payload();
   std::cout << "CameraNo" << m_payload->at(0)->camera_no() << std::endl;
   std::cout << "Commands" << m_payload->at(0)->commands() << std::endl;
   std::cout << "Timecode Frame Number" << m_payload->at(0)->timecode()->frame_number() << std::endl;
   std::cout << "Timecode Second Number" << m_payload->at(0)->timecode()->second_number() << std::endl;
   std::cout << "Timecode Minute Number" << m_payload->at(0)->timecode()->minute_number() << std::endl;
   std::cout << "Timecode Hour Number" << m_payload->at(0)->timecode()->hour_number() << std::endl;
   std::cout << "Timecode Reserved" << m_payload->at(0)->timecode()->reserved() << std::endl;
   std::cout << "PacketNo" << m_payload->at(0)->packet_no() << std::endl;
   std::cout << "Eye Position Right M" << m_payload->at(0)->eye_position_right_m() << std::endl;
   std::cout << "Eye Position Up M" << m_payload->at(0)->eye_position_up_m() << std::endl;
   std::cout << "Eye Position Forward M" << m_payload->at(0)->eye_position_forward_m() << std::endl;
   std::cout << "Look Vector Right M" << m_payload->at(0)->look_vector_right_m() << std::endl;
   std::cout << "Look Vector Up M" << m_payload->at(0)->look_vector_up_m() << std::endl;
   std::cout << "Look Vector Forward M" << m_payload->at(0)->look_vector_forward_m() << std::endl;
   std::cout << "Up Vector Right M" << m_payload->at(0)->up_vector_right_m() << std::endl;
   std::cout << "Up Vector Up M" << m_payload->at(0)->up_vector_up_m() << std::endl;
   std::cout << "Up Vector Forward M" << m_payload->at(0)->up_vector_forward_m() << std::endl;
   std::cout << "Focal Length Mm" << m_payload->at(0)->focal_length_mm() << std::endl;
   std::cout << "Aspect Ratio" << m_payload->at(0)->aspect_ratio() << std::endl;
   std::cout << "Focus Distance M" << m_payload->at(0)->focus_distance_m() << std::endl;
   std::cout << "Aperture" << m_payload->at(0)->aperture() << std::endl;
   std::cout << "Sensor Size Width Mm" << m_payload->at(0)->sensor_size_width_mm() << std::endl;
   std::cout << "Sensor Size Height Mm" << m_payload->at(0)->sensor_size_height_mm() << std::endl;
   std::cout << "Near Clip M" << m_payload->at(0)->near_clip_m() << std::endl;
   std::cout << "Far Clip M" << m_payload->at(0)->far_clip_m() << std::endl;
   std::cout << "Lens Shift Horizontal Ratio" << m_payload->at(0)->lens_shift_horizontal_ratio() << std::endl;
   std::cout << "Lens Shift Vertical Ratio" << m_payload->at(0)->lens_shift_vertical_ratio() << std::endl;
   std::cout << "Lens Distortion Radial Coefficients K1" << m_payload->at(0)->lens_distortion_radial_coefficients_k1() << std::endl;
   std::cout << "Lens Distortion Radial Coefficients K2" << m_payload->at(0)->lens_distortion_radial_coefficients_k2() << std::endl;
   std::cout << "Lens Distortion Center Point Right Mm" << m_payload->at(0)->lens_distortion_center_point_right_mm() << std::endl;
   std::cout << "Lens Distortion Center Point Up Mm" << m_payload->at(0)->lens_distortion_center_point_up_mm() << std::endl;
   std::cout << "Reserved" << m_payload->at(0)->reserved() << std::endl;

   // Free the allocated resources.
   UCAPI_FreeBuffer(serializedBuffer);
   UCAPI_FreeObject(deserializedHandle);
   UCAPI_FreeObject(defaultHandle);

   return 0;
}
#endif // _DEBUG
