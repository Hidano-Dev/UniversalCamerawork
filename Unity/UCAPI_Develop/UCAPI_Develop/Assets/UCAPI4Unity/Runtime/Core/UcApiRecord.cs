namespace UCAPI4Unity.Runtime.Core
{
    public struct UcApiRecord
    {
        public uint CameraNo;
        public ushort Commands;
        public UcApiSmpteTimecode TimeCode;  // Changed from uint to UcApiSmpteTimecode (80-bit SMPTE LTC)
        public float SubFrame;               // Changed from uint to float for precision
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