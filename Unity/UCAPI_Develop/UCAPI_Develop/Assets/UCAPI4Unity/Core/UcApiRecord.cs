using MessagePack;

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
    }
}