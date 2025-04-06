using UnityEngine;

namespace UCAPI4Unity.Core
{
    public struct UcApiRecord
    {
        public uint CameraNo;
        public ushort Commands;
        public UcApiTimeCode TimeCode;
        public byte PacketNo;
        public float EyePositionRightM;
        public float EyePositionUpM;
        public float EyePositionForwardM;
        public float LookVectorRightM;
        public float LookVectorUpM;
        public float LookVectorForwardM;
        public float UpVectorRightM;
        public float UpVectorUpM;
        public float UpVectorForwardM;
        public float FocalLengthMm;
        public float AspectRatio;
        public float FocusDistanceM;
        public float Aperture;
        public float SensorSizeWidthMm;
        public float SensorSizeHeightMm;
        public float NearClipM;
        public float FarClipM;
        public float LensShiftHorizontalRatio;
        public float LensShiftVerticalRatio;
        public float LensDistortionRadialCoefficientsK1;
        public float LensDistortionRadialCoefficientsK2;
        public float LensDistortionCenterPointRightMm;
        public float LensDistortionCenterPointUpMm;

        internal UcApiRecord(Camera cam)
        {
            CameraNo = 1;
            Commands = 0x0B; // DOF_ENABLE | LENS_DISTORTION_ENABLE 仮
            PacketNo = 1;
            TimeCode = new UcApiTimeCode
            {
                FrameNumber = 12,
                Second = 34,
                Minute = 56,
                Hour = 78,
                FrameRate = (byte)FrameRate.FrameRate60,
                DropFrame = 0
            };
            EyePositionRightM = cam.transform.position.x;
            EyePositionUpM = cam.transform.position.y;
            EyePositionForwardM = cam.transform.position.z;
            LookVectorRightM = cam.transform.forward.x;
            LookVectorUpM = cam.transform.forward.y;
            LookVectorForwardM = cam.transform.forward.z;
            UpVectorRightM = cam.transform.up.x;
            UpVectorUpM = cam.transform.up.y;
            UpVectorForwardM = cam.transform.up.z;
            FocalLengthMm = cam.focalLength;
            AspectRatio = cam.aspect;
            FocusDistanceM = cam.focusDistance;
            Aperture = cam.aperture;
            SensorSizeWidthMm = cam.sensorSize.x;
            SensorSizeHeightMm = cam.sensorSize.y;
            NearClipM = cam.nearClipPlane;
            FarClipM = cam.farClipPlane;
            LensShiftHorizontalRatio = cam.lensShift.x;
            LensShiftVerticalRatio = cam.lensShift.y;
            // Lens distortion と center point は 0 にしておく
            LensDistortionRadialCoefficientsK1 = 0f;
            LensDistortionRadialCoefficientsK2 = 0f;
            LensDistortionCenterPointRightMm = 0f;
            LensDistortionCenterPointUpMm = 0f;
        }
        
        internal void ApplyToCamera(Camera camera)
        {
            // カメラ位置・回転を反映
            var position = new Vector3(
                EyePositionRightM,
                EyePositionUpM,
                EyePositionForwardM
            );

            var forward = new Vector3(
                LookVectorRightM,
                LookVectorUpM,
                LookVectorForwardM
            );

            var up = new Vector3(
                UpVectorRightM,
                UpVectorUpM,
                UpVectorForwardM
            );

            camera.transform.position = position;
            camera.transform.rotation = Quaternion.LookRotation(forward, up);
            
            // レンズ・クリップなどを反映
            camera.focalLength = FocalLengthMm;
            camera.aspect = AspectRatio;
            camera.nearClipPlane = NearClipM;
            camera.farClipPlane = FarClipM;
            camera.sensorSize = new Vector2(SensorSizeWidthMm, SensorSizeHeightMm);
            camera.lensShift = new Vector2(LensShiftHorizontalRatio, LensShiftVerticalRatio);

            // 被写界深度など（コマンドビットによる有効化が必要なら拡張可）
            var dofEnabled = (Commands & 0x04) != 0;
            if (dofEnabled)
            {
                camera.usePhysicalProperties = true;
                camera.focusDistance = FocusDistanceM;
                camera.aperture = Aperture;
            }
        }
    }
}