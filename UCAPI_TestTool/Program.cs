using System;
using System.Runtime.InteropServices;

namespace UCAPI_TestTool
{
    public class Program
    {
        [StructLayout(LayoutKind.Sequential)]
        public struct UcApiRecord
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

        [DllImport("UCAPI_DLL", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr UCAPI_Deserialize(byte[] buffer, UIntPtr payloadCount);

        [DllImport("UCAPI_DLL", CallingConvention = CallingConvention.Cdecl)]
        private static extern int UCAPI_Serialize(IntPtr obj, out IntPtr outBuffer, out UIntPtr outSize);

        [DllImport("UCAPI_DLL", CallingConvention = CallingConvention.Cdecl)]
        private static extern void UCAPI_FreeBuffer(IntPtr buffer);

        static void Main(string[] args)
        {
            try
            {
                Console.WriteLine("UCAPI Test Tool");
                Console.WriteLine("==============");

                // テストデータの作成
                var record = CreateTestRecord();
                Console.WriteLine("Test Record Created");

                // バイナリデータへのシリアライズ
                var serializedData = SerializeRecord(record);
                Console.WriteLine($"Serialized Size: {serializedData.Length} bytes");
                Console.WriteLine($"Raw Data: {BitConverter.ToString(serializedData)}");

                // バイナリデータからのデシリアライズ
                var deserializedRecord = DeserializeRecord(serializedData);
                Console.WriteLine("Deserialization Successful");

                // 整合性チェック
                CompareRecords(record, deserializedRecord);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error: {ex.Message}");
                Console.WriteLine($"StackTrace: {ex.StackTrace}");
            }

            Console.WriteLine("Press any key to exit...");
            Console.ReadKey();
        }

        static UcApiRecord CreateTestRecord()
        {
            return new UcApiRecord
            {
                CameraNo = 1,
                Commands = 0x0001,
                TimeCode = new byte[] { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C },
                SubFrame = 0x12345678,
                PacketNo = 1,
                EyePositionRightM = 1.0f,
                EyePositionUpM = 2.0f,
                EyePositionForwardM = 3.0f,
                LookVectorRightM = 0.0f,
                LookVectorUpM = 0.0f,
                LookVectorForwardM = 1.0f,
                UpVectorRightM = 0.0f,
                UpVectorUpM = 1.0f,
                UpVectorForwardM = 0.0f,
                FocalLengthMm = 50.0f,
                AspectRatio = 1.777778f,
                FocusDistanceM = 10.0f,
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
        }

        static byte[] SerializeRecord(UcApiRecord record)
        {
            var recordPtr = Marshal.AllocHGlobal(Marshal.SizeOf(record));
            try
            {
                Marshal.StructureToPtr(record, recordPtr, false);
                
                if (UCAPI_Serialize(recordPtr, out var buffer, out var size) != 0 || buffer == IntPtr.Zero)
                {
                    throw new Exception("Serialization failed");
                }

                var managedBuffer = new byte[(int)size];
                Marshal.Copy(buffer, managedBuffer, 0, (int)size);
                UCAPI_FreeBuffer(buffer);
                return managedBuffer;
            }
            finally
            {
                Marshal.FreeHGlobal(recordPtr);
            }
        }

        static UcApiRecord DeserializeRecord(byte[] data)
        {
            var nativePtr = UCAPI_Deserialize(data, (UIntPtr)1);
            if (nativePtr == IntPtr.Zero)
            {
                throw new Exception("Deserialization failed");
            }

            try
            {
                // 注: この部分は実際のDLLの戻り値の構造に応じて修正が必要かもしれません
                return Marshal.PtrToStructure<UcApiRecord>(nativePtr);
            }
            finally
            {
                UCAPI_FreeBuffer(nativePtr);
            }
        }

        static void CompareRecords(UcApiRecord original, UcApiRecord deserialized)
        {
            Console.WriteLine("\nComparing Records:");
            Console.WriteLine($"CameraNo: {original.CameraNo} vs {deserialized.CameraNo}");
            Console.WriteLine($"Commands: {original.Commands} vs {deserialized.Commands}");
            Console.WriteLine($"TimeCode: {BitConverter.ToString(original.TimeCode)} vs {BitConverter.ToString(deserialized.TimeCode)}");
            Console.WriteLine($"SubFrame: {original.SubFrame} vs {deserialized.SubFrame}");
            Console.WriteLine($"PacketNo: {original.PacketNo} vs {deserialized.PacketNo}");
            // ... 他のフィールドも同様に比較
        }
    }
}
