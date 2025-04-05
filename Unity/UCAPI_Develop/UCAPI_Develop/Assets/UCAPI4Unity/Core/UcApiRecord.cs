using System;
using System.IO;
using MessagePack;
using UnityEngine;

namespace UCAPI4Unity.Core
{
    [MessagePackObject]
    public struct UcApiRecord
    {
        [Key(0)] public uint CameraNo;
        [Key(1)] public ushort Commands;
        [Key(2)] public UcApiTimeCode TimeCode;
        [Key(3)] public byte PacketNo;
        [Key(4)] public float EyePositionRightM;
        [Key(5)] public float EyePositionUpM;
        [Key(6)] public float EyePositionForwardM;
        [Key(7)] public float LookVectorRightM;
        [Key(8)] public float LookVectorUpM;
        [Key(9)] public float LookVectorForwardM;
        [Key(10)] public float UpVectorRightM;
        [Key(11)] public float UpVectorUpM;
        [Key(12)] public float UpVectorForwardM;
        [Key(13)] public float FocalLengthMm;
        [Key(14)] public float AspectRatio;
        [Key(15)] public float FocusDistanceM;
        [Key(16)] public float Aperture;
        [Key(17)] public float SensorSizeWidthMm;
        [Key(18)] public float SensorSizeHeightMm;
        [Key(19)] public float NearClipM;
        [Key(20)] public float FarClipM;
        [Key(21)] public float LensShiftHorizontalRatio;
        [Key(22)] public float LensShiftVerticalRatio;
        [Key(23)] public float LensDistortionRadialCoefficientsK1;
        [Key(24)] public float LensDistortionRadialCoefficientsK2;
        [Key(25)] public float LensDistortionCenterPointRightMm;
        [Key(26)] public float LensDistortionCenterPointUpMm;

        public UcApiRecord(byte[] buffer)
        {
            using var ms = new MemoryStream(buffer);
            using var br = new BinaryReader(ms);

            CameraNo = br.ReadUInt32();
            Commands = br.ReadUInt16();
            var timeCodeBytes = br.ReadBytes(4);
            TimeCode = new UcApiTimeCode(timeCodeBytes);
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
            var raw = new byte[4];
            Buffer.BlockCopy(buffer, 75, raw, 0, 4);
            Debug.Log($"FarClipM raw bytes @75: {BitConverter.ToString(raw)}");
            FarClipM = br.ReadSingle();
            LensShiftHorizontalRatio = br.ReadSingle();
            LensShiftVerticalRatio = br.ReadSingle();
            LensDistortionRadialCoefficientsK1 = br.ReadSingle();
            LensDistortionRadialCoefficientsK2 = br.ReadSingle();
            LensDistortionCenterPointRightMm = br.ReadSingle();
            LensDistortionCenterPointUpMm = br.ReadSingle();

            br.ReadBytes(25); // reserved
        }
    }
}