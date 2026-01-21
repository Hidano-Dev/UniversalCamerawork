using System;
using System.Runtime.InteropServices;
using UnityEngine;

namespace UCAPI4Unity.Runtime.Core
{
    public static class UcApiCore
    {
        // =============================================================================
        // New C-compatible API (recommended)
        // =============================================================================

        [DllImport("UCAPI_DLL", CallingConvention = CallingConvention.Cdecl)]
        private static extern int UCAPI_SerializeRecord(
            ref UcApiRecord record,
            out IntPtr outBuffer,
            out UIntPtr outSize
        );

        [DllImport("UCAPI_DLL", CallingConvention = CallingConvention.Cdecl)]
        private static extern int UCAPI_DeserializeRecord(
            byte[] buffer,
            UIntPtr bufferSize,
            out UcApiRecord outRecord
        );

        [DllImport("UCAPI_DLL", CallingConvention = CallingConvention.Cdecl)]
        private static extern void UCAPI_FreeBuffer(IntPtr buffer);

        [DllImport("UCAPI_DLL", CallingConvention = CallingConvention.Cdecl)]
        private static extern ushort UCAPI_CalcCRC16(IntPtr recordPtr, UIntPtr length, ushort poly, ushort initValue);

        /// <summary>
        /// Serialize a UcApiRecord to MessagePack format
        /// </summary>
        public static byte[] SerializeFromRecord(UcApiRecord record)
        {
            var result = UCAPI_SerializeRecord(ref record, out var buffer, out var size);
            if (result != 0 || buffer == IntPtr.Zero)
            {
                throw new Exception("Serialization to MessagePack failed.");
            }

            var managedBuffer = new byte[(int)size];
            Marshal.Copy(buffer, managedBuffer, 0, (int)size);
            UCAPI_FreeBuffer(buffer);
            return managedBuffer;
        }

        /// <summary>
        /// Deserialize MessagePack data to UcApiRecord array
        /// </summary>
        public static UcApiRecord[] DeserializeToRecord(byte[] buffer, int payloadCount)
        {
            // Note: Current API deserializes one record at a time
            // payloadCount parameter is kept for API compatibility but only first record is used
            var result = UCAPI_DeserializeRecord(buffer, (UIntPtr)buffer.Length, out var record);
            if (result != 0)
            {
                throw new Exception("Deserialization failed.");
            }

            return new[] { record };
        }

        /// <summary>
        /// Compute CRC16 checksum for a record
        /// </summary>
        public static ushort ComputeChecksum(UcApiRecord record, ushort poly = 0x1021, ushort initValue = 0xFFFF)
        {
            int size = Marshal.SizeOf(record);
            IntPtr ptr = Marshal.AllocHGlobal(size);
            try
            {
                Marshal.StructureToPtr(record, ptr, false);
                return UCAPI_CalcCRC16(ptr, (UIntPtr)size, poly, initValue);
            }
            finally
            {
                Marshal.FreeHGlobal(ptr);
            }
        }

        /// <summary>
        /// デバッグ用にレコードの内容を出力します。
        /// </summary>
        /// <param name="record"></param>
        private static void DumpRecord(UcApiRecord record)
        {
            Debug.Log("Record Dump:"
            + $"\n  CameraNo: {record.CameraNo}"
            + $"\n  Commands: {record.Commands}"
            + $"\n  TimeCode: {record.TimeCode}"
            + $"\n  SubFrame: {record.SubFrame}"
            + $"\n  PacketNo: {record.PacketNo}"
            + $"\n  EyePositionRightM: {record.EyePositionRightM}"
            + $"\n  EyePositionUpM: {record.EyePositionUpM}"
            + $"\n  EyePositionForwardM: {record.EyePositionForwardM}"
            + $"\n  LookVectorRightM: {record.LookVectorRightM}"
            + $"\n  LookVectorUpM: {record.LookVectorUpM}"
            + $"\n  LookVectorForwardM: {record.LookVectorForwardM}"
            + $"\n  UpVectorRightM: {record.UpVectorRightM}"
            + $"\n  UpVectorUpM: {record.UpVectorUpM}"
            + $"\n  UpVectorForwardM: {record.UpVectorForwardM}"
            + $"\n  FocalLengthMm: {record.FocalLengthMm}"
            + $"\n  AspectRatio: {record.AspectRatio}"
            + $"\n  FocusDistanceM: {record.FocusDistanceM}"
            + $"\n  Aperture: {record.Aperture}"
            + $"\n  SensorSizeWidthMm: {record.SensorSizeWidthMm}"
            + $"\n  SensorSizeHeightMm: {record.SensorSizeHeightMm}"
            + $"\n  NearClipM: {record.NearClipM}"
            + $"\n  FarClipM: {record.FarClipM}"
            + $"\n  LensShiftHorizontalRatio: {record.LensShiftHorizontalRatio}"
            + $"\n  LensShiftVerticalRatio: {record.LensShiftVerticalRatio}"
            + $"\n  LensDistortionRadialCoefficientsK1: {record.LensDistortionRadialCoefficientsK1}"
            + $"\n  LensDistortionRadialCoefficientsK2: {record.LensDistortionRadialCoefficientsK2}"
            + $"\n  LensDistortionCenterPointRightMm: {record.LensDistortionCenterPointRightMm}"
            + $"\n  LensDistortionCenterPointUpMm: {record.LensDistortionCenterPointUpMm}"
            + $"\n  CRC16: {ComputeChecksum(record)}");
        }
        
        
        /// <summary>
        /// デバッグ用にレコードの内容を出力します。
        /// </summary>
        /// <param name="record"></param>
        private static void DumpRecordBinary(UcApiRecord record)
        {
            var buffer = new byte[Marshal.SizeOf(record)];
            var ptr = Marshal.AllocHGlobal(Marshal.SizeOf(record));
            Marshal.StructureToPtr(record, ptr, false);
            Marshal.Copy(ptr, buffer, 0, buffer.Length);
            Marshal.FreeHGlobal(ptr);

            Debug.Log("Record Dump:"
                      + $"\n  CRC16: {ComputeChecksum(record)}"
                      + $"\n  Binary: {BitConverter.ToString(buffer).Replace("-", " ")}");
        }
    }
}