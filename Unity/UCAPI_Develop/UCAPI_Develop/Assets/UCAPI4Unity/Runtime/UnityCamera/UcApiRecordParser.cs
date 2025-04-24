using UCAPI4Unity.Runtime.Core;
using UnityEngine;

namespace UCAPI4Unity.Runtime.UnityCamera
{
    public static class UcApiRecordParser
    {
        public static UcApiRecord FromCamera(Camera cam)
        {
            var timeCode = new UcApiTimeCode
            {
                FrameNumber = 12,
                Second = 34,
                Minute = 56,
                Hour = 14,
                FrameRate = FrameRate.FrameRate60,
                DropFrame = false
            };
            var record = new UcApiRecord
            {
                CameraNo = 1,
                Commands = 0x0B, // DOF_ENABLE | LENS_DISTORTION_ENABLE 仮
                PacketNo = 1,
                TimeCode = UcApiTimeCode.ToRaw(timeCode),
                SubFrame = 0,
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

            return record;
        }
        
        public static void ToCamera(UcApiRecord record, Camera camera)
        {
            // カメラ位置・回転を反映
            var position = new Vector3(
                record.EyePositionRightM,
                record.EyePositionUpM,
                record.EyePositionForwardM
            );

            var forward = new Vector3(
                record.LookVectorRightM,
                record.LookVectorUpM,
                record.LookVectorForwardM
            );

            var up = new Vector3(
                record.UpVectorRightM,
                record.UpVectorUpM,
                record.UpVectorForwardM
            );

            camera.transform.position = position;
            camera.transform.rotation = Quaternion.LookRotation(forward, up);
            
            // レンズ・クリップなどを反映
            camera.focalLength = record.FocalLengthMm;
            camera.aspect = record.AspectRatio;
            camera.nearClipPlane = record.NearClipM;
            camera.farClipPlane = record.FarClipM;
            camera.sensorSize = new Vector2(record.SensorSizeWidthMm, record.SensorSizeHeightMm);
            camera.lensShift = new Vector2(record.LensShiftHorizontalRatio, record.LensShiftVerticalRatio);

            // 被写界深度など（コマンドビットによる有効化が必要なら拡張可）
            var dofEnabled = (record.Commands & 0x04) != 0;
            if (dofEnabled)
            {
                camera.usePhysicalProperties = true;
                camera.focusDistance = record.FocusDistanceM;
                camera.aperture = record.Aperture;
            }
        } 
    }
}