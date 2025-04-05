using MessagePack;
using UCAPI4Unity.Core;
using UnityEngine;
using uOSC;

namespace UCAPI4Unity.Sample
{
    public class UcApiSender : MonoBehaviour
    {
        [SerializeField] private uOscClient client;
        [SerializeField] private new Camera camera;

        private void Update()
        {
            var ucapi = CreateFromCamera(camera);
            client.Send("/ucapi/data", ucapi);
        }
        
        private static byte[] CreateFromCamera(Camera cam)
        {
            var obj = new UcApiObject
            {
                Magic = 0xAA55,
                Version = 0,
                NumPayload = 1,
                PayloadLength = 128,
                CRC16 = 0, // CRC16 placeholder
                Payloads = new UcApiRecord[1]
            };
            obj.Payloads[0] = new UcApiRecord
            {
                CameraNo = 1,
                Commands = 0x0B, // DOF_ENABLE | LENS_DISTORTION_ENABLE 仮
                PacketNo = 1,
                TimeCode = new UcApiTimeCode
                {
                    FrameNumber = 12,
                    Second = 34,
                    Minute = 56,
                    Hour = 78,
                    FrameRate = FrameRate.FrameRate60,
                    DropFrame = false
                },
                EyePositionRightM = cam.transform.position.x,
                EyePositionUpM = cam.transform.position.y,
                EyePositionForwardM = cam.transform.position.z,
                LookVectorRightM = cam.transform.forward.x,
                LookVectorUpM = cam.transform.forward.y,
                LookVectorForwardM = cam.transform.forward.z,
                UpVectorRightM = cam.transform.up.x,
                UpVectorUpM = cam.transform.up.y,
                UpVectorForwardM = cam.transform.up.z,
                FocalLengthMm = cam.focalLength,
                AspectRatio = cam.aspect,
                FocusDistanceM = cam.focusDistance,
                Aperture = cam.aperture,
                SensorSizeWidthMm = cam.sensorSize.x,
                SensorSizeHeightMm = cam.sensorSize.y,
                NearClipM = cam.nearClipPlane,
                FarClipM = cam.farClipPlane,
                LensShiftHorizontalRatio = cam.lensShift.x,
                LensShiftVerticalRatio = cam.lensShift.y,
                // Lens distortion と center point は 0 にしておく
                LensDistortionRadialCoefficientsK1 = 0f,
                LensDistortionRadialCoefficientsK2 = 0f,
                LensDistortionCenterPointRightMm = 0f,
                LensDistortionCenterPointUpMm = 0f
            };

            return MessagePackSerializer.Serialize(obj);
        }
    }
}