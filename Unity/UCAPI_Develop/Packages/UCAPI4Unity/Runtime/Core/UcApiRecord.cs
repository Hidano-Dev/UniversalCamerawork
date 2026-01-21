using System.Runtime.InteropServices;

namespace UCAPI4Unity.Runtime.Core
{
    /// <summary>
    /// Camera record structure matching DLL's ucapi_record_flat_t (packed, 1-byte alignment)
    /// </summary>
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct UcApiRecord
    {
        public uint CameraNo;
        public ushort Commands;
        public uint TimeCode;
        public float SubFrame;
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
    }
}