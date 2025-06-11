using UCAPI4Unity.Runtime.Core;
using Unity.Cinemachine;
using UnityEngine;
using UnityEngine.Rendering.Universal;

namespace UCAPI4Unity.Runtime.CinemachineCamera
{
    public static class UcApiRecordParser
    {
        public static UcApiRecord FromVirtualCamera(ICinemachineCamera cam)
        {
            // Create SMPTE timecode with current time (example values)
            var smpteTimeCode = new UcApiSmpteTimecode();
            smpteTimeCode.Clear();
            smpteTimeCode.Frame = 12;
            smpteTimeCode.Second = 34;
            smpteTimeCode.Minute = 56;
            smpteTimeCode.Hour = 14;
            smpteTimeCode.DropFrame = false;
            smpteTimeCode.ColorFrame = false;
            smpteTimeCode.SetSyncWord();
            
            var position = cam.State.GetFinalPosition();
            var rotation = cam.State.GetFinalOrientation();
            var record = new UcApiRecord
            {
                CameraNo = 1,
                Commands = 0x0B, // DOF_ENABLE | LENS_DISTORTION_ENABLE 仮
                PacketNo = 1,
                TimeCode = smpteTimeCode,  // Now using SMPTE LTC format
                SubFrame = 0.0f,           // Now using float for precision
                EyePositionRightM = position.x,
                EyePositionUpM = position.y,
                EyePositionForwardM = position.z,
                LookVectorRightM = rotation.x,
                LookVectorUpM = rotation.y,
                LookVectorForwardM = rotation.z,
                UpVectorRightM = cam.State.ReferenceUp.x,
                UpVectorUpM = cam.State.ReferenceUp.y,
                UpVectorForwardM = cam.State.ReferenceUp.z,
                FocalLengthMm = Camera.FieldOfViewToFocalLength(cam.State.Lens.FieldOfView, cam.State.Lens.PhysicalProperties.SensorSize.y),
                AspectRatio = cam.State.Lens.Aspect,
                FocusDistanceM = cam.State.Lens.PhysicalProperties.FocusDistance,
                Aperture = cam.State.Lens.PhysicalProperties.Aperture,
                SensorSizeWidthMm = cam.State.Lens.PhysicalProperties.SensorSize.x,
                SensorSizeHeightMm = cam.State.Lens.PhysicalProperties.SensorSize.y,
                NearClipM = cam.State.Lens.NearClipPlane,
                FarClipM = cam.State.Lens.FarClipPlane,
                LensShiftHorizontalRatio = cam.State.Lens.PhysicalProperties.LensShift.x,
                LensShiftVerticalRatio = cam.State.Lens.PhysicalProperties.LensShift.y,
                // Lens distortion と center point は 0 にしておく
                LensDistortionRadialCoefficientsK1 = 0f,
                LensDistortionRadialCoefficientsK2 = 0f,
                LensDistortionCenterPointRightMm = 0f,
                LensDistortionCenterPointUpMm = 0f
            };

            return record;
        }
        
        public static void ToVirtualCamera(UcApiRecord record, ICinemachineCamera camera, CinemachineVolumeSettings volumeSettings)
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
            
            var cam = camera as Unity.Cinemachine.CinemachineCamera;
            if (cam != null)
            {
                cam.transform.position = position;
                cam.transform.rotation = Quaternion.LookRotation(forward, up);
                
                cam.Lens.FieldOfView = Camera.FocalLengthToFieldOfView(record.FocalLengthMm, record.SensorSizeHeightMm);
                cam.Lens.PhysicalProperties.SensorSize = new Vector2(record.SensorSizeWidthMm, record.SensorSizeHeightMm);
                cam.Lens.NearClipPlane = record.NearClipM;
                cam.Lens.FarClipPlane = record.FarClipM;
                cam.Lens.PhysicalProperties.LensShift.x = record.LensShiftHorizontalRatio;
                cam.Lens.PhysicalProperties.LensShift.y = record.LensShiftVerticalRatio;
                cam.Lens.PhysicalProperties.Aperture = record.Aperture;
                cam.Lens.PhysicalProperties.FocusDistance = record.FocusDistanceM;
                
                if (volumeSettings.Profile == null || !volumeSettings.Profile.Has<LensDistortion>())
                {
                    return;
                }
                
                foreach (var profileComponent in volumeSettings.Profile.components)
                {
                    if (profileComponent is not LensDistortion lensDistortion)
                    {
                        continue;
                    }
                    lensDistortion.xMultiplier.Override(record.LensDistortionRadialCoefficientsK1);
                    lensDistortion.yMultiplier.Override(record.LensDistortionRadialCoefficientsK2);
                    lensDistortion.center.Override(new Vector2(record.LensDistortionCenterPointRightMm, record.LensDistortionCenterPointUpMm));
                }
                
                // タイムコード情報をログ出力（デバッグ用）
                Debug.Log($"Received timecode: {record.TimeCode}");
            }
            else
            {
                Debug.LogError("Invalid camera type");
            }
        }
    }
}
