using System;
using System.Runtime.InteropServices;

class Program
{
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct TestRecord
    {
        public uint CameraNo;
        public ushort Commands;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 12)]
        public byte[] TimeCode;
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
    }

    static void Main(string[] args)
    {
        // テスト用のレコードを作成
        var record = new TestRecord
        {
            CameraNo = 1,
            Commands = 0x1234,
            TimeCode = new byte[12],
            SubFrame = 0,
            PacketNo = 0x56,
            EyePositionRightM = 1.1f,
            EyePositionUpM = 2.2f,
            EyePositionForwardM = 3.3f,
            LookVectorRightM = 4.4f,
            LookVectorUpM = 5.5f,
            LookVectorForwardM = 6.6f,
            UpVectorRightM = 7.7f,
            UpVectorUpM = 8.8f,
            UpVectorForwardM = 9.9f,
            FocalLengthMm = 10.1f,
            AspectRatio = 1.777f,
            FocusDistanceM = 2.5f,
            Aperture = 2.8f,
            SensorSizeWidthMm = 36.0f,
            SensorSizeHeightMm = 24.0f,
            NearClipM = 0.1f,
            FarClipM = 1000.0f,
            LensShiftHorizontalRatio = 0.0f,
            LensShiftVerticalRatio = 0.0f,
            LensDistortionRadialCoefficientsK1 = 0.0f,
            LensDistortionRadialCoefficientsK2 = 0.0f,
            LensDistortionCenterPointRightMm = 0.0f,
            LensDistortionCenterPointUpMm = 0.0f
        };

        int structSize = Marshal.SizeOf<TestRecord>();
        IntPtr ptr = Marshal.AllocHGlobal(structSize);
        try
        {
            Marshal.StructureToPtr(record, ptr, false);
            // DLLのCRC16関数を呼び出し
            ushort crc = NativeMethods.UCAPI_CalcCRC16(ptr, (UIntPtr)structSize, 0x1021, 0xFFFF);
            Console.WriteLine($"CRC16 (from DLL): 0x{crc:X4}");
        }
        finally
        {
            Marshal.FreeHGlobal(ptr);
        }
    }
}
