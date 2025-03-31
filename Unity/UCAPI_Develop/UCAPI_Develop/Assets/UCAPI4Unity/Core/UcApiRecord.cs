using System;
using System.IO;
using System.Runtime.InteropServices;

namespace UCAPI4Unity.Core
{
    public class UcApiRecord
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