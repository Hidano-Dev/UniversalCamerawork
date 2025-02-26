#ifdef _DEBUG
#include <iostream>
#include "ucapi_dll.h"
#include <ucapi_deserialization.h>

struct UCAPI_DllObject { // Forward declaration
    ucapi_serialization_t* obj;
    kaitai::kstream* ks;
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
   ucapi_serialization_t* obj = deserializedHandle->obj;
   std::cout << "Signature: " << obj->header()->signature() << std::endl;
   std::cout << "Version: " << static_cast<int>(obj->header()->version_major()) << "." << static_cast<int>(obj->header()->version_minor()) << std::endl;
   std::cout << "Record count: " << obj->header()->record_count() << std::endl;
   std::cout << "Checksum: " << obj->header()->checksum() << std::endl;

   // Recordのフィールドを表示
   std::cout << "CameraNo" << obj->records()->at(0)->camera_no() << std::endl;
   std::cout << "Commands" << obj->records()->at(0)->commands() << std::endl;
   std::cout << "Timecode Frame Number" << obj->records()->at(0)->timecode()->frame_number() << std::endl;
   std::cout << "Timecode Second Number" << obj->records()->at(0)->timecode()->second_number() << std::endl;
   std::cout << "Timecode Minute Number" << obj->records()->at(0)->timecode()->minute_number() << std::endl;
   std::cout << "Timecode Hour Number" << obj->records()->at(0)->timecode()->hour_number() << std::endl;
   std::cout << "Timecode Reserved" << obj->records()->at(0)->timecode()->reserved() << std::endl;
   std::cout << "PacketNo" << obj->records()->at(0)->packet_no() << std::endl;
   std::cout << "Eye Position Right M" << obj->records()->at(0)->eye_position_right_m() << std::endl;
   std::cout << "Eye Position Up M" << obj->records()->at(0)->eye_position_up_m() << std::endl;
   std::cout << "Eye Position Forward M" << obj->records()->at(0)->eye_position_forward_m() << std::endl;
   std::cout << "Look Vector Right M" << obj->records()->at(0)->look_vector_right_m() << std::endl;
   std::cout << "Look Vector Up M" << obj->records()->at(0)->look_vector_up_m() << std::endl;
   std::cout << "Look Vector Forward M" << obj->records()->at(0)->look_vector_forward_m() << std::endl;
   std::cout << "Up Vector Right M" << obj->records()->at(0)->up_vector_right_m() << std::endl;
   std::cout << "Up Vector Up M" << obj->records()->at(0)->up_vector_up_m() << std::endl;
   std::cout << "Up Vector Forward M" << obj->records()->at(0)->up_vector_forward_m() << std::endl;
   std::cout << "Focal Length Mm" << obj->records()->at(0)->focal_length_mm() << std::endl;
   std::cout << "Aspect Ratio" << obj->records()->at(0)->aspect_ratio() << std::endl;
   std::cout << "Focus Distance M" << obj->records()->at(0)->focus_distance_m() << std::endl;
   std::cout << "Aperture" << obj->records()->at(0)->aperture() << std::endl;
   std::cout << "Sensor Size Width Mm" << obj->records()->at(0)->sensor_size_width_mm() << std::endl;
   std::cout << "Sensor Size Height Mm" << obj->records()->at(0)->sensor_size_height_mm() << std::endl;
   std::cout << "Near Clip M" << obj->records()->at(0)->near_clip_m() << std::endl;
   std::cout << "Far Clip M" << obj->records()->at(0)->far_clip_m() << std::endl;
   std::cout << "Lens Shift Horizontal Ratio" << obj->records()->at(0)->lens_shift_horizontal_ratio() << std::endl;
   std::cout << "Lens Shift Vertical Ratio" << obj->records()->at(0)->lens_shift_vertical_ratio() << std::endl;
   std::cout << "Lens Distortion Radial Coefficients K1" << obj->records()->at(0)->lens_distortion_radial_coefficients_k1() << std::endl;
   std::cout << "Lens Distortion Radial Coefficients K2" << obj->records()->at(0)->lens_distortion_radial_coefficients_k2() << std::endl;
   std::cout << "Lens Distortion Center Point Right Mm" << obj->records()->at(0)->lens_distortion_center_point_right_mm() << std::endl;
   std::cout << "Lens Distortion Center Point Up Mm" << obj->records()->at(0)->lens_distortion_center_point_up_mm() << std::endl;
   std::cout << "Reserved" << obj->records()->at(0)->reserved() << std::endl;

   // Free the allocated resources.
   UCAPI_FreeBuffer(serializedBuffer);
   UCAPI_FreeObject(deserializedHandle);
   UCAPI_FreeObject(defaultHandle);

   return 0;
}
#endif // _DEBUG
