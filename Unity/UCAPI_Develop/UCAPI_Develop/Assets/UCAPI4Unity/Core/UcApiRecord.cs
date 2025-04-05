using System;
using System.IO;
using System.Runtime.InteropServices;

namespace UCAPI4Unity.Core
{
    public struct UcApiRecord
    {
        public readonly uint CameraNo;
        public readonly ushort Commands;
        public UcApiTimeCode TimeCode;
        public readonly byte PacketNo;
        public readonly float EyePositionRightM;
        public readonly float EyePositionUpM;
        public readonly float EyePositionForwardM;
        public readonly float LookVectorRightM;
        public readonly float LookVectorUpM;
        public readonly float LookVectorForwardM;
        public readonly float UpVectorRightM;
        public readonly float UpVectorUpM;
        public readonly float UpVectorForwardM;
        public readonly float FocalLengthMm;
        public readonly float AspectRatio;
        public readonly float FocusDistanceM;
        public readonly float Aperture;
        public readonly float SensorSizeWidthMm;
        public readonly float SensorSizeHeightMm;
        public readonly float NearClipM;
        public readonly float FarClipM;
        public readonly float LensShiftHorizontalRatio;
        public readonly float LensShiftVerticalRatio;
        public readonly float LensDistortionRadialCoefficientsK1;
        public readonly float LensDistortionRadialCoefficientsK2;
        public readonly float LensDistortionCenterPointRightMm;
        public readonly float LensDistortionCenterPointUpMm;

        public UcApiRecord(IntPtr ptr, int size)
        {
            var buffer = new byte[size];
            Marshal.Copy(ptr, buffer, 0, size);
            
            using var ms = new MemoryStream(buffer);
            using var br = new BinaryReader(ms);
            CameraNo = br.ReadUInt32();
            Commands = br.ReadUInt16();
            TimeCode = new UcApiTimeCode(br.ReadInt32());
            PacketNo = br.ReadByte();
            EyePositionRightM = br.ReadSingle();
            EyePositionUpM = br.ReadSingle();
            EyePositionForwardM = br.ReadSingle();
            LookVectorRightM = br.ReadSingle();
            LookVectorUpM = br.ReadSingle();
            LookVectorForwardM = br.ReadSingle();
            UpVectorRightM = br.ReadSingle();
            UpVectorUpM = br.ReadSingle();
            UpVectorForwardM = br.ReadSingle();
            FocalLengthMm = br.ReadSingle();
            AspectRatio = br.ReadSingle();
            FocusDistanceM = br.ReadSingle();
            Aperture = br.ReadSingle();
            SensorSizeWidthMm = br.ReadSingle();
            SensorSizeHeightMm = br.ReadSingle();
            NearClipM = br.ReadSingle();
            FarClipM = br.ReadSingle();
            LensShiftHorizontalRatio = br.ReadSingle();
            LensShiftVerticalRatio = br.ReadSingle();
            LensDistortionRadialCoefficientsK1 = br.ReadSingle();
            LensDistortionRadialCoefficientsK2 = br.ReadSingle();
            LensDistortionCenterPointRightMm = br.ReadSingle();
            LensDistortionCenterPointUpMm = br.ReadSingle();
        }
    }
}