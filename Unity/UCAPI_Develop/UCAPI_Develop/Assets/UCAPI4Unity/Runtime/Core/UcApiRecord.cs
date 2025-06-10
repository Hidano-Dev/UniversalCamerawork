using System;
using System.Runtime.InteropServices;

namespace UCAPI4Unity.Runtime.Core
{
    [StructLayout(LayoutKind.Sequential)]
    public struct UcApiRecord
    {
        public uint CameraNo;
        public ushort Commands;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 12)]
        public byte[] TimeCode; // SMPTE 80bit + subframe (12バイト)
        public uint SubFrame;
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

        // 静的ファクトリーメソッドを使用して初期化された構造体を作成
        public static UcApiRecord Create()
        {
            return new UcApiRecord
            {
                TimeCode = new byte[12],
                CameraNo = 0,
                Commands = 0,
                SubFrame = 0,
                PacketNo = 0,
                EyePositionRightM = 0,
                EyePositionUpM = 0,
                EyePositionForwardM = 0,
                LookVectorRightM = 0,
                LookVectorUpM = 0,
                LookVectorForwardM = 0,
                UpVectorRightM = 0,
                UpVectorUpM = 0,
                UpVectorForwardM = 0,
                FocalLengthMm = 0,
                AspectRatio = 0,
                FocusDistanceM = 0,
                Aperture = 0,
                SensorSizeWidthMm = 0,
                SensorSizeHeightMm = 0,
                NearClipM = 0,
                FarClipM = 0,
                LensShiftHorizontalRatio = 0,
                LensShiftVerticalRatio = 0,
                LensDistortionRadialCoefficientsK1 = 0,
                LensDistortionRadialCoefficientsK2 = 0,
                LensDistortionCenterPointRightMm = 0,
                LensDistortionCenterPointUpMm = 0
            };
        }
    }
}